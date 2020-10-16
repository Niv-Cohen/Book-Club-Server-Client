package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.MessageEncoderDecoderImpl;
import bgu.spl.net.api.StompMessagingProtocolImpl;
import bgu.spl.net.impl.rci.ObjectEncoderDecoder;
import bgu.spl.net.impl.rci.RemoteCommandInvocationProtocol;
import bgu.spl.net.srv.Server;

import java.util.function.Supplier;

public class StompServer {

    public static void main(String[] args) {

//        Server.threadPerClient(
//                6666, //port
//                () -> new StompMessagingProtocolImpl(), //protocol factory
//                () -> new MessageEncoderDecoderImpl<>() //message encoder decoder factory
//        ).serve();
//
        //}

//        Server.reactor(Runtime.getRuntime().availableProcessors(),6666, StompMessagingProtocolImpl::new, //protocol factory
//                MessageEncoderDecoderImpl::new).serve(); //message encoder decoder factory
//         .serve();

        if (args.length < 2) {
            System.out.println("Enter port and server mode as program argument");
        } else {
            int nThreads = Runtime.getRuntime().availableProcessors();
            //ConnectionsImpl conn = ConnectionsImpl.getInstance();
            int port = Integer.parseInt(args[0]);
            switch (args[1].toLowerCase()) {
                case "tpc":
                    System.out.println("Thread per client mode");
                    Server.threadPerClient(port,
                            () -> new StompMessagingProtocolImpl(),
                            () -> new MessageEncoderDecoderImpl<>()).serve();
                    break;
                case "reactor":
                    System.out.println("Reactor mode");
                    Server.reactor(nThreads, port,
                            () -> new StompMessagingProtocolImpl(),
                            () -> new MessageEncoderDecoderImpl<>()).serve();
                    break;
            }
        }

    }
}