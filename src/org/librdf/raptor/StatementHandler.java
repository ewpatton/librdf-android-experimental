package org.librdf.raptor;

import org.librdf.shared.Term;

public interface StatementHandler {
    boolean handleStatement( Term subj, Term pred, Term obj, Term graph );
}
