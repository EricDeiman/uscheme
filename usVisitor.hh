#ifndef USVISITOR_HH
#define USVISITOR_HH

#include <iostream>
#include <stack>

#include "uscheme.hh"
#include "environment.hh"

// ======================================================================

class usVisitor {
public:
  virtual void visit( usCons* ) = 0;
  virtual void visit( usSymbol* ) = 0;
  virtual void visit( usInteger* ) = 0;
  virtual void visit( usNil* ) = 0;
};

// ----------------------------------------------------------------------

class usPrintVisitor : public usVisitor {
public:
  usPrintVisitor( ostream& dest, bool addTypes = false ) : os( dest ),
                                                           types( addTypes ) {
    addParens.push( true );
  }

  void visit( usCons* cons ) {
    if( addParens.top() ) {
      os << "(";
    }

    if( !addParens.top() ) {
      addParens.push( true );
      cons->car()->accept( this );
      addParens.pop();
    }
    else {
      cons->car()->accept( this );
    }

    auto cdr = cons->cdr();
    if( cdr != nil ) {
      os << " ";
      addParens.push( false );
      cdr->accept( this );
      addParens.pop();
    }

    if( addParens.top() ) {
      os << ")";
    }

    if( types ) {
      os << " : cons";
    }
  }

  void visit( usSymbol* sym ) {
    os << sym->getName();
    if( types ) {
      os << " : sym";
    }
  }

  void visit( usInteger* i ) {
    os << i->getValue();
    if( types ) {
      os << " : int";
    }
  }

  void visit( usNil* ) {
    os << "()";
    if( types ) {
      os << " : nil";
    }
  }

protected:
  ostream& os;
  bool types;
  stack< bool > addParens;
};

// ----------------------------------------------------------------------

class usEvalVisitor : public usVisitor {
public:

  usEvalVisitor( environment& env ) : theEnv( env ) {}

  /*
   * Evaluating a cons cell means calling a function.
   */
  void visit( usCons* c ) {
    c->car()->accept( this );
    if( value->isSymbol() ) {
      usSymbol* sym = static_cast< usSymbol* >( value );
      // For now, the only symbol we handle is 'define'.
      // In the future, an environment lookup will go here, and the value will need
      // to be a closure.
      if( sym->getName() == "define" ) {
        if( c->cdr()->isCons() ) {
          usCons* next = static_cast< usCons* >( c->cdr() );
          if( next->car()->isSymbol() ) {
            usSymbol* nameSymbol = static_cast< usSymbol* >( next->car() );
            string name = nameSymbol->getName();
            if( next->cdr()->isCons() ) {
              next = static_cast< usCons* >( next->cdr() );
              next->car()->accept( this );
              theEnv[ name ] = shared_ptr< usObj >( value );
            }
            else {
              clog << "Trying to define something other than a value in the last "
                   << "position";
            }
          }
          else {
            clog << "Trying to evaluate define with something other than a symbol "
                 << "in the second position";
          }
        }
        else {
          clog << "Trying to evaluate a define with something other than a cons in the "
               << "second position";
        }
      }
      else {
        clog << "Trying to evaluate a list with something other than 'define' in the "
             << "fn position";
      }
    }
    else {
      clog << "Trying to evaluate a list with a non-symbol in the fn position";
    }
    value = nil;
  }

  void visit( usSymbol* k ) {
    if( theEnv.count( k->getName() ) ) {
      value = theEnv[ k->getName() ].get();
    }
    else {
      value = k;
    }
  }

  void visit( usInteger* i ) {
    value = i;
  }

  void visit( usNil* ) {
  }

  usObj* value;

protected:
  environment& theEnv;
};

#endif
