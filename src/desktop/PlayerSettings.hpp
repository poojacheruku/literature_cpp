#ifndef LITERATURE_PLAYER_SETTINGS_H
#define LITERATURE_PLAYER_SETTINGS_H

#include <set>
#include <string>
using namespace std;

class PlayerSettings {
private:
    /* data */
    set<string> m_games;	// all games I am currently playing
	string m_player_id;		// my player id
	string m_display_name;	// display name
	string m_folder_path;	// settings folder path
	string m_file_path;		// settings file path
	bool m_exists;			// player already exists

    /* constructor */
    PlayerSettings();
    PlayerSettings(PlayerSettings const&);	// Don't implement
    void operator=(PlayerSettings const&);	// Don't implement

	void Initialize();
	void LoadSettings();

public:
    static PlayerSettings& GetInstance();
	void AddNewGame(string gameCode) {
		m_games.insert(gameCode);
		SaveSettings();
	}
	set<string> GetCurrentGames() { return m_games; }
	string GetPlayerId() { return m_player_id; }
	string GetDisplayName() { return m_display_name; }
	bool isPlayerExists() { return m_exists; }
	void SetPlayer(string playerId, string displayName) {
		m_player_id = playerId;
		m_display_name = displayName;
		SaveSettings();
	}

    void SaveSettings();
};

#endif // LITERATURE_PLAYER_SETTINGS_H