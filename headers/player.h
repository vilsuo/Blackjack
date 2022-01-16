#ifndef PLAYER_H
#define PLAYER_H

#include "card.h"
#include "deck.h"

#include <stack>
#include <string>
#include <vector>

class Player {
	public:
        using hand_type = std::vector<Deck>;
		
    private:
        std::string m_name{};
        int m_balance{0};
		
		hand_type m_hand{};

        std::stack<hand_type::size_type> m_indexStack{};
        std::vector<hand_type::size_type> m_finishedHands{};	// implement as bit vector!!
        hand_type::size_type m_handIndex{0};					// index of the current hand

        static inline int s_idGenerator{0};
        int m_id{};

    public:
        Player(std::string name, int balance);

        int getId() const { return m_id; }
        std::string getName() const { return m_name; };
        int getBalance() const { return m_balance; };
		
		
		bool addCard(const Card&); // return false if adding card causes the end of players turn

        bool stand();   // true if still has hands

        void split();

        bool bust() const;

        void printHands() const;
		
        
        void addToBalance(int amount);
        bool bet(int mount);            // if there is enough balance

        void printInfo() const;
};

#endif