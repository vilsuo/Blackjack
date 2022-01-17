#include "deck.h"
#include "card.h"
#include "settings.h"

#include <algorithm>
#include <ctime>        // for std::time
#include <iostream>
#include <iterator>
#include <random>       // for std::mt19937

Deck::Deck(int nDecks) {
    constexpr auto nSuits{static_cast<Deck::deck_type::size_type>(CardSuit::max_suits)};
    constexpr auto nRanks{static_cast<Deck::deck_type::size_type>(CardRank::max_ranks)};

	int amount{0};
    while (amount < nDecks) {
        for (deck_type::size_type suit{0}; suit < nSuits; ++suit) {
            for (deck_type::size_type rank{0}; rank < nRanks; ++rank) {
                m_deck.emplace_back( static_cast<CardRank>(rank), static_cast<CardSuit>(suit) );
            }
        }
        ++amount;
    }
    shuffleDeck();
}

void Deck::printDeck() const {
	std::cout << "# ";
    for(const Card& card : m_deck) {
        card.printCard();
        std::cout << ' ';
    }
	std::cout << '#';
}

void Deck::shuffleDeck() {
    static std::mt19937 mt{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };

    std::shuffle(std::begin(m_deck), std::end(m_deck), mt);
}

int Deck::getNumberOfCardsInADeck() const {
    return m_deck.size();
}

int Deck::getDeckTotalValue() const {
	const Card ace{ CardRank::rank_ace, CardSuit::suit_spades };
	  
    int totalValue{0};
	int nAces{0};
    for (const Card& card : m_deck) {
        totalValue += card.getCardValue();
		if (card.isSameValue(ace)) {
			++nAces;
		}
    }
	
	/*
	if total value of cards is over bustValue,
	treat aces as 1 until total value is less than on equal to bustValue
	*/
	while (nAces && (totalValue > settings::bustValue)) {
		totalValue -= (ace.getCardValue() - 1);
		--nAces;
	}
	
    return totalValue;
}

void Deck::addCard(const Card& card) {
    m_deck.push_back(card);
}

Card Deck::popLastCard() {
    Card card{ m_deck.back() };
    m_deck.pop_back();
    return card;
}

void Deck::removeAllCards() {
    m_deck.clear();
}