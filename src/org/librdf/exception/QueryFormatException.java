package org.librdf.exception;

public class QueryFormatException extends QueryException {
    private static final long serialVersionUID = 157741589392223287L;

    public QueryFormatException() {
        super();
    }

    public QueryFormatException( String message ) {
        super( message );
    }

    public QueryFormatException( Throwable tr ) {
        super( tr );
    }

    public QueryFormatException( String message, Throwable tr ) {
        super( message, tr );
    }

    public QueryFormatException( String[] messages ) {
        super( messages );
    }

    public QueryFormatException( String[] messages, Throwable tr ) {
        super( messages, tr );
    }
}
