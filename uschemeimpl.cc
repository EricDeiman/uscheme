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

usObjPtr usCons::car() {
  return head;
}

usObjPtr usCons::cdr() {
  return tail;
}

usCons::usType usCons::getType() {
  return tUsCons;
}

void usCons::accept( usVisitor* v, usObjPtr self ) {
  v->visit( this, self );
}

// ----------------------------------------------------------------------

usSymbol::usType usSymbol::getType() {
  return tUsSymbol;
}

string usSymbol::getName() {
  return name;
}

void usSymbol::accept( usVisitor* v, usObjPtr self ) {
  v->visit( this, self );
}

// ----------------------------------------------------------------------

usInteger::usType usInteger::getType() {
  return tUsInteger;
}

int usInteger::getValue() {
  return value;
}

void usInteger::accept( usVisitor* v, usObjPtr self ) {
  v->visit( this, self );
}

// ----------------------------------------------------------------------

usNil::usType usNil::getType() {
  return tUsCons;
}

void usNil::accept( usVisitor* v, usObjPtr self ) {
  v->visit( this, self );
}

// ----------------------------------------------------------------------

usClosure::usType usClosure::getType() {
  return tUsClosure;
}

usObjPtr usClosure::apply( usConsPtr, usVisitor* ) {
  return nil;
}

void usClosure::accept( usVisitor* v, usObjPtr self ) {
  v->visit( this, self );
}

// ----------------------------------------------------------------------

usObjPtr usDefine::apply( usConsPtr args, usVisitor* v ) {
  usEvalVisitor* ev = static_cast< usEvalVisitor* >( v ); 
  // A special form doesn't have to evaluate its arguments.

  // define is looking for a symbol followed by an expression in the args
  if( args->car()->getType() == args->car()->tUsSymbol ) {
    usSymbolPtr sym = static_pointer_cast< usSymbolPtr::element_type >( args->car() );
    if( args->cdr() != nil &&
        args->cdr()->getType() == args->cdr()->tUsCons ) {
      usConsPtr cons = static_pointer_cast< usConsPtr::element_type >( args->cdr() );
      cons->car()->accept(ev, cons->car() );
      ( *myEnv.getParent() )[ sym->getName() ] = ev->value;
    }
    else {
      clog << "attempting to define a symbol to nothing";
    }
  }
  else {
    clog << "attempting to define a non-symbol";
  }

  return nil;
}

void usDefine::accept( usVisitor* v, usObjPtr self ) {
  v->visit( this, self );
}
