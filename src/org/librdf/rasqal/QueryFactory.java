package org.librdf.rasqal;

import android.util.Log;

public final class QueryFactory {
    private static final String TAG = QueryFactory.class.getSimpleName();
    public synchronized static Query createQuery( RasqalWorld world, String queryText ) {
        Query query = new Query( world.getRawPointer() );
        Log.i( TAG, "Created query object" );
        query.parseQuery( queryText );
        Log.i( TAG, "Parsed query text" );
        return query;
    }
}
