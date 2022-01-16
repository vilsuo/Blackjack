#ifndef GAME_H
#define GAME_H

#include "deck.h"
#include "player.h"
#include "settings.h"

#include <vector>

class Game {
    private:
        std::vector<Player> m_players{};
        std::vector<int> m_playersBets{};
        Deck m_deck{settings::nInitialDecks};
        Deck m_house{};

    public:
        void run();

    private:
        void play();
        void createNewPlayer();
        void deletePlayer();
        void printPlayers() const;
        void addBalance();

        std::vector<Player>::iterator findPlayer(int id);
};

#endif