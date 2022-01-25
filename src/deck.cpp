#include "deck.h"
#include "card.h"
#include "settings.h"

#include <algorithm>
#include <cassert>
#include <ctime>        // for std::time
#include <iostream>
#include <iterator>
#include <random>       // for std::mt19937

Deck::Deck(int nDecks) {
	constexpr auto nSuits{static_cast<deck_type::size_type>(CardSuit::max_suits)};
	constexpr auto nRanks{static_cast<deck_type::size_type>(CardRank::max_ranks)};
	m_deck.reserve(nSuits * nRanks * static_cast<deck_type::size_type>(nDecks));																// this ok?

	int amount{0};
    while (amount < nDecks) {
        for (auto suit{ static_cast<deck_type::size_type>(0) }; suit < nSuits; ++suit) {
            for (auto rank{ static_cast<deck_type::size_type>(0) }; rank < nRanks; ++rank) {
                m_deck.emplace_back( static_cast<CardRank>(rank), static_cast<CardSuit>(suit) );			// push_back instead?
            }
        }
        ++amount;
    }
    shuffleDeck();
}

const Card& Deck::operator[] (int index) const {
	assert(index >= 0 && static_cast<deck_type::size_type>(index) < m_deck.size() && "Trying to access deck out of bounds");
	
	return m_deck[static_cast<deck_type::size_type>(index)];
}

std::ostream& operator<< (std::ostream& out, const Deck& deck) {
	out << "# ";
    for(const Card& card : deck.m_deck) {
		out << card << ' ';
	}
	std::cout << '#';
	
	return out;
}

void Deck::shuffleDeck() {
    static std::mt19937 mt{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };

    std::shuffle(std::begin(m_deck), std::end(m_deck), mt);
}

int Deck::getDeckTotalValue() const {
	// for reference later
	const Card ace{ CardRank::rank_ace, CardSuit::suit_spades };
	const int aceValue{ ace.getCardValue() };
	  
    int totalValue{0};
	int nAces{0};
    for (const Card& card : m_deck) {
        totalValue += card.getCardValue();
		if (card.isSameRank(ace)) {
			++nAces;
		}
    }
	
	/**
		If total value of cards is over settings::bustValue,
		treat aces as 1 until total value is less than on equal to bustValue
	*/
	while (nAces && (totalValue > settings::bustValue)) {
		totalValue -= (aceValue - 1);
		--nAces;
	}
	
    return totalValue;
}

void Deck::addCard(const Card& card) {															// move?
    m_deck.push_back(card);
}

Card Deck::popLastCard() {																		// move?
	assert(!m_deck.empty() && "Trying to pop from an empty deck");
	
    Card card{ m_deck.back() };
    m_deck.pop_back();
    return card;
}


void Deck::removeAllCards() {
    m_deck.clear();
}