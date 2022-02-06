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
		// Creates a card deck with nDecks complete decks, cards are in random order
        Deck(int nDecks = 0);
		
		const Card& operator[] (deck_type::size_type index) const;
		friend std::ostream& operator<< (std::ostream& out, const Deck& deck);
		
        void shuffleDeck();
        int getNumberOfCardsInADeck() const { return m_deck.size(); };
		
		/**
			If deck contains aces:
			- treat aces as a value 1 until the total value of deck
			  is less than or equal to settings::blackjackValue
		*/
        int getDeckTotalValue() const;

        void addCard(const Card& card) { m_deck.push_back(card); };
        Card popLastCard();

        void removeAllCards() { m_deck.clear(); };
		
		bool isBlackjack() const;
};

#endif