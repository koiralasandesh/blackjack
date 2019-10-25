#include "rules.h"
#include <iostream>

Rules_window::Rules_window() : box(Gtk::ORIENTATION_VERTICAL)
{
    set_size_request(500, 200);
    set_title("--Rules Page--");

    add(box);

    rules_list.set_text("List of rules to play black jack.");
    rules_list.set_padding(5, 5);
    box.pack_start(rules_list);

    close.set_label("Close");
    close.signal_pressed().connect(sigc::mem_fun(*this, &Rules_window::close));
    box.pack_start(close);

    show_all_children();
}

Rules_window::~Rules_window(){};