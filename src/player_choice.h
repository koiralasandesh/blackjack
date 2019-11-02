#ifndef PLAYER_CHOICE_H
#define PLAYER_CHOICE_H

#include <gtkmm.h>

using namespace std;

class Player
{
    public:
        string player_name;
        Player();
};

class Player_choice_window : public Gtk::Window
{
    public:
        Player_choice_window(string player_name);
        virtual ~Player_choice_window();

    protected:
        Gtk::VBox vbox;
        Gtk::Button hit_button, stand_button, double_button, split_button, insurance_button;
        Gtk::Label nameLabel;

        void on_hit_button_click();
        void on_stand_button_click();
        void on_double_button_click();
        void on_split_button_click();
        void on_insurance_button_click();

};
#endif