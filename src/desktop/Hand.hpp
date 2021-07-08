#ifndef LITERATURE_HAND_H
#define LITERATURE_HAND_H

#include "Card.hpp"

#include <string>
#include <vector>
#include <set>
using namespace std;

class Hand {
  private:
    vector<Card> m_hand;
    vector<Card> m_spades;
    vector<Card> m_hearts;
    vector<Card> m_diamonds;
    vector<Card> m_clubs;

    /* constructor */
    Hand();
    Hand(Hand const&);              // Don't implement
    void operator=(Hand const&);    // Don't implement

    void PrintTop(vector<Card> hand);
    void PrintBottom(vector<Card> hand);
    void PrettyPrintSuit(vector<Card> suit);
    vector<Card>& GetSuitVector(Card& card);

  public:
    static Hand& GetInstance();
    void Initialize(vector<Card> hand);
    bool IsCardInHand(int suit, int value);
    Card& GetCard(int suit, int value);
    void AddCard(int suit, int value);
    void AddCard(Card& card);
    void RemoveCard(int suit, int value);
    void Print();
    void PrettyPrint();
};

#endif  // LITERATURE_HAND_H