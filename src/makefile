CXX=g++

CPPFLAGS=-I../asio-1.13.0/include
CXXFLAGS=-Wall -g -std=c++11 -O0

GTKLINKFLAGS=$(shell pkg-config --libs gtk+-2.0)
GTKCOMPILEFLAGS=$(shell pkg-config --cflags gtk+-2.0)

TARGETS=bjd bjp unit_test1 unit_test2

all:${TARGETS}

bjp: bjp.cpp hand.o card.o
	${CXX} ${CXXFLAGS} ${GTKCOMPILEFLAGS} ${CPPFLAGS} $< hand.o card.o -o $@  \
           ${GTKLINKFLAGS} -lpthread 

bjd: bjd.cpp chat_message.o card.o shoe.o hand.o player.o dealer.o
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -o $@ card.o shoe.o hand.o player.o dealer.o $< -lpthread
	
unit_test1: unit_test1.cpp card.o shoe.o dealer.o
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -o $@ card.o shoe.o dealer.o $< -lpthread

unit_test2: unit_test2.cpp card.o shoe.o hand.o dealer.o
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -o $@ card.o shoe.o hand.o dealer.o $< -lpthread

chat_message.o: chat_message.hpp hand.o
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -o $@ -c $<

dealer.o: dealer.cpp
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -o $@ -c $<

player.o: player.cpp
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -o $@ -c $<

card.o: card.cpp card.h
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -o $@ -c $<
	
shoe.o: shoe.cpp shoe.h
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -o $@ -c $<
	
hand.o: hand.cpp hand.h
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -o $@ -c $<

clean:
	-rm -f ${TARGETS} *.o
