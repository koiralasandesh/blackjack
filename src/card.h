typedef enum {SPADE, CLUB, DIAMOND, HEART} Suit;
typedef enum {ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING} Face;

class Card {
	private:
		int _value; 	// initially only 1-10
		Suit _suit;		// SPADE = 0, CLUB = 1, etc.
		Face _face;	
	public:
		Card(int value, Suit suit, Face face);
		int value();		// value of card, face cards set to = 10
		int suit();
		int face();			// called face, but actually means the card's name
		bool is_ace();		// returns true if card face is ace
		void ace_to_11();	// called when hand value will not exceed 21
							// may be simpler to set ace initial value = 11?
};

// written: Rebecca B 11/2

