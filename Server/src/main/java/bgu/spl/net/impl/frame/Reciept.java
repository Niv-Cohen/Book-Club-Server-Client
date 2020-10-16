package bgu.spl.net.impl.frame;

public class Reciept implements Frame {
    private String frame="";
    public Reciept(String recipt){
        frame+="RECEIPT"+'\n'+""+"receipt-id:"+recipt+'\n'+'\n'+'\u0000';
    }

    @Override
    public String getFrame() {
        return frame;
    }
}
