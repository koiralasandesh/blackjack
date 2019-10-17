CXX=g++
CPPFLAGS=-I../asio-1.13.0/includ
CXXFLAGS=-Wall -g -std=c++11 

TARGETS=bjp bjd

all:${TARGETS}


bjp: bjp.cpp chat_message.hpp
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -o $@ $< -lpthread

bjd: bjd.cpp chat_message.hpp
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -o $@ $< -lpthread

clean:
	-rm -f ${TARGETS}
