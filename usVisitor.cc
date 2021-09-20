/*
  The uScheme programming language
  Copyright 2016 Eric J. Deiman

  This file is part of the uScheme programming language.
  The uScheme programming language is free software: you can redistribute it
  and/ormodify it under the terms of the GNU General Public License as published by the
  Free Software Foundation, either version 3 of the License, or (at your option) any
  later version.
  
  The uScheme programming language is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
  You should have received a copy of the GNU General Public License along with the
  uScheme programming language. If not, see <https://www.gnu.org/licenses/>
*/

#include "usVisitor.hh"

void usPrintVisitor::visit(usCons *cons, usObjPtr)  {
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

void usPrintVisitor::visit(usSymbol *sym, usObjPtr)  {
  os << sym->getName();
  if( types ) {
    os << " : sym";
  }
}

void usPrintVisitor::visit(usInteger *i, usObjPtr)  {
  os << i->getValue();
  if( types ) {
    os << " : int";
  }
}

void usPrintVisitor::visit(usNil *, usObjPtr)  {
  os << "()";
  if( types ) {
    os << " : nil";
  }
}

void usPrintVisitor::visit(usClosure *, usObjPtr self)  {
  os << "#<closure " << hex << &self << ">";
}

void usPrintVisitor::visit(usDefine *, usObjPtr self)  {
  os << "#special form " << hex << &self << ">";
}

// ----------------------------------------------------------------------

void usEvalVisitor::visit(usCons *c, usObjPtr)  {
  c->car()->accept( this, c->car() );
  if( value->getType() == value->tUsClosure ) {
    usClosurePtr closure = static_pointer_cast< usClosurePtr::element_type >( value );
    if( c->cdr()->getType() == c->cdr()->tUsCons ) {
      usConsPtr tail = static_pointer_cast< usConsPtr::element_type >( c->cdr() );
      value = closure->apply( tail, this );
    }
    else {
      clog << "Trying to call closure without arguments";
    }
  }
  else {
    clog << "Trying to evaluate a list with a non-closure in the fn position";
  }
}

void usEvalVisitor::visit(usSymbol *k, usObjPtr self)  {
  if( theEnv.count( k->getName() ) ) {
    value = theEnv[ k->getName() ];
  }
  else {
    value = self;
  }
}

void usEvalVisitor::visit(usInteger *, usObjPtr self)  {
  value = self;
}

void usEvalVisitor::visit(usNil *, usObjPtr)  {
}

void usEvalVisitor::visit(usClosure *, usObjPtr self)  {
  value = self;
}

void usEvalVisitor::visit(usDefine *, usObjPtr self)  {
  value = self;
}

// ----------------------------------------------------------------------

void usApplyVisitor::visit(usDefine *, usObjPtr self) {
  
}
