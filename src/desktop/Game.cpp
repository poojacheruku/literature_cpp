#include "Game.hpp"
#include "Actions.hpp"
#include "LogIt.hpp"
#include "Hand.hpp"

#include <iostream>
#include <random>
using namespace std;

#include "firebase/firestore.h"
using ::firebase::firestore::FieldValue;

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
    vector<Card> hand;
    for(size_t i = 0; i < m_cardDeck.size(); ++i) 
    {
        int index = i % m_numberOfPlayers;
        m_players[index].addCard(m_cardDeck[i]);
    }

    vector<FieldValue> players;

    for(size_t i = 0; i < m_players.size(); ++i)
    {
        FieldValue playerField;
        m_players[i].build(playerField);
        players.push_back(playerField);
    }
    Actions::UpdatePlayers(m_gameCode, players, "DEAL");
}

void Game::PrintGameInfo()
{
    cout << "Game code: " << m_gameCode << endl;
    cout << "Number of players: " << m_numberOfPlayers << endl;
    cout << "Hands:" << endl;
    cout << endl;

    for (size_t i = 0; i < m_players.size(); ++i) {
        m_players[i].print(); 
    }
}

void Game::Initialize()
{
    CreateAndShuffleDeck(); 
    DealCards(); 
    Hand::GetInstance().Initialize(m_players[0].getHand()); 
}

// vector<string> Game::GetPlayerNames()
// {
//     logIt(logINFO) << "In GetPlayerNames..."; 
//     vector<string> playerNames; 
//     logIt(logINFO) << m_players.size(); 
//     for(int i = 0; i < m_players.size(); i++)
//     {
//         string playerName = m_players[i].getDisplayName(); 
//         logIt(logINFO) << playerName; 
//         playerNames.push_back(playerName);
//     }
//     return playerNames; 

// }

