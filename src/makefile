CXX=g++
CPPFLAGS=-I../asio-1.13.0/include
CXXFLAGS=-Wall -g -std=c++11 
GTKFLAGS = `/usr/bin/pkg-config gtkmm-3.0 --cflags --libs`

TARGETS=bjp bjd

all:${TARGETS}

blackjack: main.o mainwin.o card.o bjp.o bjd.o hand.o *.h
	${CXX} $(CXXFLAGS) ${CPPFLAGS} -o blackjack main.o mainwin.o card.o bjp.o bjd.o $(GTKFLAGS)

main.o: main.cpp *.h
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -c main.cpp $(GTKFLAGS)

mainwin.o: mainwin.cpp *.h
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -c mainwin.cpp $(GTKFLAGS)

hand.o: hand.cpp *.h
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -c hand.cpp $(GTKFLAGS)

card.o : card.cpp *.h
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -c card.cpp $(GTKFLAGS)

shoe.o : shoe.cpp *.h
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -c shoe.cpp $(GTKFLAGS)
	
player.o : player.cpp *.h
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -c player.cpp $(GTKFLAGS)
	
dealer.o : dealer.cpp *.h
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -c dealer.cpp $(GTKFLAGS)


bjp: bjp.cpp chat_message.hpp
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -o $@ $< -lpthread
bjd: bjd.cpp chat_message.hpp
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -o $@ $< -lpthread

clean:
	-rm -f ${TARGETS}
	-rm -f *gch *.o a.out ${TARGETS}
