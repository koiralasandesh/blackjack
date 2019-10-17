CXX=g++
CPPFLAGS=-I../asio-1.13.0/includ
CXXFLAGS=-Wall -g -std=c++11 

TARGETS=chat_client chat_server

all:${TARGETS}


chat_client: chat_client.cpp chat_message.hpp
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -o $@ $< -lpthread

chat_server: chat_server.cpp chat_message.hpp
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -o $@ $< -lpthread

clean:
	-rm -f ${TARGETS}
