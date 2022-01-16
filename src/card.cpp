#include "card.h"

#include <cassert>      // for assert
#include <iostream>

Card::Card(CardRank rank, CardSuit suit) : m_rank{rank}, m_suit{suit}
{}

void Card::printCard() const {
    switch (m_rank) {
    case CardRank::rank_2:
        std::cout << '2';
        break;
    case CardRank::rank_3:
        std::cout << '3';
        break;
    case CardRank::rank_4:
        std::cout << '4';
        break;
    case CardRank::rank_5:
        std::cout << '5';
        break;
    case CardRank::rank_6:
        std::cout << '6';
        break;
    case CardRank::rank_7:
        std::cout << '7';
        break;
    case CardRank::rank_8:
        std::cout << '8';
        break;
    case CardRank::rank_9:
        std::cout << '9';
        break;
    case CardRank::rank_10:
        std::cout << 'T';
        break;
    case CardRank::rank_jack:
        std::cout << 'J';
        break;
    case CardRank::rank_queen:
        std::cout << 'Q';
        break;
    case CardRank::rank_king:
        std::cout << 'K';
        break;
    case CardRank::rank_ace:
        std::cout << 'A';
        break;
    default:
        assert(false && "Unknown CardRank");
    }

    switch (m_suit) {
    case CardSuit::suit_clubs:
        std::cout << 'C';
        break;
    case CardSuit::suit_diamonds:
        std::cout << 'D';
        break;
    case CardSuit::suit_hearts:
        std::cout << 'H';
        break;
    case CardSuit::suit_spades:
        std::cout << 'S';
        break;
    default:
        assert(false && "Unknown CardSuit");
    }
}

int Card::getCardValue() const {
    switch (m_rank) {
        case CardRank::rank_2:        return 2;
        case CardRank::rank_3:        return 3;
        case CardRank::rank_4:        return 4;
        case CardRank::rank_5:        return 5;
        case CardRank::rank_6:        return 6;
        case CardRank::rank_7:        return 7;
        case CardRank::rank_8:        return 8;
        case CardRank::rank_9:        return 9;
        case CardRank::rank_10:       return 10;
        case CardRank::rank_jack:     return 10;
        case CardRank::rank_queen:    return 10;
        case CardRank::rank_king:     return 10;
        case CardRank::rank_ace:      return 11;	// value 1 implemented in Deck::getDeckTotalValue()
        default:
            assert(false && "No value for this CardRank");
            return 0;
    }
}

bool Card::isSameRank(const Card& card) const {
    return m_rank == card.m_rank;
}

bool Card::isSameValue(const Card& card) const {
    return (getCardValue() == card.getCardValue());
}