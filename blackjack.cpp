#include "card.h"
#include "player.h"

#include <iostream>

/**
TODO
	- REDO
		- Player class 
			- implement multiple splits
			- class contains too much game functionality?
	
		- Game class
			- move functionality from player class?
*/

int main() {
    Deck deck{4};
    Player p{"ville", 100};
	p.printHands();

    while (true) {

        std::cout << "adding a card...\n";
        if(!p.addCard( deck.popLastCard() )) {
			std::cout << "card added\n";
			p.printHands();
			std::cout << "Game over!\n";
			break;
		}
        p.printHands();
        std::cout << "card added\n";

        int val{0};
        std::cout << "enter 1 to split: ";
        std::cin >> val;

        if (val == 1) {
            p.split();
        }
		std::cout << '\n';
		
    }
	
    return 0;
}
