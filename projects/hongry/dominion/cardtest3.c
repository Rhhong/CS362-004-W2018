#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define testcard "minion"

int minionTest(int choice1, int choice2, int player, struct gameState *state, int handPos){
	int i;

	struct gameState pre;
	memcpy(&pre, state, sizeof(struct gameState));

	int res = cardEffect(minion, choice1, choice2, 0, state, handPos, NULL);

	pre.numActions++;
	discardCard(handPos, player, &pre, 0);
	if(choice1){
		pre.coins += 2;
	}
	else{
		for(i = 0; i < pre.numPlayers; i++){
			if(i == player){
				pre.discardCount[player] += pre.handCount[player];
				pre.handCount[player] = 4;
			}
			else if(pre.handCount[i] >= 5){
				pre.discardCount[i] += pre.handCount[i];
				pre.handCount[i] = 4;
			}
		}
	}

	int a, b;
	if(choice1){
		a = pre.coins == state->coins;
		b = memcmp(&pre, state, sizeof(struct gameState)) == 0;

		printf("Testing two coins added: %d\n", a);
		printf("Testing state of game before and after are equal: %d\n", b);
	}
	else{
		a = 1;
		for(i = 0; i < pre.numPlayers; i++){
			a = a && pre.discardCount[i] == state->discardCount[i];
		}
		b = 1;
		for(i = 0; i < pre.numPlayers; i++){
			b = b && pre.handCount[i] == state->handCount[i];
		}

		printf("Testing number of cards in discard for each player: %d\n", a);
		printf("Testing number of cards in hand for each player: %d\n", b);
	}

	int c = pre.numActions == state->numActions;
	int d = res == 0;

	printf("Testing one action added: %d\n", c);
	printf("Testing function output correct: %d\n", d);

	return a && b && c && d;
}

int main(){
	int numPlayers = 4;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,sea_hag, tribute, smithy, council_room};
	int seed = 1000;
	struct gameState G;
	int i, j;

	initializeGame(numPlayers, k, seed, &G);

	printf("\nTESTING MINION CARD\n\n");

	//Test 1: Test option 1
	int player = 0;
	int res;
	int allRes = 1;
	G.hand[player][G.handCount[player]] = minion;
	G.handCount[player]++;
	for(i = 1; i < G.numPlayers; i++){
		for(j = 0; j < 5; j++){
			drawCard(i, &G);
		}
	}
	res = minionTest(1, 0, player, &G, G.handCount[player]-1);
	allRes = allRes && res;
	printf("Result of test option 1: %d\n\n", res);

	//Test 2: Test option 2 with each player having 5 cards in hand.
	initializeGame(numPlayers, k, seed, &G);
	res = 1;
	G.hand[player][G.handCount[player]] = minion;
	G.handCount[player]++;
	for(i = 1; i < G.numPlayers; i++){
		for(j = 0; j < 5; j++){
			drawCard(i, &G);
		}	
	}
	res = res && minionTest(0, 1, player, &G, G.handCount[player]-1);
	allRes = allRes && res;
	printf("Result of option 2 when all players have 5 cards: %d\n\n", res);

	//TEST 3: Test option 2 with each player having 3 cards in hand. (Should not discard)
	initializeGame(numPlayers, k, seed, &G);
	res = 1;
	G.hand[player][G.handCount[player]] = minion;
	G.handCount[player]++;
	for(i = 1; i < G.numPlayers; i++){
		for(j = 0; j < 3; j++){
			drawCard(i, &G);
		}
	}
	res = res && minionTest(0, 1, player, &G, G.handCount[player]-1);
	allRes = allRes && res;
	printf("Result of option 2 when all players have 3 cards: %d\n\n", res);

	if(allRes){
		printf("Result of all tests: SUCCESS\n");
	}
	else {
		printf("Result of all tests: FAIL\n");
	}

	return 0;
}