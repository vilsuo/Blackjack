#ifndef PLAYER_H
#define PLAYER_H

#include "card.h"
#include "deck.h"

#include <stack>
#include <string>
#include <vector>

class Player
{
public:
	using hands_type = std::vector<Deck>;
	
private:
	std::string 		m_name{};
	int 				m_balance{0};
	int 				m_id{};
	static inline int 	s_idGenerator{0};
	
	std::vector<int> 	m_bets{0};
	hands_type		 	m_hands{ Deck{} };
	
	// Keeps track of the unfinished hands.
	// Current hand is pushed here when splitting.
	// Hand is popped after the current hand is finished
	std::stack<hands_type::size_type> m_handStack{};
	
	// Index of the finished hand is pushed here when the hand is finished
	std::vector<hands_type::size_type> m_finishedHands{};
	
	// m_hands[m_currentHandIndex] is the current hand
	// and m_bets[m_currentHandIndex] is the current bet
	hands_type::size_type m_currentHandIndex{0};

public:
	explicit Player(std::string name, int balance);
	int getId() const 				{ return m_id; }
	std::string getName() const		{ return m_name; };
	int getBalance() const 			{ return m_balance; };
	void setBalance(int amount) 	{ m_balance = amount; };
	void setCurrentBet(int amount);
	void addCardToCurrentHand(const Card& card);
	
	// Sets the current hand as finished.
	// Does NOT move to the next hand.
	void standCurrentHand();

	// Must be able to split the current hand (indicated by the @func canSplitCurrentHand).
	// Moves the second card from the current hand to a new hand.
	// Copies the bet to the new hand and updates the balance.
	// @param firstCardToAdd is added to the hand where the card was moved.
	// @param secondCardToAdd is added to the new hand.
	// Automatically stands (sets the hands as finished) the current hand if adding
	//   a card causes bust or blackjack.
	// Tries to move to next hand in the hands stack
	// @return true if there are hands left after splitting.
	bool splitCurrentHand(const Card& firstCardToAdd, const Card& secondCardToAdd);

	// Must be able to double down the current hand (indicated by the
	//   @func canDoubleDownCurrentHand).
	// The current hand bet is doubled.
	// @param cardToAdd is added to the current hand
	// Automatically stands (sets the current hand as finished).
	// Tries to move to next hand in the hands stack.
	// @return true if there are hands left after doubling down.
	bool doubleDownCurrentHand(const Card& cardToAdd);

	// Only allowed as a initial actions.
	// (In the end of the game Player must get half of the bet back)
	// Clears current hand (The only way the hand can be empty after playing).
	// Automatically stands (sets the current hand as finished).
	void surrender();
	
	bool canDoubleDownCurrentHand() const;
	bool canSplitCurrentHand() const;

	// @return true if the current deck total value is over settings::blackjackValue.
	bool isCurrentHandBusted() const;
	bool isCurrentHandBlackjack() const;
		
	// Prints player id, player name and player balance.
	void printInfo() const;

	// Prints players finished and unfinished hands with their bets
	void printHands() const;
	
	// Compares the players hands to houses hand, displays the results
	//   and pays the wins back to the player if necessary.
	void payAndDisplayResults(const int houseValue, const bool houseHasBlackJack, const bool houseBust);
		
	// Resets all hand and bet information.
	void reset();
		
	// @return true if there are unfinished hands left and if so then move to it)
	bool tryToMoveToNextHand();
		
	// @return true if current hand is bust, blackjack or equals blackjack value
	bool isCurrentHandStandRequired() const;
		
private:
	int getCurrentBet() const;
	Deck& getCurrentHandRef();
	const Deck& getCurrentHandRef() const;
};

#endif