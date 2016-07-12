#ifndef USVISITOR_HH
#define USVISITOR_HH

#include <iostream>

#include "uscheme.hh"


// ======================================================================

class usVisitor {
public:
  virtual void visit( usCons* ) = 0;
  virtual void visit( usSymbol* ) = 0;
  virtual void visit( usInteger* ) = 0;
};

// ----------------------------------------------------------------------

class usPrintVisitor : public usVisitor {
public:
  usPrintVisitor( ostream& dest ) : os( dest ) {}

  void visit( usCons* cons ) {
    os << cons->prn();
  }

  void visit( usSymbol* sym ) {
    os << sym->prn();
  }

  void visit( usInteger* i ) {
    os << i->prn();
  }

protected:
  ostream& os;
};

// ----------------------------------------------------------------------

class usEvalVisitor : public usVisitor {
public:

  usEvalVisitor( usObj* env ) : theEnv( env ) {}

  /*
   * Evaluating a cons cell means calling a function.
   */
  void visit( usCons* ) {

  }

  void visit( usSymbol* ) {

  }

  void visit( usInteger* ) {

  }

protected:
  usObj* theEnv;
  usObj* lastResult;
};

#endif
