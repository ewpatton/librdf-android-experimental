package org.librdf.rasqal;

import java.io.Closeable;
import java.io.IOException;
import java.io.Serializable;
import java.util.NoSuchElementException;

import org.librdf.shared.Term;
import org.librdf.shared.Variable;

public final class ResultSet implements Iterable<Binding>, Serializable, Closeable {
    private static final long serialVersionUID = -2242979285438498493L;

    private long id = 0;

    private native boolean isFinished();
    private native boolean nextBinding();
    private native Variable[] getVariables();
    private native Term[] getTerms();
    private native void freeResultSet();

    ResultSet(long id) {
        this.id = id;
    }

    @Override
    public java.util.Iterator<Binding> iterator() {
        // TODO currently we create an iterator every time but operations
        // are done on a shared result set in native code. two threads therefore
        // affect whether or not the other's iterator advances. we should cache
        // the results within the ResultSet so that iterators will not interfere
        // with one another.
        return new Iterator();
    }

    long getId() {
        return this.id;
    }

    @Override
    public void close() throws IOException {
        if ( id != 0 ) {
            freeResultSet();
        }
    }

    class Iterator implements java.util.Iterator<Binding> {

        @Override
        public boolean hasNext() {
            return !isFinished();
        }

        @Override
        public Binding next() {
            if ( !nextBinding() ) {
                throw new NoSuchElementException("Attempted to read past last row of ResultSet");
            }
            Variable[] vars = getVariables();
            Term[] terms = getTerms();
            Binding b = new Binding();
            for ( int i = 0; i < vars.length; i++ ) {
                b.put( vars[i], terms[i] );
            }
            return b;
        }

        @Override
        public void remove() {
            throw new UnsupportedOperationException("Remove attempted on read-only ResultSet");
        }
    }
}
