#include "bet.h"
#include <iostream>
#include <string.h>

using namespace std;

Bet_window::Bet_window() : box(Gtk::ORIENTATION_VERTICAL), max_bet_button("MAX BET"), min_bet_button("MIN BET"), custom_bet_button("CUSTOM")
{
    set_size_request(500, 200);
    set_title("--Bet Page--");

    add(vbox);

    max_bet_button.signal_clicked().connect(sigc::mem_fun(*this, &Bet_window::on_max_bet_button_click));
    vbox.pack_start(max_bet_button);

    min_bet_button.signal_clicked().connect(sigc::mem_fun(*this, &Bet_window::on_min_bet_button_click));
    vbox.pack_start(min_bet_button);
    
    entryBet.set_max_length(30);
    entryBet.set_text("Enter name: ");
    entryBet.select_region(0, entryBet.get_text_length());
    vbox.pack_start(entryBet);

    vbox.pack_start(betBox);

    custom_bet_button.signal_clicked().connect(sigc::mem_fun(*this, &Bet_window::on_custom_bet_button_click));
    vbox.pack_start(custom_bet_button);

    show_all_children();
}

Bet_window::~Bet_window() {}

void Bet_window::on_max_bet_button_click()
{
    bet_amount = 100;
    hide();
}

void Bet_window::on_min_bet_button_click()
{
    bet_amount = 5;
    hide();
}

void Bet_window::on_custom_bet_button_click()
{
    bet_amount = stoi(entryBet.get_text());
    hide();
}