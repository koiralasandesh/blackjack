#ifndef RULES_H
#define RULES_H

#include <gtk/gtk.h>

class Rules_window : public Gtk::Window
{
    public:
    Rules_window();
    virtual ~Rules_window();

    protected:
    Gtk::Box box;
    Gtk::Label rules_list;
    Gtk::Button hide;
};

#endif