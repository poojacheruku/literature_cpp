#include "PlayerSettings.hpp"

#include <iostream>
#include <string>
#include <set>
#include <exception>
#include <iostream>
using namespace std;

#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
namespace pt = boost::property_tree;
namespace fs = boost::filesystem;

/* constructor */
PlayerSettings::PlayerSettings()
{
    const string home = getenv("HOME");
    m_folder_path = home + "/.literature";
    m_file_path = m_folder_path + "/.settings";
}

PlayerSettings& PlayerSettings::GetInstance()
{
    static PlayerSettings instance;       // Gets destroyed at the end
    return instance;
}

void PlayerSettings::SaveSettings()
{
    // Create an empty property tree object.
    pt::ptree tree;

    // Put the simple values into the tree. The integer is automatically
    // converted to a string. Note that the "debug" node is automatically
    // created if it doesn't exist.
    tree.put("player.playerid", m_player_id);

    // Add all the modules. Unlike put, which overwrites existing nodes, add
    // adds a new node at the lowest level, so the "modules" node will have
    // multiple "module" children.
    BOOST_FOREACH(const std::string &name, m_games)
        tree.add("player.games.game", name);

    // Write property tree to XML file
    pt::write_xml(m_file_path, tree);
}

void PlayerSettings::LoadSettings()
{
    // Create empty property tree object
    pt::ptree tree;

    // Parse the XML into the property tree.
    pt::read_xml(m_file_path, tree);

    // Use the throwing version of get to find the debug filename.
    // If the path cannot be resolved, an exception is thrown.
    m_player_id = tree.get<std::string>("player.playerid");

    // Use get_child to find the node containing the modules, and iterate over
    // its children. If the path cannot be resolved, get_child throws.
    // A C++11 for-range loop would also work.
    BOOST_FOREACH(pt::ptree::value_type &v, tree.get_child("player.games")) {
        // The data function is used to access the data stored in a node.
        m_games.insert(v.second.data());
    }

}

// Check and create .literature folder in $HOME if not exists
// Load settings file if it exists
void PlayerSettings::Initialize() {
    fs::path settingsFolder(m_folder_path);
    fs::path settingsFile(m_file_path);

    if (fs::exists(settingsFolder))    // does path p actually exist?
    {
        // .literature is a directory
        if (fs::is_directory(settingsFolder)) { 
            if(fs::exists(settingsFile) && fs::is_regular_file(settingsFile)) {
                LoadSettings();
            }
        }
        else {
            cout << m_folder_path << " exists, but is not a directory, cannot continue!!!" << endl;
        }
    }
    else {
        fs::create_directory(settingsFolder);
    }
}
