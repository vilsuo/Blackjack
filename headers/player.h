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
        std::size_t m_currentHandIndex{0};					// index of the current hand
		
		std::array<int, 2> m_currentBets{0,0};

    public:
        Player(std::string name, int balance);

        int getId() const { return m_id; }
        std::string getName() const { return m_name; };
        int getBalance() const { return m_balance; };
		void setBalance(int amount) { m_balance += amount; };
		Deck& getCurrentHand();
		int& getCurrentBet();
		
		bool addCard(const Card&); 	// false if adding card causes the end of players turn
		

        bool stand();   			// true if still has other hand left

        void split();
		
		/**
			does NOT add a card
		*/
		bool doubleDown(); 			// true if still has other hand left
									// todo
		void surrender();

        bool bust(); 				// true if deck total value is over settings::bustValue
		
        bool bet(int mount);            // true if there is enough balance

        void printInfo() const;
		void printHands() const;
		
		void reset();
};

#endif