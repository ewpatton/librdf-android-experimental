package org.librdf.raptor;

import java.io.BufferedInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

final class ParserInputStreamWrapper {
    private final static int EOF = -1;
    private final static Logger LOG =
            LoggerFactory.getLogger( ParserInputStreamWrapper.class );
    private final InputStream stream;

    ParserInputStreamWrapper( InputStream stream ) {
        this.stream = new BufferedInputStream( stream );
    }

    int readBytes( ByteBuffer bytes, int nmemb ) {
        try {
            int read = stream.read( bytes.array(), 0,
                    Math.min( bytes.capacity(), nmemb ) );
            // TODO read more if read < min( bytes.length, nmemb ) and look
            // for EOF on stream
            if ( read == -1 ) {
                return 0;
            }
            return read;
        } catch (IOException e) {
            LOG.warn( "Unable to read bytes from stream", e );
            return -1;
        }
    }

    boolean isEOF() {
        stream.mark( 2 );
        try {
            if ( stream.read() == EOF ) {
                stream.reset();
                return true;
            }
            stream.reset();
        } catch (IOException e) {
            LOG.warn( "Unable to test InputStream for EOF", e );
        }
        return false;
    }

    void finish() {
        try {
            stream.close();
        } catch (IOException e) {
            LOG.warn( "Unable to close input stream", e );
        }
    }
}
