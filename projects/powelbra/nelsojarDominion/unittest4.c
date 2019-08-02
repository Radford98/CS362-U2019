/*
Author: Brad Powell
Unit test for the Tribute dominion card.

Include in your makefile (cardEffect only if you have a separate file for card effects)
unittest4: unittest4.c dominion.o rngs.o cardEffect.o testHelp.o
	gcc -o unittest4 -g unittest4.c dominion.o rngs.o cardEffect.o testHelp.o $(CFLAGS)
*/

#include "dominion.h"
#include "dominion_helpers.h"
#include "cardEffect.h"
#include "testHelp.h"
#include "rngs.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define TESTCARD "Tribute"

int main() {
	int i, currentPlayer, player;
	int handPos = 0, choice1 = 0, choice2 = 0, choice3 = 0;

	// Initialize the game
	int seed = 100;
	int numPlayers = 3;
	struct gameState G, testG;
	int k[10] = { baron, minion, ambassador, tribute, mine, gardens, village, smithy, adventurer, great_hall };

	printf("--------------- Testing Card: %s ---------------\n", TESTCARD);

	// ---- Test 1: 2 action cards -----
	printf("\n----- TEST 1: 2 action cards; 2 cards in enemy deck -----\n");

	// Set up game
	initializeGame(numPlayers, k, seed, &G);
	currentPlayer = whoseTurn(&G);
	player = currentPlayer + 1;
	G.deck[player][G.deckCount[player] - 1] = baron;
	G.deck[player][G.deckCount[player] - 2] = smithy;
	memcpy(&testG, &G, sizeof(struct gameState));

	// Add tribute to hand
	testG.hand[currentPlayer][testG.handCount[currentPlayer]] = tribute;
	handPos = testG.handCount[currentPlayer];
	testG.handCount[currentPlayer]++;

	// Play card
	playCard(handPos, choice1, choice2, choice3, &testG);

	printf("\n*~*~*~* Unit Tests *~*~*~*\n");
	printf("Expected buys: 1\t\tActual buys: %d\n", testG.numBuys);
	printf("Expected actions: 4\t\tActual actions: %d\n", testG.numActions);
	printf("Expected coins: +0\t\tActual coins: +%d\n", testG.coins - G.coins);
	printf("Expected handCount: 5\t\tActual handCount: %d\n", testG.handCount[currentPlayer]);
	printf("Expected played cards: 1\tActual played cards: %d\n", testG.playedCardCount);

	if (handCheck(currentPlayer, &G, &testG) == 1) {
		printf("Hand ok.\t");
	}
	if (deckCheck(currentPlayer, &G, &testG) == 1) {
		printf("Deck ok.\t");
	}
	if (discardCheck(currentPlayer, &G, &testG) == 1) {
		printf("Discard ok.\n");
	}
	if (kingdomNoChange(&G, &testG) == 1) {
		printf("Kingdom piles ok.\n");
	}

	// Check changes in next player
	player = currentPlayer + 1;
	printf("Next player expected dicardCount: 2\tActual discardCount: %d\n", testG.discardCount[player]);
	printf("Next player expected deckCount: %d\tActual deckCount: %d\n", G.deckCount[player] - 2, testG.deckCount[player]);
	if (handCheck(player, &G, &testG) == 1) {
		printf("Next player's hand ok.\n");
	}

	// Check next next player is untouched
	player++;
	if (deckCheck(player, &G, &testG) == 1) {
		printf("Third player deck ok.\t");
	}
	if (handCheck(player, &G, &testG) == 1) {
		printf("Third player hand ok.\t");
	}
	if (discardCheck(player, &G, &testG) == 1) {
		printf("Third player discard ok.\n");
	}



	// ---- Test 2: 2 treasure cards -----
	printf("\n----- TEST 2: 2 treasure cards; 1 card in enemy deck, 1 in discard -----\n");

	// Set up game
	initializeGame(numPlayers, k, seed, &G);
	currentPlayer = whoseTurn(&G);
	player = currentPlayer + 1;
	G.deck[player][0] = copper;
	G.deckCount[player] = 1;
	G.discard[player][0] = silver;
	G.discardCount[player] = 1;
	memcpy(&testG, &G, sizeof(struct gameState));

	// Add tribute to hand
	testG.hand[currentPlayer][testG.handCount[currentPlayer]] = tribute;
	handPos = testG.handCount[currentPlayer];
	testG.handCount[currentPlayer]++;

	// Play card
	playCard(handPos, choice1, choice2, choice3, &testG);

	printf("\n*~*~*~* Unit Tests *~*~*~*\n");
	printf("Expected actions: 0\t\tActual actions: %d\n", testG.numActions);
	printf("Expected coins: +4\t\tActual coins: +%d\n", testG.coins - G.coins);
	printf("Expected handCount: 5\t\tActual handCount: %d\n", testG.handCount[currentPlayer]);
	printf("Expected played cards: 1\tActual played cards: %d\n", testG.playedCardCount);

	if (handCheck(currentPlayer, &G, &testG) == 1) {
		printf("Hand ok.\t");
	}
	if (deckCheck(currentPlayer, &G, &testG) == 1) {
		printf("Deck ok.\t");
	}
	if (discardCheck(currentPlayer, &G, &testG) == 1) {
		printf("Discard ok.\n");
	}
	if (kingdomNoChange(&G, &testG) == 1) {
		printf("Kingdom piles ok.\n");
	}

	// Check changes in next player
	printf("Next player expected dicardCount: 2\tActual discardCount: %d\n", testG.discardCount[player]);
	printf("Next player expected deckCount: 0\tActual deckCount: %d\n", testG.deckCount[player]);


	// ---- Test 3: 2 victory cards -----
	printf("\n----- TEST 3: 2 victory cards; 0 card in enemy deck, 2 in discard -----\n");

	// Set up game
	initializeGame(numPlayers, k, seed, &G);
	currentPlayer = whoseTurn(&G);
	player = currentPlayer + 1;
	G.deckCount[player] = 0;
	G.discard[player][0] = estate;
	G.discard[player][1] = duchy;
	G.discardCount[player] = 2;
	memcpy(&testG, &G, sizeof(struct gameState));

	// Add tribute to hand
	testG.hand[currentPlayer][testG.handCount[currentPlayer]] = tribute;
	handPos = testG.handCount[currentPlayer];
	testG.handCount[currentPlayer]++;

	// Play card
	playCard(handPos, choice1, choice2, choice3, &testG);

	// Expected coins in this case should be equal to the coppers in hand, which might be higher after the draw
	int copCount;
	for (i = 0; i < testG.handCount[currentPlayer]; i++) {
		if (testG.hand[currentPlayer][i] == copper) {
			copCount++;
		}
	}
	printf("\n*~*~*~* Unit Tests *~*~*~*\n");
	printf("Expected actions: 0\t\tActual actions: %d\n", testG.numActions);
	printf("Expected coins: %d\t\tActual coins: %d\n", copCount, testG.coins);
	printf("Expected handCount: 9\t\tActual handCount: %d\n", testG.handCount[currentPlayer]);
	printf("Expected played cards: 1\tActual played cards: %d\n", testG.playedCardCount);
	// Drawing cards could cause a shuffle, so the total discard/deck combo needs to be consistent with drawing cards
	printf("Expected deck/discard combo: %d\t Actual combo: %d\n", G.deckCount[currentPlayer] + G.discardCount[currentPlayer] - 4, testG.deckCount[currentPlayer] + testG.discardCount[currentPlayer]);

	if (kingdomNoChange(&G, &testG) == 1) {
		printf("Kingdom piles ok.\n");
	}

	// Check changes in next player
	printf("Next player expected dicardCount: 2\tActual discardCount: %d\n", testG.discardCount[player]);
	printf("Next player expected deckCount: %d\tActual deckCount: %d\n", G.discardCount[player] - 2, testG.deckCount[player]);


	// ---- Test 4: Duplicatecards -----
	printf("\n----- TEST 4: Duplicate cards -----\n");

	// Set up game
	initializeGame(numPlayers, k, seed, &G);
	currentPlayer = whoseTurn(&G);
	player = currentPlayer + 1;
	G.deck[player][G.deckCount[player] - 1] = baron;
	G.deck[player][G.deckCount[player] - 2] = baron;
	memcpy(&testG, &G, sizeof(struct gameState));

	// Add tribute to hand
	testG.hand[currentPlayer][testG.handCount[currentPlayer]] = tribute;
	handPos = testG.handCount[currentPlayer];
	testG.handCount[currentPlayer]++;

	// Play card
	playCard(handPos, choice1, choice2, choice3, &testG);

	printf("\n*~*~*~* Unit Tests *~*~*~*\n");
	printf("Expected actions: 2\t\tActual actions: %d\n", testG.numActions);
	printf("Expected coins: +0\t\tActual coins: +%d\n", testG.coins - G.coins);
	printf("Expected handCount: 5\t\tActual handCount: %d\n", testG.handCount[currentPlayer]);
	printf("Expected played cards: 1\tActual played cards: %d\n", testG.playedCardCount);

	if (handCheck(currentPlayer, &G, &testG) == 1) {
		printf("Hand ok.\t");
	}
	if (deckCheck(currentPlayer, &G, &testG) == 1) {
		printf("Deck ok.\t");
	}
	if (discardCheck(currentPlayer, &G, &testG) == 1) {
		printf("Discard ok.\n");
	}
	if (kingdomNoChange(&G, &testG) == 1) {
		printf("Kingdom piles ok.\n");
	}

	// Check changes in next player
	printf("Next player expected dicardCount: 2\tActual discardCount: %d\n", testG.discardCount[player]);
	printf("Next player expected deckCount: %d\tActual deckCount: %d\n", G.deckCount[player] - 2, testG.deckCount[player]);


	// ---- Test 5: 1 card in deck -----
	printf("\n----- TEST 5: Only 1 card in deck -----\n");

	// Set up game
	initializeGame(numPlayers, k, seed, &G);
	currentPlayer = whoseTurn(&G);
	player = currentPlayer + 1;
	G.deck[player][0] = baron;
	G.deckCount[player] = 1;
	memcpy(&testG, &G, sizeof(struct gameState));

	// Add tribute to hand
	testG.hand[currentPlayer][testG.handCount[currentPlayer]] = tribute;
	handPos = testG.handCount[currentPlayer];
	testG.handCount[currentPlayer]++;

	// Play card
	playCard(handPos, choice1, choice2, choice3, &testG);

	printf("\n*~*~*~* Unit Tests *~*~*~*\n");
	printf("Expected actions: 2\t\tActual actions: %d\n", testG.numActions);
	printf("Expected coins: +0\t\tActual coins: +%d\n", testG.coins - G.coins);
	printf("Expected handCount: 5\t\tActual handCount: %d\n", testG.handCount[currentPlayer]);
	printf("Expected played cards: 1\tActual played cards: %d\n", testG.playedCardCount);

	if (handCheck(currentPlayer, &G, &testG) == 1) {
		printf("Hand ok.\t");
	}
	if (deckCheck(currentPlayer, &G, &testG) == 1) {
		printf("Deck ok.\t");
	}
	if (discardCheck(currentPlayer, &G, &testG) == 1) {
		printf("Discard ok.\n");
	}
	if (kingdomNoChange(&G, &testG) == 1) {
		printf("Kingdom piles ok.\n");
	}

	// Check changes in next player
	printf("Next player expected dicardCount: 1\tActual discardCount: %d\n", testG.discardCount[player]);
	printf("Next player expected deckCount: 0\tActual deckCount: %d\n", testG.deckCount[player]);


	// ---- Test 6: 1 card in discard-----
	printf("\n----- TEST 6: Only 1 card in discard -----\n");

	// Set up game
	initializeGame(numPlayers, k, seed, &G);
	currentPlayer = whoseTurn(&G);
	player = currentPlayer + 1;
	G.discard[player][0] = baron;
	G.discardCount[player] = 1;
	G.deckCount[player] = 0;
	memcpy(&testG, &G, sizeof(struct gameState));

	// Add tribute to hand
	testG.hand[currentPlayer][testG.handCount[currentPlayer]] = tribute;
	handPos = testG.handCount[currentPlayer];
	testG.handCount[currentPlayer]++;

	// Play card
	playCard(handPos, choice1, choice2, choice3, &testG);

	printf("\n*~*~*~* Unit Tests *~*~*~*\n");
	printf("Expected actions: 2\t\tActual actions: %d\n", testG.numActions);
	printf("Expected coins: +0\t\tActual coins: +%d\n", testG.coins - G.coins);
	printf("Expected handCount: 5\t\tActual handCount: %d\n", testG.handCount[currentPlayer]);
	printf("Expected played cards: 1\tActual played cards: %d\n", testG.playedCardCount);

	if (handCheck(currentPlayer, &G, &testG) == 1) {
		printf("Hand ok.\t");
	}
	if (deckCheck(currentPlayer, &G, &testG) == 1) {
		printf("Deck ok.\t");
	}
	if (discardCheck(currentPlayer, &G, &testG) == 1) {
		printf("Discard ok.\n");
	}
	if (kingdomNoChange(&G, &testG) == 1) {
		printf("Kingdom piles ok.\n");
	}

	// Check changes in next player
	printf("Next player expected dicardCount: 1\tActual discardCount: %d\n", testG.discardCount[player]);
	printf("Next player expected deckCount: 0\tActual deckCount: %d\n", testG.deckCount[player]);



	// ---- Test 7: no cards -----
	printf("\n----- TEST 7: No cards -----\n");

	// Set up game
	initializeGame(numPlayers, k, seed, &G);
	currentPlayer = whoseTurn(&G);
	player = currentPlayer + 1;
	G.deck[player][0] = -1;
	G.deckCount[player] = 0;
	memcpy(&testG, &G, sizeof(struct gameState));

	// Add tribute to hand
	testG.hand[currentPlayer][testG.handCount[currentPlayer]] = tribute;
	handPos = testG.handCount[currentPlayer];
	testG.handCount[currentPlayer]++;

	// Play card
	playCard(handPos, choice1, choice2, choice3, &testG);

	printf("\n*~*~*~* Unit Tests *~*~*~*\n");
	printf("Expected actions: 0\t\tActual actions: %d\n", testG.numActions);
	printf("Expected coins: +0\t\tActual coins: +%d\n", testG.coins - G.coins);
	printf("Expected handCount: 5\t\tActual handCount: %d\n", testG.handCount[currentPlayer]);
	printf("Expected played cards: 1\tActual played cards: %d\n", testG.playedCardCount);

	if (handCheck(currentPlayer, &G, &testG) == 1) {
		printf("Hand ok.\t");
	}
	if (deckCheck(currentPlayer, &G, &testG) == 1) {
		printf("Deck ok.\t");
	}
	if (discardCheck(currentPlayer, &G, &testG) == 1) {
		printf("Discard ok.\n");
	}
	if (kingdomNoChange(&G, &testG) == 1) {
		printf("Kingdom piles ok.\n");
	}

	// Check changes in next player
	printf("Next player expected dicardCount: 0\tActual discardCount: %d\n", testG.discardCount[player]);
	printf("Next player expected deckCount: 0\tActual deckCount: %d\n", testG.deckCount[player]);

	printf("\n");
	return 0;
}