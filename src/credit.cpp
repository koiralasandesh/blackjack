#include "credit.h"
#include <cstdlib>

Credit::Credit(int credits, int wager)
	: _credits{credits}, _wager{wager} {
}

void Credit::refresh_credits() {
	if(!_credits) {
		_credits = 500;
	}
}

void Credit::wager_credits(int wager) {
	_wager = wager;
}

void Credit::win_credits(int wager) {
	_credits += wager * 1.5
}

void Credit::lose_credits(int wager) {
	_credits -= wager;
}

int num_credits() {
	return _credits;
}

int current_wager() {
	return _wager;
}
