#include <vector>
#include "card.h"

class Shoe {
	private:
		std::vector<Card*> _shoe;
	public:
		void create_cards();	// generates 6 decks
		Card* next_card();		// returns last card in shoe
		void remove_card();		// pops last card
		void shuffle();			// randomize shoe
};

