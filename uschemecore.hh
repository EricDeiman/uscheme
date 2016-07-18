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
