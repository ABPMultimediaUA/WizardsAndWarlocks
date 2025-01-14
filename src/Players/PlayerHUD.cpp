#include "PlayerHUD.h"

#include <Assets.h>
#include "Player.h"
#include "./Minimap.h"
#include "./../Spells/Hechizo.h"
#include "./../Objects/Potion.h"
#include "./../Managers/TrapManager.h"
#include "./../Managers/SpellManager.h"
#include <GraphicEngine/GraphicEngine.h>
#include <GraphicEngine/GSprite.h>
#include <GraphicEngine/GText2D.h>
#include <GraphicEngine/GRect.h>

//--------------------------------------------------------------------------------------//
//---------------------------------------PUBLIC-----------------------------------------//
//--------------------------------------------------------------------------------------//

PlayerHUD::PlayerHUD(Player* p){
    m_player = p;
    p_alliance = -1;
    p_potion = nullptr;

    health_orb = nullptr;
    mana_orb = nullptr;
    stamina_bkg = nullptr;
    stamina_bar = nullptr;
    stamina_light = nullptr;
    stamina_grid = nullptr;
    spell_slot = nullptr;
    potion_slot = nullptr;
    trap_slot = nullptr;
    trap_usings_slot = nullptr;
    
    m_orb_height = 0.0f;
    m_stamina_bar_width = 0.0f;

    m_minimap = nullptr;

    g_engine = GraphicEngine::getInstance();
}

PlayerHUD::~PlayerHUD(){
	Erase();

    g_engine = nullptr;
    p_potion = nullptr;

    delete spell_slot;
}

void PlayerHUD::InitHUD(){
    if(health_orb == nullptr && mana_orb == nullptr && stamina_bar == nullptr){
        p_initPlayerOrbs();
        if(health_orb != nullptr) p_initPlayerPotion();
        if(mana_orb != nullptr){ 
            p_initPlayerSpellSelector();
            if(p_alliance != ALLIANCE_WIZARD) p_initPlayerTrap();
        }
        if(spell_slot!= nullptr && health_orb!=nullptr && mana_orb != nullptr) p_initStaminaBar();

	}
    else{
        health_orb->SetHeight(m_orb_height);
	    mana_orb->SetHeight(m_orb_height);
    	stamina_bar->SetWidth(m_stamina_bar_width);
    }

    /************************************MINIMAP*************************************/
    if(m_minimap == nullptr) m_minimap = new HUD_Minimap(m_player);
    else m_minimap->InitMinimap();
}

void PlayerHUD::Draw(){
    p_drawPlayerOrbs();
    p_drawStaminaBar();
    p_drawPlayerSpellSelector();
    p_drawPlayerPotion();
    p_drawMinimap();

    if(p_alliance == -1) p_alliance = m_player->GetAlliance();
    if(p_alliance != m_player->GetAlliance()){
        p_erasePlayerTrap();
        p_alliance = m_player->GetAlliance();
        mana_orb->SetColor(p_alliance);
        if(p_alliance == ALLIANCE_WARLOCK) g_engine->SetAimColor(0.8f,0.0f,0.8f);
        else g_engine->SetAimColor(0.0f,0.0f,1.0f);
    }
    if(p_alliance == ALLIANCE_WARLOCK) p_drawPlayerTrap();
}

void PlayerHUD::Erase(){
    p_erasePlayerOrbs();
    p_eraseStaminaBar();
    p_erasePlayerSpellSelector();
    p_erasePlayerPotion();
    p_erasePlayerTrap();
    if(spell_slot != nullptr){ delete spell_slot; spell_slot = nullptr; }
    if(m_minimap != nullptr){ delete m_minimap; m_minimap = nullptr; }
}

void PlayerHUD::ShowEnemyInMap(Player* p){
    //si es el hud del jugador 1 y p no es nullptr

    if(m_minimap!=nullptr && p_alliance != p->GetAlliance()){                   // si la alianza es diferente es un enemigo
        std::map<Player*,float>::iterator it = m_minimap->p_enemies.find(p);    // buscamos si ya se encuentra dentro del mapa de enemigos
        if(it == m_minimap->p_enemies.end()){                                   // si no se encuentra se añade
            m_minimap->p_enemies.insert(std::pair<Player*,float>(p,5.0f));      // | se inserta el enemigo y los 5s que dura en el minimapa
            
            std::string path = TEXTUREMAP[TEXTURE_ORB_SCROLL_FILL_MASK];        // | se crea el sprite
            
            GSprite* newSprite = m_minimap->CreatePlayerSprite();               // |
            newSprite->SetTexture(path);                                        // |
            newSprite->SetColor(1,0,0);                                         // |

            m_minimap->m_enemies.push_back(newSprite);                          // \ se añade al vector de sprites enemigos
        }
        else{                                                                   // si se encuentra le sumamos tiempo
            it->second += 5.0f;                                                 // \ +5s al tiempo actual
        }
    }
}

//--------------------------------------------------------------------------------------//
//---------------------------------------PRIVATE----------------------------------------//
//--------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------//
//----------------------------------------INITS-----------------------------------------//
//--------------------------------------------------------------------------------------//


void PlayerHUD::p_initPlayerOrbs(){
    //float H = g_engine->GetScreenHeight();		
    float W = g_engine->GetScreenWidth();
    vector2di tex_dims = GraphicEngine::getInstance()->GetTextureDims(TEXTUREMAP[TEXTURE_ORB_BACK]);

    //orb dimensions
    //(original height / original width) x new width = new height
    float new_width = W/5.0f;
    //float new_height = ratio * new_width;
    vector2df orb_dims = vector2df(new_width,new_width);
    if(orb_dims.X > tex_dims.X) orb_dims = vector2df(tex_dims.X,tex_dims.Y);

    //orb position
    vector2df pos = vector2df(0,0);

    //HEALTH
    health_orb = new HUD_Orb();
    health_orb->bkg = g_engine->addSprite(TEXTUREMAP[TEXTURE_ORB_BACK],pos,orb_dims);
    
    health_orb->fill = g_engine->addSprite(TEXTUREMAP[TEXTURE_ORB_FILL],pos,orb_dims);
    health_orb->fill->SetColor(1,0,0);
    
    health_orb->scroll_fill = g_engine->addSprite(TEXTUREMAP[TEXTURE_ORB_SCROLL_FILL],pos,orb_dims);
    health_orb->scroll_fill->SetMask(TEXTUREMAP[TEXTURE_ORB_SCROLL_FILL_MASK]);
    health_orb->scroll_fill->SetColor(0.5,0,0,0.8);

    health_orb->scroll_lip = g_engine->addSprite(TEXTUREMAP[TEXTURE_ORB_SCROLL_LIP],pos,orb_dims);
    health_orb->scroll_lip->SetMask(TEXTUREMAP[TEXTURE_ORB_SCROLL_FILL_MASK]);
    health_orb->scroll_lip->SetColor(0.3,0,0,0.8);

    health_orb->front = g_engine->addSprite(TEXTUREMAP[TEXTURE_ORB_FRONT],pos,orb_dims);

    std::string hp = std::to_string((int) m_player->GetHP());
    
    health_orb->info = g_engine->add2DText(hp);
    health_orb->info->SetTextSize(0.1f);
    float info_posX = (pos.X + orb_dims.X/2) - health_orb->info->GetSize().X/2;
    float info_posY = 0;
    health_orb->info->SetPosition(info_posX, info_posY);
    
    health_orb->xPos = pos.X;
    health_orb->yPos = pos.Y;
    health_orb->width = orb_dims.X;
    health_orb->height = orb_dims.Y;

    //MANA
    pos = vector2df(g_engine->GetScreenWidth()-orb_dims.X,0);
    mana_orb = new HUD_Orb();
    mana_orb->bkg = g_engine->addSprite(TEXTUREMAP[TEXTURE_ORB_BACK],pos,orb_dims);
    
    mana_orb->fill = g_engine->addSprite(TEXTUREMAP[TEXTURE_ORB_FILL],pos,orb_dims);
    
    mana_orb->scroll_fill = g_engine->addSprite(TEXTUREMAP[TEXTURE_ORB_SCROLL_FILL],pos,orb_dims);
    mana_orb->scroll_fill->SetMask(TEXTUREMAP[TEXTURE_ORB_SCROLL_FILL_MASK]);

    mana_orb->scroll_lip = g_engine->addSprite(TEXTUREMAP[TEXTURE_ORB_SCROLL_LIP],pos,orb_dims);
    mana_orb->scroll_lip->SetMask(TEXTUREMAP[TEXTURE_ORB_SCROLL_FILL_MASK]);

    mana_orb->front = g_engine->addSprite(TEXTUREMAP[TEXTURE_ORB_FRONT],pos,orb_dims);

    mana_orb->SetColor(ALLIANCE_WARLOCK);

    std::string mp = std::to_string((int) m_player->GetMP());
    mana_orb->info = g_engine->add2DText(mp);
    mana_orb->info->SetTextSize(0.1f);
    info_posX = W-orb_dims.X/2 - (mana_orb->info->GetSize().X/2);
    info_posY = 0;
    mana_orb->info->SetPosition(info_posX, info_posY);

    mana_orb->xPos = pos.X;
    mana_orb->yPos = pos.Y;
    mana_orb->width = orb_dims.X;
    mana_orb->height = orb_dims.Y;

    //INITIAL ORB FILL HEIGHT
    m_orb_height = health_orb->fill->GetHeight();
}

void PlayerHUD::p_initStaminaBar(){
    int W = g_engine->GetScreenWidth();	
    float size = 10.0f;			            // Height of the bar

    float xInit = health_orb->width;		// Calculate the Init and End of the bar on X axis
    float xEnd =  W - mana_orb->width;		

    float yInit = spell_slot->GetHeight();
    float yEnd = yInit + size;

    stamina_bkg = g_engine->addSprite(TEXTUREMAP[TEXTURE_STAMINA_BKG],vector2df(xInit,yInit),vector2df(xEnd-xInit, yEnd-yInit));
    stamina_bar = g_engine->add2DRect(vector2df(xInit,yInit),vector2df(xEnd-xInit, yEnd-yInit));
    stamina_light = g_engine->addSprite(TEXTUREMAP[TEXTURE_STAMINA_LIGHT],vector2df(xInit,yInit),vector2df(xEnd-xInit, yEnd-yInit));
    stamina_grid = g_engine->addSprite(TEXTUREMAP[TEXTURE_STAMINA_GRID],vector2df(xInit,yInit),vector2df(xEnd-xInit, yEnd-yInit));

    m_stamina_bar_width = stamina_bar->GetWidth();
    m_stamina_xPos = xInit;
    m_stamina_yPos = yInit;
}

void PlayerHUD::p_initPlayerSpellSelector(){
    int W = g_engine->GetScreenWidth();	
    m_spell_size = W * 0.075;
    m_num_spells = SpellManager::GetInstance()->GetNumSpells();
    //m_spell_space = 10;
    m_spell_space = ( W - mana_orb->width - health_orb->width - m_num_spells*m_spell_size)/ m_num_spells;

    float new_height = m_spell_size+10;
    float new_width = m_num_spells* (m_spell_size + m_spell_space);
    vector2df slot_dims = vector2df(new_width,new_height);

    if(slot_dims.Y > m_spell_size + 10) slot_dims.Y = m_spell_size + 10;

    vector2df slot_pos = vector2df(W-mana_orb->width-slot_dims.X, 0);

    spell_slot = g_engine->addSprite(TEXTUREMAP[TEXTURE_SPELL_SLOT_BKG],slot_pos,slot_dims);
}

void PlayerHUD::p_initPlayerPotion(){
    int W = g_engine->GetScreenWidth();	
    //float ratio = g_engine->GetAspectRatio();
    vector2di tex_dims = GraphicEngine::getInstance()->GetTextureDims(TEXTUREMAP[TEXTURE_ITEM_SLOT]);

    float new_width = W/10.0f;
    float new_height = W/10.0f;
    vector2df slot_dims = vector2df(new_width,new_height);
    if(slot_dims.X > tex_dims.X) slot_dims = vector2df(tex_dims.X,tex_dims.Y);

    potion_slot = new ItemSlot();
    potion_slot->bkg = g_engine->addSprite(TEXTUREMAP[TEXTURE_ITEM_SLOT],vector2df(0,health_orb->height),slot_dims);
    
    potion_slot->xPos = 0;
    potion_slot->yPos = health_orb->height;
    potion_slot->width = slot_dims.X;
    potion_slot->height = slot_dims.Y;
}

void PlayerHUD::p_initPlayerTrap(){
    if(mana_orb != nullptr){
        int W = g_engine->GetScreenWidth();	
        
        //float ratio = g_engine->GetAspectRatio();
        vector2di tex_dims = GraphicEngine::getInstance()->GetTextureDims(TEXTUREMAP[TEXTURE_ITEM_SLOT]);

        float new_width = W/10.0f;
        float new_height = W/10.0f;
        vector2df slot_dims = vector2df(new_width,new_height);
        if(slot_dims.X > tex_dims.X) slot_dims = vector2df(tex_dims.X,tex_dims.Y);

        //TRAP SLOT
        if(trap_slot == nullptr){
            trap_slot = new ItemSlot();
            trap_slot->bkg = g_engine->addSprite(TEXTUREMAP[TEXTURE_ITEM_SLOT],vector2df(W-slot_dims.X,mana_orb->height),slot_dims);
            
            trap_slot->xPos = W-slot_dims.X;
            trap_slot->yPos = mana_orb->height;
            trap_slot->width = slot_dims.X;
            trap_slot->height = slot_dims.Y;
        }

        //TRAP USINGS SLOT
        if(trap_usings_slot == nullptr){
            slot_dims = vector2df(slot_dims.X/3,slot_dims.Y/3);
            trap_usings_slot = new ItemSlot();
            trap_usings_slot->bkg = g_engine->addSprite(TEXTUREMAP[TEXTURE_ITEM_SLOT],vector2df(W-slot_dims.X,mana_orb->height),slot_dims);
            
            trap_usings_slot->xPos = W-slot_dims.X;
            trap_usings_slot->yPos = mana_orb->height;
            trap_usings_slot->width = slot_dims.X;
            trap_usings_slot->height = slot_dims.Y;
        }
        
        m_trap_usings = TrapManager::GetInstance()->getPlayerUsings(m_player);
    }
}

//--------------------------------------------------------------------------------------//
//----------------------------------------DRAWS-----------------------------------------//
//--------------------------------------------------------------------------------------//


void PlayerHUD::p_drawMinimap(){
    if(m_minimap!=nullptr){
        float rotation = m_player->GetRotY();
        m_minimap->SetRotation(rotation);

        m_minimap->UpdateScroll();
        m_minimap->DrawPlayers();
    }
}

void PlayerHUD::p_drawPlayerOrbs() const{
    if(health_orb != nullptr && mana_orb != nullptr){
        float HP = m_player->GetHP();
        float MP = m_player->GetMP();

		health_orb->SetHeight(m_orb_height*(HP/100));
		mana_orb->SetHeight(m_orb_height*(MP/100));

		health_orb->Update(0.005, std::to_string((int) m_player->GetHP()));
		mana_orb->Update(0.005, std::to_string((int) m_player->GetMP()));
	}
}

void PlayerHUD::p_drawStaminaBar(){
    if(stamina_bar != nullptr){
        float SP = m_player->GetSP();
        float SP_w = m_stamina_bar_width*(SP/100);
        float colorG = (SP_w/m_stamina_bar_width);
        float colorR = 1-colorG;
        stamina_bar->SetXPos(m_stamina_xPos+(m_stamina_bar_width/2 - SP_w/2));
		stamina_bar->SetWidth(SP_w);
        stamina_bar->SetColor(colorR,colorG - 0.15,0,0.8);
    }
}

void PlayerHUD::p_drawPlayerSpellSelector(){
    if(mana_orb != nullptr){
        float W =  g_engine->GetScreenWidth();
        float yPos = 3;                                                 // Y position
    	float outline = 5;			                                    // Borde de los hechizos
        int current = m_player->GetCurrentSpell();                      //m_currentSpell

        std::vector<Hechizo*> hechizos = SpellManager::GetInstance()->GetSpells(m_player);

    	float xPos = health_orb->width +m_spell_space/2;
    	float xInitSpell = 0.0f;
        float xInitNextSpell = 0.0f;
        bool drawSeparators = false;
        GSprite* sep = nullptr;

        if(m_separators.empty()) drawSeparators = true;
        
        if(m_player != nullptr){
            if(drawSeparators){
                sep = g_engine->addSprite(TEXTUREMAP[TEXTURE_SPELL_SEP_R],vector2df(health_orb->width,0),vector2df( xPos + - health_orb->width, m_spell_size/2.0f));
                m_separators.push_back(sep);
                sep = g_engine->addSprite(TEXTUREMAP[TEXTURE_SPELL_SEP_R],vector2df(health_orb->width,spell_slot->GetHeight()),vector2df( xPos + - health_orb->width,- m_spell_size/2.0f));
                m_separators.push_back(sep);
            }
            for(int i = 0; i<hechizos.size();i++){
                if(hechizos[i]!=nullptr){
                    bool disabled = hechizos[i]->CheckMP(m_player->GetMP());
                    xInitSpell = xPos + (m_spell_size + m_spell_space)*i;	// Calcula la X inicial de cada hechizo
                    xInitNextSpell = xPos + (m_spell_size + m_spell_space)*(i+1);
                    
                    if(drawSeparators){
                        if(i<hechizos.size()-1){
                            sep = g_engine->addSprite(TEXTUREMAP[TEXTURE_SPELL_SEP],vector2df(xInitSpell+m_spell_size,0),vector2df(xInitNextSpell-xInitSpell-m_spell_size, m_spell_size/2.0f));
                            m_separators.push_back(sep);
                            sep = g_engine->addSprite(TEXTUREMAP[TEXTURE_SPELL_SEP],vector2df(xInitSpell+m_spell_size,spell_slot->GetHeight()),vector2df(xInitNextSpell-xInitSpell-m_spell_size,- m_spell_size/2.0f));
                            m_separators.push_back(sep);
                        }
                        else{
                            sep = g_engine->addSprite(TEXTUREMAP[TEXTURE_SPELL_SEP_L],vector2df(xInitSpell+m_spell_size,0),vector2df((W-mana_orb->width)-(xInitSpell+m_spell_size), m_spell_size/2.0f));
                            m_separators.push_back(sep);
                            sep = g_engine->addSprite(TEXTUREMAP[TEXTURE_SPELL_SEP_L],vector2df(xInitSpell+m_spell_size,spell_slot->GetHeight()),vector2df((W-mana_orb->width)-(xInitSpell+m_spell_size),- m_spell_size/2.0f));
                            m_separators.push_back(sep);
                        }
                    }
                    hechizos[i]->DrawHUD(xInitSpell, yPos, m_spell_size, outline, i==current, !disabled, p_alliance);
                }
            }
        }
    }
}

void PlayerHUD::p_drawPlayerPotion(){
    if(p_potion != m_player->GetPotion()){
        if(p_potion != nullptr) potion_slot->RemoveItem();
        p_potion = m_player->GetPotion();

        if(p_potion != nullptr && potion_slot->item == nullptr) potion_slot->AddItem(p_potion->GetHUDTexture());
    }
}

void PlayerHUD::p_drawPlayerTrap(){
    std::string tex = TrapManager::GetInstance()->GetPathFromEnum(TrapManager::GetInstance()->getPlayerTrap(m_player));
    float usings = TrapManager::GetInstance()->getPlayerUsings(m_player);
    //SI NO ESTA INICIALIZADO
    if(trap_slot == nullptr){
        p_initPlayerTrap();
    }
    //SI NO HAY ITEM
    else if(trap_slot->item == nullptr && trap_usings_slot->item == nullptr){
        m_trap_usings = TrapManager::GetInstance()->getPlayerUsings(m_player);
        std::string usingsTex = TrapManager::GetInstance()->GetPathFromUsings(m_trap_usings);

        trap_slot->AddItem(tex);
        trap_usings_slot->AddItem(usingsTex);
    }
    //SI LA TEXTURA HA CAMBIADO
    else if(trap_slot->item->GetTexture() != tex){
        trap_slot->item->SetTexture(tex);
    }
    //SI LOS USOS HAN CAMBIADO
    if(m_trap_usings != usings){
        m_trap_usings = usings;
        std::string usingsTex = TrapManager::GetInstance()->GetPathFromUsings(m_trap_usings);
        trap_usings_slot->item->SetTexture(usingsTex);
    }
}

//--------------------------------------------------------------------------------------//
//---------------------------------------ERASES-----------------------------------------//
//--------------------------------------------------------------------------------------//


void PlayerHUD::p_erasePlayerOrbs(){
    if(health_orb != nullptr){ delete health_orb; health_orb = nullptr;} 
	if(mana_orb != nullptr){ delete mana_orb; mana_orb = nullptr;}
}

void PlayerHUD::p_eraseStaminaBar(){
    if(stamina_bar != nullptr){ delete stamina_bar; stamina_bar = nullptr;}
    if(stamina_bkg != nullptr){delete stamina_bkg; stamina_bkg = nullptr;}
    if(stamina_light != nullptr){delete stamina_light; stamina_light = nullptr;}
    if(stamina_grid != nullptr){delete stamina_grid; stamina_grid = nullptr;}
}

void PlayerHUD::p_erasePlayerSpellSelector(){
    std::vector<Hechizo*> hechizos = SpellManager::GetInstance()->GetSpells(m_player);
    if(m_player != nullptr){
        for(int i = 0; i<hechizos.size();i++){
            if(hechizos[i]!=nullptr) hechizos[i]->EraseHUD();
        }
    }
    for(int i = 0; i<m_separators.size();i++){
        delete m_separators[i];
    }
    m_separators.clear();
}

void PlayerHUD::p_erasePlayerPotion(){
    if(p_potion != nullptr) potion_slot->RemoveItem();
    p_potion = nullptr;

    delete potion_slot;
    potion_slot = nullptr;
}

void PlayerHUD::p_erasePlayerTrap(){
    if(trap_slot != nullptr) delete trap_slot;
    if(trap_usings_slot != nullptr) delete trap_usings_slot;
    
    trap_slot = nullptr;
    trap_usings_slot = nullptr;
}

//--------------------------------------------------------------------------------------//
//------------------------------------HUD ORB STRUCT------------------------------------//
//--------------------------------------------------------------------------------------//

PlayerHUD::HUD_Orb::HUD_Orb(){
    front = nullptr;
    bkg = nullptr;
    fill = nullptr;
    scroll_lip = nullptr;
    scroll_fill = nullptr;
    info = nullptr;
}

PlayerHUD::HUD_Orb::~HUD_Orb(){
	delete front;
	delete bkg;
	delete fill;
	delete scroll_lip;
	delete scroll_fill;
    delete info;

	front = nullptr;
	bkg = nullptr;
	fill = nullptr;
	scroll_lip = nullptr;
	scroll_fill = nullptr;
	scroll_lip = nullptr;
    info = nullptr;
}

void PlayerHUD::HUD_Orb::SetHeight(float v){
	float lip_height = 3;
	fill->SetRect(0,fill->GetTextureHeight()-v,fill->GetWidth(),v);
	scroll_fill->SetRect(0,scroll_fill->GetTextureHeight()-v,scroll_fill->GetWidth(),v);
	scroll_lip->SetRect(0,scroll_lip->GetTextureHeight()-v,scroll_lip->GetWidth(),v - (v - lip_height));
	scroll_lip->SetPosition(fill->GetPosX(),fill->GetHeight() - lip_height);
}

void PlayerHUD::HUD_Orb::SetColor(int alliance){
    if(alliance == ALLIANCE_WARLOCK){
		fill->SetColor(1.0f,0,1.0f);
		scroll_fill->SetColor(0.0f,0.0f,0.5f,0.5f);
		scroll_lip->SetColor(0.3f,0.0f,0.3f,0.3f);
    }
    else{
        fill->SetColor(0.0f,0.0f,1.0f);
        scroll_fill->SetColor(0.0f,0.0f,0.5f,0.5f);
        scroll_lip->SetColor(0.0f,0.0f,0.3f,0.3f);
    }   
}   

void PlayerHUD::HUD_Orb::Update(float vel, std::string inf){
	if(scroll_fill != nullptr) scroll_fill->ScrollV(vel);
	if(scroll_lip != nullptr) scroll_lip->ScrollH(-vel);
    if(info!=nullptr){ 
        info->SetText(inf);
        float info_posX = (bkg->GetPosition().X + bkg->GetWidth()/2) - info->GetSize().X/2;
        float info_posY = 0;
        info->SetPosition(info_posX, info_posY);
    }
}

PlayerHUD::ItemSlot::ItemSlot(){
    bkg = nullptr;
    item = nullptr;
    xPos = 0.0f;
    yPos = 0.0f;
    width = 0.0f;
    height = 0.0f;
}

PlayerHUD::ItemSlot::~ItemSlot(){
    if(bkg != nullptr) delete bkg;
    if(item != nullptr) delete item;

    bkg = nullptr;
    item = nullptr;
}

void PlayerHUD::ItemSlot::AddItem(std::string texPath){
    if(item != nullptr) delete item;
    item = GraphicEngine::getInstance()->addSprite(texPath, vector2df(xPos,yPos), vector2df(width,height));
    item->SetMask(TEXTUREMAP[TEXTURE_ITEM_SLOT_MASK]);
}

void PlayerHUD::ItemSlot::RemoveItem(){
    delete item;
    item = nullptr;
}
