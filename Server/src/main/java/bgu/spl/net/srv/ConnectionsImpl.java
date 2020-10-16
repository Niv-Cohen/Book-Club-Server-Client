package bgu.spl.net.srv;

import bgu.spl.net.impl.frame.Frame;
import javafx.util.Pair;

import java.util.*;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicInteger;

public class ConnectionsImpl<T> implements Connections<T> {

    //LinkedList<ConnectionHandler> listOfConnections =new LinkedList();
    private Map<String, Pair<String, Boolean>> namePassMap;
    private Map<Integer,String> idByName;
    private Map<Integer, ConnectionHandler> idConnectionHandler;
    //<genre,<<user id,sub id>>>
    private Map<String, LinkedList<Pair<Integer, Integer>>> genreId;
    //check if login was succsesful and return id
    private Map<Integer, LinkedList<Pair<Integer, String>>> userId_SubId_NameMap;
    //private Map<String, LinkedList<Integer>> requestedBooks_IdMap;
    private AtomicInteger MessageReceipt = new AtomicInteger(1);

    public ConnectionsImpl() {
        namePassMap = new ConcurrentHashMap<>();
        idConnectionHandler = new ConcurrentHashMap<>();
        genreId = new ConcurrentHashMap<>();
        userId_SubId_NameMap = new ConcurrentHashMap<>();
        idByName=new ConcurrentHashMap<>();
    }

    //    public void addUserId_Namemap(int connectionId,int subId,String genre){
//        userId_SubId_NameMap.putIfAbsent(connectionId,new LinkedList<>());
//        userId_SubId_NameMap.get(connectionId).add(new Pair<>(subId,genre));
//    }
    public void addNewHandler(int id, ConnectionHandler handler) {
        idConnectionHandler.putIfAbsent(id, handler);
    }


    public int findSubscriptionNum(String destination, int connectionId) {
        LinkedList<Pair<Integer,Integer>>  list = genreId.get(destination);
        if(list!=null) {
            for (Pair<Integer, Integer> p : list) {
                if (p.getKey() == connectionId) {
                    return p.getValue();
                }
            }
        }
        //user isn't subscribed to that genre
        return -1;
    }

    //add new subscription id with it's genre to Map
    public void addSubIdToClient(int id, int subid, String genre) {
        if (!userId_SubId_NameMap.containsKey(id)) {
            userId_SubId_NameMap.put(id, new LinkedList<>());
        }
        userId_SubId_NameMap.get(id).push(new Pair(subid, genre));
    }

    //search for the book among the clients who are subscribed to the genre
    public void findBook(int connectionId, Frame messageFrame, String client, String bookName, String genre) {
        String msg = messageFrame.getFrame();
        msg += client + " wish to borrow " + bookName + '\n' + '\u0000';
        //requestedBooks_IdMap.put(bookName, new LinkedList<>());
        //requestedBooks_IdMap.get(bookName).push(connectionId);
        send(genre, msg);
    }

    public void clinetHasBook(Frame messageFrame, String client, String bookName, String genre) {
        String msg = messageFrame.getFrame();
        msg += client + " has " + bookName + '\n' + '\u0000';
        send(genre, msg);
    }

    public void addTo_id_HandlerMap(int id, ConnectionHandler handler) {
        idConnectionHandler.put(id, handler);
    }
    //notify all subscribers a new book was added

    public void notifyAllSubsAdded(Frame messageFrame, String client, String bookName, String genre) {
        String msg = messageFrame.getFrame();

        msg += client + " has added the book " + bookName + '\n' + '\u0000';
        //send a broadcast to all subscribers to the same genre
        send(genre, msg);
    }
    public void notifyAllSubsFound(Frame messageFrame, String client, String bookName, String genre) {
        String msg = messageFrame.getFrame();

        msg += client + " has the book " + bookName + '\n' + '\u0000';
        //send a broadcast to all subscribers to the same genre
        send(genre, msg);
    }
    //send a message that the book was taking from the client
    public void borrowBook(int connnectionId, Frame messageFrame, String client, String bookName, String genre) {
        String msg = messageFrame.getFrame();
        msg += "Taking " + bookName + " from " + client+ '\n' + '\u0000';
//        if (requestedBooks_IdMap.containsKey(bookName)) {
//            if (requestedBooks_IdMap.get(bookName).size() == 1) {
//                requestedBooks_IdMap.remove(bookName);
//            } else {
//                requestedBooks_IdMap.get(bookName).remove(connnectionId);
//            }
            send(genre, msg);
        }

    public void returnBook(int connectionId, Frame messageFrame, String client, String bookName, String genre) {
        String msg = messageFrame.getFrame();
        msg += "Returning " + bookName + " to " + client + '\n' + '\u0000';
        send(genre, msg);
    }

    public void bookStatus(Frame messageFrame, String genre) {
        String msg = messageFrame.getFrame();
        msg += "Book status" + '\n' + '\u0000';
        send(genre, msg);
    }

    public void notifyStatus(Frame messageFrame, String genre,String status) {
        String msg = messageFrame.getFrame();
        msg += status + '\n' + '\u0000';

        send(genre, msg);
    }

    //send to all subscribers the client's inventory
    public void clientGenreInventory(Frame messageFrame, String genre, String command) {
        String msg = messageFrame.getFrame();
        msg += command +'\n'+ '\u0000';
        send(genre, msg);
    }

    public int getMessageReceipt() {
        return MessageReceipt.getAndIncrement();
    }

    public boolean subscribeToGenre(int subId, int connectionId, String genre) {
        //if the genre isn't on the list create new list.
        if (!genreId.containsKey(genre)) {
            genreId.put(genre, new LinkedList<>());
        }
        for (Pair pair : genreId.get(genre)) {
            if (pair.getKey().equals(connectionId)) {
                return false;
            }
        }

        for(int i=0;i<genreId.get(genre).size();i++){
            System.out.println(genreId.get(genre).get(i));
        }

        //push pair to list
        genreId.get(genre).add(new Pair<>(connectionId, subId));
        return true;
    }

    public boolean unsubscribe(String[] splitMsg, int connectionId) {
        //remove pair from list.
        int subId = Integer.parseInt(splitMsg[1].substring(splitMsg[1].indexOf(":") + 1));

        LinkedList<Pair<Integer, String>> list = userId_SubId_NameMap.get(connectionId);
        String genre="";
        Pair<Integer, String> toRemove = null;
        for (Pair<Integer, String> pair : list) {
            if (pair.getKey() == subId) {
                genre = pair.getValue();
                toRemove = pair;
                break;
            }
        }
        if (toRemove != null) {
            userId_SubId_NameMap.get(connectionId).remove(toRemove);
        }
        if(genre!="") {
            for (Pair pair : genreId.get(genre)) {
                if (pair.getKey().equals(connectionId)) {
                    genreId.get(genre).remove();
                }
            }
            return true;
        }
        return false;
    }



    public int checkLogin(String[] splitMsg,int connectionId){
        int cases;
        String name=splitMsg[3].substring(splitMsg[3].indexOf(":")+1);
        String pass=splitMsg[4].substring(splitMsg[4].indexOf(":")+1);
        //if it is a new user

        if(!namePassMap.containsKey(name)){
            namePassMap.put(name,new Pair<>(pass,true));
            idByName.putIfAbsent(connectionId,name);
            cases= 1;
        }
        //if exists and has the right password return id
        else if(namePassMap.get(name).getKey().equals(pass))
        {
            //if the user is already logged in.
            if(namePassMap.get(name).getValue()){  cases= 3;}
            else{
                namePassMap.remove(name);
                namePassMap.put(name,new Pair<>(pass,true));
                        idByName.putIfAbsent(connectionId,name);
                cases=1;

            }
        }
        //if the password is incorrect
        else{
            cases= 2;
        }
        return cases;
    }

    /**
     *
     */

    public boolean send(int connectionId, Object msg) {
       idConnectionHandler.get(connectionId).send(msg);
       return true;
    }

    public void send(String channel, Object msg) {
        if(genreId.get(channel)!=null) {
            for (Pair<Integer, Integer> pair : genreId.get(channel)) {
                send(pair.getKey(), msg);
            }
        }
        else{
            System.out.println("no subscribers to genre: " +channel );
        }
    }

    public void clearSubs(int connectionId){
        for (String genre:genreId.keySet()) {
            LinkedList <Pair<Integer,Integer>> list = genreId.get(genre);
            int index=0;
            for (Pair<Integer,Integer> pair:list) {
                if(pair.getKey()==connectionId){
                    genreId.get(genre).remove(index);
                }
                index++;
            }
        }
    }
    public void disconnect(int connectionId) {
        idConnectionHandler.remove(connectionId);
        clearSubs(connectionId);
        String name=idByName.get(connectionId);
        System.out.println(namePassMap.get(name).getKey());
        String pass=namePassMap.get(name).getKey();
        namePassMap.remove(name);
        namePassMap.put(name,new Pair<String,Boolean>(pass,false));
    }
}
