#include "player.h"
#include "card.h"
#include "deck.h"
#include "settings.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

Player::Player(std::string name, int balance)
    : m_name{name},
      m_balance{balance},
      m_id{s_idGenerator++}
{}


bool Player::addCard(const Card& card) {
	// check if we need to create a new hand
    if (m_handIndex >= m_hand.size()) {
        m_hand.push_back(Deck{});
    }
	// add card in to the current hand
    m_hand[m_handIndex].addCard(card);

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
    m_finishedHands.push_back(m_handIndex);

	// check if there is no hands left to handle
    if (m_indexStack.empty()) {
        return false;
    }

	// point to next hand to handle
    m_handIndex = m_indexStack.top();
    m_indexStack.pop();
    return true;
}

void Player::split() {
    // take the card from current hand
    Card card { m_hand[m_handIndex].popLastCard() };

    // add the card to a new hand
    m_indexStack.push(m_handIndex);
    m_handIndex = m_hand.size();

    Player::addCard(card);
}

bool Player::bust() const {
    return m_hand[m_handIndex].getDeckTotalValue() > settings::bustValue;
}

void Player::printHands() const {
	for(hand_type::size_type i{0}; i < m_hand.size(); ++i) {
		bool found{false};
		for (const auto& finishedHand : m_finishedHands) {
			if (finishedHand == i) {
				found = true;
				break;
			}
		}
		if (found) continue;
		
        m_hand[i].printDeck();
        std::cout << "# ";
    }
	
    // print finished hands in reverse order
    for(auto it{ m_finishedHands.rbegin() }; it != m_finishedHands.rend(); ++it) {
		std::cout << "f:";
		
        m_hand[*it].printDeck();
		std::cout << "# ";
    }
	
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