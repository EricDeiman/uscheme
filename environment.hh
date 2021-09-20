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
