#include "shoe.h"
#include <algorithm>
#include <random>
#include <iostream>
#include <chrono>

void Shoe::create_cards() {
	int decks, s, f, value = 1;
	for (decks = 0; decks < 6; decks++) {
		for (s = 0; s < 4; s++) {
			for (f = 1; f < 14; f++) {
				Card* new_card;
				if (Face(f) < JACK) {	// initialize face cards to correct value
					new_card = new Card{value++, Suit(s), Face(f)};
				}
				else {
					new_card = new Card{10, Suit(s), Face(f)};
				}
				_shoe.push_back(new_card);	// add card to shoe
			}
			value = 1;
		}
	}
}

Card* Shoe::next_card() { return _shoe.back(); }

void Shoe::remove_card() { _shoe.pop_back(); }

void Shoe::shuffle() {
	// randomized w/ time-based seed
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::shuffle(_shoe.begin(), _shoe.end(), std::default_random_engine(seed));
}

