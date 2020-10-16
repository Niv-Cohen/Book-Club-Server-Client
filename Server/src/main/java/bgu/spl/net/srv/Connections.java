package bgu.spl.net.srv;

import bgu.spl.net.impl.frame.Frame;

import java.io.IOException;

public interface Connections<T> {
/**
 Sends a message T to client represented by the given connectionId
 */
    boolean send(int connectionId, T msg);
    /**
     Sends a message T to clients subscribed to channel.
     */
    void send(String channel, T msg);
    /**
     Removes an active client connectionId from the map
     */
    void disconnect(int connectionId);
//    public int findSubscriptionNum(String destination,int connectionId);
//    public void addSubIdToClient(int id,int subid,String genre);
//    public void findBook(int connectionId, Frame messageFrame, String client, String bookName, String genre);
//    public void clinetHasBook(Frame messageFrame, String client, String bookName, String genre);
//    public void addTo_id_HandlerMap(int id,ConnectionHandler handler);
//    public void notifyAllSubs(Frame messageFrame, String client, String bookName, String genre);
//    public void borrowBook(int connnectionId,Frame messageFrame, String client, String bookName, String genre);
//    public void returnBook(int connectionId,Frame messageFrame, String client, String bookName, String genre);
//    public int checkLogin(String[] splitMsg);
//    public void bookStatus(Frame messageFrame,String genre);
//    public void clientGenreInventory(Frame messageFrame,String genre,String command);
//    public int getMessageReceipt();
//    public int unsubscribe(String[] splitMsg,int connectionId);
//    public void subscribeToGenre(int subId, int userid, String genre);



    }
