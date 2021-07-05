#ifndef LITERATURE_HOOKS_H
#define LITERATURE_HOOKS_H

#include "firebase/firestore.h"
#include <string>

using namespace std;
using firebase::firestore::DocumentReference;

class Hooks {
private:
    /* data */
	static bool gameUpdated;
	static bool playerUpdated;

public:
	static void listenToGameChanges(DocumentReference doc_ref);
	static void listenToPlayerChanges(DocumentReference doc_ref);
    static void setGameUpdated(bool updated) { gameUpdated = updated; }
    static bool isGameUpdated() { return gameUpdated; }

};

#endif // LITERATURE_HOOKS_H