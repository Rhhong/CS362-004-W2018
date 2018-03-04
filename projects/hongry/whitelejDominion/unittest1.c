#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

int testBuyCard(int supplyPos, struct gameState *state){
	struct gameState pre;
	memcpy(&pre, state, sizeof(struct gameState));

	int res;
	res = buyCard(supplyPos, state);

	if(pre.numBuys < 1 || supplyCount(supplyPos, &pre) < 1 || pre.coins < getCost(supplyPos)){
		return (memcmp(&pre, state, sizeof(struct gameState)) == 0 && res == -1);
	}
	
	pre.phase = 1;
	pre.coins = (pre.coins) - (getCost(supplyPos));
	pre.numBuys--;
	pre.supplyCount[supplyPos]--;
	int player = pre.whoseTurn;
	pre.discard[player][pre.discardCount[player]] = supplyPos;
	pre.discardCount[player]++;

	return (res == 0 && memcmp(&pre, state, sizeof(struct gameState)) == 0);
}

int main(){
	int numPlayers = 2;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,sea_hag, tribute, smithy, council_room};
	int seed = 1000;
	struct gameState G;
	int i;
	int res;
	int allRes = 1;

	initializeGame(numPlayers, k, seed, &G);

	printf("TESTING buyCard() FUNCTION\n\n");

	//TEST 1: test buyCard for each type of supply card.
	//Each call to buyCard should be successful.
	res = 1;
	for(i = 0; i < treasure_map+1; i++){
		G.coins = 100;
		G.numBuys = 1;

		res = res && testBuyCard(i, &G);
	}
	allRes = allRes && res;
	printf("Result of buyCard() success for each supply card: %d\n", res);


	//TEST 2: test buyCard when the player does not have any buys left.
	//Each call to buyCard should fail.
	res = 1;
	for(i = 0; i < treasure_map+1; i++){
		G.coins = 50;
		G.numBuys = 0;
		res = res && testBuyCard(i, &G);
	}
	allRes = allRes && res;
	printf("Result of buyCard() when player does not have any buys: %d\n", res);


	//TEST 3: test buyCard when the player does not have enough coins
	//Call to buyCard should fail.
	res = 1;
	for(i = 0; i < treasure_map+1; i++){
		G.coins = -5;
		G.numBuys = 1;
		res = res && testBuyCard(i, &G);
	}
	allRes = allRes && res;
	printf("Result of buyCard() when player does not have enough coins: %d\n", res);

	//TEST 4: test buyCard when the player does not have supply
	//Call to buyCard should fail.
	res = 1;
	for(i = 0; i < treasure_map+1; i++){
		G.coins = 50;
		G.numBuys = 1;
		G.supplyCount[i] = 0;
		res = res && testBuyCard(i, &G);
	}
	allRes = allRes && res;
	printf("Result of buyCard() when player does not have any supply: %d\n", res);

	if(allRes){
		printf("Result of all tests: SUCCESS\n");
	}
	else {
		printf("Result of all tests: FAIL\n");
	}
	

	return 0;
}