#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define testcard "smithy"

int smithyTest(int player, struct gameState *state, int handPos){
	struct gameState pre;
	memcpy(&pre, state, sizeof(struct gameState));

	int res = cardEffect(smithy, 0, 0, 0, state, handPos, NULL);

	int i;
	for(i = 0; i < 3; i++){
		drawCard(player, state);
	}
	discardCard(handPos, player, state, 0);

	int a = pre.handCount[player] == state->handCount[player];
	int b = pre.discardCount[player] == state->discardCount[player];
	int c = res == 0;
	//int d = pre.numActions - 1 == state->numActions;
	int e = state->playedCards[state->playedCardCount-1] == smithy;

	printf("Testing player drew 3 cards: %d\n", a);
	printf("Testing player discarded smithy card: %d\n", b);
	printf("Testing function output: %d\n", c);
	//printf("Testing numActions decremented: %d\n", d);
	printf("Testing card added to playedCards array: %d\n", e);

	return a && b && c && e;
}

int main(){
	int numPlayers = 2;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,sea_hag, tribute, smithy, council_room};
	int seed = 1000;
	struct gameState G;

	initializeGame(numPlayers, k, seed, &G);

	printf("\nTESTING SMITHY CARD\n\n");

	//Test 1: test smithy card played with full deck
	int player = 0;
	int res;
	int allRes = 1;
	G.hand[player][G.handCount[player]] = smithy;
	G.handCount[player]++;
	res = smithyTest(player, &G, G.handCount[player]-1);
	allRes = res && allRes;
	printf("Result of smithy card with full deck: %d\n\n", res);

	//Test 2: test smithy card with less than 3 cards in deck.
	G.deckCount[player] = 2;
	G.hand[player][G.handCount[player]] = smithy;
	G.handCount[player]++;
	G.discard[player][G.discardCount[player]] = smithy;
	G.discardCount[player]++;
	G.discard[player][G.discardCount[player]] = smithy;
	G.discardCount[player]++;
	res = smithyTest(player, &G, G.handCount[player]-1);
	allRes = res && allRes;
	printf("Result of smithy card with less than 3 cards in deck: %d\n\n", res);

	if(allRes){
		printf("Result of all tests: SUCCESS\n");
	}
	else {
		printf("Result of all tests: FAIL\n");
	}

	return 0;
}