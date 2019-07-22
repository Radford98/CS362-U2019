/*
Author: Brad Powell
Random test for Baron.

Include in your makefile (cardEffect only if you have a separate file for card effects)
cardtest1: cardtest1.c dominion.o rngs.o cardEffect.o
	gcc -o cardtest1 -g cardtest1.c dominion.o rngs.o cardEffect.o $(CFLAGS)
*/

#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#define TESTCARD "Baron"

int main() {
	srand(time(0));
	struct gameState pre, post;


	int i, j, bonus, handPos;
	for (i = 0; i < 2000; i++) {
		// Generate a random game state with bytes from 0-255
		for (j = 0; j < sizeof(struct gameState); j++) {
			((char*)&post)[j] = rand() % 256;
		}

		// Set important variables to reasonable values
		post.numPlayers = (rand() % 3) + 2;
		post.whoseTurn = (rand() % 3) + 2;
		post.numBuys = (rand() % INT_MAX) - 1;	// Must be 1 less than the max so Baron can add 1 to it
		post.supplyCount[estate] = rand() % 3;
		post.discardCount[post.whoseTurn] = (rand() % MAX_DECK) - 1;
		post.handCount[post.whoseTurn] = rand() % MAX_HAND;
		post.playedCardCount = (rand() % MAX_HAND) - 1;
		bonus = (rand() % INT_MAX) - 4;
		handPos = rand() % post.handCount[post.whoseTurn];

		// Copy post to pre
		memcpy(&pre, &post, sizeof(struct gameState));



	}



	int numPlayers, i, j;
	int k[10] = { baron, minion, ambassador, tribute, mine, gardens, village, smithy, adventurer, great_hall };
	int seed = 100;
	struct gameState G[5];	// Array of gamestructs for different player counts
	char cardName[MAX_STRING_LENGTH];

	printf("---------- Randomly Testing: %s ----------\n", TESTCARD);

	// ----- Test 1: Number of Players -----
	printf("\n----- TEST 1: Number of players\nShould return -1 on <2 or >4 players, 0 otherwise -----\n");
	// Initialize the five gameStates with different player counts, different seeds
	for (numPlayers = 1; numPlayers <= 5; numPlayers++) {
		printf("%d players: %d\n", numPlayers, initializeGame(numPlayers, k, seed*numPlayers, &G[numPlayers - 1]));
	}




	return 0;
}