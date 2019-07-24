/*
Author: Brad Powell
Random test for Tribute.

Include in your makefile (cardEffect only if you have a separate file for card effects)
cardtest1: randomtestcard3.c dominion.o rngs.o cardEffect.o
	gcc -o randomtestcard3 -g randomtestcard3.c dominion.o rngs.o cardEffect.o $(CFLAGS)
*/

#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include "cardEffect.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TESTCARD "Tribute"

int main() {
	printf("---------- Randomly Testing: %s ----------\n", TESTCARD);

	srand(time(0));
	struct gameState pre, post;
	int i, j, k, preBonus, postBonus, handPos, cp, np, trib1, trib2, action, treasure, victory;
	int failCount = 0,
		allGood = 1;

	for (i = 1; i <= 5000; i++) {
		// Generate a random game state with bytes from 0-255
		for (j = 0; j < sizeof(struct gameState); j++) {
			((char*)&post)[j] = rand() % 256;
		}

		// Set important variables to reasonable values
		post.numPlayers = (rand() % 3) + 2;					// 2-4 players
		cp = post.whoseTurn = (rand() % post.numPlayers);	// Set current player (cp) to random player from 0 to numPlayers-1
		np = (cp + 1) % post.numPlayers;					// Set next player (np) to the next player. Modulus will loop back to 0 if cp is 3.
		post.numActions = (rand() % 100);
		post.playedCardCount = (rand() % 100);

		// Set up the current player with reasonable numbers
		// In the event of a shuffle, the discard and deck will be combined, so their total shouldn't exceed MAX_DECK
		post.discardCount[cp] = (rand() % MAX_DECK);
		post.deckCount[cp] = (rand() % (MAX_DECK - post.discardCount[j]));
		post.handCount[cp] = (rand() % (MAX_HAND - 4));		// The current player could draw up to 4 cards

		// Handcount must be at least 1 to be the "Tribute" (handPos for discardCard in function)
		if (post.handCount[cp] == 0) {
			post.handCount[cp] = 1;
		}

		// Set the next player with reasonable numbers
		// Their deck/discard are set with much lower random possibilities to help test the situations where the deck
		// is low and when there aren't enough cards to give to tribute
		post.discardCount[np] = (rand() % 10);
		post.deckCount[np] = (rand() % 10);

		// Tribute needs actual cards to key off of
		for (j = 0; j < post.discardCount[np]; j++) {
			post.discard[np][j] = (rand() % 27);		// Enum from curse (0) to treasure map (26)
		}
		for (j = 0; j < post.deckCount[np]; j++) {
			post.deck[np][j] = (rand() % 27);
		}

		// Randomize inputs for Tribute
		postBonus = preBonus = (rand() % 11);
		handPos = rand() % post.handCount[cp];

		// Copy post to pre
		memcpy(&pre, &post, sizeof(struct gameState));

		// Test Tribute
		playTribute(&post, handPos, &postBonus);

		// Tribute should always be discarded/moved to the played area
		discardCard(handPos, cp, &pre, 0);

		// Set card counters
		victory = treasure = action = 0;

		// Option 1: Next player's deck has 2+ cards
		if (pre.deckCount[np] >= 2) {
			// Store the tribute cards, mostly for error reporting later.
			trib1 = pre.deck[np][pre.deckCount[np] - 1];
			trib2 = pre.deck[np][pre.deckCount[np] - 2];

			// If the cards are the same, only check one card
			// j is set to 1 or 2 to look at the appropriate top card of the next player's deck
			if (trib1 == trib2) {
				j = 2;
			}
			else {
				j = 1;
			}

			// Use j's value from above - run either once or twice
			for (; j < 3; j++) {
				// If top card is a victory card
				if ((pre.deck[np][pre.deckCount[np] - j] >= 0 && pre.deck[np][pre.deckCount[np] - j] <= 3)
					|| pre.deck[np][pre.deckCount[np] - j] == great_hall
					|| pre.deck[np][pre.deckCount[np] - j] == gardens) {
					victory++;
				}
				// If treasure card
				if (pre.deck[np][pre.deckCount[np] - j] >= 4 && pre.deck[np][pre.deckCount[np] - j] <= 6) {
					treasure++;
				}
				// If an action card
				if ((pre.deck[np][pre.deckCount[np] - j] >= 7)
					&& pre.deck[np][pre.deckCount[np] - j] != gardens) {
					action++;
				}
			}

			// Update actions and coins: 2 actions/coins for each revealed card of the appropriate type
			pre.numActions += 2 * action;
			preBonus += 2 * treasure;

			// Draw cards if necessary
			if (victory) {
				victory *= 2;	// Update victory to the number of cards that need to be drawn
				// Enough cards without a shuffle
				if (pre.deckCount[cp] > = victory) {
					// Draw cards
					for (j = 0; j < victory; j++) {
						pre.deckCount[cp]--;
						pre.hand[cp][j] = pre.deck[cp][pre.deckCount[cp]];
						pre.handCount[cp]++;
					}
				}
				// 'Shuffle' first, trusting drawCard
				else {
					// Calculate what the new handsize will be, depending on the size of the deck and discard before the shuffle
					int newHand,
						totalDeck = pre.deckCount[cp] + pre.discardCount[cp];
					if (totalDeck < victory) {
						newHand = totalDeck;
					}
					else {
						newHand = victory;
					}

					// Need to overwrite all the memory of pre's deck, discard, and hand with post's
					// We need to cover all of pre's discard (since it will be bigger than post's)
					// and all of post's hand (since it will be bigger than pre's)
					// and all pre's discard and deck combined (cards before the draw)
					memcpy(pre.discard[cp], post.discard[cp], sizeof(int)*pre.discardCount[cp]);
					memcpy(pre.hand[cp], post.hand[cp], sizeof(int) * post.handCount[cp]);
					memcpy(pre.deck[cp], post.deck[cp], sizeof(int)*totalDeck);

					pre.deckCount[cp] = totalDeck - newHand;
					pre.discardCount[cp] = 0;
					pre.handCount[cp] = newHand;


				}
			}
			

		}










	}

	// If no errors, print success message.
	if (allGood) {
		printf("All tests passed.\n");
	}
	// If any gameState failures, print number of failures
	else if (failCount > 0) {
		printf("Total times gameStates didn't match: %d\nCheck gcov for patterns.\n", failCount);
	}
	// No need for default else since the bonus failures are already reported above.

	return 0;
}
