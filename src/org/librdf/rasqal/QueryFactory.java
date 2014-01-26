package org.librdf.rasqal;

public final class QueryFactory {
    public synchronized static Query createQuery( RasqalWorld world, String queryText ) {
        Query query = new Query( world.getRawPointer() );
        query.parseQuery( queryText );
        return query;
    }
}
