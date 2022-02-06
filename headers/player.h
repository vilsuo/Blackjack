#ifndef PLAYER_H
#define PLAYER_H

#include "card.h"
#include "deck.h"

#include <stack>
#include <string>
#include <vector>

class Player {
	public:
		using hands_type = std::vector<Deck>;
		
	private:
		std::string 	m_name{};
        int 			m_balance{0};
		int 			m_id{};
		
		static inline int s_idGenerator{0};
		
		std::vector<int> 					m_bets{0};
		hands_type		 					m_hands{ Deck{} };
		std::stack<hands_type::size_type> 	m_handStack{};
		std::vector<hands_type::size_type> 	m_finishedHands{};
        hands_type::size_type 				m_currentHandIndex{0};		// index of the current hand


    public:
        Player(std::string name, int balance);

        int getId() const 				{ return m_id; }
        std::string getName() const		{ return m_name; };
		
        int getBalance() const 			{ return m_balance; };
		void setBalance(int amount) 	{ m_balance = amount; };
		
		void setCurrentBet(int amount);

		void addCardToCurrentHand(const Card& card);
		
		/**
			- marks the current hand as finished
			
			- DOES NOT MOVE TO THE NEXT HAND
		*/
        void standCurrentHand();

		/**
			- moves the second card to a new hand and adds one card to both hands
			- copies the bet to the new hand and updates the balance
			- sets m_currentHandIndex to point to the new hand
			
			- MUST BE ABLE TO SPLIT WHEN CALLING THIS FUNCTION
			
			return true if there are hands left after splitting (hands can "bust" or blackjack here)
		*/
        bool splitCurrentHand(const Card& firstCardToAdd, const Card& secondCardToAdd);
		
		/**
			- adds a card and doubles the current bet and updates the balance accordingly
			- sets the current hand as finished
			
			- DOES NOT MOVE TO THE NEXT HAND!
			- MUST BE ABLE TO DOUBLE DOWN WHEN CALLING THIS FUNCTION
		*/
		void doubleDownCurrentHand(const Card& cardToAdd);
		
		/**
			- ONLY ALLOWED AS A INITIAL ACTION, MUST END PLAYERS TURN
			- player gets half of the bet back instantly
				- PAYS THE PLAYER!
			
			- sets current bet to 0
			- sets current hand as finished
		*/
		void surrender();
		
		bool canDoubleDownCurrentHand() const;
		bool canSplitCurrentHand() const;

		// returns true if deck total value is over settings::blackjackValue
        bool isCurrentHandBusted() const;
		bool isCurrentHandBlackjack() const;
		
        void printInfo() const;
		void printHands() const;
		
		void payAndDisplayResults(const int houseValue, const bool houseHasBlackJack, const bool houseBust);
		
		void reset();
		
		// return true if there are unfinished hands left (and then move to it)
		bool tryToMoveToNextHand();
		
	private:
		int getCurrentBet() const;
		int getNHands() const { return m_hands.size(); };
		
		Deck& getCurrentHandRef();
		const Deck& getCurrentHandRef() const;
		
		/**
			returns true even if there is no hands left in the stack,
			but the current hand is not marked as finished!
		*/
		bool hasHandLeft() const;
		void moveToNextHand();
};

#endif