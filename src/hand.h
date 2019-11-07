#include "card.h"
#include <vector>

class Hand {
	private:
		std::vector<Card> _hand;
		std::vector<Card> _split_hand;
		int _total_value;
	public:
		std::vector<Card> get_hand();
		std::vector<Card> get_split_hand();
		void reset_hands();
		Card get_card(int index);
		void add_card_hand(Card card);
		int get_total_value();		// ADD: count split hand cards value
		void set_total_value();
		int number_of_cards();
		bool can_split();			// returns true if 1st 2 cards are identical
		//void split_cards();		// WIP - move second card to _split_hand
};

// maybe need method to display/return hand's contents/cards?

// written: Rebecca B 11/4

