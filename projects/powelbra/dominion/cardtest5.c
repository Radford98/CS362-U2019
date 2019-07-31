/*
Author: Brad Powell
Unit test for the drawCard dominion function.

Include in your makefile (cardEffect only if you have a separate file for card effects)
cardtest5: cardtest5.c dominion.o rngs.o cardEffect.o testHelp.o
	gcc -o cardtest5 -g cardtest5.c dominion.o rngs.o cardEffect.o testHelp.o $(CFLAGS)
*/

#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define TESTFUNCTION "drawCard"

int main() {
	int numPlayers = 2, i, seed = 10;
	int k[10] = { baron, minion, ambassador, tribute, mine, gardens, village, smithy, adventurer, great_hall };
	struct gameState G;
	int topCard, errTest, prevHandC, prevDeckC, shuffleCheck;


	initializeGame(numPlayers, k, seed, &G);


	printf("---------- Testing Function: %s ----------\n", TESTFUNCTION);
	printf("---Testing 30 draws on Deck = 15, Discard = 10 ---\n");

	G.deckCount[0] = 15;
	for (i = 0; i < G.deckCount[0]; i++) {
		G.deck[0][i] = i;	// Curse, estate... village
	}
	G.discardCount[0] = 10;
	for (i = 0; i < G.discardCount[0]; i++) {
		G.discard[0][i] = i + 15;	// Baron, great hall... salvager
	}


	for (i = 0; i < 30; i++) {
		topCard = G.deck[0][G.deckCount[0] - 1];	// Set topCard to top card of the deck
		prevHandC = G.handCount[0];	// Store handcount
		prevDeckC = G.deckCount[0];	// Store deckcount
		if (prevDeckC == 0) {	// Store discard count when shuffling
			shuffleCheck = G.discardCount[0];
			if (G.discardCount[0] == 0) {
				shuffleCheck = 1;	// Silence error message when no more cards to draw
			}
		}
		errTest = drawCard(0, &G);
		printf("Draw %d return: %d\n", i + 1, errTest);
		if (prevDeckC == 0) {
			printf("Shuffling...\n");
			if (G.deckCount[0] != shuffleCheck - 1) {	// shuffleCheck - 1 because a card was drawn
				printf("Not all of the cards were shuffled!\n");
			}
			if (G.discardCount[0] != 0) {
				printf("Discard count off.\n");
			}
		}
		else {
			if (G.hand[0][G.handCount[0] - 1] != topCard) {
				printf("Drawn card is different from what was the top card.\n");
			}
			if (G.handCount[0] != prevHandC + 1) {
				printf("Hand count off by %d\n", G.handCount[0] - (prevHandC + 1));
			}
			if (G.deckCount[0] != prevDeckC - 1) {
				printf("Deck count off by %d\n", G.deckCount[0] - (prevHandC - 1));
			}
		}
	}

	// Test played area
	printf("\n--- Testing if cards in played area stay put (they should!) ---\n");
	printf("5 cards in deck, 3 in discard. 10 Draws.\n");
	initializeGame(numPlayers, k, seed, &G);
	G.discardCount[0] = 3;
	for (i = 0; i < G.discardCount[0]; i++) {
		G.discard[0][i] = minion;
	}
	G.playedCardCount = 5;
	for (i = 0; i < G.playedCardCount; i++) {
		G.playedCards[i] = baron;
	}

	for (i = 0; i < 10; i++) {
		topCard = G.deck[0][G.deckCount[0] - 1];	// Set topCard to top card of the deck
		prevHandC = G.handCount[0];	// Store handcount
		prevDeckC = G.deckCount[0];	// Store deckcount
		if (prevDeckC == 0) {	// Store discard count when shuffling
			shuffleCheck = G.discardCount[0];
			if (G.discardCount[0] == 0) {
				shuffleCheck = 1;	// Silence error message when no more cards to draw
			}
		}
		errTest = drawCard(0, &G);
		printf("Draw %d return: %d\n", i + 1, errTest);
		if (prevDeckC == 0) {
			printf("Shuffling...\n");
			if (G.deckCount[0] != shuffleCheck - 1) {	// shuffleCheck - 1 because a card was drawn
				printf("Not all of the cards were shuffled!\n");
			}
			if (G.discardCount[0] != 0) {
				printf("Discard count off.\n");
			}
		}
		else {
			if (G.hand[0][G.handCount[0] - 1] != topCard) {
				printf("Drawn card is different from what was the top card.\n");
			}
			if (G.handCount[0] != prevHandC + 1) {
				printf("Hand count off by %d\n", G.handCount[0] - (prevHandC + 1));
			}
			if (G.deckCount[0] != prevDeckC - 1) {
				printf("Deck count off by %d\n", G.deckCount[0] - (prevHandC - 1));
			}
		}
	}

	int sentinel = 1;
	if (G.playedCardCount != 5) {
		printf("playedCardCount off by %d\n", G.playedCardCount - 5);
		sentinel = 0;
	}
	else {
		for (i = 0; i < 5; i++) {
			if (G.playedCards[i] != baron) {
				printf("Played card at %d changed to %d.\n", i, G.playedCards[i]);
				sentinel = 0;
			}
		}
	}
	if (sentinel) {
		printf("Played card area unaffected!\n");
	}

	printf("\n");
	return 0;
}