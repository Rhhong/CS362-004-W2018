#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define testcard "council_room"

int councilRoomTest(int player, struct gameState *state, int handPos){
	struct gameState pre;
	memcpy(&pre, state, sizeof(struct gameState));

	int res = cardEffect(council_room, 0, 0, 0, state, handPos, NULL);

	discardCard(handPos, player, &pre, 0);
	pre.numBuys++;
	pre.deckCount[player] -= 4;
	pre.handCount[player] += 4;

	int i;
	for(i = 0; i < state->numPlayers; i++){
		if(i != player){
			pre.deckCount[i] -= 1;
			pre.handCount[i] += 1;
		}
	}

	int a = pre.numBuys == state->numBuys;
	int b = 1;
	for(i = 0; i < state->numPlayers; i++){
		b = b && pre.deckCount[i] == state->deckCount[i];
	}
	int c = 1;
	for(i = 0; i < state->numPlayers; i++){
		c = c && pre.handCount[i] == state->handCount[i];
	}
	int d = res == 0;
	int e = 1;
	for(i = 0; i < state->numPlayers; i++){
		e = e && pre.discardCount[i] == state->discardCount[i];
	}

	printf("Testing buy added: %d\n", a);
	printf("Testing correct deck size for all players: %d\n", b);
	printf("Testing correct hand size for all players: %d\n", c);
	printf("Testing function output: %d\n", d);
	printf("Testing discard size for all players: %d\n", e);

	return a && b && c && d && e;
}

int main(){
	int numPlayers = 2;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,sea_hag, tribute, smithy, council_room};
	int seed = 1000;
	struct gameState G;

	initializeGame(numPlayers, k, seed, &G);

	printf("\nTESTING COUNCIL ROOM CARD\n\n");

	//Test 1: Test council_room after initializing game.
	int player = 0;
	int res;
	G.hand[player][G.handCount[player]] = council_room;
	G.handCount[player]++;
	res = councilRoomTest(player, &G, G.handCount[player]-1);
	printf("Result of test after init: %d\n", res);

	if(res){
		printf("Result of all tests: SUCCESS\n");
	}
	else {
		printf("Result of all tests: FAIL\n");
	}

	return 0;
}