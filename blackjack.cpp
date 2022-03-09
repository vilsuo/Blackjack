#include "game.h"

/*
TODO
	- what amounts can the player bet?
		- always divisible by two?
	
	- constructor, copy, assignment?
		
	- implement a hint function 
	
	- input validation!
		
	- Game::deletePlayer(), Game::addToBalance()
		- add 'q' to cancel

*/

int main() {
	Game game{};
	
	game.run();
	
    return 0;
}
