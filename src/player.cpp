#include "player.h"
#include "card.h"
#include "deck.h"
#include "settings.h"

#include <cassert>
#include <iostream>

Player::Player(std::string name, int balance)
    : m_name{name},
      m_balance{balance},
      m_id{s_idGenerator++}
{}

Deck& Player::getCurrentHand() {
	if (m_currentHandIndex == static_cast<std::size_t>(0)) 		return m_hand[0];
	else if (m_currentHandIndex == static_cast<std::size_t>(1)) return m_hand[1];
	else assert(false && "invalid m_currentHandIndex");
}

int& Player::getCurrentBet() {
	if (m_currentHandIndex == static_cast<std::size_t>(0))		return m_currentBets[0];
	else if (m_currentHandIndex == static_cast<std::size_t>(1)) return m_currentBets[1];
	else assert(false && "invalid m_currentHandIndex");
}

bool Player::addCard(const Card& card) {
	// add card in to the current hand
    Player::getCurrentHand().addCard(card);

	// check if adding the card causes bust
    if (Player::bust()) {
		std::cout << "Bust!\n";
		
		// stand and check if player has no hands left
        if(!Player::stand()) {
			std::cout << "No hands left\n";
			return false;
		}
		
    } else if (Player::getCurrentHand().getDeckTotalValue() == 21) {					// also check that player has two cards
		std::cout << "Blackjack!\n";
		if(!Player::stand()) {
			std::cout << "No hands left\n";
			return false;
		}
	}
	return true;
}

bool Player::stand() {
	// add current hand to finished hands
    if (m_currentHandIndex == static_cast<std::size_t>(0)) {
		m_handIsFinished[0] = true;
	
		// no hands left
		return false;
		
	} else if (m_currentHandIndex == static_cast<std::size_t>(1)) {
		m_handIsFinished[1] = true;
		
		// switch to first hand
		m_currentHandIndex = static_cast<std::size_t>(0);
		return true;
		
	} else {
		// handle error
		assert(false && "invalid m_currentHandIndex");
	}
}

void Player::split() {
	if (m_currentHandIndex == static_cast<std::size_t>(0)) {
		// take a card from current hand...
		Card card { Player::getCurrentHand().popLastCard() };
		// and the current bet
		int currentBet{ Player::getCurrentBet() };
		
		// Switch to a new hand...
		m_currentHandIndex = static_cast<size_t>(1);
		
		// and bet the same amount and take a new card
		Player::bet(currentBet);
		Player::getCurrentHand().addCard(card);
	} else {
		assert(false && "Invalid m_currentHandIndex: Player can only split after the initial deal");
	}
}

bool Player::doubleDown() {
	if (m_currentHandIndex == static_cast<std::size_t>(0)) {
		// take the current bet
		int currentBet{ Player::getCurrentBet() };
		
		// double the bet
		Player::bet(currentBet);
		return false;
	} else {
		assert(false && "Invalid m_currentHandIndex: Player can only double down before splitting");
		return true;
	}
}

void Player::surrender() {
	// allowed only as a first action
	if (m_currentHandIndex == static_cast<std::size_t>(0)) {
		m_balance += m_currentBets[0] / 2;
		m_currentBets = {0, 0};
	} else {
		// currently does not check that it is the first action,
		// only checks that player has not splitted
		assert(false && "Invalid m_currentHandIndex: Player can only surrender as a first action");
	}
}

bool Player::bust() {
    return Player::getCurrentHand().getDeckTotalValue() > settings::bustValue;
}

void Player::printHands() const {
	std::cout << m_hand[0] << m_hand[1] << '\n';
}

bool Player::bet(int amount) {
    if (m_balance < amount) return false;
    m_balance -= amount;
	
	Player::getCurrentBet() += amount;
	
    return true;
}

void Player::printInfo() const {
    std::cout << "Player id:\t" << m_id << '\n';
    std::cout << "Player name:\t" << m_name << '\n';
    std::cout << "Balance:\t" << m_balance << '\n';
}

void Player::reset() {
	m_hand = {Deck{}, Deck{}};				// clear both hands?
	m_handIsFinished = {false, false};
	m_currentHandIndex = 0;
	m_currentBets = {0,0};
}