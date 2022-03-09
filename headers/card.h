#ifndef CARD_H
#define CARD_H

#include <iostream>

enum class CardRank
{
    rank_2,
    rank_3,
    rank_4,
    rank_5,
    rank_6,
    rank_7,
    rank_8,
    rank_9,
    rank_10,
    rank_jack,
    rank_queen,
    rank_king,
    rank_ace,

    max_ranks,
};

enum class CardSuit
{
    suit_clubs,
    suit_diamonds,
    suit_hearts,
    suit_spades,

    max_suits,
};

class Card
{
private:
	CardRank m_rank{};
	CardSuit m_suit{};
	
public:
	explicit Card(CardRank rank, CardSuit suit);
		
	/*
	// copy constructor
	Card(const Card&) = delete;
	
	// move constructor
	Card(Card&&); noexcept = default;
	
	// copy assignment
	Card& operator= (const Card&) = delete
	
	// move assignment
	Card& operator= (Card&&) noexcept = default;
	*/
		
	friend std::ostream& operator<< (std::ostream&, const Card&);
	
	int getCardValue() const;
	bool isSameRank(const Card& card) const { return m_rank == card.m_rank; };
	bool isSameValue(const Card& card) const { return getCardValue() == card.getCardValue(); };
};

#endif