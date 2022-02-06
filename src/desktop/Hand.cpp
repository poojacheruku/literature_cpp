#include "Hand.hpp"
#include "Game.hpp"
#include "Player.hpp"

using ::firebase::firestore::FieldValue;

#include <iostream>
#include <set>
#include <stdexcept>
#include <random>
#include <vector>
#include <boost/tokenizer.hpp>

using namespace std;
using namespace boost;

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

set<Card>& Hand::GetSuitVector(const Card& card)
{
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

// void Hand::SortCards()
// {
//     for (vector<Card>::iterator it = m_hand.begin() ; it != m_hand.end(); ++it) {
//         GetSuitVector(*it).push_back(*it);
//     }
// }

void Hand::TokenizeCardString(vector<string>& tokens, string card)
{
    char_separator<char> sep("-");
    tokenizer<char_separator<char>> toks(card, sep);
    for (const auto& t : toks) {
        tokens.push_back(t);
    }
}

void Hand::AddCard(string text)
{
    vector<string> tokens;
    TokenizeCardString(tokens, text);
    Card card(tokens[0], tokens[1]);
    AddCard(card);
}

void Hand::AddCard(int suit, int value) {
    Card card(suit, value);
    AddCard(card);
}

void Hand::AddCard(Card& card) {
    GetSuitVector(card).insert(card);
    // m_hand.push_back(card);
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

void Hand::PrettyPrint(vector<string> hand) {
    Initialize(hand);
    PrettyPrint();
}

void Hand::PrettyPrint(const DocumentSnapshot& snapshot) {
    Initialize(snapshot);
    PrettyPrint();
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

void Hand::Initialize(vector<string> hand)
{
    m_hand = hand;
    m_spades.clear();
    m_hearts.clear();
    m_clubs.clear();
    m_diamonds.clear();
    for(int i=0; i < hand.size(); i++)
    {
        AddCard(hand[i]); 
    }

}

void Hand::Initialize(const DocumentSnapshot& snapshot)
{
    m_spades.clear();
    m_hearts.clear();
    m_clubs.clear();
    m_diamonds.clear();

    int playerIndex = Player::GetInstance().GetPlayerIndex();
    vector<FieldValue> playerList = snapshot.Get("players").array_value();
    MapFieldValue playerMap = playerList[playerIndex].map_value();;
    vector<FieldValue> playerHand = playerMap["hand"].array_value();
    vector<string> playerHandString;

    for(int i=0; i < playerHand.size(); i++)
    {
        playerHandString.push_back(playerHand[i].string_value()); 
    }
    Initialize(playerHandString);
}

void Hand::Print(const DocumentSnapshot& snapshot)
{
    Initialize(snapshot);
    Print();
}

void Hand::Print(vector<string>& hand)
{
    cout << "Your hand:" << endl; 
    
    for(int i=0; i < hand.size(); i++)
    {
        cout << hand[i] << " "; 
    }
    cout << endl; 
}

void Hand::Print()
{
    Print(m_hand);
}

