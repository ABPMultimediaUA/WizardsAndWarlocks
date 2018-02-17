#ifndef MAINMENU_H
#define MAINMENU_H

#include <GUIEngine/Menu.h>
#include <NetworkEngine/NetSeeker.h>
#include <Assets.h>
#include <SoundEngine/SoundSystem.h>

class MainMenu : public Menu{
public:
    MainMenu();
    ~MainMenu();
    void Update(bool* open, float deltaTime);
    void Drop();

private:
    SoundEvent* soundEvent;

    static const int N_BUTTONS = 4;
    static const int MAX_STRING_SIZE = 21;
    static const int MAX_NAME_SIZE = 16;

    ImVec2 buttonSize;

    static bool m_options;
    static bool m_exit;
    static bool m_multiplayer;
    bool m_none_selected;
    bool m_some_selected;
    bool m_direct_connection;
    bool m_start_host;

    static char player_name[MAX_NAME_SIZE];
    static char server_name[MAX_STRING_SIZE];
    static char ip_address[MAX_STRING_SIZE];

    //MULTIPLAYER OPTIONS
    NetSeeker*		netSeeker;
    std::vector<ServerData> serverList;
    static int m_selected_server;
    
    const char * buttonLayouts[N_BUTTONS]   = { TEXTUREMAP[TEXTURE_MAINMENU_SINGLEPLAYER].c_str(),
                                                TEXTUREMAP[TEXTURE_MAINMENU_MULTIPLAYER].c_str(),
                                                TEXTUREMAP[TEXTURE_MAINMENU_OPTIONS].c_str(),
                                                TEXTUREMAP[TEXTURE_MAINMENU_EXIT].c_str()};


    irr::video::ITexture* texture[N_BUTTONS];
    IrrIMGUI::IGUITexture * imageid[N_BUTTONS];
    const char * descriptions[N_BUTTONS] ={ "Play as a warlock against our AI",
                                            "Play with your friends in LAN",
                                            "Customize the game options",
                                            "Exit the game"};

    void PrepareClient(bool proprietary = false);
    void PlaySound();

    static void SinglePlayer(bool*);
    static void MultiPlayer(bool*);
    static void GameOptions(bool*);
    static void ExitGame(bool*);

    static void closeMenu(bool*);

    typedef void (*METHOD_PTR)(bool*);
    METHOD_PTR actions[N_BUTTONS] = {&MainMenu::SinglePlayer,&MainMenu::MultiPlayer,&MainMenu::GameOptions,&MainMenu::ExitGame};
};

#endif