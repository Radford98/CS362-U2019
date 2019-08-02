/*
Author: Brad Powell
Unit test for the endTurn dominion function.

Include in your makefile (cardEffect only if you have a separate file for card effects)
cardtest3: cardtest3.c dominion.o rngs.o cardEffect.o testHelp.o
	gcc -o cardtest3 -g cardtest3.c dominion.o rngs.o cardEffect.o testHelp.o $(CFLAGS)
*/

#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define TESTFUNCTION "endTurn"

int main() {
	int i, errTest;
	int k[10] = { baron, minion, ambassador, tribute, mine, gardens, village, smithy, adventurer, great_hall };
	int seed = 100;
	struct gameState G, testG;

	initializeGame(3, k, seed, &G);

	printf("---------- Testing Function: %s ----------\n", TESTFUNCTION);

	// ----- Test 1: Normal checks
	printf("\n----- TEST 1: Normal endTurn -----\n");

	// Set up players with estates in deck and coppers in hand
	G.handCount[0] = G.handCount[1] = G.handCount[2] = 5;
	G.deckCount[0] = G.deckCount[1] = G.deckCount[2] = 5;
	G.discardCount[0] = G.discardCount[1] = G.discardCount[2] = 0;
	for (i = 0; i < G.handCount[0]; i++) {
		G.hand[0][i] = G.hand[1][i] = G.hand[2][i] = copper;
		G.deck[0][i] = G.deck[1][i] = G.deck[2][i] = estate;
	}

	memcpy(&testG, &G, sizeof(struct gameState));


	errTest = endTurn(&testG);

	printf("Returned value should be 0: %d\n", errTest);
	printf("Previous player:\n");
	printf("Expected discardCount: %d\tActual discardCount: %d\n", G.discardCount[0] + G.handCount[0] + G.playedCardCount, testG.discardCount[0]);
	printf("Expected handCount: 5\t\tActual handCount: %d\n", testG.handCount[0]);
	printf("Expected deckCount: %d\t\tActual deckCount: %d\n", G.deckCount[0] - 5, testG.deckCount[0]);

	printf("Current player:\n");
	printf("Expected whoseTurn: 1\t\tActual whoseTurn: %d\n", whoseTurn(&testG));
	printf("Expected discardCount: 0\tActual discardCount: %d\n", testG.discardCount[1]);
	printf("Expected handCount: 5\t\tActual handCount: %d\n", testG.handCount[1]);
	printf("Expected deckCount: 5\t\tActual deckCount: %d\n", testG.deckCount[1]);
	printf("Expected buys: 1\t\tActual buys: %d\n", testG.numBuys);
	printf("Expected actions: 1\t\tActual actions: %d\n", testG.numActions);
	printf("Expected coins: 5\t\tActual coins: %d\n", testG.coins);
	printf("Expected playedCards: 0\t\tActual playedCards: %d\n", testG.playedCardCount);

	// ----- Test 2: Played card
	printf("\n----- TEST 2: Played cards moved to discard; Test player loop -----\n");
	
	// Reset testG
	memcpy(&testG, &G, sizeof(struct gameState));
	testG.whoseTurn = 2;	// Set last player to current player
	// Add cards to played area
	testG.playedCardCount = G.playedCardCount = 3;
	for (i = 0; i < testG.playedCardCount; i++) {
		testG.playedCards[i] = G.playedCards[i] = i + 7;	// Adventurer, council_room, feast
	}

	errTest = endTurn(&testG);

	printf("Returned value should be 0: %d\n", errTest);
	printf("Previous player:\n");
	printf("Expected discardCount: %d\tActual discardCount: %d\n", G.discardCount[2] + G.handCount[2] + G.playedCardCount, testG.discardCount[2]);
	printf("Expected handCount: 5\t\tActual handCount: %d\n", testG.handCount[2]);
	printf("Expected deckCount: %d\t\tActual deckCount: %d\n", G.deckCount[2] - 5, testG.deckCount[2]);

	printf("Current player:\n");
	printf("Expected whoseTurn: 0\t\tActual whoseTurn: %d\n", whoseTurn(&testG));
	printf("Expected discardCount: 0\tActual discardCount: %d\n", testG.discardCount[0]);
	printf("Expected handCount: 5\t\tActual handCount: %d\n", testG.handCount[0]);
	printf("Expected deckCount: 5\t\tActual deckCount: %d\n", testG.deckCount[0]);
	printf("Expected buys: 1\t\tActual buys: %d\n", testG.numBuys);
	printf("Expected actions: 1\t\tActual actions: %d\n", testG.numActions);
	printf("Expected coins: 5\t\tActual coins: %d\n", testG.coins);
	printf("Expected playedCards: 0\t\tActual playedCards: %d\n", testG.playedCardCount);
	


	printf("\n");
	return 0;
}