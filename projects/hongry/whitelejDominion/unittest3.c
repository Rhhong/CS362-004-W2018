#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

int gainCardTest(int supplyPos, struct gameState *state, int toFlag, int player){
	struct gameState pre;
	memcpy(&pre, state, sizeof(struct gameState));

	int res;
	res = gainCard(supplyPos, state, toFlag, player);

	if(supplyPos < 0 || supplyPos > treasure_map || pre.supplyCount[supplyPos] < 1){
		return (memcmp(&pre, state, sizeof(struct gameState)) == 0 && res == -1);
	}

	pre.supplyCount[supplyPos]--;

	if(toFlag == 1){
		pre.deck[player][pre.deckCount[player]] = supplyPos;
		pre.deckCount[player]++;
		return (memcmp(&pre, state, sizeof(struct gameState)) == 0 && res == 0);
	}
	else if(toFlag == 2){
		pre.hand[player][pre.handCount[player]] = supplyPos;
		pre.handCount[player]++;
		return (memcmp(&pre, state, sizeof(struct gameState)) == 0 && res == 0);
	}
	else{
		pre.discard[player][pre.discardCount[player]] = supplyPos;
		pre.discardCount[player]++;
		return (memcmp(&pre, state, sizeof(struct gameState)) == 0 && res == 0);
	}
}

int main(){
	int numPlayers = 2;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,sea_hag, tribute, smithy, council_room};
	int seed = 1000;
	struct gameState G;
	int allRes = 1;

	initializeGame(numPlayers, k, seed, &G);

	printf("\nTESTING gainCard() FUNCTION\n\n");

	//TEST 1: Add a card of each type from the supply pile to the player's deck.
	int i;
	int res = 1;
	for(i = 0; i < treasure_map+1; i++){
		res = res && gainCardTest(i, &G, 1, 0);
	}
	allRes = allRes && res;
	printf("Result of gainCard() for each supply card to deck: %d\n", res);

	//TEST 2: Add a card of each type from the supply pile to the player's hand.
	res = 1;
	for(i = 0; i < treasure_map+1; i++){
		res = res && gainCardTest(i, &G, 2, 0);
	}
	allRes = allRes && res;
	printf("Result of gainCard() for each supply card to hand: %d\n", res);

	//Test 3: Add a card of each type from the supply pile to the player's discard pile.
	res = 1;
	for(i = 0; i < treasure_map+1; i++){
		res = res && gainCardTest(i, &G, 0, 0);
	}
	allRes = allRes && res;
	printf("Result of gainCard() for each supply card to discard: %d\n", res);

	//Test 4: Attempt to add a card from the supply pile that does not exist.
	res = 1;
	res = gainCardTest(-1, &G, 0, 0);
	res = res && gainCardTest(27, &G, 0, 0);
	allRes = allRes && res;
	printf("Result of gainCard() for supplyPos out of range: %d\n", res);

	//Test 5: Attempt to add a card when the supply pile is empty for that type.
	res = 1;
	for(i = 0; i < treasure_map+1; i++){
		G.supplyCount[i] = 0;
		res = res && gainCardTest(i, &G, 1, 0);
	}
	allRes = allRes && res;
	printf("Result of gainCard() for empty supply pile: %d\n", res);

	if(allRes){
		printf("Result of all tests: SUCCESS\n");
	}
	else {
		printf("Result of all tests: FAIL\n");
	}

	return 0;
}