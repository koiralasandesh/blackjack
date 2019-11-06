#include "mainwin.h"
#include "player_choice.h"

//#include "card.h"
#include "rules.h"
#include "bet.h"
#include <iostream>

using namespace std;

#include <gtkmm.h>


int main(int argc, char** argv) {

    Gtk::Main app(argc, argv);
    Mainwin_window mainwin;
    Gtk::Main::run(mainwin);  

    return 0;

}


//make file to run Himal code GUI
/*
CXX = g++
CXXFLAGS += -std=c++11
GTKFLAGS = `/usr/bin/pkg-config gtkmm-3.0 --cflags --libs`

ex: main.o mainwin.o player_choice.o rules.o bet.o
	$(CXX) $(CXXFLAGS) -o run main.o mainwin.o player_choice.o rules.o bet.o $(GTKFLAGS)
main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp $(GTKFLAGS)
mainwin.o: mainwin.h mainwin.cpp
	$(CXX) $(CXXFLAGS) -c mainwin.cpp $(GTKFLAGS)
player_choice.o: player_choice.h player_choice.cpp
	$(CXX) $(CXXFLAGS) -c player_choice.cpp $(GTKFLAGS)
rules.o: rules.h rules.cpp
	$(CXX) $(CXXFLAGS) -c rules.cpp $(GTKFLAGS)	
bet.o: bet.h bet.cpp
	$(CXX) $(CXXFLAGS) -c bet.cpp $(GTKFLAGS)
*/