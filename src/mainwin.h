#ifndef MAINWIN_H
#define MAINWIN_H

#include <gtkmm.h>

class Main_window : public Gtk::Window
{
public:
    Main_window();
    virtual ~Main_window();

protected:
    Gtk::Label welcome_title_label;
    Gtk::Button play_button, rules_button, quit_button;
    Gtk::VBox vbox;

    void on_play_button_click();
    void on_rules_button_click();
};

#endif