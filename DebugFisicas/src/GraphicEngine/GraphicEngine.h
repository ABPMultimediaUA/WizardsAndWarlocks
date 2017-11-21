#ifndef GRAPHICENGINE_H
#define GRAPHICENGINE_H

#include <irrlicht/irrlicht.h>
#include "GBody.h"
#include "vector3d.h"

class GraphicEngine{

public:
    /**
     * Returns Instance of the engine. If it not created, this method initializes it.
    */
    static GraphicEngine* getInstance();
    
    /**
     * Executed in main while loop of the game
    */
    bool run();
    
    /**
     * Drops the device to finish program
    */
    bool drop();

    /**
     * Hides or shows mouse cursor
     * visible: value(true/false)
    */
    void setCursorVisible(bool visible);

    /**
     * Adds a camera scene node with an animator appropriate for FPS.

    */
    void addCameraSceneNodeFPS();

    /**
     * Returns current virtual time in milliseconds
    */
    int getTime();
    
    /**
     * Application must call this method before performing any rendering.
    */
    bool beginScene();

    /**
     * Application must call this method before performing any rendering (default begin).
    */
    bool beginSceneDefault();

    /**
     * Presents the rendered image to the screen.
    */
    bool endScene();

    /**
     * Draws all the scene nodes.
    */
    void drawAll();

    /**
     * Draws all gui elements by traversing the GUI environment starting at the root node.
    */
    void drawAllGUI();

    /**
     * Adds a sphere to scene and returns body
     * position: vector which contains position of the body 
     * rotation: vector which contains rotation of the body 
     * scale: vector which contains scale of the body
     * id: id of node
    */
    GBody* addCube2Scene(
        vector3df position = vector3df(0,0,0), 
        vector3df rotation = vector3df(0,0,0), 
        vector3df scale = vector3df(1,1,1),
        float size = 10.f,
        int id = -1
        );

    /**
     * Adds a sphere to scene and returns body
     * position: vector which contains position of the body 
     * rotation: vector which contains rotation of the body 
     * scale: vector which contains scale of the body
     * radius: radius of sphere
     * id: id of node
    */
    GBody* addSphere2Scene(
        vector3df position = vector3df(0,0,0), 
        vector3df rotation = vector3df(0,0,0), 
        vector3df scale = vector3df(1,1,1),
        float radius = 5.f,
        int id = -1
        );

    /**
     * Sets a texture to the giben body
     * body: pointer to body where to apply texture
     * path: relative path to texture
    */
    void setTextureToBody(GBody* body, std::string path);

    /**
     * Sets a texture flag to the giben body
     * body: pointer to body where to apply texture
     * flag: name of the flag to be set
     * value: value of flag to be set 
    */
    void setTextureFlag(GBody* body, std::string flag, bool value);

private:
    GraphicEngine();

    /** Irrlicht provisional devices*/
    irr::IrrlichtDevice*        privateDevice;
    irr::video::IVideoDriver*   privateDriver;
    irr::scene::ISceneManager*  privateSManager;
    irr::gui::IGUIEnvironment*  privateGUIEnv;

};

#endif