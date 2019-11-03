typedef enum {SPADE, CLUB, DIAMOND, HEART} Suit;
typedef enum {ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING} Face;

class Card {
	private:
		int _value; // 1 - 11, face cards are all 10
		Suit _suit;
		Face _face;
	public:
		Card(int value, Suit suit, Face face);
		int value();
		int suit();
		int face();
		bool is_ace();
		void ace_to_11();
};

