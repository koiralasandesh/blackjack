#include "hand.h"

// maybe i should set default values for same_cards?

Card* Hand::get_hand(int which_hand) {
	if (which_hand) {
		return _split_hand;
	}
	else {
		return _hand;
	}
}

Card Hand::get_card(int index, int which_hand) {
	if (which_hand) {
		return _split_hand[index];
	}
	else {
		return _hand[index];
	}
}

void Hand::add_card_hand(Card card, int which_hand) {
	if (which_hand) {
		if (_split_hand[_split_index].get_value() == -1) {
			_split_hand[_split_index] = card;
			_split_index++;
		}
	}
	else {
		if (_hand[_hand_index].get_value() == -1) {
			_hand[_hand_index] = card;
			_hand_index++;
		}
	}
}

int Hand::get_hand_value(int which_hand) {
	if (which_hand) {
		return _split_value;
	}
	else {
		return _hand_value;
	}
}

void Hand::set_hand_value(int which_hand) {
	int i;
	//_hand_value = 0;
	_split_value = 0;
	if (which_hand) {
		for (i = _split_index-1; i >= 0; i--) {
			_split_value += _split_hand[i].get_value();
		}
	}
	else {
		/*
		for (i = _hand_index-1; i >= 0; i--) {
			_hand_value += _hand[i].get_value();
		}
		*/
		_hand_value += _hand[_hand_index-1].get_value();
	}
	/*
	// Vector version
	for (auto& c : _hand) {
		_total_value += c.get_value();
	}
	*/
}
// check the hand index after card is added
int Hand::get_hand_index(int which_hand){
	if (which_hand) {
		return _hand_index;
	}
	else {
		return _hand_index;
	}
}

bool Hand::can_split() {
	if (_hand_index == 2) {
		if (_hand[0].get_face() == _hand[1].get_face()) {
			return true;		// hand is splittable if card face values are identical
		}
	}
	return false;
}

int Hand::num_cards_in_hand(int which_hand) {
	if (which_hand) {
		return _split_index;
	}
	else {
		return _hand_index;
	}
}

void Hand::reset_hands() {
	int i;
	for (i = 0; i < 13; i++) {
		_hand[i].set_value(-1);
		_hand[i].set_suit(NO_SUIT);
		_hand[i].set_face(NO_FACE);
		_split_hand[i].set_value(-1);
		_split_hand[i].set_suit(NO_SUIT);
		_split_hand[i].set_face(NO_FACE);
	}
	_hand_index = 0;
	
	_hand_value = 0;
}
/*
void Hand::split_cards() {
	
}


*/

// written: Rebecca B 11/4

