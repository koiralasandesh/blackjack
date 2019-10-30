#include "rules.h"
#include <iostream>

Rules_window::Rules_window() : box(Gtk::ORIENTATION_VERTICAL)
{
    set_size_request(500, 200);
    set_title("--Rules Page--");

    add(box);

    rules_list.set_text("List of rules to play black jack.
    1. Hand sign must be used by player as guided by the dealer. Player are not allowed to touch cards.
    2. The minimum bet player can do in $1 and max bet is $5.
    3. Will be payed 1.5 times the bet if player got blackjack.
    4. Players are strictly restricted to count cards.
    5. Dealer has to hit always till it reach soft 17.s
    ");
    rules_list.set_padding(5, 5);
    box.pack_start(rules_list);

    close.set_label("Close");
    close.signal_pressed().connect(sigc::mem_fun(*this, &Rules_window::close));
    box.pack_start(close);

    show_all_children();
}

Rules_window::~Rules_window(){};