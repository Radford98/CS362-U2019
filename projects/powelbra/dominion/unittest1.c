/*
Author: Brad Powell
Unit test for the Baron dominion card.

Include in your makefile (cardEffect only if you have a separate file for card effects)
unittest1: unittest1.c dominion.o rngs.o cardEffect.o testHelp.o
	gcc -o unittest1 -g unittest1.c dominion.o rngs.o cardEffect.o testHelp.o $(CFLAGS)
*/

#include "dominion.h"
#include "dominion_helpers.h"
#include "cardEffect.h"
#include "testHelp.h"
#include "rngs.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define TESTCARD "Baron"




int main() {
	int i;
	int handPos = 0, choice1 = 0, choice2 = 0, choice3 = 0;

		
	// Initialize the game
	int seed = 100;
	int numPlayers = 2;
	struct gameState G, testG;
	int k[10] = { baron, minion, ambassador, tribute, mine, gardens, village, smithy, adventurer, great_hall };

	initializeGame(numPlayers, k, seed, &G);

	printf("--------------- Testing Card: %s ---------------\n", TESTCARD);
	
	// ---- Test 1: Discard estate with estate in hand -----
	printf("\n----- TEST 1: Choice1 = 1; discard estate -----\n");
	int currentPlayer = whoseTurn(&G);

	// Make sure the hand has an estate card
	G.hand[currentPlayer][2] = estate;
	choice1 = 1;

	// copy the game state for comparison
	memcpy(&testG, &G, sizeof(struct gameState));

	// Add baron to the hand
	testG.hand[currentPlayer][testG.handCount[currentPlayer]] = baron;
	handPos = testG.handCount[currentPlayer];
	testG.handCount[currentPlayer]++;

	playCard(handPos, choice1, choice2, choice3, &testG);

	printf("\n*~*~*~* Unit Tests *~*~*~*\n");
	printf("Expected buys: 2\t\tActual buys: %d\n", testG.numBuys);
	printf("Expected actions: 0\t\tActual actions: %d\n", testG.numActions);
	printf("Expected coins: +4\t\tActual coins: +%d\n", testG.coins - G.coins);
	printf("Expected handCount: 4\t\tActual handCount: %d\n", testG.handCount[currentPlayer]);
	printf("Expected discardCount: 1\tActual discardCount: %d\n", testG.discardCount[currentPlayer]);
	printf("Expected top discard: %d\t\tActual top discard: %d\n", estate, testG.discard[currentPlayer][testG.discardCount[currentPlayer] - 1]);
	printf("Expected deckCount: 5\t\tActual deckCount: %d\n", testG.deckCount[currentPlayer]);

	// Count estates in current and previous hands
	int prevEstates = 0, curEstates = 0;
	for (i = 0; i < testG.handCount[currentPlayer]; i++) {
		if (testG.hand[currentPlayer][i] == estate) {
			curEstates++;
		}
		if (G.hand[currentPlayer][i] == estate) {
			prevEstates++;
		}
	}
	// Previous hand has one more card, so check if that last card is an estate
	if (G.hand[currentPlayer][G.handCount[currentPlayer] - 1] == estate) {
		prevEstates++;
	}
	if (curEstates == prevEstates) {
		printf("Estate count in hand incorrect!\n");
	}
	if (kingdomNoChange(&G, &testG) == -1) {
		printf("A kingdom supply changed!\n");
	}
	if (oppNoChange(&G, &testG) == -1) {
		printf("An opponent's state changed!\n");
	}


	// ---- Test 2: Gain an estate -----
	printf("\n----- TEST 2: Choice1 = 0; gain estate -----\n");
	
	// Reset variables
	initializeGame(numPlayers, k, seed, &G);
	currentPlayer = whoseTurn(&G);
	choice1 = 0;
	memcpy(&testG, &G, sizeof(struct gameState));

	// Add baron to the hand
	testG.hand[currentPlayer][testG.handCount[currentPlayer]] = baron;
	handPos = testG.handCount[currentPlayer];
	testG.handCount[currentPlayer]++;

	// Play card
	playCard(handPos, choice1, choice2, choice3, &testG);

	printf("\n*~*~*~* Unit Tests *~*~*~*\n");
	printf("Expected buys: 2\t\tActual buys: %d\n", testG.numBuys);
	printf("Expected actions: 0\t\tActual actions: %d\n", testG.numActions);
	printf("Expected coins: +0\t\tActual coins: +%d\n", testG.coins - G.coins);
	printf("Expected handCount: 5\t\tActual handCount: %d\n", testG.handCount[currentPlayer]);
	printf("Expected discardCount: 1\tActual discardCount: %d\n", testG.discardCount[currentPlayer]);
	printf("Expected top discard: %d\t\tActual top discard: %d\n", estate, testG.discard[currentPlayer][testG.discardCount[currentPlayer] - 1]);
	printf("Expected deckCount: 5\t\tActual deckCount: %d\n", testG.deckCount[currentPlayer]);
	printf("Expected estate supply: %d\tActual estate supply: %d\n", G.supplyCount[estate] - 1, testG.supplyCount[estate]);
	
	if (oppNoChange(&G, &testG) == -1) {
		printf("An opponent's state changed!\n");
	}
	if (handCheck(currentPlayer, &G, &testG) == -1) {
		printf("The hand was changed!\n");
	}
	if (deckCheck(currentPlayer, &G, &testG) == -1) {
		printf("The deck was changed!\n");
	}
	// Reset testG's estate supply to G so kingdomNoChange can be run to detect other changes
	testG.supplyCount[estate] = G.supplyCount[estate];
	if (kingdomNoChange(&G, &testG) == -1) {
		printf("A (non-estate) kingdom supply has changed!\n");
	}



	// ---- Test 3: Try to discard estate when none in hand -----
	printf("\n----- TEST 3: Choice1 = 1, but no estates in hand -----\n");

	// Reset variables
	initializeGame(numPlayers, k, seed, &G);
	currentPlayer = whoseTurn(&G);
	choice1 = 1;
	// Make sure no estates are in hand
	for (i = 0; i < G.handCount[currentPlayer]; i++) {
		G.hand[currentPlayer][i] = copper;
	}
	// Update coins now that hand has changed
	updateCoins(currentPlayer, &G, 0);
	memcpy(&testG, &G, sizeof(struct gameState));

	// Add baron to the hand
	testG.hand[currentPlayer][testG.handCount[currentPlayer]] = baron;
	handPos = testG.handCount[currentPlayer];
	testG.handCount[currentPlayer]++;

	// Play card
	playCard(handPos, choice1, choice2, choice3, &testG);

	printf("\n*~*~*~* Unit Tests *~*~*~*\n");
	printf("Expected buys: 2\t\tActual buys: %d\n", testG.numBuys);
	printf("Expected actions: 0\t\tActual actions: %d\n", testG.numActions);
	printf("Expected coins: +0\t\tActual coins: +%d\n", testG.coins - G.coins);
	printf("Expected handCount: 5\t\tActual handCount: %d\n", testG.handCount[currentPlayer]);
	printf("Expected discardCount: 1\tActual discardCount: %d\n", testG.discardCount[currentPlayer]);
	printf("Expected top discard: %d\t\tActual top discard: %d\n", estate, testG.discard[currentPlayer][testG.discardCount[currentPlayer] - 1]);
	printf("Expected deckCount: 5\t\tActual deckCount: %d\n", testG.deckCount[currentPlayer]);
	printf("Expected estate supply: %d\tActual estate supply: %d\n", G.supplyCount[estate] - 1, testG.supplyCount[estate]);

	if (oppNoChange(&G, &testG) == -1) {
		printf("An opponent's state changed!\n");
	}
	if (handCheck(currentPlayer, &G, &testG) == -1) {
		printf("The hand was changed!\n");
	}
	if (deckCheck(currentPlayer, &G, &testG) == -1) {
		printf("The deck was changed!\n");
	}
	// Reset testG's estate supply to G so kingdomNoChange can be run to detect other changes
	testG.supplyCount[estate] = G.supplyCount[estate];
	if (kingdomNoChange(&G, &testG) == -1) {
		printf("A (non-estate) kingdom supply has changed!\n");
	}



	printf("\n");
	return 0;
}


