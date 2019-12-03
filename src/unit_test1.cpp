#include "dealer.cpp"

int main(int argc, char *argv[]) 
{
	// unit test for shoe refilling
	Dealer dealer;
	dealer.shoe.create_cards();
	dealer.shoe.shuffle();

	while (dealer.shoe._current_card > -1)
	{
		dealer.shoe.remove_card();
	}

	if (dealer.shoe._shoe[0].get_value() != -1)
	{
		std::cerr << "test failed. -> " << dealer.shoe._shoe[0].get_value();
	}

	return 0;
}