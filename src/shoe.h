#include <vector>
#include "card.h"

// TO BE INSTANCED ONLY ONCE BY DEALER

class Shoe {
	public:
		//std::vector<Card> _shoe;
		Card _shoe[312]; 			// max 312 cards in 6 decks
		int _current_card = 311;	// index for shoe
	//public:
		//std::vector<Card> get_shoe();
		virtual ~Shoe();
		Card* get_shoe();		// shoe getter
		void create_cards();	// generates 6 decks
		Card next_card();		// returns last card in shoe
		void remove_card();		// clears last card in shoe[current]. removed cards return "null" value.
		void shuffle();			// randomize shoe
		void print_cards();		// testing purposes only
};

// written: Rebecca B 11/2

