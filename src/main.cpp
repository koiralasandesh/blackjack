#include "mainwin.h"
#include <gtkmm.h>

int main(int argc, char** argv) {
   // Create the application
   auto app = Gtk::Application::create(argc, argv, "edu.uta.cse3310.blackjack");

   // Instance a window
   Mainwin win;

   // Give control to gtkmm
   return app->run(win);
}
