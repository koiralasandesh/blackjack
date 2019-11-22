#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include "asio.hpp"
#include "chat_message.hpp"
//#include "hand.h"
#include "shoe.h"
#include "card.h"

using asio::ip::tcp;

typedef std::deque<chat_message> chat_message_queue;
/*
class hand {
  public:
    hand () { 
      cards[0] = 0;
      cards[1] = 0;
      cards[2] = 0;
    }
    int cards[3];  // this is just an example
};
*/
class player
{
public:
  std::string name;
  int bet = 0;
  int credits;
  bool joined = false;
  Hand player_hand;
  player() : credits{100}
  {
    std::cout << "Creating a new player " << std::endl;
    std::cout << "Credits is " << credits << std::endl;
  }
  virtual ~player() {}
  virtual void deliver(const chat_message &msg) = 0;
};

typedef std::shared_ptr<player> player_ptr;

class dealer
{
public:
  Shoe shoe;
  int dealer_credits;
  Hand dealer_hand;
  bool round = false;
  dealer() : dealer_credits{500}
  {
    std::cout << "Creating a dealer" << std::endl;
    shoe.create_cards();
    shoe.shuffle();
    current_player = NULL;
    card_idx = 0;
    //added card in dealer hand as soon as player is joined
    std::cerr << "dealer hand index: " << dealer_hand.get_hand_index(0) << std::endl;
    dealer_hand.add_card_hand(shoe.next_card(), 0);
    dealer_hand.set_hand_value(0);
    std::cerr << dealer_hand.get_hand_value(0) << std::endl;
    shoe.remove_card();

    std::cerr << "dealer hand index: " << dealer_hand.get_hand_index(0) << std::endl;
    dealer_hand.add_card_hand(shoe.next_card(), 0);
    dealer_hand.set_hand_value(0);
    std::cerr << dealer_hand.get_hand_value(0) << std::endl;
    shoe.remove_card();
  }
  /*
    void deal() {
      Hand.cards[card_idx] = 8;
      card_idx++;//
      if (card_idx > 2) {
        card_idx = 0;
      }
    }
    */
  void next_player(player_ptr p)
  {
    current_player = p;
  }
  player_ptr current_player;
  //hand Hand;
  /*
  current_player->player_hand.add_card_hand(shoe.next_card(), 0);
    current_player->player_hand.set_hand_value(0);
    std::cerr << current_player->player_hand.get_hand_value(0) << std::endl;
    shoe.remove_card();
    current_player->player_hand.add_card_hand(shoe.next_card(), 0);
    current_player->player_hand.set_hand_value(0);
    std::cerr << current_player->player_hand.get_hand_value(0) << std::endl;
    shoe.remove_card();
    */

  int card_idx;
  /**/
  int play()
  {
    if (current_player == NULL)
    {
      std::cerr << "current player is null" << std::endl;
    }
    //next_player();
    //FIRST DEALER MUST KEEP HITTING ON ITS HAND TILL IT OVER 17
    //ONCE TOTAL CARD VALUE IS OVER 17 CHCEK IF ITS 21 OR OVER 21
    //OVER 21 PLAYER AUTOMATICALLY WINS SIMILARLY WE HAD DONE SAME FOR PLAYER WHILE HITTING IF OVER 21 DEALER AUTOMATICALLY
    //WINS AND DOESNT HAVE TO PLAY
    //IF BOTH ARE 21 THEN TIE
    //OTHER THAN THAT COMPARE BOTH VALUES AND GREATER WINS
    //

    if (current_player->player_hand.get_hand_value(0) > 21)
    {
      dealer_credits += current_player->bet;
      std::cerr << "dealer credits " << dealer_credits << std::endl;
    }
    else
    {
      while (dealer_hand.get_hand_value(0) < 17)
      {
        std::cerr << dealer_hand.get_hand_index(0) << std::endl;
        dealer_hand.add_card_hand(shoe.next_card(), 0);
        dealer_hand.set_hand_value(0);
        std::cerr << dealer_hand.get_hand_value(0) << std::endl;
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

        if (current_player->player_hand.get_hand_value(0) == 21)
        { //if player has blackjack
          current_player->credits += 2.5 * current_player->bet;
        }
        else
        { //player wins and 2*bet added to its credit
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

class blackjack_table
{ // chat room class
public:
  void join(player_ptr player_)
  {
    std::cout << "joining the table" << std::endl;
    players.insert(player_);
    /*
      for (auto msg: recent_msgs_) {
          player_->deliver(msg);
      }
      */
  }

  void leave(player_ptr player_)
  {
    players.erase(player_);
  }

  void deliver(const chat_message &msg)
  {
    recent_msgs_.push_back(msg);
    while (recent_msgs_.size() > max_recent_msgs)
    {
      recent_msgs_.pop_front();
    }

    for (auto player_ : players)
    {
      player_->deliver(msg);
    }
  }

  bool all_players_have_a_name()
  {
    bool retval = true;
    for (auto player_ : players)
    {
      retval = retval && (player_->name > "");
    }
    return retval;
  }
  dealer Dealer;

private:
  std::set<player_ptr> players;
  enum
  {
    max_recent_msgs = 100
  };
  chat_message_queue recent_msgs_;
};

class blackjack_player : public player, public std::enable_shared_from_this<blackjack_player>
{
public:
  blackjack_player(tcp::socket socket, blackjack_table &table) : socket_(std::move(socket)), table_(table)
  {
    std::cout << "Creating a blackjack_player " << std::endl;
  }

  void start()
  {
    table_.join(shared_from_this());
    do_read_header();
  }

  void deliver(const chat_message &msg)
  {
    bool write_in_progress = !write_msgs_.empty();
    write_msgs_.push_back(msg);
    if (!write_in_progress)
    {
      do_write();
    }
  }

private:
  void do_read_header()
  {
    auto self(shared_from_this());
    asio::async_read(socket_, asio::buffer(read_msg_.data(), chat_message::header_length),
                     [this, self](std::error_code ec, std::size_t /*length*/) {
                       if (!ec && read_msg_.decode_header())
                       {
                         do_read_body();
                       }
                       else
                       {
                         table_.leave(shared_from_this());
                       }
                     });
  }

  void do_read_body()
  {
    auto self(shared_from_this()); // this current player
    asio::async_read(socket_,
                     asio::buffer(read_msg_.body(), read_msg_.body_length()), [this, self](std::error_code ec, std::size_t /*length*/) {
                       if (!ec)
                       {
                         // Here is where messages arrive from the client
                         // this is where the design makes it easy or hard
                         // ignore anything in the message body

                         read_msg_.body_length(0);
                         read_msg_.gs.dealer_cards_valid = false;
                         read_msg_.gs.player_cards_valid = false;
                         // is it a join
                         if (read_msg_.ca.join && read_msg_.ca.name_valid)
                         {
                           std::cerr << self->joined << std::endl;
                           if (!self->joined)
                           {
                             table_.Dealer.next_player(self);
                             std::cout << "Player name is " << read_msg_.ca.name << std::endl;
                             std::string m = std::string(read_msg_.ca.name) + " has joined.";

                             //added card in player hand as soon as player is joined
                             std::cerr << "player hand index: " << self->player_hand.get_hand_index(0) << std::endl;
                             self->player_hand.add_card_hand(table_.Dealer.shoe.next_card(), 0);
                             self->player_hand.set_hand_value(0);
                             std::cerr << self->player_hand.get_hand_value(0) << std::endl;
                             table_.Dealer.shoe.remove_card();

                             std::cerr << "player hand index: " << self->player_hand.get_hand_index(0) << std::endl;
                             self->player_hand.add_card_hand(table_.Dealer.shoe.next_card(), 0);
                             self->player_hand.set_hand_value(0);
                             std::cerr << self->player_hand.get_hand_value(0) << std::endl;
                             table_.Dealer.shoe.remove_card();

                             strcpy(read_msg_.body(), m.c_str());
                             read_msg_.body_length(strlen(read_msg_.body()));
                             self->name = std::string(read_msg_.ca.name);
                             self->joined = true;
                           }
                         }
                         if (self->name > "")
                         { // quick way to see if they have entered a name

                           //if condition to check round is over or not
                           /*if (table_.Dealer.round)
                           {
                             self->player_hand.reset_hands();
                             table_.Dealer.dealer_hand.reset_hands();
                             read_msg_.ca.hit = false;
                             read_msg_.ca.bet = false;
                             read_msg_.ca.stand = false;
                             read_msg_.ca.surrender = false;
                             read_msg_.ca.split = false;
                             read_msg_.ca.doubleDown = false;
                             read_msg_.ca.name_valid = false;
                             
                             strcpy(read_msg_.body(), m.c_str());
                             read_msg_.body_length(strlen(read_msg_.body()));
                             table_.Dealer.round = false;
                           }*/

                           if (read_msg_.ca.hit)
                           { // also need to check in order, since everyone has a turn
                             std::string m;

                             if (!self->bet)
                             {
                               m = self->name + " has not placed a bet.";
                             }
                             /*
                             else if (read_msg_.ca.doubleDown)
                             {
                               std::cerr << "can't hit after double down " << self->bet << std::endl;
                             }
                             */
                             else
                             {

                               // call the dealer class with some kind of method and argument
                               m = self->name + " has asked for a hit.";

                               std::cerr << "player hand index: " << self->player_hand.get_hand_index(0) << std::endl;
                               self->player_hand.add_card_hand(table_.Dealer.shoe.next_card(), 0);
                               self->player_hand.set_hand_value(0);

                               std::cout << "dealt card is ";
                               std::cout << "value->" << table_.Dealer.shoe.next_card().get_value() << ", ";
                               std::cout << "face->" << table_.Dealer.shoe.next_card().get_face() << ", ";
                               std::cout << "suit->" << table_.Dealer.shoe.next_card().get_suit() << std::endl;
                               std::cout << "shoe size is now " << table_.Dealer.shoe._current_card + 1 << std::endl;
                               table_.Dealer.shoe.remove_card();

                               //printing hand value of player
                               std::cerr << "printing hand value of player = " << self->player_hand.get_hand_value(0) << std::endl;
                               if (self->player_hand.get_hand_value(0) == 21)
                               {
                                 std::cerr << "Your already have 21  = " << self->player_hand.get_hand_value(0) << std::endl;
                                 //go to dealer function play
                                 self->credits = table_.Dealer.play();
                                 m = "round is over starting another round. place another bet";
                                 self->bet = 0;
                                 std::cerr << "self->bet = " << self->bet << std::endl;
                               }
                               else if (self->player_hand.get_hand_value(0) > 21)
                               {
                                 std::cerr << "Your are over 21 so busted dealer wins automatically  = " << self->player_hand.get_hand_value(0) << std::endl;
                                 //can end the round adding credits to dealer
                                 self->credits = table_.Dealer.play();
                                 self->bet = 0;
                                 m = "round is over starting another round. place another bet";
                                 std::cerr << "self->bet = " << self->bet << std::endl;
                               }
                               //table_.Dealer.shoe.remove_card();
                               // refilling shoe
                               if (table_.Dealer.shoe._current_card <= 0)
                               {
                                 std::cout << "refilling shoe... ";
                                 table_.Dealer.shoe.create_cards();
                                 table_.Dealer.shoe.shuffle();
                                 std::cout << "shoe refilled." << std::endl;
                               }
                             }
                             strcpy(read_msg_.body(), m.c_str());
                             read_msg_.body_length(strlen(read_msg_.body()));
                             std::cerr << "self->bet = " << self->bet << std::endl;
                             strcpy(read_msg_.body(), m.c_str());
                             read_msg_.body_length(strlen(read_msg_.body()));
                             // also set read_msg.gs.XXX to whatever needs to go to the clients
                           }

                           if (read_msg_.ca.bet)
                           {
                             std::string m;
                             if (self->bet)
                             {
                               m = self->name + " already has a bet.";
                             }
                             else
                             {
                               if (read_msg_.ca.bet_amt == 0)
                               {
                                 m = "invalid bet. bet must be from 1 to 5 credits.";
                               }
                               else if (self->credits <= 0 || read_msg_.ca.bet_amt > self->credits)
                               {
                                 m = self->name + "does not have enough credits.";
                               }
                               else if (self->credits < 20)
                               {
                                 m = self->name + "has less than 20 credits.";
                               }
                               else
                               {
                                 std::string m = self->name + " has placed a bet.";
                                 self->bet = read_msg_.ca.bet_amt;
                                 self->credits -= self->bet;
                                 std::cerr << "bet placed: " << self->bet << std::endl;
                                 std::cerr << "remaining credits: " << self->credits << std::endl;
                               }
                             }
                             strcpy(read_msg_.body(), m.c_str());
                             read_msg_.body_length(strlen(read_msg_.body()));
                           }

                           if (read_msg_.ca.split)
                           {
                             std::string m = self->name + " has requested to split.";
                             strcpy(read_msg_.body(), m.c_str());
                             read_msg_.body_length(strlen(read_msg_.body()));
                           }

                           if (read_msg_.ca.doubleDown)
                           {
                             // check bet , then double it
                             std::string m;
                             if (!self->bet)
                             {
                               //m = "invalid bet. bet must be from 1 to 5 credits.";
                               std::cerr << "Place the bet first." << self->bet << std::endl;
                             }
                             else if (self->player_hand.get_hand_index(0) - 1 == 1)
                             {
                               //std::string m = self->name + " has placed a bet.";
                               self->bet = 2 * self->bet;
                               self->credits -= self->bet / 2;
                               std::cerr << "bet has been doubled: " << self->bet << std::endl;
                               std::cerr << "remaining credits: " << self->credits << std::endl;

                               std::cerr << "hand index: " << self->player_hand.get_hand_index(0) << std::endl;
                               self->player_hand.add_card_hand(table_.Dealer.shoe.next_card(), 0);
                               self->player_hand.set_hand_value(0);
                               table_.Dealer.shoe.remove_card();

                               std::cout << "dealt card is ";
                               std::cout << "value->" << table_.Dealer.shoe.next_card().get_value() << ", ";
                               std::cout << "face->" << table_.Dealer.shoe.next_card().get_face() << ", ";
                               std::cout << "suit->" << table_.Dealer.shoe.next_card().get_suit() << std::endl;
                               std::cout << "shoe size is now " << table_.Dealer.shoe._current_card + 1 << std::endl;

                               //printing hand value of player
                               std::cerr << "printing hand value of player = " << self->player_hand.get_hand_value(0) << std::endl;

                               if (table_.Dealer.shoe._current_card <= 0)
                               {
                                 std::cout << "refilling shoe... ";
                                 table_.Dealer.shoe.create_cards();
                                 table_.Dealer.shoe.shuffle();
                                 std::cout << "shoe refilled." << std::endl;
                               }
                             }
                             else
                             {
                               std::cerr << "You cannot double down after you hit card. hand size is: " << self->player_hand.get_hand_index(0) << std::endl;
                             }
                           }
                           //strcpy(read_msg_.body(), m.c_str());
                           //read_msg_.body_length(strlen(read_msg_.body()));

                           if (read_msg_.ca.stand)
                           {
                             //now is dealer turn maybe method in dealer class to get card in its hand till less then 17 then get total and check with player
                             std::string m;
                             if (read_msg_.ca.doubleDown) //stand has following string if doubleDown is clicked
                             {
                               m = self->name + " has requested to doubleDown.";
                             }
                             else
                             {
                               m = self->name + " has requested to stand.";
                             }

                             strcpy(read_msg_.body(), m.c_str());
                             read_msg_.body_length(strlen(read_msg_.body()));

                             self->credits = table_.Dealer.play();
                             m = "round is over starting another round. place another bet";
                             self->bet = 0;
                             std::cerr << "self->bet = " << self->bet << std::endl;
                           }

                           if (read_msg_.ca.surrender)
                           {
                             std::string m = self->name + " has requested to surrender.";
                             //we can some functions in hand to clear its contents in _Hand array

                             //bet deducted in player
                             //clear dealer hand
                             strcpy(read_msg_.body(), m.c_str());
                             read_msg_.body_length(strlen(read_msg_.body()));
                           }

                           // display the cards if everyone has joined
                           if (table_.all_players_have_a_name())
                           {
                             //will display gs information to client window
                           }
                           if (table_.Dealer.round)
                           {
                             self->player_hand.reset_hands();
                             table_.Dealer.dealer_hand.reset_hands();
                             table_.Dealer.round = false;
                           }

                           read_msg_.encode_header(); // so the body text above gets sent
                           table_.deliver(read_msg_);
                           do_read_header();
                         }
                       }
                       else
                       {
                         table_.leave(shared_from_this());
                       }
                     });
  }
  //}
  void do_write()
  {
    auto self(shared_from_this());
    asio::async_write(socket_, asio::buffer(write_msgs_.front().data(), write_msgs_.front().length()), [this, self](std::error_code ec, std::size_t /*length*/) {
      if (!ec)
      {
        write_msgs_.pop_front();
        if (!write_msgs_.empty())
        {
          do_write();
        }
      }
      else
      {
        table_.leave(shared_from_this());
      }
    });
  }
  tcp::socket socket_;
  blackjack_table &table_;
  chat_message read_msg_;
  chat_message_queue write_msgs_;
};

class blackjack_game
{
public:
  blackjack_game(asio::io_context &io_context, const tcp::endpoint &endpoint)
      : acceptor_(io_context, endpoint)
  {
    std::cout << "Creating a blackjack_game" << std::endl;
    do_accept();
  }

private:
  void do_accept()
  {
    acceptor_.async_accept([this](std::error_code ec, tcp::socket socket) {
      if (!ec)
      {
        std::make_shared<blackjack_player>(std::move(socket), table_)->start();
      }

      do_accept();
    });
  }

  tcp::acceptor acceptor_;
  blackjack_table table_;
};

int main(int argc, char *argv[])
{
  try
  {
    if (argc < 2)
    {
      std::cerr << "Usage: blackjack_game <port> [<port> ...]\n";
      return 1;
    }
    asio::io_context io_context;

    std::list<blackjack_game> servers;
    for (int i = 1; i < argc; ++i)
    {
      tcp::endpoint endpoint(tcp::v4(), std::atoi(argv[i]));
      servers.emplace_back(io_context, endpoint);
    }

    io_context.run();
  }
  catch (std::exception &e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }
  return 0;
}
