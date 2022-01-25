#ifndef SETTINGS_H
#define SETTINGS_H

/**
	contains rules of the game
*/
namespace settings {
	extern const int nInitialDecks;
	
	extern const int bustValue;
	extern const int houseHitIfLessThan;
	
	extern const bool splitOnSameRank;
    extern const bool canSurrender;
	
	
	
	// not implemented
	extern const bool canDoubleDownAfterASplit;

	// not implemented
	extern const int maxSplits;
}

#endif