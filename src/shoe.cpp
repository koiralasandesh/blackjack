#include "shoe.h"
#include <algorithm>
#include <random>
#include <chrono>
#include <bits/stdc++.h>
#include <iostream>

Shoe::~Shoe() { }

Card* Shoe::get_shoe() {
	return _shoe;
}

void Shoe::create_cards() {
	int decks, suit, face, value = 1, card_count = 0;
	_current_card = 311;
	for (decks = 0; decks < 6; decks++) {
		for (suit = 1; suit < 5; suit++) {
			for (face = 1; face < 14; face++) {
				Card new_card;
				if (Face(face) == ACE) {  // ace has custom values
					new_card.set_value(11);
					new_card.is_ace();
					value++;
				}
				else if (Face(face) < JACK) {	// initialize face cards to correct value
					new_card.set_value(value++);
				}
				else {	// Jack, Queen, King
					new_card.set_value(10);
				}
				new_card.set_suit(Suit(suit));
				new_card.set_face(Face(face));
				_shoe[card_count] = new_card;
				card_count++;
			}
			value = 1;
		}
	}
}

Card Shoe::next_card() {
	return _shoe[_current_card];
}

void Shoe::remove_card() {
	_shoe[_current_card].set_value(-1);
	_shoe[_current_card].set_suit(NO_SUIT);
	_shoe[_current_card].set_face(NO_FACE);
	_current_card--;
}

void Shoe::shuffle() {	// randomized w/ time-based seed
	int n = sizeof(_shoe) / sizeof(_shoe[0]);
	std::srand(std::time(0));
	std::random_shuffle(_shoe, _shoe+n);
	
}

void Shoe::print_cards() {
	int i;
	for (i = 0; i < 312; ++i) {
		std::cout << "Card " << i+1 << " -> value: " << _shoe[i].get_value() << std::endl;
		std::cout << "\t    suit:  " << _shoe[i].get_suit() << std::endl;
		std::cout << "\t    face:  " << _shoe[i].get_face() << std::endl;
	}
}
