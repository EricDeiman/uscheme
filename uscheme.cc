#include "uscheme.hh"
#include "usVisitor.hh"

usObj* eval( usObj* input ) {
  return input;
}

void print( ostream& os, usObj* input ) {
  usPrintVisitor printer( os );
  input->accept( &printer );
  os << endl;
}

void repl( ) {
  while( true ) {
    cout << "uscheme> ";
    print( cout, eval( read( cin ) ) );
  }
}

int main( int, char** ) {
  init();
  repl();
  return 0;
}
