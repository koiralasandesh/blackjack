#include "mainwin.h"
#include "bjd.h"
#include "bjp.h"
#include "card.h"
#include "hand.h"
#include <iostream>

using namespace std;

#include <gtkmm.h>


int main(int argc, char** argv) {

   /*Himal

    Gtk::Main app(argc, argv);
    Mainwin_window mainwin;
    Gtk::Main::run(mainwin);  

    return 0;
   */


   // Create the application
   auto app = Gtk::Application::create(argc, argv, "edu.uta.cse3310.blackjack");

   // Instance a window
   Mainwin win;

   // Give control to gtkmm
   return app->run(win);
}
