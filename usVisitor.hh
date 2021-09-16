#ifndef USVISITOR_HH
#define USVISITOR_HH

#include <iostream>
#include <stack>

using namespace std;

#include "uscheme.hh"

// ======================================================================

class usPrintVisitor : public usVisitor {
public:
  usPrintVisitor( ostream& dest, bool addTypes = false ) : os( dest ),
                                                           types( addTypes ) {
    addParens.push( true );
  }

  void visit( usCons* cons, usObjPtr );
  void visit( usSymbol* sym, usObjPtr );
  void visit( usInteger* i, usObjPtr );
  void visit( usNil*, usObjPtr );
  void visit( usClosure*, usObjPtr self );
  void visit( usDefine*, usObjPtr self );

protected:
  ostream& os;
  bool types;
  stack< bool > addParens;
};

// ----------------------------------------------------------------------

class usEvalVisitor : public usVisitor {
public:

  usEvalVisitor( environment< usObjPtr >& env ) : theEnv( env ) {}

  /*
   * Evaluating a cons cell means calling a function.
   */
  void visit( usCons* c, usObjPtr );
  void visit( usSymbol* k, usObjPtr self );
  void visit( usInteger*, usObjPtr self );
  void visit( usNil*, usObjPtr );
  void visit( usClosure*, usObjPtr self );
  void visit( usDefine*, usObjPtr self );
  usObjPtr value;

protected:
  environment< usObjPtr >& theEnv;
};

// ----------------------------------------------------------------------

class notImplementedException : public exception {
public:
  notImplementedException( string m ) : message( m ) {}

  virtual const char* what() const throw() {
    string msg = "Not implemented ";
    msg.assign( message );
    return msg.c_str();
  }

protected:
  string message;
};

class usApplyVisitor : public usVisitor {
public:

  void visit( usCons* , usObjPtr ) {
    auto ex = notImplementedException("apply cons");
    throw ex;
  }

  void visit( usSymbol* , usObjPtr ) {
    auto ex = notImplementedException("apply symbol");
    throw ex;
  }

  void visit( usInteger*, usObjPtr ) {
    auto ex = notImplementedException("apply integer");
    throw ex;
  }

  void visit( usNil*, usObjPtr ) {
    auto ex = notImplementedException("apply nil");
    throw ex;
  }

  void visit( usClosure*, usObjPtr ) {
    auto ex = notImplementedException("apply closure");
    throw ex;
  }
  
  void visit( usDefine*, usObjPtr self );

protected:

};

#endif
