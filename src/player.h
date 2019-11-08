#include "hand.h"
#include <string>

class Player {
	private:
		std::string _name;
		int _player_number;
		bool _playing = true;		// player is/was participating
		Hand _hand;				// player's cards - always >= 1 element
		bool _hit = false;			// Hit state - always F at round beginning
		bool _stand = false;		// Stand state - always F at round beginning
									// NEVER have _hit and _stand T simultaneously
		int _credits = 100;			// player's reserve
		int _bet = 0;				// current round bet
		bool _has_split = false;	// referenced by hand->split_cards()
		bool _has_doubled_down = false;
		bool _has_surrendered = false;	// dont need getter, only setter
		bool _is_waiting = false;	// player has went, and waiting on other players to finish
									// main var to be checked before asking for player actions
	public:
		Player(std::string name, int player_number);
		Hand get_hand();			//hand getter
		void place_bet(int bet);
		bool get_playing();			// playing getter
		void change_playing();		// change playing status to opposite
		bool get_waiting();			// waiting getter
		void waiting();				// change waiting status to opposite
		bool get_has_split();
		void split();
		bool get_doubled_down();
		void doubled_down(Card card);
		void surrender();
		void reset();				// reset everything to default values
		void want_hit(Card card);	// hit = true
		void want_stand();			// stand = true
		void reset_hit();
		void reset_stand();
		/*
		void win();				// where to put? maybe inside stand/doubledown/split???
		void lose();			// also where to put?? maybe inside dealer and hit method
		*/

		//increase or decrease credits after comparing with dealer
        void set_win_credits();
        void set_lose_credits();
        //added to get player bet and add to dealer credits
        int get_bet();
};

