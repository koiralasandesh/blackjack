#include "card.h"

Card::Card(int value, Suit suit, Face face) : _value{value}, _suit{suit}, _face{face} { }

int Card::get_value() {
	return _value;
}

void Card::set_value() {
	if (_face == ACE) {
		_value += 10;
	}
}

int Card::get_suit() {
	return _suit;
}

int Card::get_face() {
	return _face;
}

bool Card::is_ace() {
	if (_face == ACE) {
		return true;
	}
	else {
		return false;
	}
}

void Card::ace_to_11() {
	_value = 11;
}

// written: Rebecca B 11/2

