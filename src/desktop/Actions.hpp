#ifndef LITERATURE_ACTIONS_H
#define LITERATURE_ACTIONS_H

#include "Card.hpp"

#include <string>
using namespace std;

#include "firebase/firestore.h"
using ::firebase::firestore::FieldValue;

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
	static void setExitGame(bool exitGame);
	static void waitForGameExit();
	static string CreatePlayer(string displayName);
	static void CreateGame(string gameCode, string displayName, string playerId);
	static int JoinGame(string gameCode, string displayName, string playerId);
	static void setRequestReturned(bool returned) { requestReturned = returned; }
	static void setGameCreated(bool created) { gameCreated = created; }
	static void setDocExists(bool exists) { docExists = exists; }
	static bool isDocExists() { return docExists; }
	static void AddPlayerHand(vector<Card> hand, string playerId);
	static void DealCards(vector<Card>& cardDeck);
	static void UpdatePlayers(string gameCode, vector<FieldValue>& players, string reason);

	enum gameStatus
	{
		GAME_STATUS_WAITING, 
		GAME_STATUS_STARTED,
		GAME_STATUS_ENDED
	};

	enum action
	{
		ACTION_NONE,
		ACTION_REQUEST,
		ACTION_DECLARE
	};

	enum actionStatus
	{
		ACTION_STATUS_WAITING,
		ACTION_STATUS_ACCEPTED,
		ACTION_STATUS_REJECTED
	};
};

#endif // LITERATURE_ACTIONS_H