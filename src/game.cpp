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
    int option{-1};
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
	// ask bet
	
	// CHECK IF CAN POP!!!
    // deal initial round
    m_house.addCard(m_deck.popLastCard());
    for (Player& player : m_players) {
        player.addCard(m_deck.popLastCard());
    }
	for (Player& player : m_players) {
        player.addCard(m_deck.popLastCard());
    }
    
    // ask players one by one for action
    // results
    // pay
    // remove all cards from players
    std::cout << "Returning from game\n";
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
    std::cout << "--------------------\n";
    for(const Player& player : m_players) {
        player.printInfo();
        std::cout << "--------------------\n";
    }
}

void Game::addBalance() {
    Game::printPlayers();
    int id{};
    std::cout << "Enter player id to add balance to: ";
    std::cin >> id;

    auto player{ Game::findPlayer(id) };

    int balance{};
    std::cout << "Enter amount to add to balance: ";
    std::cin >> balance;

    player->addToBalance(balance);
    std::cout << "Balance added\n";
}

std::vector<Player>::iterator Game::findPlayer(int id) {
    return std::find_if(std::begin(m_players), std::end(m_players),
        [id](const Player& player) {
            return player.getId() == id;
        }
    );
}