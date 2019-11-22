#ifndef CHAT_MESSAGE_HPP
#define CHAT_MESSAGE_HPP

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "hand.h"

// these two classes are examples of sending an entire structure as part of the header
class game_state {
  public:
    bool dealer_cards_valid;
    Hand dealer_hand;
    bool player_cards_valid;
    Hand player_hand; // [players][cards]
    // note you can't use std::string or pointers
    //DEALER AND PLAYER CREDITS
    //int dealer_credits;
    //int player_credits;
};

class client_action {
  public:
    bool hit;   
    bool stand;
    bool surrender;
    bool split;
    bool doubleDown;
    bool join;
    bool name_valid;
    char name[25];
    bool bet;
    int bet_amt;
};

class chat_message { 
  public:
    enum { header_length = 4 + sizeof(client_action) + sizeof(game_state) };
    enum { max_body_length = 512 };

    chat_message() : body_length_(0) {}

    const char* data() const {
      return data_;
    }

    char* data() {
      return data_;
    }

    std::size_t length() const {
      return header_length + body_length_;
    }

    const char* body() const {
      return data_ + header_length;
    }

    char* body() {
      return data_ + header_length;
    }

    std::size_t body_length() const {
      return body_length_;
    }

    void body_length(std::size_t new_length) {
      body_length_ = new_length;
      if (body_length_ > max_body_length) {
        body_length_ = max_body_length;
      }
    }

    bool decode_header() {
      char header[header_length + 1] = "";
      std::strncat(header, data_, 4);
      body_length_ = std::atoi(header);
      
      char *p = data_ + 4; // skip the integer
      std::memcpy(&ca,p,sizeof(client_action));
      std::memcpy(&gs,p+sizeof(client_action),sizeof(game_state));
      
      if (body_length_ > max_body_length) {
        body_length_ = 0;
        return false;
      }
      return true;
    }

    void encode_header() {
      char header[4 + 1] = "";
      std::sprintf(header, "%4d", static_cast<int>(body_length_));
      std::memcpy(data_, header, 4);

      char *p = data_+4; // skip over the int we just extracted
      std::memcpy(p,&ca,sizeof(client_action));
      std::memcpy(p+sizeof(client_action),&gs,sizeof(game_state));
    }

  private:
    char data_[header_length + max_body_length];
    std::size_t body_length_;
  public:
    client_action ca;
    game_state gs;
};

#endif // CHAT_MESSAGE_HPP

/*
old make file
#test: card.h card.cpp test.cpp shoe.h shoe.cpp hand.h hand.cpp player_struct.h player_struct.cpp player.h player.cpp dealer.h dealer.cpp
#	g++ -o test test.cpp card.cpp shoe.cpp hand.cpp player.cpp dealer.cpp

#clean:
#	rm test

CXX=g++

CPPFLAGS=-I../asio-1.13.0/include
CXXFLAGS=-Wall -g -std=c++11 -O0

GTKLINKFLAGS=$(shell pkg-config --libs gtk+-2.0)
GTKCOMPILEFLAGS=$(shell pkg-config --cflags gtk+-2.0)

TARGETS=bjd bjp

all:${TARGETS}

bjp:  bjp.cpp hand.o card.o
	${CXX} ${CXXFLAGS} ${GTKCOMPILEFLAGS} ${CPPFLAGS} $< hand.o card.o -o $@  \
           ${GTKLINKFLAGS} -lpthread 

bjd: bjd.cpp chat_message.o card.o shoe.o hand.o
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -o $@ card.o shoe.o hand.o $< -lpthread

chat_message.o: chat_message.hpp hand.o
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -o $@ -c $<
  
card.o: card.cpp card.h
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -o $@ -c $<
	
shoe.o: shoe.cpp shoe.h
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -o $@ -c $<
	
hand.o: hand.cpp hand.h
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -o $@ -c $<

clean:
	-rm -f ${TARGETS} *.o
*/