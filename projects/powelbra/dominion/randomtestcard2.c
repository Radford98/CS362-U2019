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
	int failCount = 0,
		allGood = 1;

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

		//  --------------------------- Choice1 > 0: Gain two coins -------------------------------
		if (choice1 > 0) {
			preBonus += 2;
		}
		// --------------------------- Choice1 < 1 && Choice2 > 0: Redraw ------------------------
		else if (choice1 < 1 && choice2 > 0) {
			

			// Opponents discard their hands and draw new cards on 4 or higher
			int redraw;		// bool to trigger rewdraw or not
			for (j = 0; j < pre.numPlayers; j++) {
				// Current player always redraws their hand
				if (j == cp) {
					redraw = 1;
				}
				// Opponents only redraw if they have at least 5 cards
				else if (pre.handCount[j] >= 5) {
					rewdraw = 1;
				}

				// If the j-th player needs to redraw their hand:
				if (redraw) {
					// If there were at least 4 cards to draw, draw them!
					if (pre.deckCount[j] >= 4) {
						// Discard cards from last to first
						while (pre.handCount[j] > 0) {
							pre.handCount[j]--;
							pre.discard[j][pre.discardCount[c]] = pre.hand[j][pre.handCount[j]];
							pre.discardCount++;
							pre.hand[j][pre.handCount[j]] = -1;
						}
						// Draw cards
						for (j = 0; j < 4; j++) {
							pre.deckCount[j]--;
							pre.hand[j][j] = pre.deck[j][pre.deckCount[j]];
							pre.handCount[j]++;
						}
					}
					// If there aren't 4 cards to draw, we need to trust the drawCard function in Minion
					// since any sort of shuffle here will come up with a different number due to random()
					else {
						// Calculate what the new handsize will be, depending on the size of the deck and discard and hand before the shuffle
						int newHand,
							totalDeck = pre.deckCount[j] + pre.discardCount[j] + pre.handCount[j];
						if (totalDeck < 4) {
							newHand = totalDeck;
						}
						else {
							newHand = 4;
						}
						// Need to overwrite all the memory of pre's deck, discard, and hand with post's
						// For discard and hand, we need to use pre's numbers so there aren't 'loose cards' beyond the boundary of what will be the new hand/discardCount.
						memcpy(pre.discard[j], post.discard[j], sizeof(int)*pre.discardCount[j]);
						memcpy(pre.hand[j], post.hand[j], sizeof(int) * pre.handCount[j]);

						// For the deck, we need to use the new deckCount so all of the 'cards' are properly transferred
						// deckCount is 1) previous deck, 2) previous discard, and 3) previous hand minus minion (taken care of above), minus number of cards drawn (newHand)
						// However, after the shuffle there are totalDeck cards before newHand cards are drawn and we want to make sure those blocks of memory are the same.
						memcpy(pre.deck[j], post.deck[j], sizeof(int)*totalDeck);

						// Discard will always be empty after a shuffle and handCount will be newHand as calculated above
						pre.deckCount[j] = totalDeck - newHand;
						pre.discardCount[j] = 0;
						pre.handCount[j] = newHand;
					}
				}
			}
		}



	}
	return 0;
}