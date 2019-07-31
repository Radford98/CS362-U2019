/*
Author: Brad Powell
Unit test for the getWinners dominion function.

Include in your makefile (cardEffect only if you have a separate file for card effects)
cardtest4: cardtest4.c dominion.o rngs.o cardEffect.o testHelp.o
	gcc -o cardtest4 -g cardtest4.c dominion.o rngs.o cardEffect.o testHelp.o $(CFLAGS)
*/

#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include "interface.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define TESTFUNCTION "getWinners"

int main() {
	int i;
	int k[10] = { baron, minion, ambassador, tribute, mine, gardens, village, smithy, adventurer, great_hall };
	int seed = 100;
	struct gameState G;
	int players[MAX_PLAYERS];

	initializeGame(4, k, seed, &G);

	
	printf("---------- Testing Function: %s ----------\n", TESTFUNCTION);


	// ----- Test 1: Winner 0 -----
	printf("\n----- TEST 1: Player 0 wins -----\n");
	G.discard[0][0] = estate;
	G.discardCount[0] = 1;
	printScores(&G);
	getWinners(players, &G);
	for (i = 0; i < G.numPlayers; i++) {
		printf("Player %d: %d\n", i, players[i]);
	}

	// ----- Test 2: Winner 1 -----
	printf("\n----- TEST 2: Player 1 wins -----\n");
	G.discard[1][0] = duchy;
	G.discardCount[1] = 1;
	printScores(&G);
	getWinners(players, &G);
	for (i = 0; i < G.numPlayers; i++) {
		printf("Player %d: %d\n", i, players[i]);
	}

	// ----- Test 3: Winner 2 -----
	printf("\n----- TEST 3: Player 2 wins -----\n");
	G.discard[2][0] = province;
	G.discardCount[2] = 1;
	printScores(&G);
	getWinners(players, &G);
	for (i = 0; i < G.numPlayers; i++) {
		printf("Player %d: %d\n", i, players[i]);
	}

	// ----- Test 4: Winner 3 -----
	printf("\n----- TEST 4: Player 3 wins -----\n");
	G.discard[3][0] = G.discard[3][1] = province;
	G.discardCount[3] = 2;
	printScores(&G);
	getWinners(players, &G);
	for (i = 0; i < G.numPlayers; i++) {
		printf("Player %d: %d\n", i, players[i]);
	}

	// ---- Test 5: Ties ----
	printf("\n----- TEST 5: Initial Game State; All scores equal -----\n");
	initializeGame(4, k, seed, &G);
	printf("Game just initialized. All scores should be 3, so players 1-3 should win.\n");
	printScores(&G);
	getWinners(players, &G);
	for (i = 0; i < G.numPlayers; i++) {
		printf("Player %d: %d\n", i, players[i]);
	}

	// ---- Test 6		 -----
	printf("\n----- TEST 6: 4 Players, 3-way tie; Varied victory cards -----\n");
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

	// Player 3: 3 duchies, 3 great halls. Score = 12
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


	G.whoseTurn = 1;
	getWinners(players, &G);
	printf("Player 2 is the current player, so players 3 and 4 should both win (value == 1).");
	printScores(&G);
	for (i = 0; i < G.numPlayers; i++) {
		printf("Player %d: %d\n", i + 1, players[i]);
	}

	// ----- Test 7: tied, same number of turns
	printf("\n----- TEST 7: Total 4-way tie -----\n");
	initializeGame(4, k, seed, &G);
	G.whoseTurn = 3;
	printScores(&G);
	getWinners(players, &G);
	for (i = 0; i < G.numPlayers; i++) {
		printf("Player %d: %d\n", i, players[i]);
	}

	// ----- Test 8: 3 players
	printf("\n----- TEST 8: 3 Players -----\n");
	initializeGame(3, k, seed, &G);
	printScores(&G);
	getWinners(players, &G);
	for (i = 0; i < G.numPlayers; i++) {
		printf("Player %d: %d\n", i, players[i]);
	}

	// ----- Test 9: 2 players
	printf("\n----- TEST 9: 2 Players -----\n");
	initializeGame(2, k, seed, &G);
	printScores(&G);
	getWinners(players, &G);
	for (i = 0; i < G.numPlayers; i++) {
		printf("Player %d: %d\n", i, players[i]);
	}

	printf("\n");
	return 0;
}