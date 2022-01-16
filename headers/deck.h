#ifndef DECK_H
#define DECK_H

#include "card.h"

#include <vector>

class Deck {
    public:
        using deck_type = std::vector<Card>;

    private:
        deck_type m_deck{};

    public:
        Deck(int nDecks = 0);

        void printDeck() const;
        void shuffleDeck();
        int getNumberOfCardsInADeck() const;
		
		/*
		If deck contains aces:
			- treat aces as a value 1 until the total value of deck
			  is less than or equal to settings::bustValue
			- else treat aces as a value 11
		*/
        int getDeckTotalValue() const;

        void addCard(const Card&);
        Card popLastCard();

        void removeAllCards();
};

#endif