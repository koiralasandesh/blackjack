#ifndef HAND_H__
#define HAND_H__

#include "card.h"

class Hand {
	private:
		Card _hand[13];			// can't possibly have more than 13 cards w/o winning/busting
		Card _split_hand[13];
		int _hand_value = 0;
		int _split_value = 0;
		int _hand_index = 0;	// index values will be the index of the next empty card spot
		int _split_index = 0;								// eg. hand w/ 2 cards will have index = 2
		bool _hand_has_ace = false;
		bool _split_has_ace = false;
		int ace_count = 0;
		int split_ace_count = 0;
		
	public:
		Card* get_hand(int which_hand);					// index for card 1 should be 0, etc.
		Card get_card(int index, int which_hand);		// which_hand = indexing to halve # of methods
		void add_card_hand(Card card, int which_hand);	// 0 = primary hand, 1 = split hand
		int get_hand_value(int which_hand);
		void set_hand_value(int which_hand);
		bool can_split();								// returns true if 1st 2 cards are identical
		void reset_hands();								// resets BOTH hands. only used at end of rounds
		void split_cards();		// WIP - move second card to _split_hand
		int get_hand_index(int which_hand);
};

#endif

// maybe need method to display/return hand's contents/cards?

// written: Rebecca B 11/4

