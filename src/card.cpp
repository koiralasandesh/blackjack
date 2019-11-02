#include "card.h"

Card::Card(int value, Suit suit, Face face) : _value{value}, _suit{suit}, _face{face} { }

bool Card::is_ace() {
	if(_face == ACE) {
		return true;
	}
	else {
		return false;
	}
}

