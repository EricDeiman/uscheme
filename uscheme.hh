#ifndef USCHEME_HH
#define USCHEME_HH

#include <cctype>
#include <iostream>
#include <limits>
#include <memory>
#include <sstream>
#include <string>

using namespace std;

#include "uschemecore.hh"
#include "environment.hh"

extern usObjPtr nil;

// ----------------------------------------------------------------------

class usCons : public usObj {
public:
  usCons( usObjPtr car, usObjPtr cdr ) : head( car ), tail( cdr ) {}

  usObjPtr car();
  usObjPtr cdr();
  void accept( usVisitor* v, usObjPtr self );
  usType getType();

protected:
  usObjPtr head, tail;
};

typedef shared_ptr< usCons > usConsPtr;

// ----------------------------------------------------------------------

class usSymbol : public usObj {
public:
  usSymbol( string val ) : name( val ) {}

  void accept( usVisitor* v, usObjPtr self );
  string getName();
  usType getType();

protected:
  string name;
};

typedef shared_ptr< usSymbol > usSymbolPtr;

// ----------------------------------------------------------------------

class usInteger : public usObj {
public:
  usInteger( int i  ) : value( i ) {}

  void accept( usVisitor* v, usObjPtr self );
  usType getType();
  int getValue();

protected:
  int value;
};

typedef shared_ptr< usInteger > usIntegerPtr;

// ----------------------------------------------------------------------

class usNil : public usObj {
public:
  usType getType();
  void accept( usVisitor* v, usObjPtr self );
};

typedef shared_ptr< usNil > usNilPtr;

// ----------------------------------------------------------------------

class usClosure : public usObj {
public:
  usClosure( environment< usObjPtr >* parent, usObjPtr& params, usObjPtr& body ) :
    myEnv( parent ), paramList( params ), bodyExp( body ) { }

  void accept( usVisitor* v, usObjPtr self );
  usType getType();
  virtual usObjPtr apply( usConsPtr, usVisitor* );

protected:
  environment< usObjPtr > myEnv;
  usObjPtr& paramList;
  usObjPtr& bodyExp;
};

typedef shared_ptr< usClosure > usClosurePtr;

// ......................................................................

// class usSpecialForm : public usClosure {
class usDefine : public usClosure {
public:
  usDefine( environment< usObjPtr >* parent, usObjPtr& params, usObjPtr& body ) :
    usClosure( parent, params, body ) {}

  void accept( usVisitor* v, usObjPtr self );
  usObjPtr apply( usConsPtr args, usVisitor* v );
};

typedef shared_ptr< usDefine > usDefinePtr;

// ======================================================================

void init();

usObjPtr read( istream& );
usObjPtr read( stringstream& );
usSymbolPtr readSymbol( istream& input );
usObjPtr readSequence( istream& input );

#endif
