#ifndef ENVIRONMENT_HH
#define ENVIRONMENT_HH

#include <unordered_map>
#include <memory>

#include "uscheme.hh"

// A hash table  will create dynamic environments, which I do not want.
// Some kind of tree will be needed to allow sharing to work statically.

typedef unordered_map< string, usObjPtr > store;

class environment {
public:
  environment( environment* parent = NULL ) : outter( parent ) {}

  usObjPtr& operator[]( const string& k ) {
    if( data.count( k ) == 0 && outter && outter->count( k ) > 0 ) {
      return outter->operator[]( k );
    }
    return data[ k ];
  }

  size_t count( const string& k ) {
    if( data.count( k ) == 0 && outter ) {
      return outter->count( k );
    }
    return data.count( k );
  }

  bool insert( string key, usObjPtr val ) {
    return insert( pair< string, usObjPtr >( key, val ) );
  }

  bool insert( pair< string, usObjPtr > val ) {
    auto res = data.insert( val );
    return res.second;
  }

protected:
  environment* outter;
  store data;
};

#endif
