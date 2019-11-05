#include <iostream>
#include <vector>
#include "shoe.h"
#include "hand.h"

using std::cout;
using std::endl;

int main(int argv, char *argc[]) {

// ######### SHOE (AND CARD) TESTING #########
	Shoe *shoe = new Shoe;
	shoe->create_cards();
	shoe->shuffle();
	int count;
	cout << "number of cards in shoe before: " << shoe->get_shoe().size() << endl;
	for (count = 0; count < 20; count++) {
		cout << "Value: " << shoe->next_card()->get_value();
		cout << "\tSuit: " << shoe->next_card()->get_suit();
		cout << "\t\tFace: " << shoe->next_card()->get_face() << endl;
		shoe->remove_card();
	}
	cout << "number of cards in shoe after: "<< shoe->get_shoe().size() << endl;
	
// ######### HAND TESTING #########
	Hand *hand = new Hand;
	hand->add_card_hand(shoe->next_card());
	shoe->remove_card();
	hand->add_card_hand(shoe->next_card());
	shoe->remove_card();
	
	cout << "first card value: " << hand->get_card(0)->get_value() << endl;
	cout << "second card value: " << hand->get_card(1)->get_value() << endl;
	hand->set_total_value();
	cout << "hand value: " << hand->get_total_value() << endl;
	bool split = hand->can_split();
	if (split) {
		cout << "can split" << endl;
	}
	else {
		cout << "can't split" << endl;
	}
	
	return 0;
}



