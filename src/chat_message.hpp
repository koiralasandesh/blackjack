#ifndef CHAT_MESSAGE_HPP
#define CHAT_MESSAGE_HPP

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "hand.h"

class game_state {
  public:
    bool dealer_cards_valid;
    Hand dealer_hand;
    bool player_cards_valid;
    Hand player_hand;
    bool player_split;
    int player_credits;
    int player_bet;
    int player_split_bet;
    int dealer_credits; // for debugging
};

class client_action {
  public:
    bool hit = false;   
    bool stand = false;
    bool surrender = false;
    bool split = false;
    bool split_stand = false;
    bool double_down = false;
    bool hit_split = false;
    bool bet = false;
    bool name_valid = false;
    int bet_amt;
    bool join;
    char name[25];
};

class chat_message { 
  public:
    client_action ca;
    game_state gs;
    
    enum { header_length = 4 + sizeof(client_action) + sizeof(game_state) };
    enum { max_body_length = 512 };

    chat_message() : body_length_(0) {}

    const char* data() const {
      return data_;
    }

    char* data() {
      return data_;
    }

    std::size_t length() const {
      return header_length + body_length_;
    }

    const char* body() const {
      return data_ + header_length;
    }

    char* body() {
      return data_ + header_length;
    }

    std::size_t body_length() const {
      return body_length_;
    }

    void body_length(std::size_t new_length) {
      body_length_ = new_length;
      if (body_length_ > max_body_length) {
        body_length_ = max_body_length;
      }
    }

    bool decode_header() {
      char header[header_length + 1] = "";
      std::strncat(header, data_, 4);
      body_length_ = std::atoi(header);
      
      char *p = data_ + 4; // skip the integer
      std::memcpy(&ca,p,sizeof(client_action));
      std::memcpy(&gs,p+sizeof(client_action),sizeof(game_state));
      
      if (body_length_ > max_body_length) {
        body_length_ = 0;
        return false;
      }
      return true;
    }

    void encode_header() {
      char header[4 + 1] = "";
      std::sprintf(header, "%4d", static_cast<int>(body_length_));
      std::memcpy(data_, header, 4);

      char *p = data_+4; // skip over the int we just extracted
      std::memcpy(p,&ca,sizeof(client_action));
      std::memcpy(p+sizeof(client_action),&gs,sizeof(game_state));
    }
  private:
    char data_[header_length + max_body_length];
    std::size_t body_length_;
};

#endif // CHAT_MESSAGE_HPP

