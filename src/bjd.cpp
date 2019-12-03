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
  Dealer dealer;

private:
  std::set<player_ptr> players;
  enum
  {
    max_recent_msgs = 100
  };
  chat_message_queue recent_msgs_;
};

class blackjack_player : public Player, public std::enable_shared_from_this<blackjack_player>
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
                                   // Here is where messages arrive from the client. This is where the design makes it easy or hard.
                                   // ignore anything in the message body
                 read_msg_.body_length(0);
                 if (read_msg_.ca.join && read_msg_.ca.name_valid)
                 {
                                     if (!self->joined) // if player is brand new
                                     {
                                       table_.dealer.next_player(self);
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

      //////////////////////////////////////////////////////
      //    B E T
      //////////////////////////////////////////////////////

                                     if (read_msg_.ca.bet)
                                     {
                                       std::string m;
                                       if (self->bet)
                                       {
                                         m = self->name + " already has a bet.";
                                       }
                                       else
                                       {
                                         if (read_msg_.ca.bet_amt < 1 || read_msg_.ca.bet_amt > 5) // includes failed atoi case
                                         {
                                           m = "Invalid bet. Bet must be from 1 to 5 credits.";
                                         }
                                         else if (read_msg_.ca.bet_amt > self->credits)
                                         {
                                           m = self->name + " does not have enough credits.";
                                         }
                                         else if (self->credits < 20)
                                         {
                                           m = self->name + " has less than 20 credits. Game will not start.";
                                         }
                                         else
                                         { 
                                           m = self->name + " has placed a bet.";
                                           self->bet = read_msg_.ca.bet_amt;
                                           self->credits -= self->bet;
                                           std::cout << self->name + " has placed a bet of " << self->bet << std::endl;

                                            // first 2 cards dealt only if round is new and bet has been placed
                                            if (table_.dealer.round && self->bet != 0) // probably have to change to after bets are placed
                                            {
                                              table_.dealer.round = false;
                                              for (int count = 0; count < 2; count++)
                                              {
                                                self->player_hand.add_card_hand(table_.dealer.shoe.next_card(), 0);
                                                std::cout << "Dealing card to " << self->name << ":\n";
                                                table_.dealer.print_dealt_card();
                                                table_.dealer.shoe.remove_card();
                                                
                                                table_.dealer.dealer_hand.add_card_hand(table_.dealer.shoe.next_card(), 0);
                                                std::cout << "Dealing card to dealer:\n";
                                                table_.dealer.print_dealt_card();
                                                table_.dealer.shoe.remove_card();
                                              }
                                              self->player_hand.set_hand_value(0);
                                              table_.dealer.dealer_hand.set_hand_value(0);
                                              std::cout << self->name << " hand value: " << self->player_hand.get_hand_value(0) << std::endl;
                                              std::cout << "Dealer hand value: " << table_.dealer.dealer_hand.get_hand_value(0) << "\n\n";
                                            }
                                            if (self->player_hand.get_hand_value(0) == 21)
                                            {
                                              std::cout << self->name << " has gotten blackjack on 1st hand.\n";
                                              m = "Blackjack on 1st hand!";
                                              table_.dealer.dealer_credits -= 0.5 * self->bet;
                                              self->credits += 1.5 * self->bet;
                                              self->bet = 0;
                                              table_.dealer.round = true;
                                            }
                                          }
                                        }

                                        strcpy(read_msg_.body(), m.c_str());
                                        read_msg_.body_length(strlen(read_msg_.body()));
                                      }

      //////////////////////////////////////////////////////////////////////////////////////////////////////////
      //    S P L I T
      //////////////////////////////////////////////////////////////////////////////////////////////////////////

                                      if (read_msg_.ca.split)
                                      {
                                       //std::string m = self->name + " has requested to split.";
                                       std::string m;
                                       std::cout << self->name << " has requested to split." << std::endl;

                                       if (self->player_hand.can_split()) {
                                        self->split_bet = self->bet;
                                        std::cout << "Splitting " << self->name << "'s cards." << std::endl;
                                        m = "Splitting cards.";
                                        self->player_hand.split_cards();
                                        self->split = true;
                                      }
                                      else {
                                        std::cout << "Cannot split " << self->name << "'s cards." << std::endl;
                                        m = "Cannot split! Cards not of identical value.";
                                      }

                                      strcpy(read_msg_.body(), m.c_str());
                                      read_msg_.body_length(strlen(read_msg_.body()));
                                    }

      //////////////////////////////////////////////////////////////////////////////////////////////////////////
      //    H I T   S P L I T
      //////////////////////////////////////////////////////////////////////////////////////////////////////////

                                    // when the round is new AND player has placed a bet, give dealer and players their cards
                                    if (read_msg_.ca.hit_split)
                                    {
                                      std::string m;
                                      if (!self->split)
                                      {
                                        m = "Must split hand first.";
                                      }
                                      else {
                                        std::cout << self->name + " has requested to hit on split hand.\n";
                                        self->player_hand.add_card_hand(table_.dealer.shoe.next_card(), 1);
                                        std::cout << "Dealing card to " << self->name << std::endl;
                                        table_.dealer.print_dealt_card();
                                        table_.dealer.shoe.remove_card();
                                        self->player_hand.set_hand_value(1);
                                        if (self->player_hand.get_hand_value(1) == 21)
                                        {

                                        }
                                      }
                                      strcpy(read_msg_.body(), m.c_str());
                                      read_msg_.body_length(strlen(read_msg_.body()));
                                    }

      //////////////////////////////////////////////////////////////////////////////////////////////////////////
      //    H I T
      //////////////////////////////////////////////////////////////////////////////////////////////////////////

                                    if (read_msg_.ca.hit)
                                      { // also need to check in order, since everyone has a turn
                                        std::string m;
                                        if (!self->bet)
                                        {
                                         m = self->name + " has not placed a bet.";
                                       }
                                       else
                                       {
                                         // call the dealer class with some kind of method and argument
                                         std::cout << self->name << " has asked for a hit.\n";
                                         m = "Hitting hand.";

                                         self->player_hand.add_card_hand(table_.dealer.shoe.next_card(), 0);
                                         std::cout << "Dealing card to " << self->name << std::endl;
                                         table_.dealer.print_dealt_card();
                                         table_.dealer.shoe.remove_card();

                                         self->player_hand.set_hand_value(0);

                                         std::cout << self->name << " hand value: " << self->player_hand.get_hand_value(0) << std::endl;
                                         if (self->player_hand.get_hand_value(0) == 21)
                                         {
                                            self->hand_stand = true;
                                            std::cout << "21!" << std::endl;
                                            m = "21!";
                                          }
                                          if (self->player_hand.get_hand_value(0) > 21)
                                          {
                                            self->hand_bust = true;
                                            m = "Bust! Dealer wins automatically.";
                                            table_.dealer.dealer_credits += self->bet;
                                            self->bet = 0;
                                            //table_.dealer.round = true;
                                          }
                                           // refilling shoe
                                          table_.dealer.check_shoe();
                                        }
                                        strcpy(read_msg_.body(), m.c_str());
                                        read_msg_.body_length(strlen(read_msg_.body()));
                                      }

      //////////////////////////////////////////////////////////////////////////////////////////////////////////
      //    D O U B L E   D O W N
      //////////////////////////////////////////////////////////////////////////////////////////////////////////

                                    if (read_msg_.ca.double_down)
                                    {
                                     std::string m = self->name + " has requested to double down.";
                                     if (self->player_hand.get_hand_index(0) == 2 && !self->split)
                                     {
                                       std::cout << "1. Doubling " << self->name << "'s bet: " << self->bet << std::endl;
                                       self->credits -= self->bet;
                                       self->bet *= 2;

                                       std::cout << "2. Hit" << std::endl;
                                       self->player_hand.add_card_hand(table_.dealer.shoe.next_card(), 0);
                                       table_.dealer.print_dealt_card();
                                       self->player_hand.set_hand_value(0);
                                       table_.dealer.shoe.remove_card();

                                       if (self->player_hand.get_hand_value(0) > 21)
                                       {
                                        m = "Bust!";
                                        table_.dealer.round = true;
                                        table_.dealer.dealer_credits += self->bet;
                                        self->bet = 0;
                                      }
                                      else
                                      {
                                        std::cout << self->name << " hand value: " << self->player_hand.get_hand_value(0) << std::endl;
                                        self->hand_stand = true;
                                        std::cout << "3. Stand" << std::endl;
                                      }

                                         // add dealer's turn here

                                      m = "Doubled down.";
                                    }
                                    else if (!self->bet)
                                    {
                                     m = "Place a bet first.";
                                   }
                                   else
                                   {
                                     m = "Cannot double down.";
                                   }
                                   strcpy(read_msg_.body(), m.c_str());
                                   read_msg_.body_length(strlen(read_msg_.body()));
                                 }


      //////////////////////////////////////////////////////////////////////////////////////////////////////////
      //    S T A N D
      //////////////////////////////////////////////////////////////////////////////////////////////////////////

                                 if (read_msg_.ca.stand)
                                 {
                                   //now is dealer turn maybe method in dealer class to get card in its hand till less then 17 then get total and check with player
                                   std::string m = self->name + " has requested to stand.";

                                   if (self->bet && !self->hand_bust)
                                   {
                                    self->hand_stand = true;
                                    //self->credits = table_.dealer.play();
                                    //m = "New round starting. Place another bet.";
                                    //self->bet = 0;
                                  }
                                  else
                                  {
                                    m = "Place a bet first.";
                                  }

                                  strcpy(read_msg_.body(), m.c_str());
                                  read_msg_.body_length(strlen(read_msg_.body()));
                                }

      //////////////////////////////////////////////////////////////////////////////////////////////////////////
      //    S P L I T   S T A N D
      //////////////////////////////////////////////////////////////////////////////////////////////////////////

                                if (read_msg_.ca.split_stand)
                                {
                                 std::string m = self->name + " has requested to stand on split hand.";

                                 if (self->split)
                                 {
                                  self->split_stand = true;
                                  //self->credits = table_.dealer.play();
                                  //self->bet = 0;
                                }
                                else
                                {
                                  m = "Must have split hand.";
                                }

                                strcpy(read_msg_.body(), m.c_str());
                                read_msg_.body_length(strlen(read_msg_.body()));
                              }

      //////////////////////////////////////////////////////////////////////////////////////////////////////////
      //    S U R R E N D E R
      //////////////////////////////////////////////////////////////////////////////////////////////////////////

                              if (read_msg_.ca.surrender)
                              {
                               std::string m;
                               std::cout << self->name << " has requested to surrender.\n";
                               if (self->split) {
                                std::cout << "Cannot surrender after splitting.\n";
                                m = "Cannot surrender after splitting.";
                              }
                              else if (self->player_hand.get_hand_index(0) != 2)
                              {
                                std::cout << "Can only surrender at the first hand.\n";
                                m = "Can only surrender at the first hand.";
                              }
                              else {
                                if(self->player_hand.get_hand_value(0) == 21)
                                {
                                  m = "Blackjack! Cannot surrender.";
                                  self->credits = table_.dealer.play();
                                  self->bet = 0;
                                }
                                else
                                {
                                  m = "Surrendered.";
                                  table_.dealer.dealer_credits += 0.5;
                                  self->credits += 0.5 * self->bet;
                                  self->bet = 0;
                                }
                                table_.dealer.round  = true;
                              }
                              strcpy(read_msg_.body(), m.c_str());
                              read_msg_.body_length(strlen(read_msg_.body()));
                            }



                             // reset routine
                            if (table_.dealer.round)
                            {
                             self->hand_bust = false;
                             self->split_bust = false;
                             self->hand_stand = false;
                             self->split_stand = false;
                             self->bet = 0; // probably not needed here, but just for assurance
                             self->player_hand.reset_hands();
                             table_.dealer.dealer_hand.reset_hands();
                           }
                           else
                           {
                            if (self->split)
                            {
                              if ((hand_bust && split_bust) || (hand_stand && split_bust) ||
                                 (hand_bust && split_stand) || (hand_stand && split_stand))
                              {
                                table_.dealer.play();
                              }
                            }
                            else
                            {
                              if (hand_stand || hand_bust)
                              {
                                table_.dealer.play();
                              }
                            }
                           }

                           // display information to client window if everyone has joined
                           if (table_.all_players_have_a_name())
                           {
                             read_msg_.gs.player_credits = self->credits;
                             read_msg_.gs.dealer_credits = table_.dealer.dealer_credits;
                             read_msg_.gs.player_split = self->split;
                             read_msg_.gs.player_bet = self->bet;
                             read_msg_.gs.player_bet = self->split_bet;
                             read_msg_.gs.player_hand = self->player_hand;
                             read_msg_.gs.dealer_hand = table_.dealer.dealer_hand;
                           }

                           // send body text
                           read_msg_.encode_header();
                           table_.deliver(read_msg_);
                           do_read_header();
                         }
                       }
                       else
                       {
                         std::cout << self->name << "has left the game." << std::endl;
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
