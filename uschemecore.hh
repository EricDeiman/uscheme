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

#ifndef USCHEMECORE_HH
#define USCHEMECORE_HH

#include <memory>
#include <unordered_map>

using namespace std;

class usVisitor;

class usObj {
public:
  // This type stuff is temporary.  I hope.
  enum usType { tUsNil, tUsCons, tUsInteger, tUsSymbol, tUsClosure };

  virtual ~usObj() {}
  virtual void accept( usVisitor*, shared_ptr< usObj > ) = 0;
  virtual usType getType() = 0;
  short gcMark;
};

typedef shared_ptr< usObj > usObjPtr;

class usCons;
class usSymbol;
class usInteger;
class usNil;
class usClosure;
class usDefine;

class usVisitor {
public:
  virtual void visit( usCons*, usObjPtr ) = 0;
  virtual void visit( usSymbol*, usObjPtr ) = 0;
  virtual void visit( usInteger*, usObjPtr ) = 0;
  virtual void visit( usNil*, usObjPtr ) = 0;
  virtual void visit( usClosure*, usObjPtr ) = 0;
  virtual void visit( usDefine*, usObjPtr ) = 0;
};

#endif
