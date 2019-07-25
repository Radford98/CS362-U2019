#include "cardEffect.h"
#include "dominion.h"
#include "dominion_helpers.h"
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>

void playBaron(int choice, struct gameState *state, int handPos, int* bonus) {
	int currentPlayer = whoseTurn(state);

	state->numBuys++;//Increase buys by 1!
	if (choice > 0) {//Boolean true or going to discard an estate
		int p = 0;//Iterator for hand!
		int card_not_discarded = 1;//Flag for discard set!
		while (card_not_discarded) {
			if (state->hand[currentPlayer][p] == estate) {//Found an estate card!
				bonus += 4;//Add 4 coins to the amount of coins
				state->discard[currentPlayer][state->discardCount[currentPlayer]] = state->hand[currentPlayer][p];	// Add to the discard pile
				state->discardCount[currentPlayer]++;
				for (p = 0; p < state->handCount[currentPlayer]; p++) {	// Starts from current position in hand and continues
					state->hand[currentPlayer][p] = state->hand[currentPlayer][p + 1];
				}
				state->hand[currentPlayer][state->handCount[currentPlayer]] = -1;
				state->handCount[currentPlayer]--;
				card_not_discarded = 0;//Exit the loop
			}
			else if (p > state->handCount[currentPlayer]) {
				if (DEBUG) {
					printf("No estate cards in your hand, invalid choice\n");
					printf("Must gain an estate if there are any\n");
				}
				gainCard(estate, state, 0, currentPlayer);
				card_not_discarded = 0;//Exit the loop
			}

			else {
				p++;//Next card
			}
		}
	}

	else {
		gainCard(estate, state, 0, currentPlayer);
	}

	// Discard card, using 0 to put it in the played pile
	discardCard(handPos, currentPlayer, state, 0);

}

void playMinion(int choice1, int choice2, struct gameState *state, int handPos, int *bonus) {
	int currentPlayer = whoseTurn(state);
	int i, j;	// Variables for loops
	//+1 action
	state->numActions++;

	if (choice1 > 0) {		//+2 coins
		bonus += 2;
		//discard card from hand
		discardCard(handPos, currentPlayer, state, 0);
	}

	else if (choice2 > 0) {		//discard hand, redraw 4, other players with 5+ cards discard hand and draw 4
		//discard hand ********* May need to be changed to a manual discard since this function puts cards into the played area ***********
		while (numHandCards(state) > 0) {
			discardCard(state->handCount[currentPlayer], currentPlayer, state, 0);
		}

		//draw 4
		for (i = 0; i < 4; i++) {
			drawCard(currentPlayer, state);
		}

		//other players discard hand and redraw if hand size > 4
		for (i = 0; i < state->numPlayers; i++) {
			if (i != currentPlayer) {
				if (state->handCount[i] > 4) {
					//discard hand
					while (state->handCount[i] > 0) {
						discardCard(state->handCount[i]-1, i, state, 1);
					}

					//draw 4
					for (j = 0; j < 4; j++) {
						drawCard(i, state);
					}
				}
			}
		}
	}
}

int playAmbassador(int choice1, int choice2, struct gameState *state, int handPos) {
	int currentPlayer = whoseTurn(state);
	int i, j;	// Variables for loops
	// Store enum of chosen card for use - most important later once cards are being discarded,
	// as hand[currentPlayer][choice1] will not stay the same card.
	int chosenCard = state->hand[currentPlayer][choice1];

	// Choice1 is which card is being revealed. It cannot be the same card as this Ambassador.
	// (It could be another one, but that would have a different handPos).
	if (choice1 == handPos) {
		return -1;
	}

	// Choice2 is how many cards the player wants to return. "Up to 2 cards" means it could be
	// 0, 1, or 2 cards that are being returned.
	if (choice2 > 2 || choice2 < 0) {
		return -1;
	}
	
	/* Compare each card in hand (except the played Ambassador) to the revealed card (choice1).
	If they match, increment j. Then use j to ensure the player can actually discard the number of cards
	they want to (e.g. the player wants to return 2 estates, but they only have 1 in hand)	*/
	j = 0;
	for (i = 0; i < state->handCount[currentPlayer]; i++) {
		if (state->hand[currentPlayer][i] == chosenCard) {
			j++;
		}
	}
	if (j < choice2) {
		return -1;
	}


	if (DEBUG)
		printf("Player %d reveals card number: %d\n", currentPlayer, chosenCard);

	//increase supply count for choosen card by amount being discarded
	state->supplyCount[chosenCard] += choice2;

	//each other player gains a copy of revealed card
	for (i = 0; i < state->numPlayers; i++) {
		if (i != currentPlayer) {
			gainCard(chosenCard, state, 0, i);
		}
	}

	//discard played card from hand
	discardCard(handPos, currentPlayer, state, 0);

	//trash copies of cards returned to supply - as of now the cards
	// were duplicated. Trashing prevents creating copies.
	for (j = 0; j < choice2; j++) {
		for (i = 0; i < state->handCount[currentPlayer]; i++) {
			if (state->hand[currentPlayer][i] == chosenCard) {
				discardCard(i, currentPlayer, state, 1);
			}
		}
	}

	return 0;
}

// Note: All cards grabbed by tribute are moved in playedCards by the end of the function,
// which is why they seem to be disappearing.
void playTribute(struct gameState *state, int handPos, int* bonus) {
	int currentPlayer = whoseTurn(state);
	int nextPlayer = (currentPlayer + 1) % state->numPlayers;
	int tributeRevealedCards[2] = { -1, -1 };
	int i;

	// Check if there is a single or no cards in the next player's deck and discard combined.
	if ((state->discardCount[nextPlayer] + state->deckCount[nextPlayer]) <= 1) {
		// If that single card is in the deck, add it to the tribute.
		if (state->deckCount[nextPlayer] > 0) {
			tributeRevealedCards[0] = state->deck[nextPlayer][state->deckCount[nextPlayer] - 1];
			state->discard[nextPlayer][state->discardCount[nextPlayer]] = state->deck[nextPlayer][state->deckCount[nextPlayer] - 1];
			state->discardCount[nextPlayer]++;
			state->deckCount[nextPlayer]--;
		}
		// If that single card is in the discard, add it to tribute. 
		else if (state->discardCount[nextPlayer] > 0) {
			tributeRevealedCards[0] = state->discard[nextPlayer][state->discardCount[nextPlayer] - 1];
			state->discardCount[nextPlayer]--;
		}
		else {
			//No Card to Reveal
			if (DEBUG) {
				printf("No cards to reveal\n");
			}
		}
	}

	// If there is more than 1 card in the deck
	else {
		// check if there are cards in the deck and shuffle if not
		if (state->deckCount[nextPlayer] == 0) {
			for (i = 0; i < state->discardCount[nextPlayer]; i++) {
				state->deck[nextPlayer][i] = state->discard[nextPlayer][i];//Move to deck
				state->deckCount[nextPlayer]++;
				state->discard[nextPlayer][i] = -1;
			}

			state->discardCount[nextPlayer] = 0;

			shuffle(nextPlayer, state);//Shuffle the deck
		}
		// Add the top two cards of the deck to tribute, decrementing deckCount each time.
		for (i = 0; i <= 2; i++) {
			tributeRevealedCards[i] = state->deck[nextPlayer][state->deckCount[nextPlayer] - 1];
			state->deck[nextPlayer][state->deckCount[nextPlayer]-1] = -1;
			state->deckCount[nextPlayer]--;
		}

	}

	//If we have a duplicate card, just drop one, adding it to the playedCards.
	if (tributeRevealedCards[0] == tributeRevealedCards[1]) { 
		tributeRevealedCards[1] = -1;
	}

	// Check tribute cards and increase appropriate parameters.
	for (i = 0; i <= 2; i++) {
		if (tributeRevealedCards[i] == copper || tributeRevealedCards[i] == silver || tributeRevealedCards[i] == gold) {//Treasure cards
			bonus += 2;
		}

		else if (tributeRevealedCards[i] == estate || tributeRevealedCards[i] == duchy || tributeRevealedCards[i] == province || tributeRevealedCards[i] == gardens || tributeRevealedCards[i] == great_hall) {//Victory Card Found
			drawCard(currentPlayer, state);
			drawCard(currentPlayer, state);
		}
		// All action cards are listed after gold in the enum
		else if (tributeRevealedCards[i] > gold) {
			state->numActions = state->numActions + 2;
		}

		// Move tribute cards to playedCards, only triggers if tribute card exists (not -1)
		if (tributeRevealedCards[i] != -1) {
			state->playedCards[state->playedCardCount] = tributeRevealedCards[i];
			state->playedCardCount++;
		}
	}

	discardCard(handPos, currentPlayer, state, 0);
}

int playMine(int choice1, int choice2, struct gameState *state, int handPos) {
	int currentPlayer = whoseTurn(state);
	int i, j;

	j = state->hand[currentPlayer][choice1];  //store card we will trash

	// Verify card to trash is treasure
	if (j < copper || j > gold) {
		return -1;
	}

	// Verify card to gain is treasure
	if (choice2 < copper || choice2 > gold) {
		return -1;
	}

	// Verify choice2 is within upgrade range
	if ((getCost(j) + 3) < getCost(choice2)) {
		return -1;
	}

	//discard card from hand
	discardCard(handPos, currentPlayer, state, 0);

	// Trash selected card; Need to loop through hand to find it since, after discarding mine,
	// choice1 no longer points to the correct card.
	for (i = 0; i < state->handCount[currentPlayer]; i++) {
		if (state->hand[currentPlayer][i] == j) {
			discardCard(i, currentPlayer, state, 0);
			break;
		}
	}

	// Gain card to hand
	gainCard(choice2, state, 2, currentPlayer);
	return 0;
}