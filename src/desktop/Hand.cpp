#include "Hand.hpp"

#include <iostream>
#include <vector>
#include <stdexcept>
using namespace std;

#define RED     "\033[31m"
#define BOLDRED "\033[1m\033[31m"
#define RESET   "\033[0m"
#define SPADE   "\u2660"
#define CLUB    "\u2664"
#define HEART   "\u2665"
#define DIAMOND "\u2666"

/* constructor */
Hand::Hand()
{
}

Hand& Hand::GetInstance()
{
    static Hand instance;       // Gets destroyed at the end
    return instance;
}

vector<Card>& Hand::GetSuitVector(Card& card) {
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

void Hand::Initialize(vector<Card> hand)
{
    m_hand = hand;
    for (vector<Card>::iterator it = m_hand.begin() ; it != m_hand.end(); ++it) {
        GetSuitVector(*it).push_back(*it);
    }
}


void Hand::AddCard(int suit, int value) {
    Card card(suit, value);
    AddCard(card);
}

void Hand::AddCard(Card& card) {
    GetSuitVector(card).push_back(card);
    m_hand.push_back(card);
}

void Hand::PrintTop(vector<Card> suit) {
    int size = suit.size();
    cout << "\u256d";
    for (int i = 0; i < size - 1; ++i) {
        cout << "\u2500\u2500\u2500\u252c";
    }
    cout << "\u2500\u2500\u2500\u256e" << endl;
}

void PrintSuit(Card& card) {
    if(card.GetCardSuit() == CardSuit::HEARTS || card.GetCardSuit() == CardSuit::DIAMONDS) {
        cout << " " << BOLDRED << card.GetSuitIcon() << RESET << " \u2502";
    } else {
        cout << " " << card.GetSuitIcon() << " \u2502";
    }
}

void PrintValue(Card& card) {
    cout << " " << card.GetFaceValue() << " \u2502";
}

void Hand::PrintBottom(vector<Card> suit) {
    int size = suit.size();
    cout << "\u2570";
    for (int i = 0; i < size - 1; ++i) {
        cout << "\u2500\u2500\u2500\u2534";
    }
    cout << "\u2500\u2500\u2500\u256f" << endl;
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

void Hand::PrettyPrintSuit(vector<Card> suit) {
    if(suit.size() == 0) {
        return;
    }
    PrintTop(suit);
    cout << "\u2502";
    std::for_each(std::begin(suit), std::end(suit), PrintSuit);
    cout << "\n\u2502";
    std::for_each(std::begin(suit), std::end(suit), PrintValue);
    cout << "\n";
    PrintBottom(suit);
}
