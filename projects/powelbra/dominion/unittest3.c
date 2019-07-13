/*
Author: Brad Powell
Unit test for the Ambassador dominion card.

Include in your makefile (cardEffect only if you have a separate file for card effects)
unittest1: unittest3.c dominion.o rngs.o cardEffect.o testHelp.o
	gcc -o unittest3 -g unittest1.c dominion.o rngs.o cardEffect.o testHelp.o $(CFLAGS)
*/

#include "dominion.h"
#include "dominion_helpers.h"
#include "cardEffect.h"
#include "testHelp.h"
#include "rngs.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define TESTCARD "Ambassador"

int main() {
	int i, currentPlayer;
	int handPos = 0, choice1 = 0, choice2 = 0, choice3 = 0;

	// Initialize the game
	int seed = 100;
	int numPlayers = 4;
	struct gameState G, testG;
	int k[10] = { baron, minion, ambassador, tribute, mine, gardens, village, smithy, adventurer, great_hall };

	printf("--------------- Testing Card: %s ---------------\n", TESTCARD);

	// ---- Test 1: Reveal and return 1 card -----
	printf("\n----- TEST 1: Reveal and return 1 card -----\nDEBUG statements:\n");

	// Set up game
	initializeGame(numPlayers, k, seed, &G);
	currentPlayer = whoseTurn(&G);
	G.hand[currentPlayer][1] = smithy;	// Set second card in hand to be a smithy
	choice1 = 1;	// smithy handPos
	choice2 = 1;	// # smithies to return
	memcpy(&testG, &G, sizeof(struct gameState));

	// Add ambassador to hand
	testG.hand[currentPlayer][testG.handCount[currentPlayer]] = ambassador;
	handPos = testG.handCount[currentPlayer];
	testG.handCount[currentPlayer]++;

	// Play card
	playCard(handPos, choice1, choice2, choice3, &testG);

	printf("\n*~*~*~* Unit Tests *~*~*~*\n");
	printf("Expected buys: 1\t\tActual buys: %d\n", testG.numBuys);
	printf("Expected actions: 1\t\tActual actions: %d\n", testG.numActions);
	printf("Expected coins: +0\t\tActual coins: +%d\n", testG.coins - G.coins);
	printf("Expected handCount: 4\t\tActual handCount: %d\n", testG.handCount[currentPlayer]);
	printf("Expected playedCardCount: 1\tActual playerCardCount: %d\n", testG.playedCardCount);

	// Make sure hand is otherwise unaffected
	int handChck = 1;
	if (G.hand[currentPlayer][0] != testG.hand[currentPlayer][0]) {
		handChck = 0;
		printf("Hand not ok at position 0.\n");
	}
	// Since handPos=1 was smithy and those cards shifted left, testG hand 1 == G hand 2
	for (i = 1; i < testG.handCount[currentPlayer]; i++) {
		if (testG.hand[currentPlayer][i] != G.hand[currentPlayer][i + 1]) {
			handChck = 0;
			printf("Hand not ok at position %d.\n", i);
		}
	}
	if (handChck) {
		printf("Hand ok.\n");
	}

	// Verify current player's hand and discard are unchanged
	if (deckCheck(currentPlayer, &G, &testG) == 1) {
		printf("Deck ok.\n");
	}
	if (discardCheck(currentPlayer, &G, &testG) == 1) {
		printf("Discard ok.\n");
	}

	// Verify opponent's hands and decks are unchanged.
	// Also verify their discard count increase by 1 and the top card is a smithy
	for (i = 1; i < testG.numPlayers; i++) {
		printf("-- Player %d --\n", i);
		if (handCheck(i, &G, &testG)) {
			printf("Hand ok.\n");
		}
		if (deckCheck(i, &G, &testG)) {
			printf("Deck ok.\n");
		}

		printf("Expected discardCount: %d\tActual discardCount: %d\n", G.discardCount[i] + 1, testG.discardCount[i]);
		printf("Expected top discard: %d\tActual top discard: %d\n", smithy, testG.discard[i][testG.discardCount[i] - 1]);
	}

	// Verify smithy pile has reduced accordingly
	// Should be smithy supply + 1 - number of oppoenents; formula is +2 to account for numPlayers including current player
	printf("Expected smithy supply: %d\t\tActual smithy supply: %d\n", G.supplyCount[smithy] + 2 - G.numPlayers, testG.supplyCount[smithy]);

	// Reset smithy pile in order to run kingdomNoChange to check other piles.
	testG.supplyCount[smithy] = G.supplyCount[smithy];
	if (kingdomNoChange(&G, &testG) == 1) {
		printf("Kingdom piles ok.\n");
	}
}
