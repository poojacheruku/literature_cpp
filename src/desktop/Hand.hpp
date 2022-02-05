#ifndef LITERATURE_HAND_H
#define LITERATURE_HAND_H

#include "Card.hpp"
#include "Game.hpp"

using ::firebase::firestore::DocumentSnapshot;

#include <string>
#include <set>
#include <vector>
using namespace std;

class Hand {
  private:
    vector<Card> m_spades;
    vector<Card> m_hearts;
    vector<Card> m_diamonds;
    vector<Card> m_clubs;
    vector<string> m_hand; 

    /* constructor */
    Hand();
    Hand(Hand const&);              // Don't implement
    void operator=(Hand const&);    // Don't implement

    void PrintTop(vector<Card> hand);
    void PrintBottom(vector<Card> hand);
    void PrettyPrintSuit(vector<Card> suit);
    vector<Card>& GetSuitVector(const Card& card);
    int GetSuit(string suitIcon);
    void AddCard(string card);
    void AddCard(int suit, int value);
    void AddCard(Card& card);
    void SortCards();

  public:
    static Hand& GetInstance();
    void Initialize(vector<string> hand);
    void Initialize(const DocumentSnapshot& snapshot);

    bool IsCardInHand(int suit, int value);
    Card& GetCard(int suit, int value);
    void Print(const DocumentSnapshot& snapshot);
    void Print(vector<string>& hand);
    void Print();
    void PrettyPrint();
    void PrettyPrint(vector<string> hand);
    void PrettyPrint(const DocumentSnapshot& snapshot);
};

#endif  // LITERATURE_HAND_H