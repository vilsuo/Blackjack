/*
#include "game.h"
#include "card.h"
#include "deck.h"
#include "player.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

void Game::run() {
    int option{};
    bool exitCondition{false};
    while (true) {
        while (true) {
            std::cout << "\n0 - Play Black Jack\n";
            std::cout << "1 - Add a new player\n";
            std::cout << "2 - Delete existing player\n";
            std::cout << "3 - Display current players\n";
            std::cout << "4 - Add balance to a existing player\n";
            std::cout << "5 - Exit\n";
            std::cout << "Enter your selection: ";

            std::cin >> option;
            switch (option) {
            case 0:
                if (m_players.empty()) {
                    std::cout << "You need to add players before playing\n";
                    break;
                }
                Game::play();
                break;

            case 1:
                Game::createNewPlayer();
                break;

            case 2:
                Game::deletePlayer();
                break;

            case 3:
                Game::printPlayers();
                break;
            
            case 4:
                Game::addBalance();
                break;

            case 5:
                exitCondition = true;
                std::cout << "Good Bye!";
                break;

            default:
                std::cout << "Unknown command: " << option << '\n';
                break;
            }
            if (exitCondition) break;
        }
        if (exitCondition) break;
    }
}

void Game::play() {			
	std::cout << "Entering game\n";
	
	// ask bets
	for (Player& player : m_players) {
		std::cout << "Current player:\n";
		player.printInfo();
		Game::askBet(player);
	}
	
    // initial round: deal 1 card for house and two cards for each player
	m_house.addCard(Game::deckPopLastCard());
	for (Player& player : m_players) {
		player.addCard(Game::deckPopLastCard());
	}
	for (Player& player : m_players) {
		player.addCard(Game::deckPopLastCard());
	}
    
    
	// ask actions
	for (Player& player : m_players) {
		m_isInitialAction = true; // reset for each player
		
		std::cout << "Current player:\n";
		player.printInfo();
		
		// ask actions until player has no turns left
		while (true) {
			player.printHands();
			
			int action{ askAction(player) };
			
			// action is validated so this is okay
			bool playerHasTurnsLeft{ handleAction(player, action) };
			
			m_isInitialAction = false; // player has made an action
			
			if (!playerHasTurnsLeft) {
				player.printHands();
				std::cout << "Moving to next player...\n";
				break;
			}
		}
	}
	
	std::cout << "All players have played, next houses turn\n";
	// house hits if until total value is less than settings::houseHitIfLessThan
	while(m_house.getDeckTotalValue() < settings::houseHitIfLessThan) {
		m_house.addCard(Game::deckPopLastCard());
	}
	std::cout << "House's cards:\n";
	m_house.printDeck();
	
    // display results
	for (Player& player : m_players) {
		player.printInfo();
		player.printHands();
	}
	
    // pay (not for surrenderers)
	
	// remove all cards from players and house, reset flags...
	m_house.removeAllCards();
	for (Player& player : m_players) {
		player.reset();
	}
	
    std::cout << "Returning from game\n";
}

void Game::askBet(Player& player) {
	int betAmount{};
	while (true) {
		std::cout << "How much do you want to bet? ";
		std::cin >> betAmount;
		if (player.bet(betAmount)) break;
		else std::cout << "Bet not accepted, try again\n";
	}
}

int Game::askAction(Player& player) const {
	bool canSurrender{ Game::canSurrender(player) };
	bool canDoubleDown{ Game::canDoubleDown(player) };
	bool canSplit{ Game::canSplit(player) };
	
	int playerAction{};
	while (true) {
		// only show options that are allowed at the time
		std::cout << "\n0 - hit\n";
		std::cout << "1 - stand\n";
		if (canSurrender) 	std::cout << "2 - surrender\n";
		if (canDoubleDown) 	std::cout << "3 - double down\n";
		if (canSplit) 		std::cout << "4 - split\n";
		std::cout << "Please enter selection: ";
	
		// validate user input
		std::cin >> playerAction;
		if (playerAction == 0 || playerAction == 1) 	break;
		else if (canSurrender && playerAction == 2) 	break;
		else if (canDoubleDown && playerAction == 3) 	break;
		else if (canSplit && playerAction == 4) 		break;
		else std::cout << "Invalid action, try again\n";
	}													
	
	return playerAction;
}

bool Game::handleAction(Player& player, int action) {
	switch (action) {
		case 0: // hit
			return player.addCard(Game::deckPopLastCard());
			
		case 1: // stand
			return player.stand();
			
		case 2: // surrender
			player.surrender();
			return false;
			
		case 3: // split
			player.split();
			return true;
			
		case 4: // double down
			player.doubleDown();
			player.addCard(card);
			return false;
		
		default:
			assert(false && "Unknown player action");
	}
}

bool Game::canDoubleDown(const Player& player) const {
	// check that player has not splitted..?
	
	
	
	// can player afford?
	return player.getBalance() >= player.getCurrentBet();
}

bool Game::canSplit(const Player& player) const {
	// allowed only as a first action
	// also checks that player has no splitted
	if (!m_isInitialAction) return false;
	
	// can player afford?
	if (player.getBalance() < player.getCurrentBet()) return false;
	
	// now player only has two cards, both on the first hand
	// look at them:
	Card first{ player.getCurrentHand().cardAt(0) };
	Card second{ player.getCurrentHand().cardAt(1) };
	
	// check if splitting is allowed only on cards with the same rank...
	if (settings::splitOnSameRank) return first.isSameRank(second);
	
	// else splitting is allowed on cards with the same value
	return first.isSameValue(second);
}

bool Game::canSurrender(const Player& player) const {
	// check if allowed by the rules
	if (!settings::canSurrender) return false;
	
	// allowed only as an initial action
	return m_isInitialAction;
}

Card Game::deckPopLastCard() {
	if (m_deck.getNumberOfCardsInADeck() == 0) {
		m_deck = Deck(settings::nInitialDecks);
	}
	return m_deck.popLastCard();
}

std::vector<Player>::iterator Game::findPlayer(int id) {
    return std::find_if(std::begin(m_players), std::end(m_players),
        [id](const Player& player) {
            return player.getId() == id;
        }
    );
}

void Game::createNewPlayer() {
    std::string name{};
    std::cout << "Enter a name for the new player: ";
    std::cin >> name;
    int balance{};
    std::cout << "Enter balance for " << name << ": ";
    std::cin >> balance;

    m_players.emplace_back(name, balance);
    std::cout << "Player added\n";
}

void Game::deletePlayer() {
    Game::printPlayers();
    int id{};
    std::cout << "Enter player id to delete ";
    std::cin >> id;

    auto player{ Game::findPlayer(id) };

    if (player != std::end(m_players)) {
        m_players.erase(player);
        std::cout << "Player deleted\n";
    } else {
        std::cout << "Player not found\n";
    }
}

void Game::printPlayers() const {
    std::cout << "Current players of the game:\n";
    for(const Player& player : m_players) {
        player.printInfo();
		std::cout << '\n';
    }
}

void Game::addBalance() {
    Game::printPlayers();
    int id{};
    std::cout << "Enter player id to add balance to: ";
    std::cin >> id;

    auto player{ Game::findPlayer(id) };
	
	if (player != std::end(m_players)) {
        int balance{};
		std::cout << "Enter amount to add to balance: ";
		std::cin >> balance;

		player->setBalance(balance);
		std::cout << "Balance added\n";
		
    } else {
        std::cout << "Player not found\n";
    }

}
*/