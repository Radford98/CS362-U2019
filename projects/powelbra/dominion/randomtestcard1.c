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
#include "cardEffect.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#define TESTCARD "Baron"

int main() {
	printf("---------- Randomly Testing: %s ----------\n", TESTCARD);

	srand(time(0));
	struct gameState pre, post;
	int i, j, k, preBonus, postBonus, handPos, choice, isEstate;

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

		// Copy post to pre
		memcpy(&pre, &post, sizeof(struct gameState));

		// Randomize inputs for Baron
		postBonus = preBonus = (rand() % INT_MAX) - 4;
		handPos = rand() % post.handCount[post.whoseTurn];
		choice = (rand() % 3) - 1;	// Test choices of -1, 0, or 1

		// Test Baron
		playBaron(choice, &post, handPos, &postBonus);
		
		// Baron will always have +1 buy and the card should always be discarded
		pre.numBuys++;
		discardCard(handPos, pre.whoseTurn, &pre, 0);

		// Other post conditions of Baron depends on choice and if there's an estate
		// Store that number for use in hand shifting later
		isEstate = 0;
		for (k = 0; k < pre.handCount[pre.whoseTurn]; k++) {
			if (pre.hand[pre.whoseTurn][k] == estate) {
				isEstate = k;
				break;
			}
		}

		// Choice > 0: Discard an estate
		if (choice > 0) {
			// If there is an estate to discard
			if (isEstate) {
				preBonus += 4;
				pre.discard[pre.whoseTurn][pre.discardCount[pre.whoseTurn] = estate;
				pre.discardCount[pre.whoseTurn]++;
				// Shift cards in hand over
				for (; k < pre.handCount[pre.whoseTurn]; k++) {
					pre.hand[pre.whoseTurn][k] = pre.hand[pre.whoseTurn][k + 1];
				}
				pre.hand[pre.whoseTurn][pre.handCount[pre.whoseTurn]] = -1;
				pre.handCount[pre.whoseTurn]--;
			}
			// No estate to discard
			else {
				// If there is one in the supply
				if (pre.supplyCount[estate] > 0) {
					pre.discard[pre.whoseTurn][pre.discardCount[pre.whoseTurn]] = estate;
					pre.discardCount[pre.whoseTurn]++;
					pre.supplyCount[estate]--;
				}
			}
		}
		// Choice < 1: Gain an estate
		else {
			// If there is one in the supply
			if (pre.supplyCount[estate] > 0) {
				pre.discard[pre.whoseTurn][pre.discardCount[pre.whoseTurn]] = estate;
				pre.discardCount[pre.whoseTurn]++;
				pre.supplyCount[estate]--;
			}
		}

		if (preBonus != postBonus) {
			printf("Test %d: Bonus is %d, expected %d\n", i, postBonus, preBonus);
		}
		if (memcmp(&pre, &post, sizeof(struct gameState)) != 0) {
			printf("Test %d: gameStates do not match.\tChoice: %d\thandPos: %d\tisEstate: %d\n", i, choice, handPos, isEstate);
		}

	}




	return 0;
}