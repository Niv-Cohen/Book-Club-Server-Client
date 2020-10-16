package bgu.spl.net.api;

import java.nio.charset.StandardCharsets;
import java.util.Arrays;

public class MessageEncoderDecoderImpl<T> implements MessageEncoderDecoder<T> {
    private byte[] bytes = new byte[1 << 10]; //start with 1k
    private int len = 0;
    @Override
    public T decodeNextByte(byte nextByte) {
        if (nextByte == '\u0000') {
            return (T) popString();
        }
        pushByte(nextByte);
        return null; //not a line yet
    }

    @Override
    public byte[] encode(T message) {
        return (message+"").getBytes();
    }
    private void pushByte(byte nextByte) {
        if (len >= bytes.length) {
            bytes = Arrays.copyOf(bytes, len * 2);
        }

        bytes[len++] = nextByte;
    }

    private String popString() {
        String result = new String(bytes, 0, len, StandardCharsets.UTF_8);
        len = 0;
        return result;
    }
}