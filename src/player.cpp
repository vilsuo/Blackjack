#include "player.h"
#include "card.h"
#include "deck.h"
#include "settings.h"

#include <algorithm>
#include <iostream>

Player::Player(std::string name, int balance) : m_name{name}, m_balance{balance}, m_id{ s_idGenerator++ }
{}

int Player::getCurrentBet() const
{
	if (m_currentHandIndex >= m_bets.size())
		throw "Trying to access invalid bet";
	
	return m_bets[m_currentHandIndex];
}

void Player::setCurrentBet(int amount)
{
	if (m_currentHandIndex >= m_bets.size())
		throw "Trying to access invalid bet";
	
	m_bets[m_currentHandIndex] = amount;
}

Deck& Player::getCurrentHandRef()
{
	if (m_currentHandIndex >= m_hands.size())
		throw "Trying to access invalid hand";
	
	return m_hands[m_currentHandIndex];
}

const Deck& Player::getCurrentHandRef() const
{
	if (m_currentHandIndex >= m_hands.size())
		throw "Trying to access invalid hand";
	
	return m_hands[m_currentHandIndex];
}

void Player::addCardToCurrentHand(const Card& card)
{
    Player::getCurrentHandRef().addCard(card);
}

void Player::standCurrentHand()
{
	m_finishedHands.push_back(m_currentHandIndex);
}

bool Player::splitCurrentHand(const Card& firstCardToAdd, const Card& secondCardToAdd)
{
	if (!canSplitCurrentHand())
		throw "Trying to split but it is not allowed";
	
	std::cout << "Splitting\n";
	int bet{getCurrentBet()};
	const Card card = getCurrentHandRef().popLastCard();	// pop the second card from the current hand
	
	addCardToCurrentHand(secondCardToAdd);					// add a new card to current hand
	
	if (isCurrentHandStandRequired())						// if player finishes the hand do not remember it
		standCurrentHand();
	else
		m_handStack.push(m_currentHandIndex);
	
	m_hands.push_back(Deck{});								// create a new empty hand
	m_bets.push_back(0);									// create a placeholder for the bet 
	m_currentHandIndex = m_hands.size() - 1;				// make the new hand the current hand
	addCardToCurrentHand(card);								// add the popped card to the new hand
	addCardToCurrentHand(firstCardToAdd);					// add a new card to new hand
	
	setCurrentBet(bet);										// copy the previous bet to the new hand
	setBalance(m_balance - bet);							// update the player balance
	
	if (isCurrentHandStandRequired())						// check if new hand is finished
	{
		standCurrentHand();
		return tryToMoveToNextHand();
	} else													// else stay in the new hand
		return true;
}

bool Player::doubleDownCurrentHand(const Card& cardToAdd)
{
	if (!canDoubleDownCurrentHand())
		throw "Trying to double down but it is not allowed";
	
	std::cout << "Doubling down\n";
	const int bet{getCurrentBet()};
	
	setCurrentBet(2 * bet);				// double the current bet
	setBalance(m_balance - bet);		// update the player balance
	addCardToCurrentHand(cardToAdd);
	
	isCurrentHandStandRequired();		// this line is here just for printing
	standCurrentHand();
	
	return tryToMoveToNextHand();
}

void Player::surrender()
{
	std::cout << "Surrendering\n";
	getCurrentHandRef().removeAllCards();
	standCurrentHand();
}

bool Player::canDoubleDownCurrentHand() const
{
	// is doubling down allowed after splitting
	if (!settings::canDoubleDownAfterASplit && m_hands.size() > 1)
		return false;
	
	// can player afford?
	return getBalance() >= getCurrentBet();
}
		
bool Player::canSplitCurrentHand() const
{
	// has player reached the maximum number of allowed splits?
	if (m_hands.size() > static_cast<hands_type::size_type>(settings::maxSplits + 1))
		return false;
	
	// can player afford?
	if (getBalance() < getCurrentBet())
		return false;
	
	const Deck& currentHand{getCurrentHandRef()};
	// player has to have exactly two cards in the current hand
	if (currentHand.getNumberOfCardsInADeck() != 2)
		return false;
	
	const Card& first{currentHand[0]};
	const Card& second{currentHand[1]};
	
	// check if splitting is allowed only on cards with the same rank...
	if (settings::splitOnSameRank)
		return first.isSameRank(second);
	
	// else splitting is allowed on cards with the same value
	return first.isSameValue(second);
}

bool Player::isCurrentHandBusted() const
{
    return getCurrentHandRef().getDeckTotalValue() > settings::blackjackValue;
}

bool Player::isCurrentHandBlackjack() const
{
	const Deck& currentHand{getCurrentHandRef()};
	return currentHand.isBlackjack();
}

bool Player::isCurrentHandStandRequired() const
{
	const bool bust{isCurrentHandBusted()};
	const bool blackjack{isCurrentHandBlackjack()};
	const bool isBestValue{getCurrentHandRef().getDeckTotalValue() == settings::blackjackValue};
	
	if (bust || blackjack || isBestValue)
	{
		if (bust)
			std::cout << "Bust!\n";
		if (blackjack)
			std::cout << "Blackjack!\n";
		else if (isBestValue)
			std::cout << "Reached the best value\n";
		
		return true;
	}
	return false;
}

void Player::printInfo() const
{
    std::cout << "Id: " << m_id << ", " << m_name << ", balance " << m_balance << '\n';
}

void Player::printHands() const
{
	if (m_finishedHands.size() != m_hands.size())
	{
		std::cout << "Your are playing the hands:\n";
		std::cout << "Bet\t" << "Cards\t" << "Value\n";
		// print unfinished hands in forward order
		for (hands_type::size_type i{0}; i < m_hands.size(); ++i) {
			
			// if this hand is finished, do not print it jet
			auto found{std::find_if(m_finishedHands.begin(), m_finishedHands.end(),
				[i](hands_type::size_type finishedHand) { return i == finishedHand; })};
			if (found != m_finishedHands.end())
				continue;
			
			// print hand information
			std::cout << m_bets[i] << '\t' << m_hands[i] << '\t' << m_hands[i].getDeckTotalValue();
			if (i == m_currentHandIndex)
				std::cout << " <- current hand";
			
			std::cout << '\n';
		}
	}
	
	if (!m_finishedHands.empty())
	{
		std::cout << "You have played the hands:\n";
		std::cout << "Bet\t" << "Cards\t" << "Value\n";
		// print finished hands last in a reverse order
		for (auto it{m_finishedHands.crbegin()} ; it != m_finishedHands.crend() ; ++it)
			std::cout << m_bets[*it] << '\t' << m_hands[*it] << '\t' << m_hands[*it].getDeckTotalValue() << '\n';
	}
}

void Player::reset()
{
	m_bets = {0};
	m_hands.clear();
	m_hands.push_back( Deck{} );
	m_handStack = {};
	m_finishedHands.clear();
    m_currentHandIndex = 0;	
}

bool Player::tryToMoveToNextHand()
{
	if (!m_handStack.empty())
	{
		std::cout << "Moving to next hand\n";
		m_currentHandIndex = m_handStack.top();
		m_handStack.pop();
		
		return true;
	} else
		return false;
}

void Player::payAndDisplayResults(const int houseValue, const bool houseHasBlackJack, const bool houseBust)
{
	std::cout << "Id: " << m_id << ", " << m_name << '\n';
	std::cout << "Bet\t" << "Cards\t" << "Value\t" << "Received\n";
	
	for (const auto& iFinishedHand : m_finishedHands)
	{
		int win{0};
		const int playerHandBet{m_bets[iFinishedHand]};
		const int playerHandValue{m_hands[iFinishedHand].getDeckTotalValue()};
		const bool playerHandIsBlackjack{m_hands[iFinishedHand].isBlackjack()};
		
		std::cout << playerHandBet << '\t' << m_hands[iFinishedHand] << '\t' << playerHandValue << '\t';
		if (m_hands[iFinishedHand].getNumberOfCardsInADeck() == 0)
		{
			win = playerHandBet/2;
			std::cout << win << " (Player surrendered)\n";
			setBalance(getBalance() + win);
		}
		else
		{
			if (houseHasBlackJack)
			{
				if (playerHandIsBlackjack)
					win = playerHandBet; 		// tie
			}
			else if (houseBust)
			{
				if (playerHandIsBlackjack)
					win = 3 * playerHandBet;	// win double
				else if (playerHandValue <= settings::blackjackValue)
					win = 2 * playerHandBet;	// win
			}
			else
			{
				if (playerHandIsBlackjack)
					win = 3 * playerHandBet;	// win double
				else if (playerHandValue <= settings::blackjackValue && playerHandValue > houseValue)
					win = 2 * playerHandBet;	// win
			}
			std::cout << win << '\n';
			setBalance(getBalance() + win);
		}
	}
	std::cout << "The new balance is " << m_balance	<< "\n\n";
}