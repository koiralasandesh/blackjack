#include "card.h"

//Card::Card(int value, Suit suit, Face face) : _value{value}, _suit{suit}, _face{face} { }

Card::Card() { }

int Card::get_value() {
	return _value;
}

int Card::get_suit() {
	return _suit;
}

int Card::get_face() {
	return _face;
}

void Card::set_value(int value) {
	_value = value;
}

void Card::set_suit(Suit suit) {
	_suit = suit;
}

void Card::set_face(Face face) {
	_face = face;
}

bool Card::is_ace() {
	if (_face == ACE) {
		return true;
	}
	else {
		return false;
	}
}

void Card::ace_to_1() {
	if (_face == ACE) {
		_value = 1;
	}
}

// written: Rebecca B 11/2

