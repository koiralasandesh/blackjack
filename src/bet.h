#ifndef BET_H
#define BET_H

#include <gtkmm.h>

class Bet_window : public Gtk::Window
{
    public:
        int bet_amount;
        Bet_window();
        virtual ~Bet_window();

    protected:
        Gtk::VBox vbox;
        Gtk::Button max_bet_button, min_bet_button, custom_bet_button;
        Gtk::Box betBox;
        Gtk::Entry entryBet;

        void on_max_bet_button_click();
        void on_min_bet_button_click();
        void on_custom_bet_button_click();
}

#endif