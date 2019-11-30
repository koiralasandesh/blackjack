#include <string>
#include <iostream>
#include "chat_message.hpp"

class Player {
  public:
    Player() {
      std::cout << "Creating a new player " << std::endl;
    }
    virtual ~Player() { }
    virtual void deliver(const chat_message &msg) = 0;
    std::string name;
    Hand player_hand;
    int credits = 100;
    int bet = 0;
    int split_bet = 0;
    bool joined = false;
    bool split = false;			// main flags for dealer's turn
    bool hand_stand = false;
    bool split_stand = false;
    bool hand_bust = false;
    bool split_bust = false;
};

/*
bool Player::get_playing() {
	return _playing;
}

void Player::change_playing() {
	if (_playing) {
		_playing = false;
	}
	else {
		_playing = true;
	}
}

bool Player::get_waiting() {
	return _is_waiting;
}

void Player::waiting() {
	if (_is_waiting) {
		_playing = false;
	}
	else {
		_is_waiting = true;
	}
}

bool Player::get_has_split() {
	return _has_split;
}

void Player::split() {
	if (_hand.get_split_hand().size() == 0 && !_has_split) {
		_has_split = true;
	}
	else {
		_hand.reset_hands();
		_has_split = false;
	}
}

bool Player::get_doubled_down() {
	return _has_doubled_down;
}

void Player::doubled_down(Card card) {
	_has_doubled_down = true;
	_bet *= 2;
	_hand.add_card_hand(card);
	_stand = true;
	_is_waiting = true;
}

void Player::surrender() {
	if(_playing) {
		_has_surrendered = true;
		_bet = 0;
		_is_waiting = true;
	}
}

void Player::reset() {
	_bet = 0;
	_playing = true;
	_hand.reset_hands();
	_hit = false;
	_stand = false;
	_has_split = false;
	_has_doubled_down = false;
	_has_surrendered = false;
	_is_waiting = false;
}

void Player::want_hit(Card card) {
	_hit = true;
	_hand.add_card_hand(card);
}

void Player::want_stand() {
	_stand = true;
	_is_waiting = true;
}

void Player::reset_hit() {
	_hit = false;
}

void Player::reset_stand() {
	_stand = false;
}

void Player::win() {
	ADD: win if 21
		dealer will have comparison check
}

void Player::lose() {
	ADD: lose if > 21
		dealer will have comparison check
}


//added
void Player::set_win_credits()
{
    _credits += _bet; 
}

void Player::set_lose_credits()
{
    _credits -= _bet; 
}

int Player::get_bet()
{
    return _bet;
}*/

