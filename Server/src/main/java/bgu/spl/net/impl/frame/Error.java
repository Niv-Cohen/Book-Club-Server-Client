package bgu.spl.net.impl.frame;

public class Error implements Frame {
    String frame="";
    public Error(int cases,String receipt){

          frame+="ERROR"+'\n'+"message: ";
        switch (cases){
            case 2:
                frame+="“Wrong password"+'\n';
                break;
            case 3:
                frame+="User already logged in"+'\n';
                break;
            case 5:
                frame+="User is already subscribed"+'\n';
                break;
            case 6:
                frame+="User is not subscribed to the channel"+'\n';
            case 99:
                frame+="malformed frame received"+'\n';
                break;
        }
        if(receipt!=""){
            frame+="receipt id: "+receipt+'\n';
        }
        frame+='\n'+'\u0000';
    }
    @Override
    public String getFrame() {
        return frame;
    }
}
