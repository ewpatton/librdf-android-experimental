package org.librdf.rasqal;

import java.io.Closeable;
import java.io.IOException;

public final class RasqalWorld implements Closeable {

    private long world = 0;
    private static RasqalWorld theWorld = null;

    private static native long newWorld();
    private native void freeWorld();

    static {
        System.loadLibrary("raptortest");
    }

    public RasqalWorld() {
        world = newWorld();
    }

    public boolean isClosed() {
        return world == 0;
    }

    @Override
    public void close() throws IOException {
        if ( world != 0 ) {
            freeWorld();
        }
    }

    public static RasqalWorld getDefaultWorld() {
        if ( theWorld == null || theWorld.isClosed() ) {
            theWorld = new RasqalWorld();
        }
        return theWorld;
    }

    long getRawPointer() {
        return world;
    }

    public static native String testLibCurl();
}
