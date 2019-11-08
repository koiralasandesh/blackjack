#include "dealer.h"
#include <iostream>
#include "mainwin.h"
#include "player_choice.h"
#include "shoe.h"
#include "rules.h"
#include "hand.h"
#include "card.h"
#include <vector>
#include <time>

using namespace std;
//get player and create dealer hand
Dealer::Dealer(Player player1)
{
    this->player1 = player1;
    Hand hand;
    dealer_hand = hand.get_hand();
}

//check if player has 21 and if dealer has 21 
void Dealer::deal_card()
{
    int player_value = player1._hand.get_total_value();
    int dealer_value = dealer_hand.get_total_value();
    while(dealer_value <= 17)
    {   
        dealer_hand.add_card_hand();
        dealer_value = dealer_hand.get_total_value();
    }
    if( player_value > 21 || player_value < dealer_value)
    {
        if(dealer_value == 21)
        {
            cout << "dealer has blackjack" << endl;
        }
        else
        {
            cout << "dealer wins" << endl;
        }
        dealer_credits += player1.get_bet();
        player1.set_lose_credits();
        
    }
    elseif(player_value == dealer_value)
    {
        cout << "draw" << endl;
    }
    else
    {
        if(player_value == 21)
        {
            cout << "player has blackjack" << endl;
        }
        else
        {
            cout << "player wins" << endl;
        }
        dealer_credits -= player1.get_bet();
        player1.set_win_credits();
    }
}



void Dealer::refresh_credits()
{
    sleep(30);
    this->dealer_credits = 500;
}

//if hit or stand button in clicked by player
void Dealer::player_hit()
{
    player1._hand.add_card_hand();
}

void Dealer::player_stand()
{
    player1._stand = true;

}

//add player and remove player