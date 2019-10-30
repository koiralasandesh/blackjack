#ifndef PLAYER_CHOICE_H
#define PLAYER_CHOICE_H

#include <gtkmm.h>

class Player_choice_window : public Gtk::Window
{
    public:
        Player_choice_window();
        virtual ~Player_choice_window();

    protected:
        Gtk::VBox vbox;
        Gtk::Button hit_button, stand_button, double_button, split_button, insurance_button;

        void on_hit_button_click();
        void on_stand_button_click();
        void on_double_button_click();
        void on_split_button_click();
        void on_insurance_button_click();

}
#endif