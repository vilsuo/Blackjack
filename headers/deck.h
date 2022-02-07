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
		// Creates a deck of Cards with nDecks complete sets of cards.
		// Cards are in random order.
        Deck(int nDecks = 0);
		
		const Card& operator[] (deck_type::size_type index) const;
		friend std::ostream& operator<< (std::ostream& out, const Deck& deck);
		
        void shuffleDeck();
        int getNumberOfCardsInADeck() const { return m_deck.size(); };
		
		// CardRank::rank_ace is treated as value 1 until the total value
		// of cards is less than or equal to settings::blackjackValue.
        int getDeckTotalValue() const;

        void addCard(const Card& card) { m_deck.push_back(card); };
        Card popLastCard();

        void removeAllCards() { m_deck.clear(); };
		
		bool isBlackjack() const;
};

#endif