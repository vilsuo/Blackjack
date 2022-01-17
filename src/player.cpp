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
	if (m_currentHand == static_cast<std::size_t>(0)) {
		return m_hand[0];
		
	} else if (m_currentHand == static_cast<std::size_t>(1)) {
		return m_hand[1];
		
	} else {
		// handle error
		assert(false && "invalid m_currentValue");
	}
}


bool Player::addCard(const Card& card) {
	// add card in to the current hand
    Player::getCurrentHand().addCard(card);

	// check if adding the card causes bust
    if (Player::bust()) {
		std::cout << "Bust!\n";
		
		// check if no hands left
        if(!Player::stand()) {
			std::cout << "Player's turn is over\n";
			return false;
		}
    }
	return true;
}

bool Player::stand() {
	// add current hand to finished hands
    if (m_currentHand == static_cast<std::size_t>(0)) {
		m_handIsFinished[0] = true;
	
		// no hands left
		return false;
		
	} else if (m_currentHand == static_cast<std::size_t>(1)) {
		m_handIsFinished[1] = true;
		
		// switch to first hand
		m_currentHand = static_cast<std::size_t>(0);
		return true;
		
	} else {
		// handle error
		assert(false && "invalid m_currentValue");
	}
}

void Player::split() {
    // take the card from current hand
	if (m_currentHand == static_cast<std::size_t>(0)) {
		Card card { Player::getCurrentHand().popLastCard() };
		
		m_currentHand = static_cast<size_t>(1);
		Player::getCurrentHand().addCard(card);
	} else {
		assert(false && "Player can only split after the initial deal");
	}
}

bool Player::bust() {
    return Player::getCurrentHand().getDeckTotalValue() > settings::bustValue;
}

void Player::printHands() const {
	m_hand[0].printDeck();
	m_hand[1].printDeck();
	std::cout << '\n';
}

void Player::addToBalance(int amount) {
    m_balance += amount;
}

bool Player::bet(int amount) {
    if (m_balance < amount) return false;
    m_balance -= amount;
    return true;
}

void Player::printInfo() const {
    std::cout << "Player id:\t" << m_id << '\n';
    std::cout << "Player name:\t" << m_name << '\n';
    std::cout << "Balance:\t" << m_balance << '\n';
}