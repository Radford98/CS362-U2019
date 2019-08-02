#include "dominion.h"
#include "dominion_helpers.h"
#include "testHelp.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Verify none of the supply piles have changed
// Returns -1 On fail, 1 on success
int kingdomNoChange(struct gameState *G, struct gameState *testG) {
	int i;
	// Check all supply piles and verify no change has occured
	for (i = 0; i <= treasure_map; i++) {
		if (G->supplyCount[i] != testG->supplyCount[i]) {
			printf("Supply %d changed.\n", i);
			return -1;
		}
	}
	// If the loop finishes, no change detected
	return 1;
}

// Verify the players were unchanged
// Returns -1 on fail, 1 on success
int oppNoChange(struct gameState *G, struct gameState *testG) {
	int i, j, x = 1;	// x is returned; set to success by default
	for (i = 1; i < testG->numPlayers; i++) {
		// Verify hand, deck, and discard counts have changed.
		if (G->handCount[i] != testG->handCount[i]) {
			printf("Player %d's handCount changed.\n", i);
			x = -1;
		}
		if (G->deckCount[i] != testG->deckCount[i]) {
			printf("Player %d's deckCount changed.\n", i);
			x = -1;
		}
		if (G->discardCount[i] != testG->discardCount[i]) {
			printf("Player %d's discardCount changed.\n", i);
			x = -1;
		}
		// Verify cards in hand are same
		for (j = 0; j < testG->handCount[i]; j++) {
			if (G->hand[i][j] != testG->hand[i][j]) {
				printf("Player %d's hand change at position %d\n", i, j);
				x = -1;
			}
		}
		// Verify cards in deck are same
		for (j = 0; j < testG->deckCount[i]; j++) {
			if (G->deck[i][j] != testG->deck[i][j]) {
				printf("Player %d's deck change at position %d\n", i, j);
				x = -1;
			}
		}
		// Verify cards in discard are same
		for (j = 0; j < testG->discardCount[i]; j++) {
			if (G->discard[i][j] != testG->discard[i][j]) {
				printf("Player %d's discard change at position %d\n", i, j);
				x = -1;
			}
		}
	}
	// If loop ends, no change detected
	return x;
}




// Verify hand remains unchanged; Assumes played card was last in hand
// Returns -1 on fail, 1 on success
int handCheck(int player, struct gameState *G, struct gameState *testG) {
	int i;

	if (G->handCount[player] != testG->handCount[player]) {
		printf("Player %d's handCount changed.\n", player);
		return -1;
	}
	else {
		for (i = 0; i < G->handCount[player]; i++) {
			if (G->hand[player][i] != testG->hand[player][i]) {
				printf("Player %d's card in hand at position %d changed.\n", player, i);
				printf("Original: %d\t\tNew: %d\n", G->hand[player][i], testG->hand[player][i]);
				return -1;
			}
		}
		// If loop finished, all is well
		return 1;
	}
}

// Verify deck remains unchanged;
// Returns -1 on fail, 1 on success
int deckCheck(int player, struct gameState *G, struct gameState *testG) {
	int i;

	if (G->deckCount[player] != testG->deckCount[player]) {
		printf("Player %d's deckCount changed.\n", player);
		return -1;
	}
	else {
		for (i = 0; i < G->deckCount[player]; i++) {
			if (G->deck[player][i] != testG->deck[player][i]) {
				printf("Player %d's card in deck at position %d changed.\n", player, i);
				printf("Original: %d\t\tNew: %d\n", G->deck[player][i], testG->deck[player][i]);
				return -1;
			}
		}
		// If loop finished, all is well
		return 1;
	}
}

// Verify discard remains unchanged;
// Returns -1 on fail, 1 on success
int discardCheck(int player, struct gameState *G, struct gameState *testG) {
	int i;

	if (G->discardCount[player] != testG->discardCount[player]) {
		printf("Player %d's discardCount changed.\n", player);
		return -1;
	}
	else {
		for (i = 0; i < G->discardCount[player]; i++) {
			if (G->discard[player][i] != testG->discard[player][i]) {
				printf("Player %d's card in discard at position %d changed.\n", player, i);
				printf("Original: %d\t\tNew: %d\n", G->discard[player][i], testG->discard[player][i]);
				return -1;
			}
		}
		// If loop finished, all is well
		return 1;
	}
}