#ifndef USVISITOR_HH
#define USVISITOR_HH

#include <iostream>
#include <stack>

#include "uscheme.hh"
#include "environment.hh"

// ======================================================================

class usVisitor {
public:
  virtual void visit( usCons*, usObjPtr ) = 0;
  virtual void visit( usSymbol*, usObjPtr ) = 0;
  virtual void visit( usInteger*, usObjPtr ) = 0;
  virtual void visit( usNil*, usObjPtr ) = 0;
};

// ----------------------------------------------------------------------

class usPrintVisitor : public usVisitor {
public:
  usPrintVisitor( ostream& dest, bool addTypes = false ) : os( dest ),
                                                           types( addTypes ) {
    addParens.push( true );
  }

  void visit( usCons* cons, usObjPtr ) {
    if( addParens.top() ) {
      os << "(";
    }

    if( !addParens.top() ) {
      addParens.push( true );
      cons->car()->accept( this, cons->car() );
      addParens.pop();
    }
    else {
      cons->car()->accept( this, cons->car() );
    }

    auto cdr = cons->cdr();
    if( cdr != nil ) {
      os << " ";
      addParens.push( false );
      cdr->accept( this, cdr );
      addParens.pop();
    }

    if( addParens.top() ) {
      os << ")";
    }

    if( types ) {
      os << " : cons";
    }
  }

  void visit( usSymbol* sym, usObjPtr ) {
    os << sym->getName();
    if( types ) {
      os << " : sym";
    }
  }

  void visit( usInteger* i, usObjPtr ) {
    os << i->getValue();
    if( types ) {
      os << " : int";
    }
  }

  void visit( usNil*, usObjPtr ) {
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
  void visit( usCons* c, usObjPtr ) {
    c->car()->accept( this, c->car() );
    if( value->isSymbol() ) {
      usSymbolPtr sym = static_pointer_cast< usSymbolPtr::element_type >( value );
      // For now, the only symbol we handle is 'define'.
      // In the future, an environment lookup will go here, and the value will need
      // to be a closure.
      if( sym->getName() == "define" ) {
        if( c->cdr()->isCons() ) {
          usConsPtr next = static_pointer_cast< usConsPtr::element_type >( c->cdr() );
          if( next->car()->isSymbol() ) {
            usSymbolPtr nameSymbol = static_pointer_cast<
              usSymbolPtr::element_type >( next->car() );
            string name = nameSymbol->getName();
            if( next->cdr()->isCons() ) {
              next = static_pointer_cast< usConsPtr::element_type >( next->cdr() );
              next->car()->accept( this, next->car() );
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

  void visit( usSymbol* k, usObjPtr self ) {
    if( theEnv.count( k->getName() ) ) {
      value = theEnv[ k->getName() ];
    }
    else {
      value = self;
    }
  }

  void visit( usInteger*, usObjPtr self ) {
    value = self;
  }

  void visit( usNil*, usObjPtr ) {
  }

  usObjPtr value;

protected:
  environment& theEnv;
};

#endif
