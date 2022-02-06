#include "game.h"

#include <cassert>
#include <iostream>

/*
TODO
	IMPORTANT
	- what amounts can the player bet?
		
	- input checking
	
	- automatically stand when deck total value == settings::blackjackValue
	
	- move semantics
		
	LESS IMPORTANT	
	- implement a hint function (A*?)
	  - A*?
	  - counts cards?
	
	- read player info from data base (SQL)
	  - id/name/balance/current
	  - history
	 	- win/loss amount
		- player hand value/house hand value
		- time

*/

int main() {
    Game game{};
	game.run();

    return 0;
}
