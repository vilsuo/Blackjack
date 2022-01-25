#include "card.h"

#include <cassert>      // for assert
#include <iostream>

Card::Card(CardRank rank, CardSuit suit) : m_rank{rank}, m_suit{suit}
{}

std::ostream& operator<< (std::ostream& out, const Card& card) {
	switch (card.m_rank) {
    case CardRank::rank_2:
        out << '2';
        break;
    case CardRank::rank_3:
        out << '3';
        break;
    case CardRank::rank_4:
        out << '4';
        break;
    case CardRank::rank_5:
        out << '5';
        break;
    case CardRank::rank_6:
        out << '6';
        break;
    case CardRank::rank_7:
        out << '7';
        break;
    case CardRank::rank_8:
        out << '8';
        break;
    case CardRank::rank_9:
        out << '9';
        break;
    case CardRank::rank_10:
        out << 'T';
        break;
    case CardRank::rank_jack:
        out << 'J';
        break;
    case CardRank::rank_queen:
        out << 'Q';
        break;
    case CardRank::rank_king:
        out << 'K';
        break;
    case CardRank::rank_ace:
        out << 'A';
        break;
    default:
        assert(false && "Unknown CardRank");
    }

    switch (card.m_suit) {
    case CardSuit::suit_clubs:
        out << 'C';
        break;
    case CardSuit::suit_diamonds:
        out << 'D';
        break;
    case CardSuit::suit_hearts:
        out << 'H';
        break;
    case CardSuit::suit_spades:
        out << 'S';
        break;
    default:
        assert(false && "Unknown CardSuit");
    }

    return out;
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
        case CardRank::rank_ace:      return 11;
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