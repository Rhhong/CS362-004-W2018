#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

int numHandCardsTest(struct gameState *state){
	struct gameState pre;
	memcpy(&pre, state, sizeof(struct gameState));

	int res = numHandCards(state);

	return res == pre.handCount[whoseTurn(&pre)] && memcmp(&pre, state, sizeof(struct gameState)) == 0;
}

int main(){
	int numPlayers = 2;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,sea_hag, tribute, smithy, council_room};
	int seed = 1000;
	struct gameState G;

	initializeGame(numPlayers, k, seed, &G);

	printf("\nTESTING numHandCards() FUNCTION\n\n");

	//TEST 1: For each player, test numHandCards() and decrement the number of cards in hand by 1
	//Repeat until the player has no cards in hand.
	int i;
	int res = 1;
	for(i = 0; i < numPlayers; i++){
		while(G.handCount[i] >= 0){
			res = res && numHandCardsTest(&G);
			G.handCount[i]--;
		}
	}
	printf("Result of numHandCards() after decrementing number of cards: %d\n", res);

	return 0;
}