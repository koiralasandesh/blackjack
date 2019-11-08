#ifndef DEALER_H
#define DEALER_H

using namespace std;

class Dealer
{
    public:
        vector<Card> dealer_hand;
        //vector<Player> players;
        Player player1;          //handling only one player
        Shoe shoe;
        int dealer_credits;

        Dealer(Player player1);
        virtual ~Dealer();

        void refresh_credits();
        void deal_card();


        void player_hit();
        void player_stand();
        void add_player();
        void remove_player();
        bool win();
        bool loose();

};

#endif