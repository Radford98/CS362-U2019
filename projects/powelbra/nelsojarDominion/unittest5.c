/*
Author: Brad Powell
Unit test for the Mine dominion card.

Include in your makefile (cardEffect only if you have a separate file for card effects)
unittest5: unittest5.c dominion.o rngs.o cardEffect.o testHelp.o
	gcc -o unittest5 -g unittest5.c dominion.o rngs.o cardEffect.o testHelp.o $(CFLAGS)
*/

#include "dominion.h"
#include "dominion_helpers.h"
#include "cardEffect.h"
#include "testHelp.h"
#include "rngs.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define TESTCARD "Mine"

void testMine(int trash, int target, int expect) {
	int seed = 100;
	int numPlayers = 2;
	int k[10] = { baron, minion, ambassador, tribute, mine, gardens, village, smithy, adventurer, great_hall };
	int currentPlayer, errTest;
	int handPos = 0, choice1 = 0, choice2 = 0, choice3 = 0;
	struct gameState G, testG;

	// Set up game
	initializeGame(numPlayers, k, seed, &G);
	currentPlayer = whoseTurn(&G);
	G.hand[currentPlayer][0] = trash;	// make sure there's a treasure to use
	choice1 = 0;
	choice2 = target;
	updateCoins(currentPlayer, &G, 0);
	memcpy(&testG, &G, sizeof(struct gameState));

	// Add mine to hand
	testG.hand[currentPlayer][testG.handCount[currentPlayer]] = mine;
	handPos = testG.handCount[currentPlayer];
	testG.handCount[currentPlayer]++;

	// Play card
	errTest = playCard(handPos, choice1, choice2, choice3, &testG);

	// copper -> gold is the only invalid option, so expect different values
	if (trash == copper && target == gold) {
		printf("\n*~*~*~* Unit Tests *~*~*~*\n");
		printf("Should be -1: %d\n", errTest);
		printf("Expected buys: 1\t\tActual buys: %d\n", testG.numBuys);
		printf("Expected actions: 1\t\tActual actions: %d\n", testG.numActions);
		printf("Expected coin value: %d\t\tActual coins: %d\n", expect, testG.coins - G.coins);
		printf("Expected handCount: 6\t\tActual handCount: %d\n", testG.handCount[currentPlayer]);
		printf("Expected played count: 0\t Actual played: %d\n", testG.playedCardCount);

		if (deckCheck(currentPlayer, &G, &testG) == 1) {
			printf("Deck ok.\t");
		}
		if (discardCheck(currentPlayer, &G, &testG) == 1) {
			printf("Discard ok.\t");
		}
		if (oppNoChange(&G, &testG) == 1) {
			printf("Opponents ok.\n");
		}
		if (kingdomNoChange(&G, &testG) == 1) {
			printf("Kingdom piles ok.\n");
		}

	}
	else {

		printf("\n*~*~*~* Unit Tests *~*~*~*\n");
		printf("Should be 0: %d\n", errTest);
		printf("Expected buys: 1\t\tActual buys: %d\n", testG.numBuys);
		printf("Expected actions: 0\t\tActual actions: %d\n", testG.numActions);
		printf("Expected coin value: %d\t\tActual coins: %d\n", expect, testG.coins - G.coins);
		printf("Expected handCount: 5\t\tActual handCount: %d\n", testG.handCount[currentPlayer]);
		printf("Expected played count: 1\tActual played: %d\n", testG.playedCardCount);

		if (deckCheck(currentPlayer, &G, &testG) == 1) {
			printf("Deck ok.\t");
		}
		if (discardCheck(currentPlayer, &G, &testG) == 1) {
			printf("Discard ok.\t");
		}
		if (oppNoChange(&G, &testG) == 1) {
			printf("Opponents ok.\n");
		}

		// Check only the target treasure changes
		printf("Supply pile %d should change: ", choice2);
		if (kingdomNoChange(&G, &testG) != -1) {
			printf("Supply didn't change!\n");
		}
		else {
			printf("Expected: %d\tActual: %d\n", G.supplyCount[choice2] - 1, testG.supplyCount[choice2]);
		}
	}
}


int main() {
	printf("--------------- Testing Card: %s ---------------\n", TESTCARD);

	// Run tests
	printf("\n----- TEST 1: copper -> copper -----\n");
	testMine(copper, copper, 0);
	printf("\n----- TEST 2: copper -> silver -----\n");
	testMine(copper, silver, 1);
	printf("\n----- TEST 3: copper -> gold -----\n");
	testMine(copper, gold, 0);
	printf("\n----- TEST 4: silver -> copper -----\n");
	testMine(silver, copper, -1);
	printf("\n----- TEST 5: silver -> silver -----\n");
	testMine(silver, silver, 0);
	printf("\n----- TEST 6: silver -> gold -----\n");
	testMine(silver, gold, 1);
	printf("\n----- TEST 7: gold -> copper -----\n");
	testMine(gold, copper, -2);
	printf("\n----- TEST 8: gold -> silver -----\n");
	testMine(gold, silver, -1);
	printf("\n----- TEST 9: gold -> gold -----\n");
	testMine(gold, gold, 0);



	printf("\n");
	return 0;
}