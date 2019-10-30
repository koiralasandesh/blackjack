#include "player_choice.h"
#include <iostream>

using namespace std;

Player_choice_window::Player_choice_window() : box(Gtk::ORIENTATION_VERTICAL), hit_button("HIT"), stand_button("STAND"),
     double_button("DOUBLE"), split_button("SPLIT"), insurance_button("INSURANCE")
{
    set_size_request(500, 200);
    set_title("--Player Choice Page--");

    add(vbox);

    hit_button.signal_clicked().connect(sigc::mem_fun(*this, &Player_choice_window::on_hit_button_click));
    vbox.pack_start(hit_button);

    stand_button.signal_clicked().connect(sigc::mem_fun(*this, &Player_choice_window::on_stand_button_click));
    vbox.pack_start(stand_button);

    double_button.signal_clicked().connect(sigc::mem_fun(*this, &Player_choice_window::on_double_button_click));
    vbox.pack_start(double_button);

    split_button.signal_clicked().connect(sigc::mem_fun(*this, &Player_choice_window::on_split_button_click));
    vbox.pack_start(split_button);

    insurance_button.signal_clicked().connect(sigc::mem_fun(*this, &Player_choice_window::on_insurance_button_click));
    vbox.pack_start(insurance_button);

    show_all_children();
}

Player_choice_window::~Player_choice_window() {}

void Player_choice_window::on_hit_button_click()
{
    //asl dealer a card
    hide();
}

void Player_choice_window::on_stand_button_click()
{
    //go to next player
    hide();
}

void Player_choice_window::on_double_button_click()
{
    //ask one last card and double the amount
    hide();
}

void Player_choice_window::n_split_button_click()
{
    //split if card if equal and invest same credit as before
    hide();
}

void Player_choice_window::on_insurance_button_click()
{
    //invest half credits as before and save half if you lose
    hide();
}