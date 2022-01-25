#ifndef DECK_H
#define DECK_H

#include "card.h"

#include <iostream>
#include <vector>

class Deck {
    public:
        using deck_type = std::vector<Card>;

    private:
        deck_type m_deck{};

    public:
		/**
			Creates a card deck with nDecks complete decks,
			also shuffles the deck
		*/
        Deck(int nDecks = 0);
		
		const Card& operator[] (int index) const;
		friend std::ostream& operator<< (std::ostream&, const Deck&);
		
        void shuffleDeck();
        int getNumberOfCardsInADeck() const { return m_deck.size(); };
		
		/**
			If deck contains aces:
			- treat aces as a value 1 until the total value of deck
			  is less than or equal to settings::bustValue
			- else treat aces as a value specified by Card::getCardValue()
		*/
        int getDeckTotalValue() const;

        void addCard(const Card&);
        Card popLastCard();

        void removeAllCards();
};

#endif