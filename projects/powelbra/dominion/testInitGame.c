/*
Author: Brad Powell
Unit test for the initializeGame dominion function.
*/

#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include "interface.h"	// For cardNumToName function
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define TESTFUNCTION "initializeGame"

int main() {
	int numPlayers = 4, i, j;
	int k[10] = { baron, minion, ambassador, tribute, mine, gardens, village, smithy, adventurer, great_hall };
	int seed = 100;
	struct gameState G;	
	char cardName[MAX_STRING_LENGTH];

	printf("---------- Testing Function: %s ----------\n", TESTFUNCTION);

	printf("Players should have 10 cards: 3 estates and 7 coppers; 5 in deck, 5 in hand, 0 in discard.\n");
	initializeGame(numPlayers, k, seed, &G);


	for (i = 0; i < numPlayers; i++) {	// Check each player in gameState
		int numEst = 0, numCop = 0;
		printf("Player %d deckCount: %d\thandCount: %d\t discardCount: %d\n", i + 1, G.deckCount[i], G.handCount[i], G.discardCount[i]);
		// Count coppers/estates in hand and deck
		for (j = 0; j < G.deckCount[i]; j++) {
			if (G.deck[i][j] == estate) {
				numEst++;
			}
			else if (G.deck[i][j] == copper) {
				numCop++;
			}
			else {
				cardNumToName(G.deck[i][j], cardName);
				printf("Card %d in deck is a %s.\n", j, cardName);
			}
		}
		for (j = 0; j < G.handCount[i]; j++) {
			if (G.hand[i][j] == estate) {
				numEst++;
			}
			else if (G.hand[i][j] == copper) {
				numCop++;
			}
			else {
				cardNumToName(G.hand[i][j], cardName);
				printf("Card %d in hand is a %s.\n", j, cardName);
			}
		}
		// Report estate/copper count
		printf("Estates: %d\tCoppers: %d\n", numEst, numCop);
	}

	
	return 0;
}