#include "mainwin.h"
#include "player_choice.h"
//#include "card.h"
#include "rules.h"
#include <iostream>

using namespace std;

Mainwin_window::Mainwin_window() : box(Gtk::ORIENTATION_VERTICAL), play_button("PLAY"), rules_button("RULES"), quit_button("QUIT")
{
    set_size_request(800, 500);
    set_title("--BlackJack Main Window--");

    add(box);

    welcome_title_label.set_text("WELCOME! BLACKJACK ");
    box.pack_start(welcome_title_label);

    /*
    entryName.set_max_length(30);
    entryName.set_text("Enter name: ");
    entryName.select_region(0, entryName.get_text_length());
    box.pack_start(entryName);

    box.pack_start(nameBox);
    */

    play_button.signal_clicked().connect(sigc::mem_fun(*this, &Mainwin_window::on_play_button_click));
    box.pack_start(play_button);

    rules_button.signal_clicked().connect(sigc::mem_fun(*this, &Mainwin_window::on_rules_button_click));
    box.pack_start(rules_button);

    quit_button.signal_clicked().connect(sigc::mem_fun(*this, &Mainwin_window::close));
    box.pack_start(quit_button);

    show_all_children();
}

Mainwin_window::~Mainwin_window()
{
}

void Mainwin_window::on_play_button_click()
{
    //string playerName = entryName.get_text();
    //goes to bjp window
    /*
    Mainwin *bjp = new Mainwin();
    bjp->show();
    */

    Player you;

    Player_choice_window *player = new Player_choice_window(you.player_name);
    player->show();
}

void Mainwin_window::on_rules_button_click()
{
    //goes to rules window that just display list of rules
    Rules_window *rules = new Rules_window();
    rules->show();
}
