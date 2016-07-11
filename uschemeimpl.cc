#include "uscheme.hh"
#include "usVisitor.hh"

void usCons::accept( usVisitor* v ) {
  v->visit( this );
}

void usSymbol::accept( usVisitor* v ) {
  v->visit( this );
}

void usInteger::accept( usVisitor* v ) {
  v->visit( this );
}

void usAddOp::accept( usVisitor* v ) {
  v->visit( this );
}
