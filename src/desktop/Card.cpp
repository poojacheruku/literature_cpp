#include "Card.hpp"

#include <iostream>
using namespace std;

#define RED     "\033[31m"
#define BOLDRED "\033[1m\033[31m"
#define RESET   "\033[0m"
#define SPADE   "\u2660"
#define CLUB    "\u2664"
#define HEART   "\u2665"
#define DIAMOND "\u2666"

void Card::Print() {
    cout << m_face_values[m_value-1] << " of " << m_suits[m_suit] << endl;
}

void printTop() {
    cout << "\u256d\u2500\u2500\u2500\u256e" << endl;
}

void printSuit() {
    cout << "\u2502 " << SPADE << " \u2502" << endl;
}

void printValue() {
    cout << "\u2502 \u2491 \u2502" << endl;
}

void printBottom() {
    cout << "\u2570\u2500\u2500\u2500\u256f" << endl;
}

void Card::PrettyPrint() {
    printTop();
    printSuit();
    printValue();
    printBottom();
}