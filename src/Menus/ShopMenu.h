#ifndef SHOPMENU_H
#define SHOPMENU_H

#include <GUIEngine/Menu.h>
#include <ItemsData.h>
#include <Assets.h>
#include <map>
#include "TrapCodes.h"
#include "SpellCodes.h"

struct DDFlags{
    bool parentNullID = true;

};

class ShopMenu : public Menu{
public:
    ShopMenu();
    ~ShopMenu();
    void Update(bool* open);

private:
    void load_imagesid(int number,const char * layouts[],irr::video::ITexture* texture[], IrrIMGUI::IGUITexture* imageid[], std::vector<SPELLCODE>, std::map<IrrIMGUI::IGUITexture*,SPELLCODE>*);
    void load_imagesid(int number,const char * layouts[],irr::video::ITexture* texture[], IrrIMGUI::IGUITexture* imageid[], std::vector<TrapEnum>, std::map<IrrIMGUI::IGUITexture*,TrapEnum>*);
    void load_sockets(const char* id,const char* type, int total, int cols, IrrIMGUI::IGUITexture* imageids[],std::vector<IrrIMGUI::IGUITexture*> &items_selected);
    void load_items(const char* id,const char* type, int total, int cols, IrrIMGUI::IGUITexture* imageids[], const char * names[], const char * descriptions[]);
   
    static const int N_OSPELLS = 4;
    static const int N_DSPELLS = 2;
    static const int N_TSPELLS = 7;
    static const int N_TRAPS = 6;
    static const int N_SOCKETS = 3;

    ImVec2 buttonSize;
    ImGuiDragDropFlags imgui_ddflags;
    DDFlags ddflags;

    irr::video::ITexture* o_spelltexture[N_OSPELLS];
    irr::video::ITexture* d_spelltexture[N_DSPELLS];
    irr::video::ITexture* t_spelltexture[N_TSPELLS];
    irr::video::ITexture* trap_texture[N_TRAPS];
    irr::video::ITexture* empty_texture;

    IrrIMGUI::IGUITexture * o_spellimageid[N_OSPELLS];
    IrrIMGUI::IGUITexture * d_spellimageid[N_DSPELLS];
    IrrIMGUI::IGUITexture * t_spellimageid[N_TSPELLS];
    IrrIMGUI::IGUITexture * trap_imageid[N_TRAPS];
    IrrIMGUI::IGUITexture * empty_spell_imageid[N_SOCKETS];
    IrrIMGUI::IGUITexture * empty_trap_imageid[1];
    IrrIMGUI::IGUITexture * const_empty_image;

    std::vector<IrrIMGUI::IGUITexture*> selected_spells;
    std::vector<IrrIMGUI::IGUITexture*> selected_trap;

    IrrIMGUI::IGUITexture* selected;

    const char* emptyLayout = TEXTUREMAP[TEXTURE_SHOPMENU_NONE].c_str();

    std::vector<SPELLCODE> o_spells_codes = {SPELL_BLIZZARD, SPELL_FIRE, SPELL_POISON, SPELL_THUNDER};
    std::vector<SPELLCODE> d_spells_codes = {SPELL_DEFENSE, SPELL_UNTARGET};
    std::vector<SPELLCODE> t_spells_codes = {SPELL_CLEANSE, SPELL_DUMMY, SPELL_INVISIBILITY, SPELL_SPEED,SPELL_TELEPORT, SPELL_TELEPORTBASE, SPELL_WALL};
    std::vector<TrapEnum> traps_codes = {TENUM_DEATH_CLAWS, TENUM_DISTURBANCE, TENUM_EXPLOSIVE, TENUM_SILENCE, TENUM_SPIRITS, TENUM_TAXES};

    std::map<IrrIMGUI::IGUITexture*,SPELLCODE> spells_map;
    std::map<IrrIMGUI::IGUITexture*,TrapEnum> traps_map;

//TEXTURES
    const char * o_spellLayouts[N_OSPELLS] = {  TEXTUREMAP[TEXTURE_SPELL_BLIZZARD_HUD].c_str(),
                                                TEXTUREMAP[TEXTURE_SPELL_FIRE_HUD].c_str(),
                                                TEXTUREMAP[TEXTURE_SPELL_POISON_HUD].c_str(),
                                                TEXTUREMAP[TEXTURE_SPELL_THUNDER_HUD].c_str()};

    const char * d_spellLayouts[N_DSPELLS] = {  TEXTUREMAP[TEXTURE_SPELL_DEFENSE_HUD].c_str(),
                                                TEXTUREMAP[TEXTURE_SPELL_UNTARGET_HUD].c_str()};

    const char * t_spellLayouts[N_TSPELLS] = {  TEXTUREMAP[TEXTURE_SPELL_CLEANSE_HUD].c_str(),
                                                TEXTUREMAP[TEXTURE_SPELL_DUMMY_HUD].c_str(),
                                                TEXTUREMAP[TEXTURE_SPELL_INVISIBILITY_HUD].c_str(),
                                                TEXTUREMAP[TEXTURE_SPELL_SPEED_HUD].c_str(),
                                                TEXTUREMAP[TEXTURE_SPELL_TELEPORT_HUD].c_str(),
                                                TEXTUREMAP[TEXTURE_SPELL_TELEPORTBASE_HUD].c_str(),
                                                TEXTUREMAP[TEXTURE_SPELL_WALL_HUD].c_str()};
    
    const char * trapLayouts[N_TRAPS]     = {   TEXTUREMAP[TEXTURE_TENUM_DEATH_CLAWS].c_str(),
                                                TEXTUREMAP[TEXTURE_TENUM_DISTURBANCE].c_str(),
                                                TEXTUREMAP[TEXTURE_TENUM_EXPLOSIVE].c_str(),
                                                TEXTUREMAP[TEXTURE_TENUM_SILENCE].c_str(),
                                                TEXTUREMAP[TEXTURE_TENUM_SPIRITS].c_str(),
                                                TEXTUREMAP[TEXTURE_TENUM_TAXES].c_str()};

//NAMES 
    const char * o_spellKeys[N_OSPELLS] = { SPELLNAMES[SPELL_BLIZZARD], 
                                            SPELLNAMES[SPELL_FIRE], 
                                            SPELLNAMES[SPELL_POISON],
                                            SPELLNAMES[SPELL_THUNDER]};

    const char * d_spellKeys[N_DSPELLS] = { SPELLNAMES[SPELL_DEFENSE],
                                            SPELLNAMES[SPELL_UNTARGET]};

    const char * t_spellKeys[N_TSPELLS] = { SPELLNAMES[SPELL_CLEANSE],
                                            SPELLNAMES[SPELL_DUMMY],
                                            SPELLNAMES[SPELL_INVISIBILITY],
                                            SPELLNAMES[SPELL_SPEED],
                                            SPELLNAMES[SPELL_TELEPORT],
                                            SPELLNAMES[SPELL_TELEPORTBASE],
                                            SPELLNAMES[SPELL_WALL]};

    const char * trapKeys[N_TRAPS]      = { TRAPNAMES[TENUM_DEATH_CLAWS],
                                            TRAPNAMES[TENUM_DISTURBANCE],
                                            TRAPNAMES[TENUM_EXPLOSIVE],
                                            TRAPNAMES[TENUM_SILENCE],
                                            TRAPNAMES[TENUM_SPIRITS],
                                            TRAPNAMES[TENUM_TAXES]};

//DESCRIPTIONS
    const char * o_spell_descriptions[N_OSPELLS] = { SPELLDESC[SPELL_BLIZZARD], 
                                                     SPELLDESC[SPELL_FIRE], 
                                                     SPELLDESC[SPELL_POISON],
                                                     SPELLDESC[SPELL_THUNDER]};

    const char * d_spell_descriptions[N_DSPELLS] = { SPELLDESC[SPELL_DEFENSE],
                                                     SPELLDESC[SPELL_UNTARGET]};

    const char * t_spell_descriptions[N_TSPELLS] = { SPELLDESC[SPELL_CLEANSE],
                                                     SPELLDESC[SPELL_DUMMY],
                                                     SPELLDESC[SPELL_INVISIBILITY],
                                                     SPELLDESC[SPELL_SPEED],
                                                     SPELLDESC[SPELL_TELEPORT],
                                                     SPELLDESC[SPELL_TELEPORTBASE],
                                                     SPELLDESC[SPELL_WALL]};

    const char * trap_descriptions[N_TRAPS]      = { TRAPDESC[TENUM_DEATH_CLAWS],
                                                     TRAPDESC[TENUM_DISTURBANCE],
                                                     TRAPDESC[TENUM_EXPLOSIVE],
                                                     TRAPDESC[TENUM_SILENCE],
                                                     TRAPDESC[TENUM_SPIRITS],
                                                     TRAPDESC[TENUM_TAXES]};

};

#endif