#include "uscheme.hh"
#include "usVisitor.hh"
#include "environment.hh"

usObjPtr eval( usObjPtr input, environment& theEnv ) {
  usEvalVisitor evaluator( theEnv );
  input->accept( &evaluator, input );
  return evaluator.value;
}

void print( ostream& os, usObjPtr input ) {
  usPrintVisitor printer( os );
  input->accept( &printer, input );
}

void repl( ) {
  environment theEnv;
  while( true ) {
    cout << "uscheme> ";
    print( cout, eval( read( cin ), theEnv ) );
    cout << endl;
  }
}

int main( int, char** ) {
  init();
  repl();
  return 0;
}
