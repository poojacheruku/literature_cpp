#ifndef LITERATURE_CARD_H
#define LITERATURE_CARD_H

#include <string>
using namespace std;

enum CardSuit {
    SPADES,
    HEARTS,
    DIAMONDS,
    CLUBS
};

class Card {
  private:
    int m_suit;
    int m_value;
    const string m_suits[4] = { "Spades", "Hearts", "Diamonds", "Clubs" };
    const string m_face_values[13] = { "A", "2", "3", "4", "5", "6", "7", "8", "9", "\u2491", "J", "Q", "K" };
    const string m_suit_icons[4] = { "\u2660", "\u2665", "\u2666", "\u2663" };

    Card();

  public:
    // Constructor
    Card(int suit, int value): m_suit(suit), m_value(value) {}

    // Copy constructor
    Card(const Card &card) { m_suit = card.m_suit; m_value = card.m_value; }

    // Copy constructor using operator
    Card& operator=(const Card &card) { m_suit = card.m_suit; m_value = card.m_value; return *this; }

    string GetFaceValue() { return m_face_values[m_value - 1]; }
    string GetSuitIcon() { return m_suit_icons[m_suit]; }
    int GetCardSuit() { return m_suit; }
    void Print();
    void PrettyPrint();
};

#endif  // LITERATURE_CARD_H