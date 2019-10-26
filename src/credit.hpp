#ifndef CREDIT__
#define CREDIT__

class Credit {
	private:
		int _credits;				// Player/Dealer's credits
		int _wager;					// Current round's wager
	protected:
		void refresh_credits();		// When Dealer credits = 0
	public:
		Credit(int credits, int wager);
		void wager_credits();		// Bet specified amount
		void win_credits();			// If hand wins (150% wager prize)
		void lose_credits();		// If hand loses
		int num_credits();
		int current_bet();
}

#endif
