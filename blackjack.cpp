#include "card.h"
#include "player.h"
#include "game.h"

#include <iostream>

/*
TODO
- Game
	- check if can pop a card
- Player
	- m_finishedHands
		- implement as a bit vector
	- can only split/double down after initial deal?
- Deck
	- getDeckTotalValue
		- do not check here if over settings::bustValue,
		  check in Player class instead?
- settings
	- add amount of decks
	
make simple member functions inline?
*/

int main() {
    Deck deck{4};
    Player p{"ville", 100};

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
