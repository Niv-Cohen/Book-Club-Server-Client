package bgu.spl.net.srv;

import bgu.spl.net.api.MessageEncoderDecoder;
import bgu.spl.net.api.MessageEncoderDecoderImpl;
import bgu.spl.net.api.MessagingProtocol;
import bgu.spl.net.api.StompMessagingProtocolImpl;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.IOException;
import java.net.Socket;

public class BlockingConnectionHandler<T> implements Runnable, ConnectionHandler<T> {

    private final StompMessagingProtocolImpl protocol;
    private final MessageEncoderDecoderImpl<T> encdec;
    private final Socket sock;
    private BufferedInputStream in;
    private BufferedOutputStream out;
    private volatile boolean connected = true;
    static Object lock;

    public BlockingConnectionHandler(Socket sock, MessageEncoderDecoderImpl<T> reader, StompMessagingProtocolImpl protocol) {
        this.sock = sock;
        this.encdec = reader;
        this.protocol = protocol;
    }

    public StompMessagingProtocolImpl getProtocol() {
        return protocol;
    }

    @Override
    public void run() {
        System.out.println("starting run method");
        try (Socket sock = this.sock) { //just for automatic closing
            int read;

            in = new BufferedInputStream(sock.getInputStream());
            out = new BufferedOutputStream(sock.getOutputStream());

            while (!protocol.shouldTerminate() && connected && (read = in.read()) >= 0) {
              T nextMessage = encdec.decodeNextByte((byte) read);
                if (nextMessage != null) {
                    protocol.process((String)(nextMessage));
                }
            }

        } catch (IOException ex) {
            ex.printStackTrace();
        }

    }

    @Override
    public void close() throws IOException {
        connected = false;
        sock.close();
    }

    @Override
    public  synchronized void send(T msg) {
        try {
            out.write(encdec.encode(msg));
            out.flush();
        }
        catch (IOException e){
            e.printStackTrace();
        }
    }
}