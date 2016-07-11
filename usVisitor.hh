#ifndef USVISITOR_HH
#define USVISITOR_HH

#include "uscheme.hh"


class usVisitor {
public:
  virtual void visit( usCons* ) = 0;
  virtual void visit( usSymbol* ) = 0;
  virtual void visit( usInteger* ) = 0;
  virtual void visit( usAddOp* ) = 0;
};

class usEvalVisitor : public usVisitor {
public:

  usEvalVisitor( usObjPtr env ) : theEnv( env ) {}

  /*
   * Evaluating a cons cell means calling a function.
   */
  void visit( usCons* cons ) {
    cons->car()->accept( this );
    
  }

  void visit( usSymbol* ) {
    
  }

  void visit( usInteger* ) {
    
  }

  void visit( usAddOp* ) {
    
  }

protected:
  usObjPtr theEnv;
  usObjPtr lastResult;
};

#endif

