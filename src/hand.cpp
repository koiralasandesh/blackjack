#include "hand.h"

// maybe i should set default values for same_cards?

void Hand::reset_hands() {
	_hand.clear();
	_split_hand.clear();
}

std::vector<Card> Hand::get_hand() {
	return _hand;
}

std::vector<Card> Hand::get_split_hand() {
	return _split_hand;
}

Card Hand::get_card(int index) {
	return _hand[index];
}

void Hand::add_card_hand(Card card) {
	_hand.push_back(card);
}

int Hand::get_total_value() {
	return _total_value;
}

void Hand::set_total_value() {
	_total_value = 0;
	for (auto& c : _hand) {
		_total_value += c.get_value();
	}
}

int Hand::number_of_cards() {
	return _hand.size();
}

bool Hand::can_split() {
	if (_hand.size() == 2) {
		if (_hand[0].get_face() == _hand[1].get_face()) {
			return true;		// hand is splittable if card face values are identical
		}
	}
	else {
		return false;
	}
}

/*
void Hand::split_cards() {
	
}
*/

// written: Rebecca B 11/4

