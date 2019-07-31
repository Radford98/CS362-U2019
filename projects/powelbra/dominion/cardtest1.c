/*
Author: Brad Powell
Unit test for the initializeGame dominion function.

Include in your makefile (cardEffect only if you have a separate file for card effects)
cardtest1: cardtest1.c dominion.o rngs.o cardEffect.o testHelp.o
	gcc -o cardtest1 -g cardtest1.c dominion.o rngs.o cardEffect.o testHelp.o $(CFLAGS)
*/

#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include "interface.h"	// For cardNumToName function
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define TESTFUNCTION "initializeGame"

int main() {
	int numPlayers, i, j;
	int k[10] = { baron, minion, ambassador, tribute, mine, gardens, village, smithy, adventurer, great_hall };
	int seed = 100;
	struct gameState G[5];	// Array of gamestructs for different player counts
	char cardName[MAX_STRING_LENGTH];

	printf("---------- Testing Function: %s ----------\n", TESTFUNCTION);

	// ----- Test 1: Number of Players -----
	printf("\n----- TEST 1: Number of players\nShould return -1 on <2 or >4 players, 0 otherwise -----\n");
	// Initialize the five gameStates with different player counts, different seeds
	for (numPlayers = 1; numPlayers <= 5; numPlayers++) {
		printf("%d players: %d\n", numPlayers, initializeGame(numPlayers, k, seed*numPlayers, &G[numPlayers - 1]));
	}

	// ----- Test 2: Treasure/VP piles -----
	printf("\n----- TEST 2: Treasure and VP piles -----\n");
	for (i = 2; i < 5; i++) {
		printf("-Number of Players: %d-\n", i);
		printf("Copper should be %d: %d\n", 60 - 7 * i, G[i - 1].supplyCount[copper]);
		printf("Silver should be %d: %d\n", 40, G[i - 1].supplyCount[silver]);
		printf("Gold should be %d: %d\n", 30, G[i - 1].supplyCount[gold]);
		printf("Curses should be %d: %d\n", 10 * i - 10, G[i - 1].supplyCount[curse]);
		printf("VP piles should be 8 for 2 players, 12 otherwise\n");
		printf("Estate: %d\n", G[i - 1].supplyCount[estate]);
		printf("Duchy: %d\n", G[i - 1].supplyCount[duchy]);
		printf("Province: %d\n", G[i - 1].supplyCount[province]);
	}

	// ----- Test 3: Kingdom Piles -----
	printf("\n----- TEST 3: Kingdom Piles -----\n");
	printf("Chosen kingdom piles should be at 10, rest -1.\n");
	printf("Chosen piles:\nBaron\tMinion\tAmbassador\tTribute\tMine\nGardens\tVillage\tSmithy\tAdventurer\tGreat Hall\n");
	printf("Exception: For 2 players, Gardens and Great Hall at 8, otherwise 12.\n");
	for (numPlayers = 2; numPlayers < 5; numPlayers++) {
		printf("-Players: %d-\n", numPlayers);
		for (i = adventurer; i <= treasure_map; i++) {
			cardNumToName(i, cardName);
			printf("%s: %d", cardName, G[numPlayers - 1].supplyCount[i]);
			// Newline every 5 kingdoms
			if (i % 5 == 0) {
				printf("\n");
			}
			else {
				printf("\t");
			}
		}
		printf("\n");
	}

	// ----- Test 4: Player Decks -----
	printf("\n----- TEST 4: Player Decks -----\n");
	printf("Players should have 10 cards: 3 estates and 7 coppers; 5 in deck, 5 in hand, 0 in discard.\n");
	for (numPlayers = 2; numPlayers < 5; numPlayers++) {	// Check each gameState
		printf("-Players: %d-\n", numPlayers);
		for (i = 0; i < numPlayers; i++) {	// Check each player in gameState
			int numEst = 0, numCop = 0;
			printf("Player %d deckCount: %d\thandCount: %d\t discardCount: %d\n", i + 1, G[numPlayers - 1].deckCount[i], G[numPlayers - 1].handCount[i], G[numPlayers - 1].discardCount[i]);
			// Count coppers/estates in hand and deck
			for (j = 0; j < G[numPlayers - 1].deckCount[i]; j++) {
				if (G[numPlayers - 1].deck[i][j] == estate) {
					numEst++;
				}
				else if (G[numPlayers - 1].deck[i][j] == copper) {
					numCop++;
				}
				else {
					cardNumToName(G[numPlayers - 1].deck[i][j], cardName);
					printf("Card %d in deck is a %s.\n", j, cardName);
				}
			}
			for (j = 0; j < G[numPlayers - 1].handCount[i]; j++) {
				if (G[numPlayers - 1].hand[i][j] == estate) {
					numEst++;
				}
				else if (G[numPlayers - 1].hand[i][j] == copper) {
					numCop++;
				}
				else {
					cardNumToName(G[numPlayers - 1].hand[i][j], cardName);
					printf("Card %d in hand is a %s.\n", j, cardName);
				}
			}
			// Report estate/copper count
			printf("Estates: %d\tCoppers: %d\n", numEst, numCop);
		}
	}

	// ----- Test 5: Shuffled Decks -----
	printf("\n----- TEST 5: Shuffled decks - Players decks should be different -----\n");
	for (numPlayers = 2; numPlayers < 5; numPlayers++) {
		printf("-Players: %d-\n", numPlayers);
		// Compare each deck to the next player's
		for (i = 0; i < numPlayers-1; i++) {	// Loop through all but last player
			int sentinel = 0;
			for (j = 0; j < G[numPlayers - 1].deckCount[i]; j++) {	// Loop through each card in deck
				if (G[numPlayers - 1].deck[i][j] != G[numPlayers - 1].deck[i + 1][j]) {
					sentinel = 1;	// If a difference is found, set the sentinel
					break;
				}
			}
			if (sentinel) {
				printf("Players %d and %d have different decks.\n", i + 1, i + 2);
			}
			else {
				printf("Players %d and %d have the same deck!\n", i + 1, i + 2);
			}
		}
	}

	// ----- Test 6: Game begin -----
	printf("\n----- TEST 6: Player 1 should be ready to play -----\n");
	for (numPlayers = 2; numPlayers < 5; numPlayers++) {
		// Count up coppers
		int numCop = 0;
		for (j = 0; j < G[numPlayers - 1].handCount[0]; j++) {
			if (G[numPlayers - 1].hand[0][j] == copper) {
				numCop++;
			}
		}
		printf("-Players: %d-\n", numPlayers);
		printf("Expected buys: 1\t\tActual buys: %d\n", G[numPlayers-1].numBuys);
		printf("Expected actions: 1\t\tActual actions: %d\n", G[numPlayers - 1].numActions);
		printf("Expected coins: %d\t\tActual coins: %d\n", numCop, G[numPlayers - 1].coins);
		printf("Expected played count: 0\t Actual played count: %d\n", G[numPlayers - 1].playedCardCount);
		printf("Whose turn should be 0: %d\n", whoseTurn(&G[numPlayers - 1]));
	}
	
	printf("\n");
	return 0;
}