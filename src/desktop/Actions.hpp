#ifndef LITERATURE_ACTIONS_H
#define LITERATURE_ACTIONS_H

#include "Card.hpp"

#include <string>
using namespace std;

using ::firebase::firestore::DocumentSnapshot; 


class Actions {
private:
    /* data */
	static bool requestReturned;
	static bool gameCreated;
	static bool docExists;
	static bool exitGame;
	static bool hookCallCompleted;
	static DocumentSnapshot snapshot;

public:
	static void waitForResponse();
	static void waitForGameUpdates();
	static void waitForGameExit();
	static void waitForHookComplete();
	static void createPlayer(string displayName, string gameCode, bool newGame);
	static void setRequestReturned(bool returned) { requestReturned = returned; }
	static void setGameCreated(bool created) { gameCreated = created; }
	static void setDocExists(bool exists) { docExists = exists; }
	static bool isDocExists() { return docExists; }
	static void setDocumentSnapshot(const DocumentSnapshot& shot) {
		snapshot = shot;
	}
	static DocumentSnapshot& getDocumentSnapshot() { return snapshot; }
	static void setHookCallCompleted(bool callCompleted) { hookCallCompleted = callCompleted; }
	static void AddPlayerHand(vector<Card> hand, string playerId);

	enum gameStatus
	{
		GS_WAITING, 
		GS_IN_PROGRESS
	};
};

#endif // LITERATURE_ACTIONS_H