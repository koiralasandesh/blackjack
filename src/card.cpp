#include "card.h"

Card::Card(int value, Suit suit, Face face) : _value{value}, _suit{suit}, _face{face} { }

int Card::value() { return _value; }

int Card::suit() { return _suit; }

int Card::face() { return _face; }

bool Card::is_ace() {
	if (_face == ACE) { return true; }
	else { return false; }
}

void Card::ace_to_11() {
	_value = 11;
}
