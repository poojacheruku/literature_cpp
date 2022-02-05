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
    int m_sort_value;
    const string m_suits[4] = { "Spades", "Hearts", "Diamonds", "Clubs" };
    const string m_face_values[13] = { "2", "3", "4", "5", "6", "7", "8", "9", "\u2469", "J", "Q", "K", "A" };
    const string m_suit_icons[4] = { "\u2660", "\u2665", "\u2666", "\u2663" };

    Card();

  public:
    // Constructor
    Card(int suit, int value): m_suit(suit), m_value(value), m_sort_value(value) {
      /*
      if(value == 1) {
        m_sort_value = 14;
      } else {
        m_sort_value = value;
      }
      */
    }

    Card(string suitIcon, string faceValue) {
      int suit, value;
      for(suit = 0; suit <= 3; suit++) {
        if(m_suit_icons[suit] == suitIcon) break;
      }

      for(value = 0; value < 13; value++) {
        if(faceValue == m_face_values[value]) break;
      }
      m_suit = suit;
      m_value = value+2;
      m_sort_value = value+2;

      /*
      if(value == 1) {
        m_sort_value = 14;
      } else {
        m_sort_value = value;
      }
      */
    }

    // Copy constructor
    Card(const Card &card): m_suit(card.m_suit), m_value(card.m_value) {
      /*
      if(card.m_value == 1) {
        m_sort_value = 14;
      } else {
        m_sort_value = card.m_value;
      }
      */
      m_sort_value = card.m_value;
    }

    // Copy constructor using operator
    Card& operator=(const Card& card) {
      m_suit = card.m_suit;
      m_value = card.m_value;
      m_sort_value = card.m_value;
      /*
      if(card.m_value == 1) {
        m_sort_value = 14;
      } else {
        m_sort_value = card.m_value;
      }
      */
      return *this;
    }
    
    // Operator for comparisons (sorting)
    bool operator<(const Card& card) const {
      return (m_sort_value < card.m_sort_value);
    }
    
    string GetFaceValue() const { return m_face_values[m_value - 2]; }
    string GetSuitIcon() const { return m_suit_icons[m_suit]; }
    int GetCardSuit() const { return m_suit; }
    int GetCardValue() const { return m_value; }
    void Print();
    void PrettyPrint();
};

#endif  // LITERATURE_CARD_H