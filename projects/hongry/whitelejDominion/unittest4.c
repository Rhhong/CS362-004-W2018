#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

int supplyCountTest(int card, struct gameState *state){
	struct gameState pre;
	memcpy(&pre, state, sizeof(struct gameState));

	int res = supplyCount(card, state);

	return res == pre.supplyCount[card] && memcmp(&pre, state, sizeof(struct gameState)) == 0;
}

int main(){
	int numPlayers = 2;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,sea_hag, tribute, smithy, council_room};
	int seed = 1000;
	struct gameState G;

	initializeGame(numPlayers, k, seed, &G);

	printf("\nTESTING supplyCount() FUNCTION\n\n");

	//TEST 1: For each card in the supply pile, test supplyCount() until the supply pile is empty.
	int i;
	int res = 1;
	for(i = 0; i < treasure_map+1; i++){
		while(G.supplyCount[i] >= 0){
			res = res && supplyCountTest(i, &G);
			G.supplyCount[i]--;
		}
	}
	printf("Result of supplyCount() after decrementing each supply pile: %d\n", res);

	return 0;
}