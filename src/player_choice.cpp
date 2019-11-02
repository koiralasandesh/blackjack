#include "player_choice.h"
#include "mainwin.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

Player::Player()
{
    Gtk::Window w;

    Gtk::Dialog *dialog = new Gtk::Dialog();
    dialog->set_transient_for(w);
    dialog->set_title("Enter player name: ");

    Gtk::Label *nameLabel = new Gtk::Label("Enter item name: ");
    dialog->get_content_area()->pack_start(*nameLabel);
    nameLabel->show();

    dialog->add_button("Cancel", 0);  //when cancel is clicked it should close
    dialog->add_button("OK", 1);
    dialog->set_default_response(0);

    Gtk::Entry *entryName = new Gtk::Entry();
    entryName->set_text("Enter your name");
    entryName->set_max_length(50);
    entryName->show();
    dialog->get_vbox()->pack_start(*entryName);

    int result = dialog->run();
    player_name = entryName->get_text();

    dialog->close();

    delete dialog;
    delete nameLabel;
    delete entryName;
}

Player_choice_window::Player_choice_window(string player_name) : vbox(Gtk::ORIENTATION_VERTICAL), hit_button("HIT"), stand_button("STAND"),
     double_button("DOUBLE"), split_button("SPLIT"), insurance_button("INSURANCE")
{
    set_size_request(500, 200);
    set_title("--Player Choice Page--");

    add(vbox);

    nameLabel.set_text("Player name: " + player_name);
    nameLabel.set_padding(10, 10);
    vbox.pack_start(nameLabel);

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

void Player_choice_window::on_split_button_click()
{
    //split if card if equal and invest same credit as before
    hide();
}

void Player_choice_window::on_insurance_button_click()
{
    //invest half credits as before and save half if you lose
    hide();
}