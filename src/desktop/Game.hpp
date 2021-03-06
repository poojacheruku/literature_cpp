#ifndef LITERATURE_GAME_H
#define LITERATURE_GAME_H

#include "Card.hpp"

#include <iostream>
#include <string>
#include <vector>
using namespace std;

#include "firebase/firestore.h"
using ::firebase::firestore::FieldValue;
using ::firebase::firestore::MapFieldValue;

struct CardStruct {
    int suit;
    int value;
};

struct PlayerStruct {
    string m_displayName;
    string m_playerId;
    int m_team;
    vector<string> m_hand;

    PlayerStruct(string displayName, string playerId, int team) :
        m_displayName(displayName),
        m_playerId(playerId),
        m_team(team)
    {}

    void addCard(Card& card) {
        string cardFace = card.GetSuitIcon() + "-" + card.GetFaceValue();
        m_hand.push_back(cardFace);
    }
    
    string getDisplayName() { return m_displayName; }
    vector<string> getHand() { return m_hand; }
    
    void print() {
        cout << "Player name: " << m_displayName << endl;
        cout << "Player id: " << m_playerId << endl;
        cout << "Team: " << m_team << endl;
        for (size_t i = 0; i < m_hand.size(); ++i) {
            cout << m_hand[i] << " ";
        }
        cout << endl;
    }

    void build(FieldValue& playerField) {
        MapFieldValue player;
        player["displayName"] = FieldValue::String(m_displayName);
        player["playerId"] = FieldValue::String(m_playerId);
        player["team"] = FieldValue::Integer(m_team);
        vector<FieldValue> cards;
        for (size_t i = 0; i < m_hand.size(); ++i) 
        {
            cards.push_back(FieldValue::String(m_hand[i]));
        }
        player["hand"] = FieldValue::Array(cards);
        playerField = FieldValue::Map(player);
    }
};

class Game {
  private:
    string m_gameCode;
    vector<PlayerStruct> m_players;
    int m_numberOfPlayers;
    vector<Card> m_cardDeck; 

    /* constructor */
    Game();
    Game(Game const&);              // Don't implement
    void operator=(Game const&);    // Don't implement

  public:
    static Game& GetInstance();
    void CreateAndShuffleDeck();
    void SetGameCode(string gameCode) { m_gameCode = gameCode; }
    string GetGameCode() { return m_gameCode; }
    vector<string> GetPlayerNames(); 
    void SetNumberOfPlayers(int numberOfPlayers) { m_numberOfPlayers = numberOfPlayers; }
    void AddPlayer(string displayName, string playerId, int team) {
        PlayerStruct player(displayName, playerId, team);
        m_players.push_back(player);
    }
    void UpdatePlayers();
    void DealCards();
    void PrintGameInfo();
    void Initialize(); 

    enum Team {
        TEAM_A = 1,
        TEAM_B = 2
    };
};

#endif  // LITERATURE_GAME_H