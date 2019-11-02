#pragma once

typedef enum {SPADE, CLUB, DIAMOND, HEART} Suit;
typedef enum {ACE = 1, JACK = 11, QUEEN, KING} Face;

class Card {
	private:
		int _value; // 1 - 11, face cards are all 10
		Suite _suit;
		Face _face;
	public:
		Card(int value, Suite suit, Face face);
		bool is_ace();

