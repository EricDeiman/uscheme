CXXFLAGS = -std=c++11 -g -Wall

.DEFAULT_GOAL := uscheme

.PHONY : clean

clean :
	rm -f uscheme
	rm -f core

uscheme : uscheme.cc uschemeimpl.cc uscheme.hh usVisitor.hh
