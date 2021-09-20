#
#  The uScheme programming language
#  Copyright 2016 Eric J. Deiman
#
#  This file is part of the uScheme programming language.
#  The uScheme programming language is free software: you can redistribute it
#  and/ormodify it under the terms of the GNU General Public License as published by the
#  Free Software Foundation, either version 3 of the License, or (at your option) any
#  later version.
#  
#  The uScheme programming language is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
#  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
#  You should have received a copy of the GNU General Public License along with the
#  uScheme programming language. If not, see <https://www.gnu.org/licenses/>
#

CXXFLAGS = -std=c++11 -g -Wall

.DEFAULT_GOAL := uscheme

.PHONY : clean

clean :
	rm -f uscheme
	rm -f core

uscheme : uscheme.cc uschemeimpl.cc usVisitor.cc uscheme.hh usVisitor.hh \
          environment.hh usVisitor.hh
