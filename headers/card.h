#ifndef CARD_H
#define CARD_H

enum class CardRank {
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

enum class CardSuit {
    suit_clubs,
    suit_diamonds,
    suit_hearts,
    suit_spades,

    max_suits,
};

class Card {
    private:
        CardRank m_rank{};
        CardSuit m_suit{};

    public:
        Card(CardRank rank, CardSuit suit);

        void printCard() const;
        int getCardValue() const;
        bool isSameRank(const Card&) const;
        bool isSameValue(const Card&) const;
};

#endif