/*
Author: Brad Powell
Unit test for the getWinners dominion function.
*/

#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define TESTFUNCTION "scoreFor"

int main() {
	int i;
	int k[10] = { baron, minion, ambassador, tribute, mine, gardens, village, smithy, adventurer, great_hall };
	int seed = 100;
	struct gameState G;
	int players[MAX_PLAYERS];

	initializeGame(4, k, seed, &G);


	printf("---------- Testing Function: %s ----------\n", TESTFUNCTION);


	// ----- Test 1: Winner 0 -----
	printf("\n----- TEST 1: Just Initialized; Every has 3 estates -----\n");
	initializeGame(4, k, seed, &G);
	printScores(&G);

	

	// ---- Test 2		 -----
	printf("\n----- TEST 2: Varied victory cards -----\n");
	// Player 0: All cards in deck are estates, no cards elsewhere. Score = 10
	G.handCount[0] = 0;
	G.discardCount[0] = 0;
	G.deckCount[0] = 10;
	for (i = 0; i < G.deckCount[0]; i++) {
		G.deck[0][i] = estate;
	}

	// Player 1: Duchy in discard, province in hand, estate and 2 great halls in deck, otherwise coppers. Score = 12
	G.handCount[1] = 7;
	G.discardCount[1] = 4;
	G.deckCount[1] = 20;
	for (i = 0; i < G.handCount[1]; i++) {
		G.hand[1][i] = copper;
	}
	for (i = 0; i < G.discardCount[1]; i++) {
		G.discard[1][i] = copper;
	}
	for (i = 0; i < G.deckCount[1]; i++) {
		G.deck[1][i] = copper;
	}
	G.hand[1][4] = province;
	G.discard[1][2] = duchy;
	G.deck[1][15] = estate;
	G.deck[1][0] = G.deck[1][3] = great_hall;

	// Player 2: 30 cards total, 4 gardens. Score = 12
	G.handCount[2] = 5;
	G.discardCount[2] = 14;
	G.deckCount[2] = 11;
	for (i = 0; i < G.handCount[2]; i++) {
		G.hand[2][i] = copper;
	}
	for (i = 0; i < G.discardCount[2]; i++) {
		G.discard[2][i] = copper;
	}
	for (i = 0; i < G.deckCount[2]; i++) {
		G.deck[2][i] = copper;
	}
	G.hand[2][2] = gardens;
	G.discard[2][0] = G.discard[2][13] = gardens;
	G.deck[2][5] = gardens;

	// Player 3: 3 duchies, 3 great halls; 1 duchy in deck, 2 in hand. 1 great hall in deck, 2 in discard. Score = 12
	G.handCount[3] = 5;
	G.discardCount[3] = 10;
	G.deckCount[3] = 6;
	for (i = 0; i < G.handCount[3]; i++) {
		G.hand[3][i] = copper;
	}
	for (i = 0; i < G.discardCount[3]; i++) {
		G.discard[3][i] = copper;
	}
	for (i = 0; i < G.deckCount[3]; i++) {
		G.deck[3][i] = copper;
	}
	G.hand[3][3] = G.hand[3][4] = G.deck[3][4] = duchy;
	G.discard[3][2] = G.discard[3][9] = G.deck[3][2] = great_hall;

	printf("Player 0: All cards in deck are estates, no cards elsewhere. Score = 10\n");
	printf("Player 1: Duchy in discard, province in hand, estate and 2 great halls in deck, otherwise coppers. Score = 12\n");
	printf("Player 2: 30 cards total, 4 gardens. 1 each in hand and deck, 2 in discard. Score = 12\n");
	printf("Player 3: 3 duchies, 3 great halls; 1 duchy in deck, 2 in hand. 1 great hall in deck, 2 in discard. Score = 12\n");
	printScores(&G);

	return 0;
}