CXX=g++
CPPFLAGS=-I../asio-1.13.0/include
CXXFLAGS=-Wall -g -std=c++11 
GTKFLAGS = `/usr/bin/pkg-config gtkmm-3.0 --cflags --libs`

TARGETS=bjp bjd

#lines 8 & 9 possibly unecessary
debug: CXXFLAGS+= -g
debug: default

all:${TARGETS}

blackjack: main.o mainwin.o card.o bjp.o bjd.o hand.o *.h
	${CXX} $(CXXFLAGS) ${CPPFLAGS} -o blackjack main.o mainwin.o card.o bjp.o bjd.o $(GTKFLAGS)
	#need to add all object files for blackjack

main.o: main.cpp *.h
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -c main.cpp $(GTKFLAGS)

mainwin.o: mainwin.cpp *.h
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -c mainwin.cpp $(GTKFLAGS)

hand.o: hand.cpp *.h
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -c hand.cpp $(GTKFLAGS)

card.o : card.cpp *.h
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -c card.cpp $(GTKFLAGS)


bjp: bjp.cpp chat_message.hpp
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -o $@ $< -lpthread

bjd: bjd.cpp chat_message.hpp
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -o $@ $< -lpthread

clean:
	-rm -f *gch *.o a.out ${TARGETS}
