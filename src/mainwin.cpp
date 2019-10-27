#include "mainwin.h"
#include <iostream>

using namespace std;

Main_window::Main_window() : box(Gtk::ORIENTATION_VERTICAL), play_button("PLAY"), rules_button("RULES"), quit_button("QUIT")
{
    set_size_request(800, 500);
    set_title("--BlackJack Main Window--");

    add(vbox);

    play_button.signal_clicked().connect(sigc::mem_fun(*this, &Main_window::on_play_button_click));
    vbox.pack_start(play_button);

    rules_button.signal_clicked().connect(sigc::mem_fun(*this, &Main_window::on_rules_button_click));
    vbox.pack_start(rules_button);

    quit_button.signal_clicked().connect(sigc::mem_fun(*this, &Main_window::close));
    vbox.pack_start(quit_button);

    show_all_children();
}

Main_window::~Main_window()
{
    
}

void Main_window::on_play_button_click()
{
    //goes to bjp window
    Mainwin *bjp = new Mainwin();
    bjp->show();
}

void Main_window::on_rules_button_click()
{
    //goes to rules window that just display list of rules
    Rules_window *rules = new Rules_window();
    rules->show();
}
void Main_window::on_quit_button_click()
{
    close();
}
