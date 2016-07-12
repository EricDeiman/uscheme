#ifndef SCHEME_HH
#define SCHEME_HH

#include <cctype>
#include <limits>
#include <memory>
#include <sstream>
#include <string>

using namespace std;

class usVisitor;

class usNil;

usNil* nil;

// ======================================================================

class usObj {
public:
  virtual ~usObj() = 0;
  virtual void accept( usVisitor* ) = 0;
  short gcMark;
  friend class usVisitor;
  bool isCons() {
    return false;
  }
  bool isNil() {
    return false;
  }
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

  bool isCons() {
    return true;
  }

protected:
  usObj* head, *tail;
};

// ----------------------------------------------------------------------

class usSymbol : public usObj {
public:
  usSymbol( string val ) : name( val ) {}

  void accept( usVisitor* v );

protected:
  string name;
};

// ----------------------------------------------------------------------

class usInteger : public usObj {
public:
  usInteger( int i  ) : value( i ) {}

  void accept( usVisitor* v );

protected:
  int value;
};

// ----------------------------------------------------------------------

class usNil : public usObj {
public:
  void accept( usVisitor* v );

  bool isCons() {
    return true;
  }

  bool isNil() {
    return true;
  }
};

// ======================================================================

usObj* read( istream& );

usSymbol* readSymbol( istream& input ) {
  // A symbol ends at a white-space, ')', or '.'
  string symbol;

  while( !isspace( input.peek() ) && input.peek() != ')' && input.peek() != '.' ) {
    symbol.append( 1, input.get() );
  }

  return new usSymbol( symbol );
}

// ----------------------------------------------------------------------

usObj* readSequence( istream& input ) {
  // read the list inside parens
  ws( input );
  while( input && input.peek() != ')' ) {
    usObj* first = read( input );
    usObj* second = readSequence( input );
    if( second ) {
      return new usCons( first, second );
    }
    else {
      return first;
    }
  }
  return nil;
}

// ----------------------------------------------------------------------

// Read one item (list, symbol, or integer) from the input
usObj* read( stringstream& input ) {

    ws( input );
    int nextChar = input.peek();
    switch( nextChar ) {
    case '(': {
      // cons
      input.ignore(); // eat the (
      usObj* first = read( input );
      usObj* second = readSequence( input );
      ws( input );
      int next = input.peek();
      if( next == ')' ) {
        input.ignore();
      }
      else {
        // we've got a problem; unclosed list
      }
      return new usCons( first, second );
    }
      break;
    case ')':
      return NULL;
      break;
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
      // number
      int i;
      input >> i;
      return new usInteger( i );
      break;
    default:
      // symbol.  a symbol ends with a whitespace or ')'
      return readSymbol( input );
      break;
    }

    return NULL;
}

// ----------------------------------------------------------------------

usObj* read( istream& is ) {
  stringbuf sb;
  is.get( sb );
  stringstream instream( sb.str() );
  return read( instream );
}

#endif
