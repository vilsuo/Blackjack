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
	constexpr auto nSuits{static_cast<deck_type::size_type>( CardSuit::max_suits )};
	constexpr auto nRanks{static_cast<deck_type::size_type>( CardRank::max_ranks )};
	m_deck.reserve(nSuits * nRanks * static_cast<deck_type::size_type>( nDecks ));																// this ok?

	for (int amount{0}; amount < nDecks; ++amount) {
        for (auto suit{ static_cast<deck_type::size_type>( 0 ) }; suit < nSuits; ++suit) {
            for (auto rank{ static_cast<deck_type::size_type>( 0 ) }; rank < nRanks; ++rank) {
                m_deck.emplace_back( static_cast<CardRank>( rank ), static_cast<CardSuit>( suit ) );		// push_back instead?
            }
        }
        ++amount;
    }
    shuffleDeck();
}

const Card& Deck::operator[] (deck_type::size_type index) const {
	assert(index < m_deck.size() && "Trying to access deck out of bounds");
	
	return m_deck[index];
}

std::ostream& operator<< (std::ostream& out, const Deck& deck) {
	std::for_each(deck.m_deck.cbegin(), --deck.m_deck.cend(), [&out](const auto& card) {
		out << card << ' ';
	});
	out << deck.m_deck.back();
	return out;
}

void Deck::shuffleDeck() {
    static std::mt19937 mt{ static_cast<std::mt19937::result_type>( std::time(nullptr) ) };
	std::shuffle(std::begin(m_deck), std::end(m_deck), mt);
}

int Deck::getDeckTotalValue() const {
	const Card ace{ CardRank::rank_ace, CardSuit::suit_spades };
	const int aceValue{ ace.getCardValue() };
	  
    int totalValue{0};
	int nAces{0};
    for (const Card& card : m_deck) {
        totalValue += card.getCardValue();
		if (card.isSameRank(ace)) ++nAces;
    }
	if (totalValue <= settings::blackjackValue || aceValue == 1) return totalValue;
	
	/**
		calculate how many aces to treat as one
		
		If total value of cards is over settings::blackjackValue,
		treat aces as 1 until total value is less than on equal to settings::blackjackValue
	*/
	int nAcesToTreatAsOne{0};
	
	nAcesToTreatAsOne = (totalValue - settings::blackjackValue) / (aceValue - 1);
	if ((totalValue - settings::blackjackValue) % (aceValue - 1) != 0) ++nAcesToTreatAsOne;	// round up
	
	return totalValue - (aceValue - 1) * std::min(nAces, nAcesToTreatAsOne);
}

Card Deck::popLastCard() {
	assert(!m_deck.empty() && "Trying to pop from an empty deck");
	
    Card card{ m_deck.back() };
    m_deck.pop_back();
    return card;
}

bool Deck::isBlackjack() const {
	return (getDeckTotalValue() == settings::blackjackValue && getNumberOfCardsInADeck() == 2);
}