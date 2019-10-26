#ifndef CREDIT__
#define CREDIT__

class Credit {
	private:
		int _credits;				// Player/Dealer's credits
		int _wager;					// Current round's wager
	protected:
		void refresh_credits();		// When Dealer credits = 0				{DEALER}
	public:
		Credit(int credits, int wager);
		void wager_credits();		// Bet specified amount					{PLAYER}
		void win_credits();			// If player hand wins (+150% wager)	{PLAYER}
		void lose_credits();		// If player hand loses (-100% wager)	{PLAYER}
		int num_credits();
		int current_bet();
}

#endif
