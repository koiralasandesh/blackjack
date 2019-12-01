#ifndef CARD_H__
#define CARD_H__

typedef enum {NO_SUIT, SPADE, CLUB, DIAMOND, HEART} Suit;
typedef enum {NO_FACE, ACE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING} Face;

class Card {	// every card initialized to {-1, NO_SUIT, NO_FACE}
	private:
		int _value = -1; 			// initially only 1-10
		Suit _suit = NO_SUIT;		// SPADE = 0, CLUB = 1, etc.
		Face _face = NO_FACE;
		bool _is_ace = false;
	public:
		Card();
		int get_value();
		void set_value(int value);
		int get_suit();
		void set_suit(Suit suit);
		int get_face();		// i called it face, but actually means the card's name
		void set_face(Face face);
		void is_ace();		// returns true if card face is ace
		//void ace_to_1();	// changes ace value to 1. called when hand total value exceeds 21 when ace = 11.
};

#endif
