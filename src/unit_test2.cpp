#include "hand.h"
#include "shoe.h"
#include <iostream>

int main(int argc, char *argv[]) 
{
	// unit test for resetting hand
	Shoe shoe;
	Hand hand;
	
	shoe.create_cards();
	shoe.shuffle();
	
	for (int count = 0; count < 10; count++)
	{
	  hand.add_card_hand(shoe.next_card(), 0);
	  shoe.remove_card();
	}
	
	hand.reset_hands();
	
	if (hand.get_hand_index(0) != 0)
	{
	  std::cerr << "test failed.\n";
	}
	return 0;
}
