package bgu.spl.net.impl.frame;

public  class Connect implements Frame {
    private String frame;
    public Connect(){
         frame="CONNECTED"+'\n'+"version:1.2"+'\n'+'\n'+'\u0000';
    }

    @Override
    public String getFrame() {
        return frame;
    }
}
