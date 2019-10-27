#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include "asio.hpp"
#include "chat_message.hpp"

using asio::ip::tcp;

typedef std::deque<chat_message> chat_message_queue;

class chat_client
{
public:
  chat_client(asio::io_context& io_context,
      const tcp::resolver::results_type& endpoints)
    : io_context_(io_context),
      socket_(io_context)
  {
    do_connect(endpoints);
  }

  void write(const chat_message& msg)
  {
    asio::post(io_context_,
        [this, msg]()
        {
          bool write_in_progress = !write_msgs_.empty();
          write_msgs_.push_back(msg);
          if (!write_in_progress)
          {
            do_write();
          }
        });
  }

  void close()
  {
    asio::post(io_context_, [this]() { socket_.close(); });
  }

private:
  void do_connect(const tcp::resolver::results_type& endpoints)
  {
    asio::async_connect(socket_, endpoints,
        [this](std::error_code ec, tcp::endpoint)
        {
          if (!ec)
          {
            do_read_header();
          }
        });
  }

  void do_read_header()
  {
    asio::async_read(socket_,
        asio::buffer(read_msg_.data(), chat_message::header_length),
        [this](std::error_code ec, std::size_t /*length*/)
        {
          if (!ec && read_msg_.decode_header())
          {
            do_read_body();
          }
          else
          {
            socket_.close();
          }
        });
  }

  void do_read_body()
  {
    asio::async_read(socket_,
        asio::buffer(read_msg_.body(), read_msg_.body_length()),
        [this](std::error_code ec, std::size_t /*length*/)
        {
          if (!ec)
          {
            std::cout.write(read_msg_.body(), read_msg_.body_length());
            std::cout << "\n";
            do_read_header();
          }
          else
          {
            socket_.close();
          }
        });
  }

  void do_write()
  {
    asio::async_write(socket_,
        asio::buffer(write_msgs_.front().data(),
          write_msgs_.front().length()),
        [this](std::error_code ec, std::size_t /*length*/)
        {
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

//Bipul
/*
Class Mainwin : public Gtk::Window
{
 public:
   main_win::Mainwin() : vbox(Gtk::ORIENTATION_VERTICAL)
   {
        set_title("WELCOME TO BLACKJACK");
    set_default_size(600,200);

    
    Gtk::Box *vbox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 0));
    add(*vbox);

    Gtk::MenuBar *menubar = Gtk::manage(new Gtk::MenuBar());
        vbox->pack_start(*menubar, Gtk::PACK_SHRINK, 1);
       
       Gtk::MenuItem *menuitem_file = Gtk::manage(new Gtk::MenuItem("_MENU", true));
    menubar->append(*menuitem_file);
    Gtk::Menu *filemenu = Gtk::manage(new Gtk::Menu());
    menuitem_file->set_submenu(*filemenu);
       Gtk::MenuItem *menuitem_play = Gtk::manage(new Gtk::MenuItem("_NEW GAME", true));
    menuitem_play->signal_activate().connect(sigc::mem_fun(*this, &Mainwin::on_quit_click));
    filemenu->append(*menuitem_play);
       Gtk::MenuItem *menuitem_quit = Gtk::manage(new Gtk::MenuItem("_Quit", true));
    menuitem_quit->signal_activate().connect(sigc::mem_fun(*this, &Mainwin::on_quit_click));
    filemenu->append(*menuitem_quit);
       
       Gtk::MenuItem *menuitem_help = Gtk::manage(new Gtk::MenuItem("_            HELP", true));
    menubar->append(*menuitem_help);
    Gtk::Menu *help_menu = Gtk::manage(new Gtk::Menu());
    menuitem_help->set_submenu(*help_menu);
       Gtk::MenuItem *menuitem_rules = Gtk::manage(new Gtk::MenuItem("_RULES", true));
    menuitem_rules->signal_activate().connect(sigc::mem_fun(*this, &Mainwin::on_quit_click));
    help_menu->append(*menuitem_rules);
       
       
Gtk::MenuBar *menubar2 = Gtk::manage(new Gtk::MenuBar());
        vbox->pack_start(*menubar2, Gtk::PACK_SHRINK, 1);
       Gtk::MenuItem *menuitem_dealer = Gtk::manage(new Gtk::MenuItem("                                                                                                  DEALER", true));
          menubar2->append(*menuitem_dealer);
          Gtk::Menu *dealer_menu = Gtk::manage(new Gtk::Menu());
          menuitem_dealer->set_submenu(*dealer_menu);
       Gtk::MenuItem *menuitem_hit = Gtk::manage(new Gtk::MenuItem("_HIT", true));
                menuitem_hit->signal_activate().connect(sigc::mem_fun(*this, &Mainwin::on_quit_click));
                dealer_menu->append(*menuitem_hit);
       
       Gtk::MenuItem *menuitem_s = Gtk::manage(new Gtk::MenuItem("_STAND", true));
                menuitem_s->signal_activate().connect(sigc::mem_fun(*this, &Mainwin::on_quit_click));
                dealer_menu->append(*menuitem_s);
       
       
Gtk::MenuBar *menubar3 = Gtk::manage(new Gtk::MenuBar());
        vbox->pack_start(*menubar3, Gtk::PACK_SHRINK, 1);
       Gtk::MenuItem *menuitem_player1 = Gtk::manage(new Gtk::MenuItem("_   PLAYER1", true));
               menubar3->append(*menuitem_player1);
               Gtk::Menu *player_menu = Gtk::manage(new Gtk::Menu());
               menuitem_player1->set_submenu(*player_menu);
       Gtk::MenuItem *menuitem_hit1 = Gtk::manage(new Gtk::MenuItem("_HIT", true));
          menuitem_hit1->signal_activate().connect(sigc::mem_fun(*this, &Mainwin::on_quit_click));
          player_menu->append(*menuitem_hit1);
       Gtk::MenuItem *menuitem_stand = Gtk::manage(new Gtk::MenuItem("_STAND", true));
                menuitem_stand->signal_activate().connect(sigc::mem_fun(*this, &Mainwin::on_quit_click));
                player_menu->append(*menuitem_stand);
       Gtk::MenuItem *menuitem_double = Gtk::manage(new Gtk::MenuItem("_DOUBLE", true));
                menuitem_double->signal_activate().connect(sigc::mem_fun(*this, &Mainwin::on_quit_click));
                player_menu->append(*menuitem_double);
       
 Gtk::MenuItem *menuitem_credits = Gtk::manage(new Gtk::MenuItem("_            CREDITS", true));
    menubar3->append(*menuitem_credits);
    Gtk::Menu *credits_menu = Gtk::manage(new Gtk::Menu());
    menuitem_credits->set_submenu(*credits_menu);
       Gtk::MenuItem *menuitem_total = Gtk::manage(new Gtk::MenuItem("_TOTAL CREDITS", true));
    menuitem_total->signal_activate().connect(sigc::mem_fun(*this, &Mainwin::on_quit_click));
    credits_menu->append(*menuitem_total);

    vbox->show_all();
   }
    BJP_window::~BJP_window()
    {
        
    }

  */
 protected:
   Gtk::Box *vbox;
   Gtk::Label player;
   Gtk::Button close;

}


int main(int argc, char* argv[])
{
  try
  {
    if (argc != 3)
    {
      std::cerr << "Usage: chat_client <host> <port>\n";
      return 1;
    }

    asio::io_context io_context;

    tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(argv[1], argv[2]);
    chat_client c(io_context, endpoints);

    std::thread t([&io_context](){ io_context.run(); });

    char line[chat_message::max_body_length + 1];
    while (std::cin.getline(line, chat_message::max_body_length + 1))
    {
      chat_message msg;
      msg.body_length(std::strlen(line));
      std::memcpy(msg.body(), line, msg.body_length());
      msg.encode_header();
      c.write(msg);
    }

    c.close();
    t.join();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
