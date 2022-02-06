#ifndef SETTINGS_H
#define SETTINGS_H

/**
	contains rules of the game
*/
namespace settings {
	extern const int nInitialDecks;
	
	extern const int blackjackValue;
	extern const int houseHitIfLessThan;
	
	extern const bool splitOnSameRank;
    extern const bool canSurrender;
	extern const bool canDoubleDownAfterASplit;

	extern const int maxSplits;
}

#endif