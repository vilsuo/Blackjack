#ifndef GAME_H
#define GAME_H

#include "deck.h"
#include "player.h"
#include "settings.h"

#include <vector>

class Game
{
private:
	std::vector<Player> m_players{};
	Deck 				m_deck{settings::nInitialDecks};
	Deck 				m_house{};

public:
	Game();
	void run();

private:
	void play();
	
	void askBets();
	
	void dealInitialRound();
	
	// Deals the round after initial round.
	// Also checks if player turn ends after initial round
	void handleFinalPlayerRound();

	int askAction(const Player& player, const bool isInitialAction) const;
	
	// @return true if player has unfinished hands left after handling @param action
	bool handleAction(Player& player, int action);
	
	Card deckPopLastCard();
	
	void dealFinalHouseRound();
	
	// Displays the results of the game and payes the players
	void payAndDisplayResults();
	
	void reset();
	
	std::vector<Player>::iterator findPlayer(int id);
	void createNewPlayer();
	void deletePlayer();
	void printPlayers() const;
	void addBalance();
	void showSettings() const;
};

#endif