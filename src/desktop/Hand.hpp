#ifndef LITERATURE_HAND_H
#define LITERATURE_HAND_H

#include "Card.hpp"
#include "Game.hpp"

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

  public:
    static Hand& GetInstance();
    void SortCards();
    bool IsCardInHand(int suit, int value);
    Card& GetCard(int suit, int value);
    void AddCard(int suit, int value);
    void AddCard(Card& card);
    void RemoveCard(int suit, int value);
    void Print();
    void PrettyPrint();
    void CreateDeck(); 
    void DealCards();
    void Initialize(vector<string> hand); 
};

#endif  // LITERATURE_HAND_H