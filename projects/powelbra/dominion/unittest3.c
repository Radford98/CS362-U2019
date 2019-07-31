/*
Author: Brad Powell
Unit test for the Ambassador dominion card.

Include in your makefile (cardEffect only if you have a separate file for card effects)
unittest3: unittest3.c dominion.o rngs.o cardEffect.o testHelp.o
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
	printf("\n----- TEST 1: Reveal and return 1 card -----\n");

	// Set up game
	initializeGame(numPlayers, k, seed, &G);
	currentPlayer = whoseTurn(&G);
	G.hand[currentPlayer][1] = smithy;	// Set second card in hand to be a smithy
	choice1 = 1;	// smithy handPos
	choice2 = 1;	// # smithies to return
	updateCoins(currentPlayer, &G, 0);	// Update coins incase copper was replaced
	memcpy(&testG, &G, sizeof(struct gameState));

	// Add ambassador to hand
	testG.hand[currentPlayer][testG.handCount[currentPlayer]] = ambassador;
	handPos = testG.handCount[currentPlayer];
	testG.handCount[currentPlayer]++;

	// Play card
	playCard(handPos, choice1, choice2, choice3, &testG);

	printf("\n*~*~*~* Unit Tests *~*~*~*\n");
	printf("Expected buys: 1\t\tActual buys: %d\n", testG.numBuys);
	printf("Expected actions: 0\t\tActual actions: %d\n", testG.numActions);
	printf("Expected coins: +0\t\tActual coins: +%d\n", testG.coins - G.coins);
	printf("Expected handCount: 4\t\tActual handCount: %d\n", testG.handCount[currentPlayer]);
	printf("Expected playedCardCount: 1\tActual playedCardCount: %d\n", testG.playedCardCount);

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
	printf("Expected smithy supply: %d\tActual smithy supply: %d\n", G.supplyCount[smithy] + 2 - G.numPlayers, testG.supplyCount[smithy]);

	// Reset smithy pile in order to run kingdomNoChange to check other piles.
	testG.supplyCount[smithy] = G.supplyCount[smithy];
	if (kingdomNoChange(&G, &testG) == 1) {
		printf("Kingdom piles ok.\n");
	}



	// ---- Test 2: Reveal and return 2 cards -----
	printf("\n----- TEST 2: Reveal and return 2 cards -----\n");

	// Set up game
	initializeGame(numPlayers, k, seed, &G);
	currentPlayer = whoseTurn(&G);
	G.hand[currentPlayer][1] = smithy;	// Set second card in hand to be a smithy
	G.hand[currentPlayer][0] = smithy;	// Set first card to smithy
	choice1 = 1;	// smithy handPos
	choice2 = 2;	// # smithies to return
	updateCoins(currentPlayer, &G, 0);	// Update coins incase copper was replaced
	memcpy(&testG, &G, sizeof(struct gameState));

	// Add ambassador to hand
	testG.hand[currentPlayer][testG.handCount[currentPlayer]] = ambassador;
	handPos = testG.handCount[currentPlayer];
	testG.handCount[currentPlayer]++;

	// Play card
	playCard(handPos, choice1, choice2, choice3, &testG);

	printf("\n*~*~*~* Unit Tests *~*~*~*\n");
	printf("Expected buys: 1\t\tActual buys: %d\n", testG.numBuys);
	printf("Expected actions: 0\t\tActual actions: %d\n", testG.numActions);
	printf("Expected coins: +0\t\tActual coins: +%d\n", testG.coins - G.coins);
	printf("Expected handCount: 3\t\tActual handCount: %d\n", testG.handCount[currentPlayer]);
	printf("Expected playedCardCount: 1\tActual playedCardCount: %d\n", testG.playedCardCount);

	// Make sure hand is otherwise unaffected
	handChck = 1;
	// Since handPos=0, 1 was smithy and those cards shifted left, testG hand 0 == G hand 2
	for (i = 0; i < testG.handCount[currentPlayer]; i++) {
		if (testG.hand[currentPlayer][i] != G.hand[currentPlayer][i + 2]) {
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
	// Should be smithy supply + 2 - number of oppoenents; formula is +3 to account for numPlayers including current player
	printf("Expected smithy supply: %d\tActual smithy supply: %d\n", G.supplyCount[smithy] + 3 - G.numPlayers, testG.supplyCount[smithy]);

	// Reset smithy pile in order to run kingdomNoChange to check other piles.
	testG.supplyCount[smithy] = G.supplyCount[smithy];
	if (kingdomNoChange(&G, &testG) == 1) {
		printf("Kingdom piles ok.\n");
	}


	// ---- Test 3: Reveal and return 0 cards -----
	printf("\n----- TEST 3: Reveal and return 0 cards -----\n");

	// Set up game
	initializeGame(numPlayers, k, seed, &G);
	currentPlayer = whoseTurn(&G);
	G.hand[currentPlayer][1] = smithy;	// Set second card in hand to be a smithy
	choice1 = 1;	// smithy handPos
	choice2 = 0;	// # smithies to return
	updateCoins(currentPlayer, &G, 0);	// Update coins incase copper was replaced
	memcpy(&testG, &G, sizeof(struct gameState));

	// Add ambassador to hand
	testG.hand[currentPlayer][testG.handCount[currentPlayer]] = ambassador;
	handPos = testG.handCount[currentPlayer];
	testG.handCount[currentPlayer]++;

	// Play card
	playCard(handPos, choice1, choice2, choice3, &testG);

	printf("\n*~*~*~* Unit Tests *~*~*~*\n");
	printf("Expected buys: 1\t\tActual buys: %d\n", testG.numBuys);
	printf("Expected actions: 0\t\tActual actions: %d\n", testG.numActions);
	printf("Expected coins: +0\t\tActual coins: +%d\n", testG.coins - G.coins);
	printf("Expected handCount: 5\t\tActual handCount: %d\n", testG.handCount[currentPlayer]);
	printf("Expected playedCardCount: 1\tActual playedCardCount: %d\n", testG.playedCardCount);

	// Make sure hand is unaffected
	if (handCheck(currentPlayer, &G, &testG) == 1) {
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
	// Should be smithy supply + 0 - number of oppoenents; formula is +1 to account for numPlayers including current player
	printf("Expected smithy supply: %d\tActual smithy supply: %d\n", G.supplyCount[smithy] + 1 - G.numPlayers, testG.supplyCount[smithy]);

	// Reset smithy pile in order to run kingdomNoChange to check other piles.
	testG.supplyCount[smithy] = G.supplyCount[smithy];
	if (kingdomNoChange(&G, &testG) == 1) {
		printf("Kingdom piles ok.\n");
	}


	// ---- Test 4: Return when there isn't one -----
	printf("\n----- TEST 4: Attempt to return more cards than in hand -----\n");

	// Set up game
	initializeGame(numPlayers, k, seed, &G);
	currentPlayer = whoseTurn(&G);
	G.hand[currentPlayer][1] = smithy;	// Set second card in hand to be a smithy
	choice1 = 1;	// smithy handPos
	choice2 = 2;	// # smithies to return
	updateCoins(currentPlayer, &G, 0);	// Update coins incase copper was replaced
	memcpy(&testG, &G, sizeof(struct gameState));

	// Add ambassador to hand
	testG.hand[currentPlayer][testG.handCount[currentPlayer]] = ambassador;
	handPos = testG.handCount[currentPlayer];
	testG.handCount[currentPlayer]++;

	// Play card
	int errTest = playCard(handPos, choice1, choice2, choice3, &testG);

	printf("\n*~*~*~* Unit Tests *~*~*~*\n");
	printf("Should be -1: %d\n", errTest);
	printf("Expected buys: 1\t\tActual buys: %d\n", testG.numBuys);
	printf("Expected actions: 1\t\tActual actions: %d\n", testG.numActions);
	printf("Expected coins: +0\t\tActual coins: +%d\n", testG.coins - G.coins);
	printf("Expected handCount: 6\t\tActual handCount: %d\n", testG.handCount[currentPlayer]);
	printf("Expected playedCardCount: 0\tActual playedCardCount: %d\n", testG.playedCardCount);

	// Make sure hand is unaffected; Need to remove the added Ambassador to compare with original hand
	testG.handCount[currentPlayer]--;
	testG.hand[currentPlayer][testG.handCount[currentPlayer]] = -1;
	if (handCheck(currentPlayer, &G, &testG) == 1) {
		printf("Hand ok.\n");
	}
	// Verify current player's hand and discard are unchanged
	if (deckCheck(currentPlayer, &G, &testG) == 1) {
		printf("Deck ok.\n");
	}
	if (discardCheck(currentPlayer, &G, &testG) == 1) {
		printf("Discard ok.\n");
	}

	// Opponents should be entirely unaffected
	if (oppNoChange(&G, &testG) == 1) {
		printf("Opponents ok.\n");
	}

	// Kingdom cards should be unaffected
	if (kingdomNoChange(&G, &testG) == 1) {
		printf("Kingdom piles ok.\n");
	}


	// ---- Test 5: Attempt to return more cards -----
	printf("\n----- TEST 5: Attempt to return more than 2 cards -----\n");

	// Set up game
	initializeGame(numPlayers, k, seed, &G);
	currentPlayer = whoseTurn(&G);
	G.hand[currentPlayer][1] = G.hand[currentPlayer][0] = G.hand[currentPlayer][2] = smithy;	// Set cards in hand to be a smithy
	choice1 = 1;	// smithy handPos
	choice2 = 3;	// # smithies to return
	updateCoins(currentPlayer, &G, 0);	// Update coins incase copper was replaced
	memcpy(&testG, &G, sizeof(struct gameState));

	// Add ambassador to hand
	testG.hand[currentPlayer][testG.handCount[currentPlayer]] = ambassador;
	handPos = testG.handCount[currentPlayer];
	testG.handCount[currentPlayer]++;

	// Play card
	errTest = playCard(handPos, choice1, choice2, choice3, &testG);

	printf("\n*~*~*~* Unit Tests *~*~*~*\n");
	printf("Should be -1: %d\n", errTest);
	printf("Expected buys: 1\t\tActual buys: %d\n", testG.numBuys);
	printf("Expected actions: 1\t\tActual actions: %d\n", testG.numActions);
	printf("Expected coins: +0\t\tActual coins: +%d\n", testG.coins - G.coins);
	printf("Expected handCount: 6\t\tActual handCount: %d\n", testG.handCount[currentPlayer]);
	printf("Expected playedCardCount: 0\tActual playedCardCount: %d\n", testG.playedCardCount);

	// Make sure hand is unaffected; Need to remove the added Ambassador to compare with original hand
	testG.handCount[currentPlayer]--;
	testG.hand[currentPlayer][testG.handCount[currentPlayer]] = -1;
	if (handCheck(currentPlayer, &G, &testG) == 1) {
		printf("Hand ok.\n");
	}
	// Verify current player's hand and discard are unchanged
	if (deckCheck(currentPlayer, &G, &testG) == 1) {
		printf("Deck ok.\n");
	}
	if (discardCheck(currentPlayer, &G, &testG) == 1) {
		printf("Discard ok.\n");
	}

	// Opponents should be entirely unaffected
	if (oppNoChange(&G, &testG) == 1) {
		printf("Opponents ok.\n");
	}

	// Kingdom cards should be unaffected
	if (kingdomNoChange(&G, &testG) == 1) {
		printf("Kingdom piles ok.\n");
	}


	// ---- Test 6: Return 1 card when could've returned 2 -----
	printf("\n----- TEST 6: Return 1 card when could've returned 2 -----\n");

	// Set up game
	initializeGame(numPlayers, k, seed, &G);
	currentPlayer = whoseTurn(&G);
	G.hand[currentPlayer][1] = smithy;	// Set second card in hand to be a smithy
	G.hand[currentPlayer][0] = smithy;	// Set first card to smithy
	choice1 = 1;	// smithy handPos
	choice2 = 1;	// # smithies to return
	updateCoins(currentPlayer, &G, 0);	// Update coins incase copper was replaced
	memcpy(&testG, &G, sizeof(struct gameState));

	// Add ambassador to hand
	testG.hand[currentPlayer][testG.handCount[currentPlayer]] = ambassador;
	handPos = testG.handCount[currentPlayer];
	testG.handCount[currentPlayer]++;

	// Play card
	playCard(handPos, choice1, choice2, choice3, &testG);

	printf("\n*~*~*~* Unit Tests *~*~*~*\n");
	printf("Expected buys: 1\t\tActual buys: %d\n", testG.numBuys);
	printf("Expected actions: 0\t\tActual actions: %d\n", testG.numActions);
	printf("Expected coins: +0\t\tActual coins: +%d\n", testG.coins - G.coins);
	printf("Expected handCount: 4\t\tActual handCount: %d\n", testG.handCount[currentPlayer]);
	printf("Expected playedCardCount: 1\tActual playedCardCount: %d\n", testG.playedCardCount);

	// Make sure hand is otherwise unaffected
	// Smithy at [0] was returned, so test [0]-[3] should match original [1] - [4]
	handChck = 1;
	// Since handPos=1 was smithy and those cards shifted left, testG hand 1 == G hand 2
	for (i = 0; i < testG.handCount[currentPlayer]; i++) {
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
	printf("Expected smithy supply: %d\tActual smithy supply: %d\n", G.supplyCount[smithy] + 2 - G.numPlayers, testG.supplyCount[smithy]);

	// Reset smithy pile in order to run kingdomNoChange to check other piles.
	testG.supplyCount[smithy] = G.supplyCount[smithy];
	if (kingdomNoChange(&G, &testG) == 1) {
		printf("Kingdom piles ok.\n");
	}


	// ---- Test 7: Reveal and return 2 cards, but have 3 -----
	printf("\n----- TEST 7: Reveal and return 2 cards (reprise) -----\n");

	// Set up game
	initializeGame(numPlayers, k, seed, &G);
	currentPlayer = whoseTurn(&G);
	G.hand[currentPlayer][0] = G.hand[currentPlayer][1] = G.hand[currentPlayer][2] = smithy;	// Set cards in hand to be a smithy
	choice1 = 1;	// smithy handPos
	choice2 = 2;	// # smithies to return
	updateCoins(currentPlayer, &G, 0);	// Update coins incase copper was replaced
	memcpy(&testG, &G, sizeof(struct gameState));

	// Add ambassador to hand
	testG.hand[currentPlayer][testG.handCount[currentPlayer]] = ambassador;
	handPos = testG.handCount[currentPlayer];
	testG.handCount[currentPlayer]++;

	// Play card
	playCard(handPos, choice1, choice2, choice3, &testG);

	printf("\n*~*~*~* Unit Tests *~*~*~*\n");
	printf("Expected buys: 1\t\tActual buys: %d\n", testG.numBuys);
	printf("Expected actions: 0\t\tActual actions: %d\n", testG.numActions);
	printf("Expected coins: +0\t\tActual coins: +%d\n", testG.coins - G.coins);
	printf("Expected handCount: 3\t\tActual handCount: %d\n", testG.handCount[currentPlayer]);
	printf("Expected playedCardCount: 1\tActual playedCardCount: %d\n", testG.playedCardCount);

	// Make sure hand is otherwise unaffected
	handChck = 1;
	// Since handPos=0, 1, 2 was smithy and those cards shifted left, testG hand 0 == G hand 2
	for (i = 0; i < testG.handCount[currentPlayer]; i++) {
		if (testG.hand[currentPlayer][i] != G.hand[currentPlayer][i + 2]) {
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
	// Should be smithy supply + 2 - number of oppoenents; formula is +3 to account for numPlayers including current player
	printf("Expected smithy supply: %d\tActual smithy supply: %d\n", G.supplyCount[smithy] + 3 - G.numPlayers, testG.supplyCount[smithy]);

	// Reset smithy pile in order to run kingdomNoChange to check other piles.
	testG.supplyCount[smithy] = G.supplyCount[smithy];
	if (kingdomNoChange(&G, &testG) == 1) {
		printf("Kingdom piles ok.\n");
	}


	// ---- Test 8: Revealed Ambassador -----
	printf("\n----- TEST 8: Attempting to return the played Ambassador -----\n");

	// Set up game
	initializeGame(numPlayers, k, seed, &G);
	currentPlayer = whoseTurn(&G);
	G.hand[currentPlayer][0] = ambassador;	// Set first card to ambassador
	choice1 = 0;	// ambassador handPos
	choice2 = 2;	// # of ambassadors to return
	updateCoins(currentPlayer, &G, 0);	// Update coins incase copper was replaced
	memcpy(&testG, &G, sizeof(struct gameState));

	// Add ambassador to hand
	testG.hand[currentPlayer][testG.handCount[currentPlayer]] = ambassador;
	handPos = testG.handCount[currentPlayer];
	testG.handCount[currentPlayer]++;

	// Play card
	errTest = playCard(handPos, choice1, choice2, choice3, &testG);

	printf("\n*~*~*~* Unit Tests *~*~*~*\n");
	printf("Should be -1: %d\n", errTest);
	printf("Expected buys: 1\t\tActual buys: %d\n", testG.numBuys);
	printf("Expected actions: 1\t\tActual actions: %d\n", testG.numActions);
	printf("Expected coins: +0\t\tActual coins: +%d\n", testG.coins - G.coins);
	printf("Expected handCount: 6\t\tActual handCount: %d\n", testG.handCount[currentPlayer]);
	printf("Expected playedCardCount: 0\tActual playedCardCount: %d\n", testG.playedCardCount);

	// Make sure hand is unaffected; Need to remove the added Ambassador to compare with original hand
	testG.handCount[currentPlayer]--;
	testG.hand[currentPlayer][testG.handCount[currentPlayer]] = -1;
	if (handCheck(currentPlayer, &G, &testG) == 1) {
		printf("Hand ok.\n");
	}
	// Verify current player's deck and discard are unchanged
	if (deckCheck(currentPlayer, &G, &testG) == 1) {
		printf("Deck ok.\n");
	}
	if (discardCheck(currentPlayer, &G, &testG) == 1) {
		printf("Discard ok.\n");
	}

	// Opponents should be entirely unaffected
	if (oppNoChange(&G, &testG) == 1) {
		printf("Opponents ok.\n");
	}

	// Kingdom cards should be unaffected
	if (kingdomNoChange(&G, &testG) == 1) {
		printf("Kingdom piles ok.\n");
	}


	// ---- Test 9: Empty Supply -----
	printf("\n----- TEST 9: Supply runs out before all opponents are served-----\n");

	// Set up game
	initializeGame(numPlayers, k, seed, &G);
	currentPlayer = whoseTurn(&G);
	G.hand[currentPlayer][1] = smithy;	// Set second card in hand to be a smithy
	choice1 = 1;	// smithy handPos
	choice2 = 1;	// # smithies to return
	updateCoins(currentPlayer, &G, 0);	// Update coins incase copper was replaced
	G.supplyCount[smithy] = 1;
	memcpy(&testG, &G, sizeof(struct gameState));

	// Add ambassador to hand
	testG.hand[currentPlayer][testG.handCount[currentPlayer]] = ambassador;
	handPos = testG.handCount[currentPlayer];
	testG.handCount[currentPlayer]++;

	// Play card
	playCard(handPos, choice1, choice2, choice3, &testG);

	printf("\n*~*~*~* Unit Tests *~*~*~*\n");
	printf("Expected buys: 1\t\tActual buys: %d\n", testG.numBuys);
	printf("Expected actions: 0\t\tActual actions: %d\n", testG.numActions);
	printf("Expected coins: +0\t\tActual coins: +%d\n", testG.coins - G.coins);
	printf("Expected handCount: 4\t\tActual handCount: %d\n", testG.handCount[currentPlayer]);
	printf("Expected playedCardCount: 1\tActual playedCardCount: %d\n", testG.playedCardCount);

	// Make sure hand is otherwise unaffected
	handChck = 1;
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

	// Verify current player's deck and discard are unchanged
	if (deckCheck(currentPlayer, &G, &testG) == 1) {
		printf("Deck ok.\n");
	}
	if (discardCheck(currentPlayer, &G, &testG) == 1) {
		printf("Discard ok.\n");
	}

	// Verify opponent's hands and decks are unchanged.
	// Opponents 1 and 2 should should receive smithies, the third unchanged
	printf("Players 1 and 2 should receive smithies. The third should be unchanged.\n");
	for (i = 1; i < testG.numPlayers-1; i++) {
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

	printf("-- Player %d --\n", i);
	if (handCheck(i, &G, &testG)) {
		printf("Hand ok.\n");
	}
	if (deckCheck(i, &G, &testG)) {
		printf("Deck ok.\n");
	}

	printf("Expected discardCount: 0\tActual discardCount: %d\n", testG.discardCount[i]);
	printf("Expected top discard: 0\tActual top discard: %d\n", testG.discard[i][testG.discardCount[i] - 1]);


	// Verify smithy pile is 0
	printf("Expected smithy supply: 0\tActual smithy supply: %d\n", testG.supplyCount[smithy]);

	// Reset smithy pile in order to run kingdomNoChange to check other piles.
	testG.supplyCount[smithy] = G.supplyCount[smithy];
	if (kingdomNoChange(&G, &testG) == 1) {
		printf("Kingdom piles ok.\n");
	}

	printf("\n");
	return 0;
}
