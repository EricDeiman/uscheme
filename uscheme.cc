#include "uscheme.hh"
#include "usVisitor.hh"
#include "environment.hh"

usObj* eval( usObj* input, environment& theEnv ) {
  usEvalVisitor evaluator( theEnv );
  input->accept( &evaluator );
  // delete input  // is this safe?
  return evaluator.value;
}

void print( ostream& os, usObj* input ) {
  usPrintVisitor printer( os );
  input->accept( &printer );
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
