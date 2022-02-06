#include "game.h"
#include "card.h"
#include "deck.h"
#include "player.h"
#include "settings.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

Game::Game() {
	/*
	m_players.emplace_back("ville", 200);
	m_players.emplace_back("antti", 200);
	m_players.emplace_back("mikko", 200);
	m_players.emplace_back("jukka", 200);
	m_players.emplace_back("sari", 200);
	*/
}

void Game::run() {
    int option{};
    bool exit{false};
    while (true) {
        while (true) {
			auto nPlayers{ m_players.size() };
			
			std::cout 				<< "\nGame menu\n";
            if (nPlayers) std::cout << "0 - Play Black Jack\n";
            std::cout 				<< "1 - Add a new player\n";
            if (nPlayers) std::cout << "2 - Delete existing player\n";
            if (nPlayers) std::cout << "3 - Display current players\n";
            if (nPlayers) std::cout << "4 - Add balance to an existing player\n";
			std::cout 				<< "5 - Show settings\n";
            std::cout 				<< "6 - Exit\n";
            std::cout 				<< "Enter your selection: ";

            std::cin >> option;
            switch (option) {
            case 0:
				if (!nPlayers) {
					std::cout << "\nUnknown command\n";
					break;
				}
                Game::play();
                break;

            case 1:
                Game::createNewPlayer();
                break;

            case 2:
				if (!nPlayers) {
					std::cout << "\nUnknown command\n";
					break;
				}
                Game::deletePlayer();
                break;

            case 3:
				if (!nPlayers) {
					std::cout << "\nUnknown command\n";
					break;
				}
                Game::printPlayers();
                break;
            
            case 4:
				if (!nPlayers) {
					std::cout << "\nUnknown command\n";
					break;
				}
                Game::addBalance();
                break;
				
			case 5:
				Game::showSettings();
				break;

            case 6:
                exit = true;
                std::cout << "\nGood Bye!\n";
                break;

            default:
                std::cout << "\nUnknown command\n";
                break;
            }
            if (exit) break;
        }
        if (exit) break;
    }
}

void Game::play() {			
	std::cout << "\nEntering the game\n";
	
	Game::askBets();

	Game::dealInitialRound();
    
	Game::dealFinalRound();
	
	std::cout << "\nAll players have played, now it is houses turn to take cards\n";
	
	// house hits if until total value is less than settings::houseHitIfLessThan	
	while (m_house.getDeckTotalValue() < settings::houseHitIfLessThan) {
		m_house.addCard( Game::deckPopLastCard() );
	}
	
	int houseValue{ m_house.getDeckTotalValue() };
	std::cout << "\nHouse has the cards " << m_house << " with a total value of " << m_house.getDeckTotalValue() << '\n';
	
	const bool houseHasBlackJack{ m_house.isBlackjack() };
	const bool houseBust{ m_house.getDeckTotalValue() > settings::blackjackValue };
	if (houseHasBlackJack) 	std::cout << "House got blackjack!\n";
	if (houseBust) 			std::cout << "House busts!\n";
	
    // display results & pay to player if needed
	std::cout << "\nResults for the players:\n";
	for (auto& player : m_players) {
		// iterate through player's finished hands
		player.payAndDisplayResults(houseValue, houseHasBlackJack, houseBust);
	}
	
	Game::reset();
	
    std::cout << "Exiting to game menu\n";
}

void Game::askBets() {
	for (auto& player : m_players) {
		std::cout << "\nCurrent player:\n";
		player.printInfo();
		
		int betAmount{};
		const int balance{ player.getBalance() };
		while (true) {
			std::cout << "How much do you want to bet? ";
			std::cin >> betAmount;
			if (betAmount > 0 && betAmount <= balance) {
				player.setCurrentBet(betAmount);
				player.setBalance(balance - betAmount);
				break;
			} else {
				std::cout << "Bet not accepted, try again\n";
			}
		}
	}
}

void Game::dealInitialRound() {
	// initial round: deal 1 card for house and two cards for each player
	constexpr int nInitialCards{2};
	m_house.addCard( Game::deckPopLastCard() );
	for (auto& player : m_players) {
		for (int nCards{0}; nCards < nInitialCards; ++nCards) {
			
			/*
			// first card is ace, second is random
			if (nCards == 0) {
				player.addCardToCurrentHand( {CardRank::rank_ace, CardSuit::suit_spades} );
			} else {
				player.addCardToCurrentHand( {CardRank::rank_ace, CardSuit::suit_clubs} );
				//player.addCardToCurrentHand( Game::deckPopLastCard() );
			}
			*/
			player.addCardToCurrentHand( Game::deckPopLastCard() );
		}
	}
}

void Game::dealFinalRound() {
	std::vector<Player>::size_type iPlayer{0};
	for (auto& player : m_players) {
		++iPlayer;
		bool isInitialAction = true; // reset for each player
		
		// print house hand
		std::cout << "\nHouse has the card " << m_house << " with a value of " << m_house.getDeckTotalValue() << '\n';
		
		// ask actions until player has no turns left
		while (true) {
			std::cout << "\nCurrent player:\n";
			player.printInfo();
			player.printHands();
			
			// check if player is already finished (bust or blackjack) after the initial deal
			if (isInitialAction) {
				if ( player.isCurrentHandBusted() ) 	std::cout << "Bust!\n";
				if ( player.isCurrentHandBlackjack() ) 	std::cout << "Blackjack!\n";
				
				if ( player.isCurrentHandBusted() || player.isCurrentHandBlackjack() ) {
					player.standCurrentHand();
					if (iPlayer < m_players.size() ) std::cout << "\nMoving to next player\n";
					break;	// player only has one hand at this time
				}
			}
			
			int action{ askAction(player, isInitialAction) };
			
			// action is validated so this is okay
			bool playerHasTurnsLeft{ handleAction(player, action) };
			
			isInitialAction = false; // player has made an action
			
			if (!playerHasTurnsLeft) {
				std::cout << "\nCurrent player:\n";
				player.printInfo();
				player.printHands();
				if (iPlayer < m_players.size() ) std::cout << "\nMoving to next player\n";
				break;
			}
		}
	}
}

int Game::askAction(const Player& player, const bool isInitialAction) const {
	bool canSurrender{ settings::canSurrender && isInitialAction };
	bool canDoubleDown{ player.canDoubleDownCurrentHand() };
	bool canSplit{ player.canSplitCurrentHand() };
	
	int playerAction{};
	while (true) {
		// only show options that are allowed at the time
		std::cout << "\nAllowed actions:\n";
		std::cout << "0 - stand\n";
		std::cout << "1 - hit\n";
		if (canSurrender) 	std::cout << "2 - surrender\n";
		if (canDoubleDown) 	std::cout << "3 - double down\n";
		if (canSplit) 		std::cout << "4 - split\n";
		std::cout << "Enter your selection: ";
	
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
		case 0: // stand
			std::cout << "Standing\n";
			player.standCurrentHand();
			
			return player.tryToMoveToNextHand();
			
		case 1: // hit
			std::cout << "Hitting\n";
			player.addCardToCurrentHand( Game::deckPopLastCard() );
			
			if ( player.isCurrentHandBusted() ) 	std::cout << "Bust!\n";
			if ( player.isCurrentHandBlackjack() ) 	std::cout << "Blackjack!\n";
			
			if ( player.isCurrentHandBusted() || player.isCurrentHandBlackjack() ) {
				player.standCurrentHand();
				return player.tryToMoveToNextHand();
			}
			return true;
			
		case 2: // surrender
			std::cout << "Surrendering\n";
			player.surrender();
			return false; // only allowed as an initial action, ends players turn
			
		case 3: // double down
			std::cout << "Doubling down\n";
			player.doubleDownCurrentHand( Game::deckPopLastCard() );
			
			if ( player.isCurrentHandBusted() ) 		std::cout << "Bust!\n";
			if ( player.isCurrentHandBlackjack() ) 	std::cout << "Blackjack!\n";
			
			return player.tryToMoveToNextHand();
		
		case 4: { // split
			std::cout << "Splitting\n";
			const Card firstCard = Game::deckPopLastCard();
			const Card secondCard = Game::deckPopLastCard();
			return player.splitCurrentHand(firstCard, secondCard);
		}
		
		default:
			assert(false && "Unknown player action");
			return true;
	}
}

Card Game::deckPopLastCard() {
	if ( m_deck.getNumberOfCardsInADeck() == 0) {
		m_deck = Deck(settings::nInitialDecks);
	}
	return m_deck.popLastCard();
}

void Game::reset() {
	for (auto& player : m_players) player.reset();
	m_house.removeAllCards();
}

std::vector<Player>::iterator Game::findPlayer(int id) {
    return std::find_if(std::begin(m_players), std::end(m_players),
        [id](const auto& player) {
            return player.getId() == id;
        }
    );
}

void Game::createNewPlayer() {
	std::cout << "\nPlayer creation:\n";
    std::cout << "Enter a name for the new player: ";
	std::string name{};
    std::cin >> name;
	
    std::cout << "Enter balance for " << name << ": ";
	int balance{};
    std::cin >> balance;

    m_players.emplace_back(name, balance);
    std::cout << "Player added\n";
}

void Game::deletePlayer() {
	Game::printPlayers();
	std::cout << "\nPlayer deletion:\n";
	std::cout << "Enter id of the player to delete ";
	int id{};
	std::cin >> id;

	auto player{ Game::findPlayer(id) };
	if (player != std::end(m_players) ) {
		m_players.erase(player);
		std::cout << "Player deleted\n";
	} else {
		std::cout << "Player not found\n";
	}
}

void Game::printPlayers() const {
	std::cout << "\nPlayers:\n";
	for (const auto& player : m_players) player.printInfo();
}

void Game::addBalance() {
	Game::printPlayers();
	std::cout << "\nAdd balance:\n";
	std::cout << "Enter id of the player to add balance to: ";
	int id{};
	std::cin >> id;

	auto player{ Game::findPlayer(id) };
	if (player != std::end(m_players) ) {
		int balance{};
		std::cout << "Enter amount to add to balance: ";
		std::cin >> balance;

		player->setBalance(player->getBalance() + balance);
		std::cout << "Balance added\n";
	} else {
		std::cout << "Player not found\n";
	}
}

void Game::showSettings() const {
	std::cout << "\nSettings:\n";
	std::cout << "Number of decks being used is " 						<< settings::nInitialDecks 			<< '\n';
	std::cout << "Blackjack value is " 									<< settings::blackjackValue 		<< '\n';
	std::cout << "House hits if the total value of cards is less than " << settings::houseHitIfLessThan 	<< '\n';
	std::cout << "Splitting on the same card rank is " 	<< (settings::splitOnSameRank ? "" : "not ") << "allowed\n";
	std::cout << "Surrendering is " 					<< (settings::canSurrender ? "" : "not ") << "allowed\n";
	std::cout << "You can "								<< (settings::canDoubleDownAfterASplit ? "" : "not ")
														<< "double down after splitting\n";
	std::cout << "In one round you can split " 			<< settings::maxSplits << " times\n";
}