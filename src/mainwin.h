#ifndef MAINWIN_H
#define MAINWIN_H

#include <gtkmm.h>

class Mainwin_window : public Gtk::Window
{
public:
    Mainwin_window();
    virtual ~Mainwin_window();

protected:
    Gtk::Label welcome_title_label;
    /*
    Gtk::Box nameBox;
    Gtk::Entry entryName;
    */
    Gtk::Button play_button, rules_button, quit_button;
    Gtk::Box box;

    void on_play_button_click();
    void on_rules_button_click();
};

#endif