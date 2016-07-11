CXXFLAGS = -std=c++11 -g -Wall

default : uscheme

uscheme : uscheme.cc uschemeimpl.cc uscheme.hh usvisitor.hh
