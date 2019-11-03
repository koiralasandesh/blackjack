#include <vector>
#include "card.h"

class Shoe {
	private:
		std::vector<Card*> _shoe;
	public:
		void create_cards();	// generates 6 decks
		Card* next_card();		// pops first card and returns card fields
		void remove_card();
		void shuffle();
};

