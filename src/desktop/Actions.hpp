#ifndef LITERATURE_ACTIONS_H
#define LITERATURE_ACTIONS_H

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
	static void createPlayer(string code, string name, bool newGame);
	static void setRequestReturned(bool returned) { requestReturned = returned; }
	static void setGameCreated(bool created) { gameCreated = created; }
	static void setDocExists(bool exists) { docExists = exists; }
	static bool isDocExists() { return docExists; }

	enum gameStatus
	{
		GS_WAITING, 
		GS_IN_PROGRESS
	};
};

#endif // LITERATURE_ACTIONS_H