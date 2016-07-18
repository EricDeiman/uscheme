#include "uscheme.hh"
#include "usVisitor.hh"

void init( environment< usObjPtr >& theEnv ) {
  theEnv.insert("define", make_shared< usDefine >( &theEnv, nil, nil ) );
  
}

usObjPtr eval( usObjPtr input, environment< usObjPtr >& theEnv ) {
  usEvalVisitor evaluator( theEnv );
  input->accept( &evaluator, input );
  return evaluator.value;
}

void print( ostream& os, usObjPtr input ) {
  usPrintVisitor printer( os );
  input->accept( &printer, input );
}

void repl( ) {
  environment< usObjPtr > theEnv;
  init( theEnv );

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
