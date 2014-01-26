package org.librdf.raptor;

import java.io.Closeable;
import java.io.IOException;

public final class RaptorWorld implements Closeable {

    private long world = 0;
    private static RaptorWorld theWorld = null;

    private static native long newWorld();
    private native void freeWorld();

    public RaptorWorld() {
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

    public static RaptorWorld getDefaultWorld() {
        if ( theWorld == null || theWorld.isClosed() ) {
            theWorld = new RaptorWorld();
        }
        return theWorld;
    }

    long getRawPointer() {
        return world;
    }
}
