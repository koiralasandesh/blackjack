#include "hand.h"
#include <vector>

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
		if (_split_hand[_split_index].get_value() == 0) {
			_split_hand[_split_index] = card;
			if (card.get_face() == ACE) {
			  _split_has_ace = true;
			}
			_split_index++;
		}
	}
	else {
		if (_hand[_hand_index].get_value() == 0) {
			_hand[_hand_index] = card;
			if (card.get_face() == ACE) {
			  _hand_has_ace = true;
			}
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
	_hand_value = 0;
	_split_value = 0;
	int ace_count = 0;
	std::vector<int> ace_index;
	
	if (which_hand) {
		for (int i = _split_index-1; i >= 0; i--) {
			_split_value += _split_hand[i].get_value();
			if (_split_hand[i].get_face() == ACE) {
			  ace_index.push_back(i);
		    ace_count++;
		  }
		}
		if (_split_value > 21 && _split_has_ace) {
		  while (!ace_count && _split_value > 21) {
		    _split_value -= 10;
		    _split_hand[ace_index.back()].set_value(1);
		    ace_index.pop_back();
		    ace_count--;
		  }
		}
	}
	
	else {
		for (int i = _hand_index-1; i >= 0; i--) {
		  _hand_value += _hand[i].get_value();
		  if (_hand[i].get_face() == ACE) {
		    ace_index.push_back(i);
		    ace_count++;
		  }
		}
		if (_hand_value > 21 && _hand_has_ace) {
		  while (!ace_count && _hand_value > 21) {
		    _hand_value -= 10;
		    _hand[ace_index.back()].set_value(1);
		    ace_index.pop_back();
		    ace_count--;
		  }
		}
	}
}
// check the hand index after card is added
int Hand::get_hand_index(int which_hand){
	if (which_hand) {
		return _split_index;
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

void Hand::reset_hands() {
	for (int i = 0; i < 13; i++) {
		_hand[i].set_value(0);
		_hand[i].set_suit(NO_SUIT);
		_hand[i].set_face(NO_FACE);
		_hand[i].is_ace();
		_split_hand[i].set_value(0);
		_split_hand[i].set_suit(NO_SUIT);
		_split_hand[i].set_face(NO_FACE);
		_split_hand[i].is_ace();
	}
	_hand_has_ace = false;
	_split_has_ace = false;
	_hand_index = 0;
	_split_index = 0;
	_hand_value = 0;
	_split_value = 0;
}

void Hand::split_cards() {
	_split_hand[0].set_value(_hand[1].get_value());
	_split_hand[0].set_suit(Suit(_hand[1].get_suit()));
	_split_hand[0].set_face(Face(_hand[1].get_face()));
	_split_hand[0].is_ace();

	_hand[1].set_value(0);
	_hand[1].set_suit(NO_SUIT);
	_hand[1].set_face(NO_FACE);
	_hand[1].is_ace();

	_hand_index--;
	_split_index++;
}

// written: Rebecca B 11/4
