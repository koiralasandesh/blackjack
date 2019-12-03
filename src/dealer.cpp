#include "player.cpp"
#include "shoe.h"
#include <memory>
#include <assert.h>

typedef std::shared_ptr<Player> player_ptr;

class Dealer {
  public:
    Shoe shoe;
    Hand dealer_hand;
    int dealer_credits = 500;
    bool bust = false;
    bool round = true;
    player_ptr current_player;

    Dealer()
    {
      std::cout << "Creating a dealer." << std::endl;
      shoe.create_cards();
      shoe.shuffle();
      current_player = NULL;
     }
     
    void print_dealt_card()
    {
      std::cout << "\tvalue->" << shoe.next_card().get_value() << ", ";
      std::cout << "face->" << shoe.next_card().get_face() << ", ";
      std::cout << "suit->" << shoe.next_card().get_suit() << "\n\n";
    }
    
    void check_shoe()
    {
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
      assert(current_player);

      std::cout << "\n##### STARTING DEALER'S TURN #####\n\n";

      // get initial hand values
      current_player->player_hand.set_hand_value(0);
      current_player->player_hand.set_hand_value(1);
      dealer_hand.set_hand_value(0);

      // check if dealer has blackjack
      // all active players automatically lose unless they had blackjack
      // player blackjack is checked prior to entering method, so it can be ignored
      // cannot bust on first hand so doesn't need to be checked
      if (dealer_hand.get_hand_value(0) == 21)
      {
        dealer_credits += current_player->bet;
        std::cout << "Dealer has Blackjack!" << std::endl;
      }
      else
      {
        // finalize dealer's hand - hit until >= 17
        while (dealer_hand.get_hand_value(0) < 17)
        {
          dealer_hand.add_card_hand(shoe.next_card(), 0);
          std::cout << "Dealing card to dealer:" << std::endl;
          print_dealt_card();
          shoe.remove_card();
          dealer_hand.set_hand_value(0);
        }
        // check if dealer busted
        if (dealer_hand.get_hand_value(0) > 21)
        {
          bust = true; // if player busted, their bet will be 0; bottom if-statements will not be entered
          std::cout << "Dealer bust!\n";
          std::cout << current_player->name << " wins!\n";
        }
        // otherwise, continue to compare with player hands
        else
        {
          // comparing regular hand
          if (current_player->bet)
          {
            // player busted; doesn't matter what dealer's hand is since player will have busted first, technically
            // this if-statement may not be needed at all
            if (current_player->hand_bust)
            {
              dealer_credits += current_player->bet;
              std::cout << current_player->name << "'s hand busted!\n";
            }
            // player got 21, now check if dealer ties or loses; dealer cannot win
            if (current_player->player_hand.get_hand_value(0) == 21)
            {
              // dealer ties
              if (dealer_hand.get_hand_value(0) == 21)
              {
                current_player->credits += current_player->bet; // give player back their bet to credits
                std::cout << "Dealer and " << current_player->name << "'s hands ties!\n";
              }
              // dealer loses
              else
              {
                dealer_credits -= current_player->bet;
                current_player->credits += 2 * current_player->bet;
                std::cout << current_player->name << "'s hand wins!\n";
              }
            }
            // player stood, now check if dealer ties, wins, or loses
            if (current_player->hand_stand)
            {
              // dealer ties
              if (dealer_hand.get_hand_value(0) == current_player->player_hand.get_hand_value(0))
              {
                current_player->credits += current_player->bet;
                std::cout << "Dealer and " << current_player->name << "'s hands ties!\n";
              }
              // dealer wins
              else if (dealer_hand.get_hand_value(0) > current_player->player_hand.get_hand_value(0))
              {
                dealer_credits += current_player->bet;  // leave player credits alone, bet was deducted earlier
                std::cout << "Dealer's hand wins!\n";
              }
              // dealer loses
              else
              {
                dealer_credits -= current_player->bet;
                current_player->credits += 2 * current_player->bet;
                std::cout << current_player->name << "'s hand wins!\n";
              }
            }
          }
    
          // handling split hand
          if (current_player->split_bet)
          {
            // player busted; doesn't matter what dealer's hand is since player will have busted first, technically
            // this if-statement may not be needed at all
            if (current_player->split_bust)
            {
              dealer_credits += current_player->split_bet;
              std::cout << current_player->name << "'s split hand busted!\n";
            }
            // player got 21, now check if dealer ties or loses; dealer cannot win
            if (current_player->player_hand.get_hand_value(1) == 21)
            {
              // dealer ties
              if (dealer_hand.get_hand_value(0) == 21)
              {
                current_player->credits += current_player->split_bet; // give player back their bet to credits
                std::cout << "Dealer and " << current_player->name << "'s split hands ties!\n";
              }
              // dealer loses
              else
              {
                dealer_credits -= current_player->split_bet;
                current_player->credits += 2 * current_player->split_bet;
                std::cout << current_player->name << "'s split hand wins!\n";
              }
            }
            // player stood, now check if dealer ties, wins, or loses
            if (current_player->split_stand)
            {
              // dealer ties
              if (dealer_hand.get_hand_value(0) == current_player->player_hand.get_hand_value(1))
              {
                current_player->credits += current_player->split_bet;
                std::cout << "Dealer and " << current_player->name << "'s split hands ties!\n";
              }
              // dealer wins
              else if (dealer_hand.get_hand_value(0) > current_player->player_hand.get_hand_value(1))
              {
                dealer_credits += current_player->split_bet;  // leave player credits alone, bet was deducted earlier
                std::cout << "Dealer's hand wins!\n";
              }
              // dealer loses
              else
              {
                dealer_credits -= current_player->split_bet;
                current_player->credits += 2 * current_player->split_bet;
                std::cout << current_player->name << "'s split hand wins!\n";
              }
            }
          }
        }
      }

      // player automatically wins
      if (bust && !current_player->hand_bust)
      {
        dealer_credits -= current_player->bet;
        current_player->credits += 2 * current_player->bet;
        std::cout << "Dealer busts!\n";
      }
      if (bust && !current_player->split_bust)
      {
        dealer_credits -= current_player->split_bet;
        current_player->credits += 2 * current_player->split_bet;
        std::cout << "Dealer busts!\n";
      }
      round = true;
      bust = false;
      std::cout << "\n##### ENDING DEALER'S TURN #####\n\n";
      return current_player->credits;
    }
};

/*
void Dealer::refresh_credits()
{
    sleep(30);
    this->dealer_credits = 500;
}
*/
