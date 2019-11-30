#include <iostream>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <assert.h>
#include "asio.hpp"
#include "chat_message.hpp"
#include <gtk/gtk.h>

GtkWidget *fromView  = NULL;  // text from the chat server
GtkWidget *creditsView = NULL;
GtkWidget *dealerCardsTitle = NULL;
GtkWidget *dealerCards = NULL;
GtkWidget *playerHandTitle = NULL;
GtkWidget *playerHand = NULL;
GtkWidget *rules = NULL;
GtkWidget *rules1 = NULL;
GtkWidget *rules2 = NULL;
GtkWidget *rules3 = NULL;
GtkWidget *rules4 = NULL;

using asio::ip::tcp;

typedef std::deque<chat_message> chat_message_queue;

class chat_client {
  public:
    chat_client(asio::io_context& io_context, const tcp::resolver::results_type& endpoints)
    : io_context_(io_context), socket_(io_context) {
      do_connect(endpoints);
    }

    void write(const chat_message& msg) {
      asio::post(io_context_, [this, msg]() {
        bool write_in_progress = !write_msgs_.empty();
        write_msgs_.push_back(msg);
        if (!write_in_progress) {
          do_write();
        }
      });
    }

    void close() {
      asio::post(io_context_, [this]() { socket_.close(); });
    }

  private:
    void do_connect(const tcp::resolver::results_type& endpoints) {
      asio::async_connect(socket_, endpoints, [this](std::error_code ec, tcp::endpoint) {
        if (!ec) {
          do_read_header();
        }
      });
    }

    void do_read_header() {
      asio::async_read(socket_, asio::buffer(read_msg_.data(), chat_message::header_length),
      [this](std::error_code ec, std::size_t /*length*/) {
        if (!ec && read_msg_.decode_header()) {
          do_read_body();
        }
        else {
          socket_.close();
        }
      });
    }

    void do_read_body() {
      asio::async_read(socket_, asio::buffer(read_msg_.body(), read_msg_.body_length()),
      [this](std::error_code ec, std::size_t /*length*/) {
        if (!ec) {
          gdk_threads_enter();
          {
            char outline[read_msg_.body_length() + 2]; // '\n' + '\0' is 2 more chars
            outline[0] = '\n';
            outline[read_msg_.body_length() + 1] = '\0';
            std::memcpy ( &outline[1], read_msg_.body(), read_msg_.body_length() );
            gtk_label_set_text (GTK_LABEL(fromView), outline);
            
            std::string p_hand, d_hand, p_split_hand;
            std::string value, face, suit;
            p_hand = "Regular hand\n-----------\n";
            p_split_hand = "Split hand\n-----------\n";

            read_msg_.gs.player_hand.set_hand_value(0);
            read_msg_.gs.player_hand.set_hand_value(1);
            
            // player cards
            for (int count = 0; count < read_msg_.gs.player_hand.get_hand_index(0); count++) {
              p_hand += "V->" + std::to_string(read_msg_.gs.player_hand.get_hand(0)[count].get_value()) + " | ";
              p_hand += "F->" + std::to_string(read_msg_.gs.player_hand.get_hand(0)[count].get_face()) + " | ";
              p_hand += "S->" + std::to_string(read_msg_.gs.player_hand.get_hand(0)[count].get_suit()) + "\n";
            }
            for (int count = 0; count < read_msg_.gs.player_hand.get_hand_index(1); count++) {
              p_split_hand += "V->" + std::to_string(read_msg_.gs.player_hand.get_hand(1)[count].get_value()) + " | ";
              p_split_hand += "F->" + std::to_string(read_msg_.gs.player_hand.get_hand(1)[count].get_face()) + " | ";
              p_split_hand += "S->" + std::to_string(read_msg_.gs.player_hand.get_hand(1)[count].get_suit()) + "\n";
            }
            
            // dealer cards
            for (int count = 0; count < read_msg_.gs.dealer_hand.get_hand_index(0); count++) {
              d_hand += "V->" + std::to_string(read_msg_.gs.dealer_hand.get_hand(0)[count].get_value()) + " | ";
              d_hand += "F->" + std::to_string(read_msg_.gs.dealer_hand.get_hand(0)[count].get_face()) + " | ";
              d_hand += "S->" + std::to_string(read_msg_.gs.dealer_hand.get_hand(0)[count].get_suit()) + "\n";
            }

            p_hand += p_split_hand;
            p_hand += "Hand value: " + std::to_string(read_msg_.gs.player_hand.get_hand_value(0)) + "\n";
            p_hand += "Split value: " + std::to_string(read_msg_.gs.player_hand.get_hand_value(1)) + "\n";
            d_hand += "Hand value: " + std::to_string(read_msg_.gs.dealer_hand.get_hand_value(0));

            if (read_msg_.gs.player_hand.get_hand_index(0)) {
              gtk_label_set_text (GTK_LABEL(playerHand), p_hand.c_str() );
            }
            else {
              gtk_label_set_text (GTK_LABEL(playerHand), "(empty)");
            }

            if (read_msg_.gs.dealer_hand.get_hand_index(0)) {
              gtk_label_set_text (GTK_LABEL(dealerCards), d_hand.c_str() );
            }
            else {
              gtk_label_set_text (GTK_LABEL(dealerCards), "(empty)");
            }

            gtk_label_set_justify(GTK_LABEL(playerHand), GTK_JUSTIFY_CENTER);
            gtk_label_set_justify(GTK_LABEL(dealerCards), GTK_JUSTIFY_CENTER);
            std::string credits_text = "Credits: " + std::to_string(read_msg_.gs.player_credits) + "\n";
            credits_text += "\tBet: " + std::to_string(read_msg_.gs.player_bet) + "\n";
            credits_text += "\tSplit Bet: " + std::to_string(read_msg_.gs.player_split_bet);

            gtk_label_set_text(GTK_LABEL(creditsView), credits_text.c_str());
          }
          gdk_threads_leave();

          std::cout.write(read_msg_.body(), read_msg_.body_length());
          std::cout << "\n";
          do_read_header();
        }
        else {
          socket_.close();
        }
      });
    }

    void do_write() {
      asio::async_write(socket_, asio::buffer(write_msgs_.front().data(),
      write_msgs_.front().length()), [this](std::error_code ec, std::size_t /*length*/) {
        if (!ec) {
          write_msgs_.pop_front();
          if (!write_msgs_.empty()) {
            do_write();
          }
        }
        else {
          socket_.close();
        }
      });
    }

  private:
    asio::io_context& io_context_;
    tcp::socket socket_;
    chat_message read_msg_;
    chat_message_queue write_msgs_;
};

// global symbols
chat_client *c;

static void destroy_event(GtkWidget *widget, gpointer data) {
   gtk_main_quit();
}

static gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data) {
   return FALSE; // must return false to trigger destroy event for window
}

static void hitCallback(GtkWidget *widget, GdkEventButton *event, gpointer callback_data) {
  std::cerr << "Hit button pressed: " << event->button << std::endl;
  chat_message msg;
  msg.body_length (0);
  msg.ca.hit = true;
  msg.encode_header();
  assert(c);
  c->write(msg);
}

static void hitSplitCallback(GtkWidget *widget, GdkEventButton *event, gpointer callback_data) {
  std::cerr << "Hit Split button pressed: " << event->button << std::endl;
  chat_message msg;
  msg.body_length (0);
  msg.ca.hit_split = true;
  msg.encode_header();
  assert(c);
  c->write(msg);
}

static void standCallback(GtkWidget *widget, GdkEventButton *event, gpointer callback_data) {
  std::cerr << "Stand button pressed: " << event->button << std::endl;
  chat_message msg;
  msg.body_length (0);
  msg.ca.stand = true;
  msg.encode_header();
  assert(c);
  c->write(msg);
}

static void splitStandCallback(GtkWidget *widget, GdkEventButton *event, gpointer callback_data) {
  std::cerr << "Split Stand button pressed: " << event->button << std::endl;
  chat_message msg;
  msg.body_length (0);
  msg.ca.split_stand = true;
  msg.encode_header();
  assert(c);
  c->write(msg);
}

static void surrenderCallback(GtkWidget *widget, GdkEventButton *event, gpointer callback_data) {
  std::cerr << "Surrender button pressed: " << event->button << std::endl;
  chat_message msg;
  msg.body_length (0);
  msg.ca.surrender = true;
  msg.encode_header();
  assert(c);
  c->write(msg);
}

static void splitCallback(GtkWidget *widget, GdkEventButton *event, gpointer callback_data) {
  std::cerr << "Split button pressed: " << event->button << std::endl;
  chat_message msg;
  msg.body_length (0);
  msg.ca.split = true;
  msg.encode_header();
  assert(c);
  c->write(msg);
}

static void double_downCallback(GtkWidget *widget, GdkEventButton *event, gpointer callback_data) {
  std::cerr << "Double Down button pressed: " << event->button << std::endl;
  chat_message msg;
  msg.body_length (0);
  msg.ca.double_down = true;
  msg.encode_header();
  assert(c);
  c->write(msg);
}

static void joinCallback(GtkWidget *widget, GdkEventButton *event, gpointer callback_data) {
  std::cerr << "Join button pressed: " << event->button << std::endl;

  GtkWidget *callbackWidget = (GtkWidget*) callback_data;
  assert(callbackWidget);

  GtkTextBuffer *toBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(callbackWidget));
  GtkTextIter start;
  GtkTextIter end;
  gtk_text_buffer_get_start_iter(toBuffer, &start);
  gtk_text_buffer_get_end_iter(toBuffer, &end);

  gchar *text = gtk_text_buffer_get_text(toBuffer, &start, &end, FALSE);
  if (text){
    chat_message msg;
    msg.body_length (0);
    msg.ca.join = true;
    msg.ca.name_valid = true;
    if (strlen(text) < sizeof(msg.ca.name)) {
      strcpy(msg.ca.name, text);
    }
    msg.encode_header();
    assert (c);
    c->write(msg);
  }
}

static void betCallback(GtkWidget *widget, GdkEventButton *event, gpointer callback_data) {
  std::cerr << "Bet button pressed: " << event->button << std::endl;

  GtkWidget *callbackWidget = (GtkWidget*)callback_data;
  assert(callbackWidget);

  GtkTextBuffer *betBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW (callbackWidget));
  GtkTextIter start;
  GtkTextIter end;
  gtk_text_buffer_get_start_iter(betBuffer, &start);
  gtk_text_buffer_get_end_iter(betBuffer, &end);

  gchar *text = gtk_text_buffer_get_text(betBuffer, &start, &end, FALSE);
  
  chat_message msg;
  msg.body_length (0);
  msg.ca.bet = true;
  msg.ca.bet_amt = atoi(text);
  msg.encode_header();
  assert(c);
  c->write(msg);
}

static void rulesCallback(GtkWidget *widget, GdkEventButton *event, gpointer callback_data) {
  std::cerr << "Rules button pressed: " << event->button << std::endl;
  GtkWidget *rulesWindow = NULL;
  GtkWidget *rules = NULL;
  GtkWidget *rules1 = NULL;
  GtkWidget *rules2 = NULL;
  GtkWidget *rules3 = NULL;
  GtkWidget *rules4 = NULL;
  rulesWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  
  rules = gtk_label_new(NULL);
  rules1 = gtk_label_new(NULL);
  rules2 = gtk_label_new(NULL);
  rules3 = gtk_label_new(NULL);
  rules4 = gtk_label_new(NULL);

  gtk_label_set_text(GTK_LABEL(rules), "1. Hand sign must be used by player as guided by the dealer. Player are not allowed to touch cards.");
  gtk_label_set_text(GTK_LABEL(rules1), "2. The minimum bet player can do in 1 credit and max bet is 5 credits.");
  gtk_label_set_text(GTK_LABEL(rules2), "3. Blackjack pays 1.5x the placed bet. Otherwise, winning pays 2x the placed bet.");
  gtk_label_set_text(GTK_LABEL(rules3), "4. Players are strictly restricted to count cards.");
  gtk_label_set_text(GTK_LABEL(rules4), "5. Dealer has to hit always till it reach soft 17.");

  GtkWidget *rulesvbox = gtk_vbox_new (false, 12);
  gtk_container_add(GTK_CONTAINER(rulesWindow), rulesvbox);

  gtk_box_pack_start(GTK_BOX(rulesvbox), rules, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(rulesvbox), rules1, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(rulesvbox), rules2, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(rulesvbox), rules3, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(rulesvbox), rules4, TRUE, TRUE, 0);

  gtk_widget_show_all(rulesWindow);
}

int main(int argc, char *argv[]) {
  gdk_threads_init();
  gdk_threads_enter();

  gtk_init(&argc, &argv);

  GtkWidget *window = NULL;
  GtkWidget *debugWindow = NULL; // for debugging

  GtkWidget *hitButton,*joinButton,*standButton,*splitStandButton, *surrenderButton, *splitButton, *double_downButton, *hitSplitButton, *betButton, *rulesButton;
  GtkWidget *toView  = NULL;  // text to the chat server
  GtkWidget *betView = NULL;

  GtkTextBuffer *toBuffer = NULL;
  GtkTextBuffer *betBuffer = NULL;

  toView = gtk_text_view_new();
  betView = gtk_text_view_new();
  fromView = gtk_label_new(NULL);
  creditsView = gtk_label_new(NULL);
 
  playerHandTitle = gtk_label_new(NULL);
  dealerCardsTitle = gtk_label_new(NULL);
  dealerCards = gtk_label_new(NULL);
  playerHand = gtk_label_new(NULL);
  
  toBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW (toView));
  betBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW (betView));

  gtk_text_buffer_set_text (toBuffer, "Enter your name here", -1);
  gtk_text_buffer_set_text (betBuffer, "Place a bet", -1);
  gtk_label_set_text (GTK_LABEL(fromView), "");
  gtk_label_set_text (GTK_LABEL(creditsView), "Credits: ");
  
  std::string p_title = "Your cards:";
  std::string d_title = "Dealer cards:";
  gtk_label_set_text(GTK_LABEL(playerHandTitle), p_title.c_str());
  gtk_label_set_text(GTK_LABEL(dealerCardsTitle), d_title.c_str());

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  debugWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);  // for debugging
  hitButton = gtk_button_new_with_label("Hit");
  hitSplitButton = gtk_button_new_with_label("Hit Split Hand");
  standButton = gtk_button_new_with_label("Stand");
  splitStandButton = gtk_button_new_with_label("Stand Split Hand");
  betButton = gtk_button_new_with_label("Bet");
  joinButton = gtk_button_new_with_label("Join");
  surrenderButton = gtk_button_new_with_label("Surrender");
  splitButton = gtk_button_new_with_label("Split");
  double_downButton = gtk_button_new_with_label("Double Down");
  rulesButton = gtk_button_new_with_label("Rules");

  GtkWidget *vbox = gtk_vbox_new(false, 10);
  gtk_container_add(GTK_CONTAINER(window), vbox);

  GtkWidget *debugVbox = gtk_vbox_new(false, 10); // for debugging
  gtk_container_add(GTK_CONTAINER(debugWindow), debugVbox); // for debugging

  // make and add labels to debugVbox

  gtk_box_pack_start(GTK_BOX(vbox), toView, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), joinButton, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), betView, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), betButton, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), hitButton, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), hitSplitButton, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), standButton, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), splitStandButton, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), splitButton, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), double_downButton, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), surrenderButton, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), fromView, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), playerHandTitle, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), playerHand, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), dealerCardsTitle, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), dealerCards, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), creditsView, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), rulesButton, TRUE, TRUE, 0);

  g_signal_connect(G_OBJECT(hitButton), "button_press_event", G_CALLBACK(hitCallback), fromView);
  g_signal_connect(G_OBJECT(hitSplitButton), "button_press_event", G_CALLBACK(hitSplitCallback), fromView);
  g_signal_connect(G_OBJECT(standButton), "button_press_event", G_CALLBACK(standCallback), fromView);
  g_signal_connect(G_OBJECT(splitStandButton), "button_press_event", G_CALLBACK(splitStandCallback), fromView);
  g_signal_connect(G_OBJECT(surrenderButton), "button_press_event", G_CALLBACK(surrenderCallback), fromView);
  g_signal_connect(G_OBJECT(splitButton), "button_press_event", G_CALLBACK(splitCallback), fromView);
  g_signal_connect(G_OBJECT(double_downButton), "button_press_event", G_CALLBACK(double_downCallback), fromView);
  g_signal_connect(G_OBJECT(joinButton), "button_press_event", G_CALLBACK(joinCallback), toView);
  g_signal_connect(G_OBJECT(betButton), "button_press_event", G_CALLBACK(betCallback), betView);
  g_signal_connect(G_OBJECT(rulesButton), "button_press_event", G_CALLBACK(rulesCallback), fromView);
  
  gtk_widget_set_size_request(window, 400, 800);
  gtk_widget_show_all(window);

  gtk_widget_set_size_request(debugWindow, 400, 500); // for debugging
  gtk_widget_show_all(debugWindow); // for debugging
  
  g_signal_connect(G_OBJECT(window), "delete_event", G_CALLBACK(delete_event), NULL);
  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(destroy_event), NULL);

  g_signal_connect(G_OBJECT(debugWindow), "delete_event", G_CALLBACK(delete_event), NULL);  // for debugging
  g_signal_connect(G_OBJECT(debugWindow), "destroy", G_CALLBACK(destroy_event), NULL);  // for debugging

  if (argc != 3) {
    std::cerr << "Usage: chat_client <host> <port>\n";
    return 1;
  }

  asio::io_context io_context;

  tcp::resolver resolver(io_context);
  auto endpoints = resolver.resolve(argv[1], argv[2]);
  c = new chat_client(io_context, endpoints);
  assert(c);
  std::thread t([&io_context](){ io_context.run(); });

  gtk_main();

  gdk_threads_leave();
  
  c->close();
  t.join();

  return 0;
}
