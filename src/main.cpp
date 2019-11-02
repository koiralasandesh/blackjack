#include "mainwin.h"
#include "player_choice.h"
#include "rules.h"
#include <iostream>

using namespace std;

#include <gtkmm.h>


int main(int argc, char** argv) {

    Gtk::Main app(argc, argv);
    Mainwin_window mainwin;
    Gtk::Main::run(mainwin);  

    return 0;

}
