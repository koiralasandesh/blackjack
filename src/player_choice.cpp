#include "player_choice.h"
#include "mainwin.h"
//#include "card.h"
#include "bet.h"
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

    Gtk::Label *nameLabel = new Gtk::Label("Enter player name: ");
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

Player_choice_window::Player_choice_window(string player_name)
{
    set_size_request(500, 500);
    set_title("--BLACKJACK--");

    add(box);

    shoe.set_text("Shoe");
    shoe.set_size_request(80,32);
    player_window_grid.attach(shoe,0,0,1,5);

    shoe_image.set("Dealer_Shoe.jpg");
    shoe_image.set_size_request(150,150);
    player_window_grid.attach(shoe_image,0,6,1,1);

    //we need to able to modify dealer credit
    credits_of_dealer.set_text("Dealer Credits: $500");
    credits_of_dealer.set_size_request(20,20);
    player_window_grid.attach(credits_of_dealer,4,0,1,5);

    dealer_cards.set_text("Dealer Cards:");
    dealer_cards.set_size_request(20,20);
    player_window_grid.attach(dealer_cards,15,0,1,5);

    /*
    dealer_cards_image.set("");
    player_window_grid.attach(dealer_cards_image,15,6,1,3);
    */

    player_cards.set_text("Your Cards:");
    player_cards.set_size_request(20,20);
    player_window_grid.attach(player_cards,0,51,1,1);

    your_place.set_text(player_name);
    your_place.set_size_request(20,20);
    player_window_grid.attach(your_place,6,25,1,5);

    player2_place.set_text("Player 2");
    player2_place.set_size_request(20,20);
    player_window_grid.attach(player2_place,15,25,1,5);

    player3_place.set_text("Player 3");
    player3_place.set_size_request(20,20);
    player_window_grid.attach(player3_place,20,25,1,5);

    //modify player credits
    credits_of_player.set_text("Your Credits: ");
    credits_of_player.set_size_request(20,20);
    player_window_grid.attach(credits_of_player,0,50,1,1);

    /*
    nameLabel.set_text("Player name: " + player_name);
    nameLabel.set_padding(10, 10);
    vbox.pack_start(nameLabel);
    */

    hit_button.add_label("Hit");
    hit_button.set_size_request(10,4);
    hit_button.signal_clicked().connect(sigc::mem_fun(*this,&Player_choice_window::on_hit_button_click));
    player_window_grid.attach(hit_button,6,66,1,1);

    stand_button.add_label("Stand");
    stand_button.set_size_request(10,4);
    stand_button.signal_clicked().connect(sigc::mem_fun(*this,&Player_choice_window::on_stand_button_click));
    player_window_grid.attach(stand_button,6,67,1,1);

    bet_button.add_label("Bet");
    bet_button.set_size_request(10,4);
    bet_button.signal_clicked().connect(sigc::mem_fun(*this,&Player_choice_window::on_bet_button_click));
    player_window_grid.attach(bet_button,7,67,1,1);

    quit_button.add_label("Quit");
    quit_button.set_size_request(10,4);
    quit_button.signal_clicked().connect(sigc::mem_fun(*this,&Player_choice_window::close));
    player_window_grid.attach(quit_button,7,66,1,1);

    box.pack_start(player_window_grid);
    show_all_children();
}

Player_choice_window::~Player_choice_window() {}

void Player_choice_window::on_hit_button_click()
{
    //ask dealer a card
}

void Player_choice_window::on_stand_button_click()
{
    //go to next player
}

void Player_choice_window::on_bet_button_click()
{
    //go bet window and return credits
    Bet_window *bet = new Bet_window();
    bet->show();
}
