#include "Hand.hpp"

#include <iostream>
#include <set>
#include <stdexcept>
using namespace std;

#define RED     "\033[31m"
#define BOLDRED "\033[1m\033[31m"
#define RESET   "\033[0m"

#define TOPLEFT     "\u2554"
#define HORIZONTAL  "\u2550"
#define TOPMIDDLE   "\u2566"
#define TOPRIGHT    "\u2557"
#define VERTICAL    "\u2551"
#define BOTTOMLEFT  "\u255a"
#define BOTTOMMIDDLE "\u2569"
#define BOTTOMRIGHT "\u255d"

/* constructor */
Hand::Hand()
{
}

Hand& Hand::GetInstance()
{
    static Hand instance;       // Gets destroyed at the end
    return instance;
}

set<Card>& Hand::GetSuitVector(const Card& card) {
    switch (card.GetCardSuit())
    {
    case CardSuit::SPADES:
        return m_spades;
        break;
    
    case CardSuit::HEARTS:
        return m_hearts;
        break;
    
    case CardSuit::DIAMONDS:
        return m_diamonds;
        break;
    
    case CardSuit::CLUBS:
        return m_clubs;
        break;
    default:
        cout << "UKNOWN SUIT FOR CARD!!" << endl;
        throw std::invalid_argument( "No suitable suit found in card" );
        break;
    }
}

void Hand::Initialize(set<Card> hand)
{
    m_hand = hand;
    for (set<Card>::iterator it = m_hand.begin() ; it != m_hand.end(); ++it) {
        GetSuitVector(*it).insert(*it);
    }
}


void Hand::AddCard(int suit, int value) {
    Card card(suit, value);
    AddCard(card);
}

void Hand::AddCard(Card& card) {
    GetSuitVector(card).insert(card);
    m_hand.insert(card);
}

void Hand::PrintTop(set<Card> suit) {
    int size = suit.size();
    cout << TOPLEFT;
    for (int i = 0; i < size - 1; ++i) {
        cout << HORIZONTAL << HORIZONTAL << HORIZONTAL << TOPMIDDLE;
    }
    cout << HORIZONTAL << HORIZONTAL << HORIZONTAL << TOPRIGHT << endl;
}

void PrintSuit(const Card& card) {
    if(card.GetCardSuit() == CardSuit::HEARTS || card.GetCardSuit() == CardSuit::DIAMONDS) {
        cout << " " << BOLDRED << card.GetSuitIcon() << RESET << " " << VERTICAL;
    } else {
        cout << " " << card.GetSuitIcon() << " " << VERTICAL;
    }
}

void PrintValue(const Card& card) {
    cout << " " << card.GetFaceValue() << " " << VERTICAL;
}

void Hand::PrintBottom(set<Card> suit) {
    int size = suit.size();
    cout << BOTTOMLEFT;
    for (int i = 0; i < size - 1; ++i) {
        cout << HORIZONTAL << HORIZONTAL << HORIZONTAL << BOTTOMMIDDLE;
    }
    cout << HORIZONTAL << HORIZONTAL << HORIZONTAL << BOTTOMRIGHT << endl;
}

void Hand::PrettyPrint() {
    if(m_hand.size() == 0) {
        cout << "You have no cards in hand!" <<endl;
        return;
    } else {
        cout << "Your hand: " <<endl;
    }

    PrettyPrintSuit(m_spades);
    PrettyPrintSuit(m_hearts);
    PrettyPrintSuit(m_clubs);
    PrettyPrintSuit(m_diamonds);
}

void Hand::PrettyPrintSuit(set<Card> suit) {
    if(suit.size() == 0) {
        return;
    }
    PrintTop(suit);
    cout << VERTICAL;
    std::for_each(std::begin(suit), std::end(suit), PrintSuit);
    cout << "\n" << VERTICAL;
    std::for_each(std::begin(suit), std::end(suit), PrintValue);
    cout << "\n";
    PrintBottom(suit);
}
