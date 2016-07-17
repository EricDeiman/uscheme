#ifndef ENVIRONMENT_HH
#define ENVIRONMENT_HH

#include <unordered_map>
#include <memory>

#include "uscheme.hh"

typedef unordered_map< string, shared_ptr< usObj > > store;

class environment {
public:
  environment( environment* parent = NULL ) : outter( parent ) {}

  shared_ptr< usObj >& operator[]( const string& k ) {
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

  bool insert( pair< string, shared_ptr< usObj > > val ) {
    auto res = data.insert( val );
    return res.second;
  }

protected:
  environment* outter;
  store data;
};

#endif
