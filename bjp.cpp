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
GtkWidget *dealerCards = NULL;
GtkWidget *playerCards = NULL;
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
            /*
            std::string p = "player " + std::to_string (read_msg_.gs.player_cards[0][0]) + " " +
                                        std::to_string (read_msg_.gs.player_cards[0][1]) + " " +
                                        std::to_string (read_msg_.gs.player_cards[0][2]) + '\n' +
                                        std::to_string (read_msg_.gs.player_cards[1][0]) + " " +
                                        std::to_string (read_msg_.gs.player_cards[1][1]) + " " +
                                        std::to_string (read_msg_.gs.player_cards[1][2]);

            std::string d = "dealer " + std::to_string (read_msg_.gs.dealer_cards[0]) + " " +
                                        std::to_string (read_msg_.gs.dealer_cards[1]) + " " +
                                        std::to_string (read_msg_.gs.dealer_cards[2]);
            */
            if (read_msg_.gs.player_cards_valid) {
              //gtk_label_set_text (GTK_LABEL(playerCards),p.c_str() );
            }
            else {
              gtk_label_set_text (GTK_LABEL(playerCards),"waiting");
            }

            if (read_msg_.gs.dealer_cards_valid) {
              //gtk_label_set_text (GTK_LABEL(dealerCards),d.c_str() );
            }
            else {
              gtk_label_set_text (GTK_LABEL(dealerCards),"waiting");
            }
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
  std::cerr << "hit button pressed: " << event->button << std::endl;
  chat_message msg;
  msg.body_length (0);
  msg.ca.hit = true;
  msg.ca.bet = false;
  msg.ca.stand = false;
  msg.ca.surrender = false;
  msg.ca.split = false;
  msg.ca.doubleDown = false;
  msg.ca.name_valid = false;
  msg.encode_header();
  assert(c);  // this is a global class
  c->write(msg);
}

static void standCallback(GtkWidget *widget, GdkEventButton *event, gpointer callback_data) {
  std::cerr << "stand button pressed: " << event->button << std::endl;
  chat_message msg;
  msg.body_length (0);
  msg.ca.hit = false;
  msg.ca.bet = false;
  msg.ca.stand = true;
  msg.ca.surrender = false;
  msg.ca.split = false;
  msg.ca.doubleDown = false;
  msg.ca.name_valid = false;
  msg.encode_header();
  assert(c);  // this is a global class
  c->write(msg);
}

static void surrenderCallback(GtkWidget *widget, GdkEventButton *event, gpointer callback_data) {
  std::cerr << "surrender button pressed: " << event->button << std::endl;
  chat_message msg;
  msg.body_length (0);
  msg.ca.hit = false;
  msg.ca.stand = false;
  msg.ca.surrender = true;
  msg.ca.split = false;
  msg.ca.bet = false;
  msg.ca.doubleDown = false;
  msg.ca.name_valid = false;
  msg.encode_header();
  assert(c);  // this is a global class
  c->write(msg);
}

static void splitCallback(GtkWidget *widget, GdkEventButton *event, gpointer callback_data) {
  std::cerr << "split button pressed: " << event->button << std::endl;
  chat_message msg;
  msg.body_length (0);
  msg.ca.hit = false;
  msg.ca.stand = false;
  msg.ca.surrender = false;
  msg.ca.bet = false;
  msg.ca.split = true;
  msg.ca.doubleDown = false;
  msg.ca.name_valid = false;
  msg.encode_header();
  assert(c);  // this is a global class
  c->write(msg);
}

static void doubleDownCallback(GtkWidget *widget, GdkEventButton *event, gpointer callback_data) {
  std::cerr << "double down button pressed: " << event->button << std::endl;
  chat_message msg;
  msg.body_length (0);
  msg.ca.hit = false;
  msg.ca.stand = true;
  msg.ca.surrender = false;
  msg.ca.bet = false;
  msg.ca.split = false;
  msg.ca.doubleDown = true;
  msg.ca.name_valid = false;
  msg.encode_header();
  assert(c);  // this is a global class
  c->write(msg);
}

static void joinCallback(GtkWidget *widget, GdkEventButton *event, gpointer callback_data) {
  std::cerr << "join button pressed: " << event->button << std::endl;

  GtkWidget *callbackWidget = (GtkWidget*) callback_data;
  assert ( callbackWidget );

  GtkTextBuffer *toBuffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (callbackWidget));

  GtkTextIter start;
  GtkTextIter end;

  gtk_text_buffer_get_start_iter ( toBuffer, &start );
  gtk_text_buffer_get_end_iter ( toBuffer, &end );

  gchar *text = gtk_text_buffer_get_text ( toBuffer, &start, &end, FALSE );

  if (text){
    chat_message msg;
    msg.body_length (0);
    msg.ca.hit = false;
    msg.ca.stand = false;
    msg.ca.surrender = false;
    msg.ca.doubleDown = false;
    msg.ca.bet = false;
    msg.ca.split = false;
    msg.ca.join = true;
    msg.ca.name_valid = true;
    if (strlen(text) < sizeof(msg.ca.name)) {
      strcpy(msg.ca.name,text);
    }
    msg.encode_header();
    assert (c);
    c->write(msg);
  }
   //gtk_text_buffer_set_text ( toBuffer, "", -1 );
}

static void betCallback(GtkWidget *widget, GdkEventButton *event, gpointer callback_data) {
  std::cerr << "bet button pressed: " << event->button << std::endl;

  GtkWidget *callbackWidget = (GtkWidget*) callback_data;
  assert ( callbackWidget );

  GtkTextBuffer *toBuffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (callbackWidget));

  GtkTextIter start;
  GtkTextIter end;

  gtk_text_buffer_get_start_iter ( toBuffer, &start );
  gtk_text_buffer_get_end_iter ( toBuffer, &end );

  gchar *text = gtk_text_buffer_get_text ( toBuffer, &start, &end, FALSE );
  /*
  int bet_amt = atoi(text);
  if (bet_amt == 0) {
    std::cerr << "invalid bet" << std::endl;
  }
  */
  chat_message msg;
  msg.body_length (0);
  msg.ca.hit = false;
  msg.ca.doubleDown = false;
  msg.ca.stand = false;
  msg.ca.surrender = false;
  msg.ca.name_valid = false;
  msg.ca.split = false;
  msg.ca.bet = true;
  msg.ca.bet_amt = atoi(text);
  /*
  while (atoi(text) > 5 || atoi(text) < 1) {
    std::cerr << "bet must be from 1 to 5 credits" << std::endl;
    msg.ca.bet_amt = atoi(text);
    
  }
  //std::cout << "credit_amt = " << credit_amt << std::endl;
  
  if (bet_amt >= 1) {
    if (bet_amt <= msg.ca.credits) {
      msg.ca.bet = bet_amt;
      msg.ca.credits -= bet_amt;
    }
    else {
      std::cerr << "not enough credits to place bet" << std::endl;
      msg.ca.bet = 0;
    }
    std::cerr << "hello... credits is " << msg.ca.credits << std::endl;
  }
  */
  msg.encode_header();
  assert(c);
  c->write(msg);
   //gtk_text_buffer_set_text ( toBuffer, "", -1 );
}

static void rulesCallback(GtkWidget *widget, GdkEventButton *event, gpointer callback_data) {
  std::cerr << "rules button pressed: " << event->button << std::endl;
  GtkWidget *rulesWindow = NULL;
  //rulesWindow->set_title("Rules");
  rulesWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  GtkWidget *rules = NULL;
  rules = gtk_label_new (NULL);
  gtk_label_set_text (GTK_LABEL(rules), "1. Hand sign must be used by player as guided by the dealer. Player are not allowed to touch cards.");
  
  GtkWidget *rules1 = NULL;
  rules1 = gtk_label_new (NULL);
  gtk_label_set_text (GTK_LABEL(rules1),"2. The minimum bet player can do in $1 and max bet is $5.");

  GtkWidget *rules2 = NULL;
  rules2 = gtk_label_new (NULL);
  gtk_label_set_text (GTK_LABEL(rules2),"3. Will be payed 1.5 times the bet if player got blackjack.");

  GtkWidget *rules3 = NULL;
  rules3 = gtk_label_new (NULL);
  gtk_label_set_text (GTK_LABEL(rules3),"4. Players are strictly restricted to count cards.");

  GtkWidget *rules4 = NULL;
  rules4 = gtk_label_new (NULL);
  gtk_label_set_text (GTK_LABEL(rules4),"5. Dealer has to hit always till it reach soft 17.s");

  GtkWidget *  rulesvbox = gtk_vbox_new (false, 10);
  gtk_container_add(GTK_CONTAINER(rulesWindow), rulesvbox);

  gtk_box_pack_start(GTK_BOX(rulesvbox),rules, TRUE,TRUE,0);
  gtk_box_pack_start(GTK_BOX(rulesvbox),rules1, TRUE,TRUE,0);
  gtk_box_pack_start(GTK_BOX(rulesvbox),rules2, TRUE,TRUE,0);
  gtk_box_pack_start(GTK_BOX(rulesvbox),rules3, TRUE,TRUE,0);
  gtk_box_pack_start(GTK_BOX(rulesvbox),rules4, TRUE,TRUE,0);

  gtk_widget_show_all(rulesWindow);
  /*
  Rules_window *rules = new Rules_window();
  rules->show();
  
  chat_message msg;
  msg.body_length (0);
  msg.ca.hit = false;
  msg.ca.stand = false;
  msg.ca.surrender = false;
  msg.ca.split = false;
  msg.ca.doubleDown = false;
  msg.ca.name_valid = false;
  msg.encode_header();
  assert(c);  // this is a global class
  c->write(msg);
  */
}

int main(int argc, char *argv[]) {
  gdk_threads_init();
  gdk_threads_enter();

  gtk_init( &argc, &argv);

  GtkWidget *window = NULL;

  GtkWidget *hitButton,*joinButton,*standButton,*surrenderButton,*splitButton,*doubleDownButton, *betButton,*rulesButton = NULL;
  GtkWidget *toView  = NULL;  // text to the chat server

  GtkTextBuffer *toBuffer = NULL;

  toView = gtk_text_view_new ();
  fromView = gtk_label_new (NULL);
 
  dealerCards = gtk_label_new (NULL);
  playerCards = gtk_label_new (NULL);
  
  toBuffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (toView));

  gtk_text_buffer_set_text (toBuffer, "Enter your name here", -1);
  gtk_label_set_text (GTK_LABEL(fromView), "");

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  hitButton = gtk_button_new_with_label("Hit");
  standButton = gtk_button_new_with_label("Stand");
  betButton = gtk_button_new_with_label("Bet");
  joinButton = gtk_button_new_with_label("Join");
  surrenderButton = gtk_button_new_with_label("Surrender");
  splitButton = gtk_button_new_with_label("Split");
  doubleDownButton = gtk_button_new_with_label("Double Down");
  rulesButton = gtk_button_new_with_label("Rules");

  GtkWidget *  vbox = gtk_vbox_new (false, 10);
  gtk_container_add(GTK_CONTAINER(window), vbox);

  gtk_box_pack_start(GTK_BOX(vbox),hitButton, TRUE,TRUE,0);
  gtk_box_pack_start(GTK_BOX(vbox),standButton, TRUE,TRUE,0);
  gtk_box_pack_start(GTK_BOX(vbox),surrenderButton, TRUE,TRUE,0);
  gtk_box_pack_start(GTK_BOX(vbox),splitButton, TRUE,TRUE,0);
  gtk_box_pack_start(GTK_BOX(vbox),doubleDownButton, TRUE,TRUE,0);
  gtk_box_pack_start(GTK_BOX(vbox),joinButton, TRUE,TRUE,0);
  gtk_box_pack_start(GTK_BOX(vbox),toView, TRUE,TRUE,0);
  gtk_box_pack_start(GTK_BOX(vbox),betButton, TRUE,TRUE,0);
  gtk_box_pack_start(GTK_BOX(vbox),fromView, TRUE,TRUE,0);
  gtk_box_pack_start(GTK_BOX(vbox),dealerCards, TRUE,TRUE,0);
  gtk_box_pack_start(GTK_BOX(vbox),playerCards, TRUE,TRUE,0);
  gtk_box_pack_start(GTK_BOX(vbox),rulesButton, TRUE,TRUE,0);

  g_signal_connect(G_OBJECT(hitButton), "button_press_event", G_CALLBACK(hitCallback), fromView);
  g_signal_connect(G_OBJECT(standButton), "button_press_event", G_CALLBACK(standCallback), fromView);
  g_signal_connect(G_OBJECT(surrenderButton), "button_press_event", G_CALLBACK(surrenderCallback), fromView);
  g_signal_connect(G_OBJECT(splitButton), "button_press_event", G_CALLBACK(splitCallback), fromView);
  g_signal_connect(G_OBJECT(doubleDownButton), "button_press_event", G_CALLBACK(doubleDownCallback), fromView);
  g_signal_connect(G_OBJECT(joinButton), "button_press_event", G_CALLBACK(joinCallback), toView);
  g_signal_connect(G_OBJECT(betButton), "button_press_event", G_CALLBACK(betCallback), toView);
  g_signal_connect(G_OBJECT(rulesButton), "button_press_event", G_CALLBACK(rulesCallback), fromView);

  gtk_widget_show_all(window);
  g_signal_connect(G_OBJECT(window), "delete_event", G_CALLBACK(delete_event), NULL);
  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(destroy_event), NULL);

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
