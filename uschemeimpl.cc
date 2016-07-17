#include "uscheme.hh"
#include "usVisitor.hh"

usObjPtr nil;

// ======================================================================

void init() {
  nil = make_shared< usNil >();
}

usSymbolPtr readSymbol( istream& input ) {
  // A symbol ends at a white-space, ')', or '.'
  string symbol;

  while( input && !isspace( input.peek() ) && input.peek() != ')' &&
         input.peek() != '.' && input.peek() != istream::traits_type::eof() ) {
    symbol.append( 1, input.get() );
  }

  return make_shared< usSymbol >( symbol );
} 

// ----------------------------------------------------------------------

usObjPtr readSequence( stringstream& input ) {
  // read the list inside parens
  ws( input );

  while( input && input.peek() != ')' ) {
    usObjPtr first = read( input );
    usObjPtr second = readSequence( input );
    return make_shared< usCons >( first, second );
  }
  return nil;
}

// ----------------------------------------------------------------------

// Read one item (list, symbol, or integer) from the input
usObjPtr read( stringstream& input ) {

  ws( input );
  int nextChar = input.peek();
  switch( nextChar ) {
  case '(': {
    // cons
    input.ignore(); // eat the (
    usObjPtr first = read( input );
    usObjPtr second = readSequence( input );
    ws( input );
    int next = input.peek();
    if( next == ')' ) {
      input.ignore();
    }
    else {
      // we've got a problem; unclosed list
      clog << "Huston, we have a problem ";
      clog << "the next character is " << next << endl;
    }
    return make_shared< usCons >( first, second );
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
    return make_shared< usInteger >( i );
    break;
  default:
    // symbol.  a symbol ends with a whitespace or ')'
    return readSymbol( input );
    break;
  }

  return NULL;
}

// ----------------------------------------------------------------------

usObjPtr read( istream& is ) {
  stringbuf sb;
  is.get( sb );
  is.ignore(); // eat the \n
  stringstream instream( sb.str() );
  return read( instream );
}

// ======================================================================

void usCons::accept( usVisitor* v, usObjPtr self ) {
  v->visit( this, self );
}

void usSymbol::accept( usVisitor* v, usObjPtr self ) {
  v->visit( this, self );
}

void usInteger::accept( usVisitor* v, usObjPtr self ) {
  v->visit( this, self );
}

void usNil::accept( usVisitor* v, usObjPtr self ) {
  v->visit( this, self );
}
