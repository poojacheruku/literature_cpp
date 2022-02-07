#ifndef LITERATURE_HAND_H
#define LITERATURE_HAND_H

#include "Card.hpp"
#include "Game.hpp"

using ::firebase::firestore::DocumentSnapshot;

#include <string>
#include <set>
#include <vector>
using namespace std;

class Hand
{
private:
    set<Card> m_spades;
    set<Card> m_hearts;
    set<Card> m_diamonds;
    set<Card> m_clubs;
    vector<string> m_hand;

    /* constructor */
    Hand();
    Hand(Hand const &);           // Don't implement
    void operator=(Hand const &); // Don't implement

    void PrintTop(set<Card> hand);
    void PrintBottom(set<Card> hand);
    void PrettyPrintSuit(set<Card> suit);
    set<Card> &GetSuitVector(const Card &card);
    int GetSuit(string suitIcon);
    void AddCard(string card);
    void AddCard(int suit, int value);
    void AddCard(Card &card);
    void SortCards();

public:
    static Hand &GetInstance();
    void Initialize(vector<string> hand);
    void Initialize(const DocumentSnapshot &snapshot);

    bool IsCardInHand(int suit, int value);
    Card &GetCard(int suit, int value);
    void Print(const DocumentSnapshot &snapshot);
    void Print(vector<string> &hand);
    void Print();
    void PrettyPrint();
    void PrettyPrint(vector<string> hand);
    void PrettyPrint(const DocumentSnapshot &snapshot);
    void TokenizeCardString(vector<string> &tokens, string card);
    void RemoveSuit(vector<string>& handString, vector<FieldValue>& newHand, string setCalled);
};

#endif // LITERATURE_HAND_H