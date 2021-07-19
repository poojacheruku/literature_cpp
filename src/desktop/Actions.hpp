#ifndef LITERATURE_ACTIONS_H
#define LITERATURE_ACTIONS_H

#include "Card.hpp"

#include <string>
using namespace std;

class Actions {
private:
    /* data */
	static bool requestReturned;
	static bool gameCreated;
	static bool docExists;
	static bool exitGame;

public:
	static void waitForResponse();
	static void waitForGameUpdates();
	static void waitForGameExit();
	static string CreatePlayer(string displayName);
	static void CreateGame(string gameCode, string displayName, string playerId);
	static void JoinGame(string gameCode, string displayName, string playerId);
	static void setRequestReturned(bool returned) { requestReturned = returned; }
	static void setGameCreated(bool created) { gameCreated = created; }
	static void setDocExists(bool exists) { docExists = exists; }
	static bool isDocExists() { return docExists; }
	static void AddPlayerHand(vector<Card> hand, string playerId);
	static void DealCards(vector<Card>& cardDeck);

	enum gameStatus
	{
		GS_WAITING, 
		GS_IN_PROGRESS
	};
};

#endif // LITERATURE_ACTIONS_H