package com.evanpatton.raptortest;

import java.io.UnsupportedEncodingException;

public class Raptor {
    static {
        System.loadLibrary("raptortest");
    }

    private static native int parse(byte[] bytes);

    public static int parse(String str) {
        try {
            byte[] content = str.getBytes("UTF-8");
            return parse( content );
        } catch (UnsupportedEncodingException e) {
            throw new IllegalArgumentException("Expected a string representable in UTF-8", e);
        }
    }
}
