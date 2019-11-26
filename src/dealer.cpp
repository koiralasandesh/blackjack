#include "player.cpp"
#include "shoe.h"
#include <memory>

typedef std::shared_ptr<player> player_ptr;

class dealer
{
public:
  Shoe shoe;
  int dealer_credits;
  Hand dealer_hand;
  bool round = true;
  player_ptr current_player;
  dealer() : dealer_credits{500}
  {
    std::cout << "Creating a dealer." << std::endl;
    shoe.create_cards();
    shoe.shuffle();
    current_player = NULL;
   }
  void print_dealt_card() {
    std::cout << "\tvalue->" << shoe.next_card().get_value() << ", ";
    std::cout << "face->" << shoe.next_card().get_face() << ", ";
    std::cout << "suit->" << shoe.next_card().get_suit() << std::endl;
  }
  
  void check_shoe() {
    if (shoe._current_card <= 0)
     {
       std::cout << "refilling shoe... ";
       shoe.create_cards();
       shoe.shuffle();
       std::cout << "shoe refilled." << std::endl;
     }
  }

  void next_player(player_ptr p)
  {
    current_player = p;
  }
  
  int play()
  {
    if (current_player == NULL)
    {
      std::cout << "No current player.\n";
    }
    //FIRST DEALER MUST KEEP HITTING ON ITS HAND TILL IT OVER 17
    //ONCE TOTAL CARD VALUE IS OVER 17 CHCEK IF ITS 21 OR OVER 21
    //OVER 21 PLAYER AUTOMATICALLY WINS SIMILARLY WE HAD DONE SAME FOR PLAYER WHILE HITTING IF OVER 21 DEALER AUTOMATICALLY
    //WINS AND DOESNT HAVE TO PLAY
    //IF BOTH ARE 21 THEN TIE
    //OTHER THAN THAT COMPARE BOTH VALUES AND GREATER WINS

    if (current_player->player_hand.get_hand_value(0) > 21)
    {
      dealer_credits += current_player->bet;
      //std::cerr << "dealer credits " << dealer_credits << std::endl;
    }
    else
    {
      while (dealer_hand.get_hand_value(0) < 17)
      {
        dealer_hand.add_card_hand(shoe.next_card(), 0);
        print_dealt_card();
        dealer_hand.set_hand_value(0);
        shoe.remove_card();
      }

      if (dealer_hand.get_hand_value(0) > 21)
      {
        current_player->credits += 2 * current_player->bet;
        dealer_credits -= current_player->bet;
        //player wins automatically 2.5 times bet added to player credits
      }
      else if (dealer_hand.get_hand_value(0) == current_player->player_hand.get_hand_value(0))
      {
        current_player->credits += current_player->bet;
        //tie bet added back to player credits go for next round
      }
      else if (dealer_hand.get_hand_value(0) > current_player->player_hand.get_hand_value(0))
      {
        //dealer wins and bet added to its credit
        dealer_credits += current_player->bet;
      }
      else
      {

        if (current_player->player_hand.get_hand_value(0) == 21) //if player has blackjack, payout is 1.5x
        {
          current_player->credits += 2.5 * current_player->bet;
        }
        else // player wins and 2*bet added to its credit
        {
          current_player->credits += 2 * current_player->bet;
        }
      }
      //printing player credits
      std::cerr << "player credits = " << current_player->credits << std::endl;
    }

    round = true;
    return current_player->credits;
  }
};
/*
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

*/

