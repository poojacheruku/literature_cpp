#ifndef LITERATURE_HAND_H
#define LITERATURE_HAND_H

#include "Card.hpp"

#include <string>
#include <set>
#include <set>
using namespace std;

class Hand {
  private:
    set<Card> m_hand;
    set<Card> m_spades;
    set<Card> m_hearts;
    set<Card> m_diamonds;
    set<Card> m_clubs;

    /* constructor */
    Hand();
    Hand(Hand const&);              // Don't implement
    void operator=(Hand const&);    // Don't implement

    void PrintTop(set<Card> hand);
    void PrintBottom(set<Card> hand);
    void PrettyPrintSuit(set<Card> suit);
    set<Card>& GetSuitVector(const Card& card);

  public:
    static Hand& GetInstance();
    void Initialize(set<Card> hand);
    bool IsCardInHand(int suit, int value);
    Card& GetCard(int suit, int value);
    void AddCard(int suit, int value);
    void AddCard(Card& card);
    void RemoveCard(int suit, int value);
    void Print();
    void PrettyPrint();
};

#endif  // LITERATURE_HAND_H