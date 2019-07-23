/*
Author: Brad Powell
Random test for Minion.

Include in your makefile (cardEffect only if you have a separate file for card effects)
cardtest1: randomtestcard2.c dominion.o rngs.o cardEffect.o
	gcc -o randomtestcard2 -g randomtestcard2.c dominion.o rngs.o cardEffect.o $(CFLAGS)
*/

#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include "cardEffect.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#define TESTCARD "Minion"

int main() {
	printf("---------- Randomly Testing: %s ----------\n", TESTCARD);

	srand(time(0));
	struct gameState pre, post;
	int i, j, preBonus, postBonus, handPos, choice1, choice2, cp;
	int failCount = 0;

	for (i = 0; i < 5000; i++) {
		// Generate a random game state with bytes from 0-255
		for (j = 0; j < sizeof(struct gameState); j++) {
			((char*)&post)[j] = rand() % 256;
		}

		// Set important variables to reasonable values
		post.numPlayers = (rand() % 3) + 2;					// 2-4 players
		cp = post.whoseTurn = (rand() % post.numPlayers);	// Set current player (cp) to random player from 0 to numPlayers-1
		post.numActions = (rand() % INT_MAX) - 1;			// Must be 1 less than the max so Minion can add 1 to it
		post.playedCardCount = (rand() % MAX_HAND) - 1;

		// Set up each player with reasonable numbers
		for (j = 0; j < post.numPlayers; j++) {
			post.discardCount[j] = (rand() % MAX_DECK) - 1;
			post.handCount[j] = (rand() % MAX_HAND);
			post.deckCount[j] = (rand() % MAX_DECK) - 1;
		}

		// Handcount must be at least 1 to be the "minion" (handPos for discardCard in function)
		if (post.handCount[cp] == 0) {
			post.handCount[cp] = 1;		
		}


		// Copy post to pre
		memcpy(&pre, &post, sizeof(struct gameState));

		// Randomize inputs for Minion
		postBonus = preBonus = (rand() % 11);
		handPos = rand() % post.handCount[cp];

		// The most complicated part of Minion is when choice1 < 1 and choice2 > 0, so random generator will be skewed towards that
		choice1 = (rand() % 4) - 2;		// -2 to 1; 1/4 chance of being greater than 0
		choice2 = (rand() % 8) - 1;		// -1 to 6; 3/4 chance of being greater than 0

		// Test Minion
		playMinion(choice1, choice2, &post, handPos, &postBonus);

		// Minion should always have +1 action and discarded/moved to the played area
		pre.numActions++;
		discardCard(handPos, cp, &pre, 0);

		// Choice1 > 0: Gain two coins
		if (choice1 > 0) {
			preBonus += 2;
		}
		else if (choice1 < 1 && choice2 > 0) {
			// If there were at least 4 cards to draw, draw them!
			if (pre.deckCount[cp] >= 4) {
				// Discard cards from last to first
				while (pre.handCount[cp] > 0) {
					pre.handCount[cp]--;
					pre.discard[cp][pre.discardCount[c]] = pre.hand[cp][pre.handCount[cp]];
					pre.discardCount++;
					pre.hand[cp][pre.handCount[cp]] = -1;
				}
				// Draw cards
				for (j = 0; j < 4; j++) {
					pre.deckCount[cp]--;
					pre.hand[cp][j] = pre.deck[cp][pre.deckCount[cp]];
					pre.handCount[cp]++;
				}
			}
			// If there aren't 4 cards to draw, we need to trust the drawCard function in minion
			// Since any sort of shuffle here will come up with a different number due to random()
			// The deckCount should now be discardCount-(4-deckCount) = discardCount+deckCount-4 to account for some cards drawn.
			// E.g. 10 cards in discard, 1 in deck. 1 card in deck drawn, shuffle discard, draw 3 more for 7 cards in deck.
			else {
				memcpy(pre.deck[cp], post.deck[cp], sizeof(int)*(pre.discardCount[cp] + pre.deckCount[cp] - 4));
				memcpy(pre.discard[cp], post.discard[cp], sizeof(int)*pre.discardCount[cp]);
				memcpy(pre.hand[cp], post.hand[cp], sizeof(int) * 4);
				pre.deckCount[cp] = pre.discardCount[cp] + pre.deckCount[cp] - 4;
				pre.discardCount[cp] = 0;
				pre.handCount[cp] = 4;

			}

		}


	}
	return 0;
}