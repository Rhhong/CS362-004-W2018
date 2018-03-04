#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define testcard "adventurer"

int isTreasure(int card){
	return card == copper || card == silver || card == gold;
}

int adventurerTest(int player, struct gameState *state, int handPos){
	struct gameState pre;
	memcpy(&pre, state, sizeof(struct gameState));

	int res = cardEffect(adventurer, 0, 0, 0, state, handPos, NULL);

	int i;
	int card1 = -1;
	int card2 = -1;
	int numDiscard = 0;
	for(i = pre.deckCount[player]; i>= 0; i--){
		if((pre.deck[player][i] == copper || pre.deck[player][i] == silver || pre.deck[player][i] == gold) && card1 == -1){
			card1 = pre.deck[player][i];
		}
		else if((pre.deck[player][i] == copper || pre.deck[player][i] == silver || pre.deck[player][i] == gold) && card2 == -1){
			card2 = pre.deck[player][i];
			break;
		}
		numDiscard++;
	}

	if(card2 == -1){
		//Not enough treasure cards in deck
		//Can't do thorough test since deck gets shuffled
		pre.discardCount[player] += numDiscard+1;
		pre.handCount[player] += 1;

		int a = isTreasure(state->hand[player][pre.handCount[player]-2]);
		int b = isTreasure(state->hand[player][pre.handCount[player]-1]);
		int c = pre.discardCount[player] <= state->discardCount[player];
		int d = pre.handCount[player] == state->handCount[player];
		int e = res == 0;
		//int f = pre.numActions - 1 == state->numActions;
		int g = state->playedCards[state->playedCardCount-1] == adventurer;

		printf("Testing first card added is a treasure: %d\n", a);
		printf("Testing second card added is a treasure: %d\n", b);
		printf("Testing number of cards discarded: %d\n", c);
		printf("Testing number of cards added to hand: %d\n", d);
		printf("Testing function output: %d\n", e);
		//printf("Testing numActions decremented: %d\n", f);
		printf("Testing card added to playedCards array: %d\n", g);

		return a && b && c && d && e && g;
	}
	else{
		pre.discardCount[player] += numDiscard+1;
		pre.handCount[player] += 1;

		int a = state->hand[player][pre.handCount[player]-2] == card1;
		int b = state->hand[player][pre.handCount[player]-1] == card2;
		int c = pre.discardCount[player] == state->discardCount[player];
		int d = pre.handCount[player] == state->handCount[player];
		int e = res == 0;
		//int f = pre.numActions - 1 == state->numActions;
		int g = state->playedCards[state->playedCardCount-1] == adventurer;

		printf("Testing first card added: %d\n", a);
		printf("Testing second card added: %d\n", b);
		printf("Testing number of cards discarded: %d\n", c);
		printf("Testing number of cards added to hand: %d\n", d);
		printf("Testing function output: %d\n", e);
		//printf("Testing numActions decremented: %d\n", f);
		printf("Testing card added to playedCards array: %d\n", g);
		return a && b && c && d && e && g;
	}
}

int main(){
	int numPlayers = 2;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,sea_hag, tribute, smithy, council_room};
	int seed = 1000;
	struct gameState G;

	initializeGame(numPlayers, k, seed, &G);

	printf("\nTESTING ADVENTURER CARD\n\n");

	//TEST 1: Test adventurer card with full deck.
	int player = 0;
	int res;
	int allRes = 1;
	G.hand[player][G.handCount[player]] = adventurer;
	G.handCount[player]++;
	res = adventurerTest(player, &G, G.handCount[player]-1);
	allRes = res && allRes;
	printf("Result of adventurer card with full deck: %d\n\n", res);

	//TEST 2: Test adventurer card with deck consisting of 2 treasure cards and nothing else.
	G.deck[player][0] = silver;
	G.deck[player][1] = gold;
	G.deckCount[player] = 2;
	G.hand[player][G.handCount[player]] = adventurer;
	G.handCount[player]++;
	res = adventurerTest(player, &G, G.handCount[player]-1);
	allRes = res && allRes;
	printf("Result of adventurer card with only 2 treasure cards: %d\n\n", res);

	//TEST 2: Test adventurer card with deck consisting of 1 treasure card and nothing else.
	G.deck[player][0] = silver;
	G.deckCount[player] = 1;
	G.hand[player][G.handCount[player]] = adventurer;
	G.handCount[player]++;
	res = adventurerTest(player, &G, G.handCount[player]-1);
	allRes = res && allRes;
	printf("Result of adventurer card with only 1 treasure card: %d\n\n", res);

	//TEST 2: Test adventurer card with deck consisting of 0 treasure cards and nothing else.
	G.deckCount[player] = 0;
	G.hand[player][G.handCount[player]] = adventurer;
	G.handCount[player]++;
	res = adventurerTest(player, &G, G.handCount[player]-1);
	allRes = res && allRes;
	printf("Result of adventurer card with only 0 treasure cards: %d\n\n", res);

	if(allRes){
		printf("Result of all tests: SUCCESS\n");
	}
	else {
		printf("Result of all tests: FAIL\n");
	}

	return 0;
}
