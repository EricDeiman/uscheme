#ifndef ENVIRONMENT_HH
#define ENVIRONMENT_HH

#include <unordered_map>
#include <string>

using namespace std;

template < typename T >
class environment {
public:

  typedef unordered_map< string, T > store;

  environment( environment* parent = NULL ) : outter( parent ) {}

  T& operator[]( const string& k ) {
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

  bool insert( string key, T val )  {
    return insert( pair< string, T >( key, val ) );
  }

  bool insert( pair< string, T > val )  {
    auto res = data.insert( val );
    return res.second;
  }

  environment* getParent() {
    return outter;
  }

protected:
  environment* outter;
  store data;
};



#endif
