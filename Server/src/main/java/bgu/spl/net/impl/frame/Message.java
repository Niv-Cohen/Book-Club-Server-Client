package bgu.spl.net.impl.frame;

public class Message implements Frame {
    private String header="";

    public Message(int subId,int messageId,String destination){
     header="MESSAGE"+'\n'+ "subscription:"+subId+ '\n'+"Message-id:"+messageId+'\n'+"destination:"+destination+'\n'+'\n';
    }
    @Override
    public String getFrame() {
        return header;
    }
}
