#ifndef PLAYER_CHOICE_H
#define PLAYER_CHOICE_H

#include <gtkmm.h>
//#include "card.h"

using namespace std;

class Player
{
    public:
        string player_name;
        int player_credit = 100;
        /*
         Hand player_hand;
         */
        Player();
        int hold();
        void hit();
        int bet();
};

class Player_choice_window : public Gtk::Window
{
    public:
        Player_choice_window(string player_name);
        virtual ~Player_choice_window();

    protected:
        Gtk::Grid player_window_grid;
        Gtk::Box box;
        Gtk::Button hit_button, stand_button, bet_button, quit_button;
        Gtk::Label credits_of_player,credits_of_dealer,shoe,dealer_cards,player_cards,your_place,player2_place,player3_place;
        Gtk::Image shoe_image,dealer_cards_image;

        void on_hit_button_click();
        void on_stand_button_click();
        void on_bet_button_click();

};
#endif