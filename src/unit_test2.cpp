#include "dealer.cpp"
#include "chat_message.hpp"

int main(int argc, char *argv[]) 
{
	// unit test for if a hand can be split. using dealer's hand for this test.
	Dealer dealer;
	Card card1;
	card1.set_face(ACE);
	Card card2;
	card2.set_face(ACE);

	dealer.dealer_hand.add_card_hand(card1, 0);
	dealer.shoe.remove_card();
	dealer.dealer_hand.add_card_hand(card2, 0);
	dealer.shoe.remove_card();

	if (dealer.dealer_hand.get_card(0, 0).get_face() != dealer.dealer_hand.get_card(1, 0).get_face())
	{
		std::cerr << "test failed. card1 and card2 face not identical." << std::endl;
	}

	return 0;
}