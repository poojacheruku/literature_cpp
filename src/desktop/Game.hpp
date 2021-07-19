#ifndef LITERATURE_GAME_H
#define LITERATURE_GAME_H

#include "Card.hpp"

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
    vector<Card> m_hand;

    PlayerStruct(string displayName, string playerId, int team) :
        m_displayName(displayName),
        m_playerId(playerId),
        m_team(team)
    {

    }

    void build(FieldValue& playerField) {
        MapFieldValue player;
        player["displayName"] = FieldValue::String(displayName);
        player["playerId"] = FieldValue::String(playerId);
        player["team"] = FieldValue::Integer(team);
        vector<FieldValue> cards;
        for (size_t i = 0; i < hand.size(); ++i) 
        {
            MapFieldValue cardMap;
            cardMap["suit"] = FieldValue::Integer(hand[i].GetCardSuit());
            cardMap["value"] = FieldValue::Integer(hand[i].GetCardValue());
            cards.push_back(FieldValue::Map(cardMap));
        }
        player["hand"] = FieldValue::Array(cards);
        playerField = FieldValue::Map(player);
    }
};

class Game {
  private:
    string m_gameCode;
    string m_changeReason;
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
    void AddPlayer(string displayName, string playerId, int team) {
        PlayerStruct player(displayName, playerId, team);
        m_players.push_back(player);
    }
    void UpdatePlayers();

    void DealCards();

    enum Team {
        TEAM_A = 1,
        TEAM_B = 2
    };
};

#endif  // LITERATURE_GAME_H