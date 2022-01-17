#ifndef PLAYER_H
#define PLAYER_H

#include "card.h"
#include "deck.h"

#include <array>
#include <string>

class Player {
	private:
		static inline int s_idGenerator{0};
	
        std::string m_name{};
        int m_balance{0};
		int m_id{};
		
		std::array<Deck, 2> m_hand{};
		std::array<bool, 2> m_handIsFinished{false, false};
        std::size_t m_currentHand{0};					// index of the current hand


    public:
        Player(std::string name, int balance);

        int getId() const { return m_id; }
        std::string getName() const { return m_name; };
        int getBalance() const { return m_balance; };
		Deck& getCurrentHand();
		
		
		bool addCard(const Card&); // return false if adding card causes the end of players turn

        bool stand();   // true if still has hands

        void split();

        bool bust(); 	// true if deck total value is over settings::bustValue

        void printHands() const;
		
        
        void addToBalance(int amount);
        bool bet(int mount);            // if there is enough balance

        void printInfo() const;
};

#endif