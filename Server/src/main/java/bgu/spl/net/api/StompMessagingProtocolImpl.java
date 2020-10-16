package bgu.spl.net.api;
import bgu.spl.net.impl.frame.*;
import bgu.spl.net.impl.frame.Error;
import bgu.spl.net.srv.Connections;
import bgu.spl.net.srv.ConnectionsImpl;
import javafx.util.Pair;

import java.util.Arrays;

public class  StompMessagingProtocolImpl implements StompMessagingProtocol {
    private ConnectionsImpl<String> connections;
    private int connectionId;

    @Override

    //initialize certain ConnectionHandler
    public void start(int connectionId, Connections<String> connections) {
        this.connectionId = connectionId;
        this.connections =  (ConnectionsImpl)connections;
    }

    public boolean Subscribe(String[] splitMsg) {
        String genre = splitMsg[1].substring(splitMsg[1].indexOf(":")+1);
        int subId = Integer.parseInt(splitMsg[2].substring(splitMsg[2].indexOf(":")+1,splitMsg[2].length()));
        String recipt = splitMsg[3].substring(splitMsg[3].indexOf(":")+1);
        //subscribe to genre.
        boolean ok ;
        ok=connections.subscribeToGenre(subId, connectionId, genre);
        //add to the subscription id to Client's List by Id
        if(ok) {
            connections.addSubIdToClient(connectionId, subId, genre);
            //System.out.println("added new user ID: " + connectionId + " to genre: " + genre);
        }
        return ok;
    }

//    public boolean validConnectFrame(String[] splitMsg) {
//        if (!splitMsg[3].substring(0, 6).equals("login:") | !splitMsg[4].substring(0, 9).equals("passcode:")) {
//            return false;
//        }
//        return true;
//    }
    //All of switch commands
    private String getBookName(int index,String[] splitCmd){
        String bookName="";
        for (int i=index;i<splitCmd.length;i++){
            bookName+=splitCmd[i]+" ";
        }
        bookName=bookName.substring(0,bookName.length()-1);
        return bookName;
    }
    private String getBookName2(String[] splitcmd,String word){
        String bookName="";
        int index=1;
        while(!splitcmd[index].equals(word)){
            bookName=bookName+splitcmd[index]+" ";
            index++;
        }
        bookName=bookName.substring(0,bookName.length()-1);
        return bookName;
    }
    private String getLenderName(String[] splitCmd,String word){
        int index=1;
        while(!splitCmd[index].equals(word)){
            index++;
        }
        return splitCmd[index+1];
    }
    public void commandSwitch(Frame frameHeader, String[] splitCmd,String genre,String[] splitStatus){

       String bookName="";
       //adding new book
        if(splitCmd.length==1 && !splitCmd[0].contains(":")){
         //   System.out.println(Arrays.toString(splitCmd));
          connections.notifyStatus(frameHeader,genre,splitCmd[0]);
        }
          else if(splitCmd[0].equals("book")) {

            connections.bookStatus(frameHeader,genre);
        }
         else if(splitCmd[0].equals("Taking")){
            bookName=getBookName2(splitCmd,"from");
            String lender=getLenderName(splitCmd,"from");
            //connections.clinetHasBook(frameHeader,splitCmd[0],bookName,genre);
            connections.borrowBook(connectionId,frameHeader,lender,bookName,genre);
        }
        else if(splitCmd[0].equals("Returning")){
            bookName=getBookName2(splitCmd,"to");
            String lender=getLenderName(splitCmd,"to");
            connections.returnBook(connectionId,frameHeader,lender,bookName,genre);
        }
        if(splitCmd.length>2) {
         if (splitCmd[2].equals("added")) {
                bookName = getBookName(5, splitCmd);
                connections.notifyAllSubsAdded(frameHeader, splitCmd[0], bookName, genre);
            } else if (splitCmd[1].equals("has")) {
                bookName = getBookName(4, splitCmd);
                connections.notifyAllSubsFound(frameHeader, splitCmd[0], bookName, genre);
            } else if (splitCmd[1].equals("wish")) {
                bookName = getBookName(4, splitCmd);

                connections.findBook(connectionId, frameHeader, splitCmd[0], bookName, genre);
            }
        }
      else if(splitCmd.length==1){ //send the books status to all relevant subscribers
          String command="";
          //=splitCmd[1]+splitCmd[2];
            System.out.println();
          //splitCmd[0]=splitCmd[0].substring(splitCmd[0].indexOf(":"));
          for (int i=0;i<splitStatus.length;i++){
              command=command+splitStatus[i]+",";
          }
          command=command.substring(0,command.length()-1);
          connections.clientGenreInventory(frameHeader,genre,command);
           }

    }

    @Override
    public void process(String msg) {

        String recipt = "";
        String[] splitMsg = msg.split("\n");
        int cases;
        Frame frame;
        String destination="";
        //String command="";
        if (splitMsg[0].equals("CONNECT")) {
            cases = connections.checkLogin(splitMsg,connectionId);
                if(cases==1){
                    //System.out.println("Connecting was succsesful");
                    connections.send(connectionId,new Connect().getFrame());}
                else{
                    connections.send(connectionId,new Error(cases,"").getFrame());
                }
        }

        else if (splitMsg[0].equals("SUBSCRIBE")) {

            boolean ok= this.Subscribe(splitMsg);
            recipt = splitMsg[3].substring(splitMsg[3].indexOf(":")+1);
            if(ok) {
                connections.send(connectionId, new Reciept(recipt).getFrame());
            }
            else {
                connections.send(connectionId, new Error(5,recipt).getFrame());
            }
        }
        else if (splitMsg[0].equals("UNSUBSCRIBE")) {
            //unsubscribe to genre.
            recipt=splitMsg[2].substring(splitMsg[2].indexOf(":")+1);
            boolean ok=connections.unsubscribe(splitMsg, connectionId);
            if(ok){
                connections.send(connectionId,new Reciept(recipt).getFrame());
            }
            else {
                connections.send(connectionId,new Error(6,recipt).getFrame());
            }
        }
        else if(splitMsg[0].equals("SEND")) {
           destination=splitMsg[1].substring(splitMsg[1].indexOf(":")+1);
            String[] splitCmd;
            String[] splitStatus;

              splitCmd = splitMsg[3].split(" ");
               //System.out.println(Arrays.toString(splitCmd));
               splitStatus = splitMsg[3].split(",");
          //     System.out.println("**********"+Arrays.toString(splitStatus));
               commandSwitch(new Message(connections.findSubscriptionNum(destination,connectionId),connections.getMessageReceipt(),destination),splitCmd,destination,splitStatus);

//           else {
//               splitCmd=new String[1];
//               splitStatus=new String[1];
//               commandSwitch(new Message(connections.findSubscriptionNum(destination, connectionId), connections.getMessageReceipt(), destination), splitCmd, destination, splitStatus);
//           }
        }
        else if(splitMsg[0].equals("DISCONNECT")){
            recipt=splitMsg[1].substring(splitMsg[1].indexOf(":")+1);

            connections.send(connectionId,new Reciept(recipt).getFrame());
            connections.disconnect(connectionId);
        }
    }

        @Override
        public boolean shouldTerminate () {
            return false;
        }
    }

