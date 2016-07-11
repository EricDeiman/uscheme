#ifndef SCHEME_HH
#define SCHEME_HH

#include <memory>
#include <string>

using namespace std;

class usVisitor;
class usObj;

typedef shared_ptr< usObj > usObjPtr;
typedef shared_ptr< usVisitor > usVisitorPtr;

class usObj {
public:
  virtual ~usObj() = 0;
  virtual void accept( usVisitor* ) = 0;
  virtual string prn() = 0;
  short gcMark;
};

class usCons : public usObj {
public:
  usCons( usObjPtr car, usObjPtr cdr ) : head( car ), tail( cdr ) {}

  usObjPtr car() {
    return head;
  }

  usObjPtr cdr() {
    return tail;
  }

  string prn() {
    return "(cons " + head->prn() + " " + tail->prn() + ")";
  }

  void accept( usVisitor* v );

protected:
  usObjPtr head, tail;
};

typedef shared_ptr< usCons > usConsPtr;

class usSymbol : public usObj {
public:
  usSymbol( string val ) : name( val ) {}

  void accept( usVisitor* v );

  string prn() {
    return name;
  }

protected:
  string name;
};

typedef shared_ptr< usSymbol > usSymbolPtr;

class usInteger : public usObj {
public:
  usInteger( int i  ) : value( i ) {}

  void accept( usVisitor* v );

  string prn() {
    return to_string( value );
  }

protected:
  int value;
};

typedef shared_ptr< usInteger > usIntegerPtr;

class usAddOp : public usObj {
public:

  void accept( usVisitor* v );

  string prn() {
    return "+";
  }
};

typedef shared_ptr< usAddOp > usAddOpPtr;

class usClosure : public usObj {
  
};

#endif
