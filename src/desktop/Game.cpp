#include "Game.hpp"
#include "Actions.hpp"
#include "LogIt.hpp"


#include <iostream>
#include <random>
using namespace std;

/* constructor */
Game::Game()
{

}

Game& Game::GetInstance()
{
    static Game instance;       // Gets destroyed at the end
    return instance;
}

void Game::UpdatePlayers()
{

}

void Game::CreateAndShuffleDeck()
{
    for (int suit = 0; suit <= 3; suit++)
    {
        for (int value = 1; value <= 13; value++)
        {
            m_cardDeck.push_back(Card(suit, value)); 
        }
    }
    logIt(logINFO) << "Deck size: " << m_cardDeck.size();
    std::random_device rd;
    std::default_random_engine rng(rd());
    shuffle(m_cardDeck.begin(), m_cardDeck.end(), rng);
}

void Game::DealCards()
{
    Actions::DealCards(m_cardDeck);
}

