#ifndef LITERATURE_GAME_H
#define LITERATURE_GAME_H

#include "Card.hpp"

#include <vector>
using namespace std;

class Game {
  private:
    string m_gameCode;
    std::vector<Card> m_cardDeck; 

    /* constructor */
    Game();
    Game(Game const&);              // Don't implement
    void operator=(Game const&);    // Don't implement

  public:
    static Game& GetInstance();
    void CreateAndShuffleDeck(); 
    void DealCards(); 
};

#endif  // LITERATURE_GAME_H