#ifndef USVISITOR_HH
#define USVISITOR_HH

#include <iostream>
#include <stack>

#include "uscheme.hh"


// ======================================================================

class usVisitor {
public:
  virtual void visit( usCons* ) = 0;
  virtual void visit( usSymbol* ) = 0;
  virtual void visit( usInteger* ) = 0;
  virtual void visit( usNil* ) = 0;
};

// ----------------------------------------------------------------------

class usPrintVisitor : public usVisitor {
public:
  usPrintVisitor( ostream& dest, bool addTypes = false ) : os( dest ),
                                                           types( addTypes ) {
    addParens.push( true );
  }

  void visit( usCons* cons ) {
    if( addParens.top() ) {
      os << "(";
    }

    if( !addParens.top() ) {
      addParens.push( true );
      cons->car()->accept( this );
      addParens.pop();
    }
    else {
      cons->car()->accept( this );
    }

    auto cdr = cons->cdr();
    if( cdr != nil ) {
      os << " ";
      addParens.push( false );
      cdr->accept( this );
      addParens.pop();
    }

    if( addParens.top() ) {
      os << ")";
    }

    if( types ) {
      os << " : cons";
    }
  }

  void visit( usSymbol* sym ) {
    os << sym->getName();
    if( types ) {
      os << " : sym";
    }
  }

  void visit( usInteger* i ) {
    os << i->getValue();
    if( types ) {
      os << " : int";
    }
  }

  void visit( usNil* ) {
    os << "()";
    if( types ) {
      os << " : nil";
    }
  }

protected:
  ostream& os;
  bool types;
  stack< bool > addParens;
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

  void visit( usNil* ) {

  }

protected:
  usObj* theEnv;
  usObj* lastResult;
};

#endif
