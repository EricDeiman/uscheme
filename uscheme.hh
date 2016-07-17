#ifndef USCHEME_HH
#define USCHEME_HH

#include <cctype>
#include <limits>
#include <memory>
#include <sstream>
#include <string>

using namespace std;

class usVisitor;

// ======================================================================

class usObj {
public:
  enum usType { tUsNil, tUsCons, tUsInteger, tUsSymbol };

  virtual ~usObj() {}
  virtual void accept( usVisitor*, shared_ptr< usObj > ) = 0;
  virtual usType getType() = 0;
  virtual bool isSymbol() { return false; }
  virtual bool isCons() { return false; }
  short gcMark;
  friend class usVisitor;
};

typedef shared_ptr< usObj > usObjPtr;

extern usObjPtr nil;

// ----------------------------------------------------------------------

class usCons : public usObj {
public:
  usCons( usObjPtr car, usObjPtr cdr ) : head( car ), tail( cdr ) {}

  usObjPtr car() {
    return head;
  }

  usObjPtr cdr() {
    return tail;
  }

  void accept( usVisitor* v, usObjPtr self );

  usType getType() {
    return tUsCons;
  }

  bool isCons() {
    return true;
  }

protected:
  usObjPtr head, tail;
};

typedef shared_ptr< usCons > usConsPtr;

// ----------------------------------------------------------------------

class usSymbol : public usObj {
public:
  usSymbol( string val ) : name( val ) {}

  void accept( usVisitor* v, usObjPtr self );

  string getName() {
    return name;
  }

  usType getType() {
    return tUsSymbol;
  }

  bool isSymbol() {
    return true;
  }

protected:
  string name;
};

typedef shared_ptr< usSymbol > usSymbolPtr;

// ----------------------------------------------------------------------

class usInteger : public usObj {
public:
  usInteger( int i  ) : value( i ) {}

  void accept( usVisitor* v, shared_ptr< usObj > self );

  usType getType() {
    return tUsInteger;
  }

  int getValue() {
    return value;
  }

protected:
  int value;
};

typedef shared_ptr< usInteger > usIntegerPtr;

// ----------------------------------------------------------------------

class usNil : public usObj {
public:
  usType getType() {
    return tUsNil;
  }

  void accept( usVisitor* v, usObjPtr self );
};

typedef shared_ptr< usNil > usNilPtr;

// ======================================================================

void init();

usObjPtr read( istream& );
usObjPtr read( stringstream& );
usSymbolPtr readSymbol( istream& input );
usObjPtr readSequence( istream& input );

#endif
