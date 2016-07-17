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
  virtual ~usObj() {}
  virtual void accept( usVisitor* ) = 0;
  short gcMark;
  friend class usVisitor;
};

// ----------------------------------------------------------------------

class usCons : public usObj {
public:
  usCons( usObj* car, usObj* cdr ) : head( car ), tail( cdr ) {}

  usObj* car() {
    return head;
  }

  usObj* cdr() {
    return tail;
  }

  void accept( usVisitor* v );

protected:
  usObj* head, *tail;
};

// ----------------------------------------------------------------------

class usSymbol : public usObj {
public:
  usSymbol( string val ) : name( val ) {}

  void accept( usVisitor* v );

  string getName() {
    return name;
  }

protected:
  string name;
};

// ----------------------------------------------------------------------

class usInteger : public usObj {
public:
  usInteger( int i  ) : value( i ) {}

  void accept( usVisitor* v );

  int getValue() {
    return value;
  }

protected:
  int value;
};

// ----------------------------------------------------------------------

class usNil : public usObj {
public:
  void accept( usVisitor* v );
};

extern usNil* nil;

// ======================================================================

void init();

usObj* read( istream& );
usObj* read( stringstream& );
usSymbol* readSymbol( istream& input );
usObj* readSequence( istream& input );

#endif
