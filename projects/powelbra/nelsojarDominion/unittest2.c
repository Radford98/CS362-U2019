/*
Author: Brad Powell
Unit test for the Minion dominion card.

Include in your makefile (cardEffect only if you have a separate file for card effects)
unittest2: unittest2.c dominion.o rngs.o cardEffect.o testHelp.o
	gcc -o unittest2 -g unittest2.c dominion.o rngs.o cardEffect.o testHelp.o $(CFLAGS)
*/

#include "dominion.h"
#include "dominion_helpers.h"
#include "cardEffect.h"
#include "testHelp.h"
#include "rngs.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define TESTCARD "Minion"

int main() {
	int i, currentPlayer;
	int handPos = 0, choice1 = 0, choice2 = 0, choice3 = 0;

	// Initialize the game
	int seed = 100;
	int numPlayers = 4;
	struct gameState G, testG;
	int k[10] = { baron, minion, ambassador, tribute, mine, gardens, village, smithy, adventurer, great_hall };

	printf("--------------- Testing Card: %s ---------------\n", TESTCARD);

	// ---- Test 1: +2 coins -----
	printf("\n----- TEST 1: Choice1 = 1; +2 coins -----\n");

	// Set up game
	initializeGame(numPlayers, k, seed, &G);
	currentPlayer = whoseTurn(&G);
	choice1 = 1;
	choice2 = 0;
	memcpy(&testG, &G, sizeof(struct gameState));

	// Add minion to hand
	testG.hand[currentPlayer][testG.handCount[currentPlayer]] = minion;
	handPos = testG.handCount[currentPlayer];
	testG.handCount[currentPlayer]++;

	// Play card
	playCard(handPos, choice1, choice2, choice3, &testG);

	printf("\n*~*~*~* Unit Tests *~*~*~*\n");
	printf("Expected buys: 1\t\tActual buys: %d\n", testG.numBuys);
	printf("Expected actions: 1\t\tActual actions: %d\n", testG.numActions);
	printf("Expected coins: +2\t\tActual coins: +%d\n", testG.coins - G.coins);

	if (handCheck(currentPlayer, &G, &testG) == 1) {
		printf("Hand ok.\n");
	}
	if (deckCheck(currentPlayer, &G, &testG) == 1) {
		printf("Deck ok.\n");
	}
	if (discardCheck(currentPlayer, &G, &testG) == 1) {
		printf("Discard ok.\n");
	}
	if (oppNoChange(&G, &testG) == 1) {
		printf("Opponents ok.\n");
	}
	if (kingdomNoChange(&G, &testG) == 1) {
		printf("Kingdom piles ok.\n");
	}
	


	// ---- Test 2: Redraw -----
	printf("\n----- TEST 2: Choice2 = 1; Redraw; attack-----\n");

	// Set up game
	initializeGame(numPlayers, k, seed, &G);
	currentPlayer = whoseTurn(&G);
	choice1 = 0;
	choice2 = 1;

	// Set up other players for easy verification
	// Player 2: Starts with coppers in hand and estates in deck.
	// After: Coppers in discard and estates in hand
	int player = currentPlayer + 1;
	G.deckCount[player] = G.handCount[player] = 5;
	for (i = 0; i < G.deckCount[player]; i++) {
		G.hand[player][i] = copper;
		G.deck[player][i] = estate;
	}

	// Player 3: Starts with 4 coppers in hand and 5 estates in deck
	// After: No change
	player++;
	G.handCount[player] = 4;
	G.deckCount[player] = 5;
	for (i = 0; i < G.handCount[player]; i++) {
		G.hand[player][i] = copper;
		G.deck[player][i] = estate;
	}
	G.deck[player][4] = estate;	// set last estate

	// Player 3: Starts with 5 coppers in hand, 2 estates in deck, 3 estates in discard
	// After: At least 2 esates in hand, 4 cards in hand total, no cards in discard, and 6 cards in deck
	player++;
	G.handCount[player] = 5;
	G.deckCount[player] = 2;
	G.discardCount[player] = 3;
	for (i = 0; i < G.handCount[player]; i++) {
		G.hand[player][i] = copper;
	}
	for (i = 0; i < G.deckCount[player]; i++) {
		G.deck[player][i] = estate;
	}
	for (i = 0; i < G.discardCount[player]; i++) {
		G.discard[player][i] = estate;
	}

	memcpy(&testG, &G, sizeof(struct gameState));

	// Add minion to hand
	testG.hand[currentPlayer][testG.handCount[currentPlayer]] = minion;
	handPos = testG.handCount[currentPlayer];
	testG.handCount[currentPlayer]++;

	// Play card
	playCard(handPos, choice1, choice2, choice3, &testG);

	printf("\n*~*~*~* Unit Tests *~*~*~*\n");
	printf("Expected buys: 1\t\tActual buys: %d\n", testG.numBuys);
	printf("Expected actions: 1\t\tActual actions: %d\n", testG.numActions);
	printf("Expected handCount: 4\t\tActual handCount: %d\n", testG.handCount[currentPlayer]);
	printf("Expected deckCount: %d\t\tActual deckCount: %d\n", G.deckCount[currentPlayer] - 4, testG.deckCount[currentPlayer]);
	printf("Expected discardCount: %d\tActual discardCount: %d\n", G.discardCount[currentPlayer] + 5, testG.discardCount[currentPlayer]);

	// Coins should equal coppers in hand
	int copCount;
	for (i = 0; i < testG.handCount[currentPlayer]; i++) {
		if (testG.hand[currentPlayer][i] == copper) {
			copCount++;
		}
	}
	printf("Expected coins: %d\t\tActual coins: %d\n", copCount, testG.coins);

	if (kingdomNoChange(&G, &testG) == 1) {
		printf("Kingdom piles ok.\n");
	}

	/** Check player 2 **/
	player = currentPlayer + 1;
	// Discard should be 5 and full of coppers
	if (testG.discardCount[player] != 5) {
		printf("Opponent %d has wrong discardCount of %d, should be 5.\n", player, testG.discardCount[player]);
	}
	for (i = 0; i < testG.discardCount[player]; i++) {
		if (testG.discard[player][i] != copper) {
			printf("Opponent %d's discard at position %d is %d instead of %d.\n", player, i, testG.discard[player][i], copper);
		}
	}
	// Hand should be 4 and full of estates
	if (testG.handCount[player] != 4) {
		printf("Opponent %d has wrong handCount of %d, should be 4.\n", player, testG.handCount[player]);
	}
	for (i = 0; i < testG.handCount[player]; i++) {
		if (testG.hand[player][i] != estate) {
			printf("Opponent %d's discard at position %d is %d instead of %d.\n", player, i, testG.hand[player][i], estate);
		}
	}
	// Deck should be 1 estate
	if (testG.deckCount[player] != 1) {
		printf("Opponent %d has wrong deckCount of %d, should be 1.\n", player, testG.deckCount[player]);
	}
	if (testG.deck[player][0] != estate) {
		printf("Opponent %d's discard at position %d is %d instead of %d.\n", player, i, testG.deck[player][i], estate);
	}

	/** Check Player 3 **/
	player++;
	// Discard should be 0
	if (testG.discardCount[player] != 0) {
		printf("Opponent %d has wrong discardCount of %d, should be 0.\n", player, testG.discardCount[player]);
	}
	// Hand should be 4 and full of coppers
	if (testG.handCount[player] != 4) {
		printf("Opponent %d has wrong handCount of %d, should be 4.\n", player, testG.handCount[player]);
	}
	for (i = 0; i < testG.handCount[player]; i++) {
		if (testG.hand[player][i] != copper) {
			printf("Opponent %d's discard at position %d is %d instead of %d.\n", player, i, testG.hand[player][i], copper);
		}
	}
	// Deck should be 5 and full of estates
	if (testG.deckCount[player] != 5) {
		printf("Opponent %d has wrong deckCount of %d, should be 5.\n", player, testG.deckCount[player]);
	}
	for (i = 0; i < testG.deckCount[player]; i++) {
		if (testG.deck[player][i] != estate) {
			printf("Opponent %d's discard at position %d is %d instead of %d.\n", player, i, testG.deck[player][i], estate);
		}
	}

	/** Check Player 4 **/
	player++;
	// Discard should be 0
	if (testG.discardCount[player] != 0) {
		printf("Opponent %d has wrong discardCount of %d, should be 0.\n", player, testG.discardCount[player]);
	}
	// Hand should be 4 and have at least 2 estates
	if (testG.handCount[player] != 4) {
		printf("Opponent %d has wrong handCount of %d, should be 4.\n", player, testG.handCount[player]);
	}
	int estCount = 0;
	for (i = 0; i < testG.handCount[player]; i++) {
		if (testG.hand[player][i] == estate) {
			estCount++;
		}
	}
	if (estCount < 2) {
		printf("Opponent %d only has %d estates, should be at least 2.\n", player, estCount);
	}
	// Deck should be 6
	if (testG.deckCount[player] != 6) {
		printf("Opponent %d has wrong deckCount of %d, should be 6.\n", player, testG.deckCount[player]);
	}

	printf("\n");
	return 0;
}