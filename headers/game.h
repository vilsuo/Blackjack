#ifndef GAME_H
#define GAME_H

#include "deck.h"
#include "player.h"
#include "settings.h"

#include <vector>

class Game {
    private:
        std::vector<Player> m_players{};
        Deck m_deck{settings::nInitialDecks};
        Deck m_house{};
		
		bool m_isInitialAction{true};

    public:
        void run();

    private:
        void play();
		
		void askBet(Player&);
		int askAction(const Player&) const;
		bool handleAction(Player&, int);	// return true if player has turns left after this action
											// need to check that action is allowed before this method
		
		bool canDoubleDown(const Player&) const;	// todo
		bool canSplit(const Player&) const;
		bool canSurrender(const Player&) const;
		
		Card deckPopCard();
		
		std::vector<Player>::iterator findPlayer(int id);
		
        void createNewPlayer();
        void deletePlayer();
        void printPlayers() const;
        void addBalance();
};

#endif