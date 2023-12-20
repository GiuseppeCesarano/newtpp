#include "newtpp.hpp"
#include <iostream>

int main()
{
  newt::root_window root;

  /* Positioning is relative to the last window created *
   * in win costructor it's relative to the root window */
  newt::window win { newt::usize { 50, 8 } };

  // Now is relative to win
  newt::label text { "Hello, could you please press ok?", { 10, 1 } };
  newt::button ok_button { "ok", { 22, 3 } };

  newt::form simple_message { text, ok_button };

  simple_message.run();
}
