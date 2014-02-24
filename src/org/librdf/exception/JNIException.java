package org.librdf.exception;

public class JNIException extends RuntimeException {
    private static final long serialVersionUID = -7356802628171201203L;

    public JNIException() {
        super();
    }

    public JNIException(String message) {
        super(message);
    }

    public JNIException(Throwable tr) {
        super(tr);
    }

    public JNIException(String message, Throwable tr) {
        super(message, tr);
    }
}
