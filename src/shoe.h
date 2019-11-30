#ifndef SHOE_H__
#define SHOE_H__

#include "card.h"

// TO BE INSTANCED AND USED ONLY BY DEALER

class Shoe {
	public:
		Card _shoe[312]; 			// max 312 cards in 6 decks
		int _current_card = 311;	// index for shoe
	//public:
		virtual ~Shoe();
		Card* get_shoe();		// shoe getter
		void create_cards();	// generates 6 decks
		Card next_card();		// returns last card in shoe
		void remove_card();		// clears last card in shoe[current]. removed cards return "null" value.
		void shuffle();			// randomize shoe
		void print_cards();		// testing purposes only
};

#endif
