#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <set>
#include <utility>
#include "asio.hpp"
#include "chat_message.hpp"
#include "dealer.cpp"

using asio::ip::tcp;

typedef std::deque<chat_message> chat_message_queue;

class blackjack_table
{ // chat room class
public:
  void join(player_ptr player_)
  {
    std::cout << "Joining the table." << std::endl;
    players.insert(player_);
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
    std::cout << "Creating a blackjack_player." << std::endl;
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
                         
                         if (read_msg_.ca.join && read_msg_.ca.name_valid)
                         {
                           if (!self->joined) // if player is brand new
                           {
                             table_.Dealer.next_player(self);
                             self->name = std::string(read_msg_.ca.name);
                             self->joined = true;
                             std::cout << "Adding player: " << self->name << std::endl;
                             std::string m = self->name + " has joined.";

                             strcpy(read_msg_.body(), m.c_str());
                             read_msg_.body_length(strlen(read_msg_.body()));
                           }
                         }
                         if (self->name > "")
                         {
                            // when the round is new AND player has placed a bet, give dealer and players their cards
                            
                            if (read_msg_.ca.hit_split)
                            {
                              std::string m = self->name + " has requested to split.";
                              if (self->split)
                              {
                                self->player_hand.add_card_hand(table_.Dealer.shoe.next_card(), 1);
                               self->player_hand.set_hand_value(1);
                               /*
                               if (self->player_hand.get_hand_value(1) == 21)
                               {
                                 std::cerr << "You already have 21  = " << self->player_hand.get_hand_value(1) << std::endl;
                                 //go to dealer function play
                                 self->credits = table_.Dealer.play();
                                 m = "round is over starting another round. place another bet";
                                 self->bet = 0;
                                 std::cerr << "self->bet = " << self->bet << std::endl;
                               }
                               else if (self->player_hand.get_hand_value(1) > 21)
                               {
                                 std::cerr << "You are over 21 so busted. dealer wins automatically  = " << self->player_hand.get_hand_value(1) << std::endl;
                                 //can end the round adding credits to dealer
                                 self->credits = table_.Dealer.play();
                                 self->bet = 0;
                                 m = "round is over starting another round. place another bet";
                               }*/
                              }
                              else {
                                std::cout << "Must split hand first." << std::endl;
                              }
                             strcpy(read_msg_.body(), m.c_str());
                             read_msg_.body_length(strlen(read_msg_.body()));
                            }
                           if (read_msg_.ca.hit)
                           { // also need to check in order, since everyone has a turn
                             std::string m; // m is the string to be displayed on client window

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

                               self->player_hand.add_card_hand(table_.Dealer.shoe.next_card(), 0);
                               self->player_hand.set_hand_value(0);

                               std::cout << "Dealing card " << self->player_hand.get_hand_index(0) << " to player:"<< std::endl;
                               table_.Dealer.print_dealt_card();
                               table_.Dealer.shoe.remove_card();

                               //printing hand value of player
                               std::cerr << self->name << " hand value: " << self->player_hand.get_hand_value(0) << std::endl;
                               if (self->player_hand.get_hand_value(0) == 21)
                               {
                                
                                 std::cerr << self->player_hand.get_hand_value(0) << " - Blackjack!" << std::endl;
                                 //go to dealer function play
                                 self->credits = table_.Dealer.play();
                                 m = "New round starting. Place another bet.";
                                 self->bet = 0;
                               }
                               if (self->player_hand.get_hand_value(0) > 21)
                               {
                                self->hand_bust = true;
                                 std::cerr << self->player_hand.get_hand_value(0) << " - Bust! Dealer wins. " << std::endl;
                                 //can end the round adding credits to dealer
                                 self->credits = table_.Dealer.play();
                                 self->bet = 0;
                                 m = "New round starting. Place another bet.";
                               }
                               //table_.Dealer.shoe.remove_card();
                               // refilling shoe
                               table_.Dealer.check_shoe();
                             }
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
                                 m = self->name + "has less than 20 credits. Game will not start.";
                               }
                               else
                               {
                                 std::string m = self->name + " has placed a bet.";
                                 self->bet = read_msg_.ca.bet_amt;
                                 self->credits -= self->bet;
                                 if (table_.Dealer.round && self->bet!=0) // probably have to change to after bets are placed
                                  {
                                    table_.Dealer.round = false;
                                    for (int count = 0; count < 2; count++)
                                    {
                                      self->player_hand.add_card_hand(table_.Dealer.shoe.next_card(), 0);
                                      std::cout << "Dealing card " << self->player_hand.get_hand_index(0) << " to " << self->name << ":\n";
                                      table_.Dealer.print_dealt_card();
                                      table_.Dealer.shoe.remove_card();
                                      
                                      table_.Dealer.dealer_hand.add_card_hand(table_.Dealer.shoe.next_card(), 0);
                                      std::cout << "Dealing card to dealer:\n";
                                      table_.Dealer.print_dealt_card();
                                      table_.Dealer.shoe.remove_card();
                                    }
                                    self->player_hand.set_hand_value(0);
                                    table_.Dealer.dealer_hand.set_hand_value(0);
                                  }
                                 if (self->player_hand.get_hand_value(0) == 21) {
                                   std::cout << "Blackjack on 1st hand!\n";
                                   table_.Dealer.round = true;
                                   self->credits += 1.5 * self->bet;
                                 }
                               }
                             }
                             strcpy(read_msg_.body(), m.c_str());
                             read_msg_.body_length(strlen(read_msg_.body()));
                           }

                           if (read_msg_.ca.split)
                           {
                             std::string m = self->name + " has requested to split.";
                             if (self->player_hand.can_split()) {
                              self->split_bet = self->bet;
                              std::cout << "splitting " << self->name << " cards" << std::endl;
                              self->player_hand.split_cards();
                              self->split = true;
                             }
                             else {
                              std::cout << "cannot split" << std::endl;
                             }
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
                             else if (self->player_hand.get_hand_index(0) == 2)
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

                               std::cout << "dealt card: " << std::endl;
                               table_.Dealer.print_dealt_card();
                               std::cout << "shoe size is now " << table_.Dealer.shoe._current_card + 1 << std::endl;

                               //printing hand value of player
                               std::cerr << "printing hand value of player = " << self->player_hand.get_hand_value(0) << std::endl;
                             }
                             else
                             {
                               std::cout << "You cannot double down after you hit card. hand size is: " << self->player_hand.get_hand_index(0) << std::endl;
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
                               m = self->name + " has requested to double down.";
                             }
                             else
                             {
                               m = self->name + " has requested to stand.";
                             }

                             strcpy(read_msg_.body(), m.c_str());
                             read_msg_.body_length(strlen(read_msg_.body()));

                             
                             self->credits = table_.Dealer.play();
                             m = "New round starting. Place another bet.";
                             self->bet = 0;
                             //std::cerr << "self->bet = " << self->bet << std::endl;
                           }
                           if (read_msg_.ca.split_stand)
                           {
                             std::string m = self->name + " has requested to stand.";
                             
                             if (self->split) {
                              self->split_stand = true;
                             }

                             strcpy(read_msg_.body(), m.c_str());
                             read_msg_.body_length(strlen(read_msg_.body()));
                             //self->credits = table_.Dealer.play();
                             //self->bet = 0;
                           }

                           if (read_msg_.ca.surrender)
                           {
                             std::string m;
                             m = self->name + " has requested to surrender.";
                             //we can some functions in hand to clear its contents in _Hand array
                             //bet deducted in player
                             //clear dealer hand
                             if (self->split) {
                              std::cout << "Cannot surrender after splitting.\n";
                             }
                             else {
                                if(self->player_hand.get_hand_value(0) == 21)
                                {
                                  std::cout << "Hand value: " << self->player_hand.get_hand_value(0) << std::endl;
                                 //go to dealer function play
                                 self->credits = table_.Dealer.play();
                                 m = "Blackjack! Cannot surrender.";
                                 self->bet = 0;
                                }
                                else{ 
                                  self->credits += 0.5 * self->bet;
                                  table_.Dealer.round  = true;
                                }
                             }
                             strcpy(read_msg_.body(), m.c_str());
                             read_msg_.body_length(strlen(read_msg_.body()));
                           }
                           if (table_.Dealer.round) // reset routine
                           {
                             self->hand_bust = false;
                             self->split_bust = false;
                             self->player_hand.reset_hands();
                             table_.Dealer.dealer_hand.reset_hands();
                             
                           }
                           // display the cards if everyone has joined
                           if (table_.all_players_have_a_name())
                           {
                             read_msg_.gs.player_credits = self->credits;
                             read_msg_.gs.player_hand = self->player_hand;
                             read_msg_.gs.dealer_hand = table_.Dealer.dealer_hand;
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
