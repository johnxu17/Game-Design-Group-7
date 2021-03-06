#ifndef GAME_ENGINE_HPP
#define GAME_ENGINE_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <vector>
#include <map>

#include "engine/GameObject.hpp"
#include "engine/GameScreen.hpp"
#include "engine/EventManager.hpp"
#include "engine/Gamepad.hpp"

// Basically a state manager
class GameEngine
{
public:
    void start();
    void stop(); // pause
    void exit(); // exit the game

    void update(float dt);
    void draw();

    void changeGameScreen(std::string s);
    void addGameScreen(std::string id, std::unique_ptr<GameScreen> s);
    void setDebugMode(bool m){ isDebugMode = m; };

    void setWindowRect(sf::IntRect dim){ winDim = dim; };
    void setWindowRect(int t, int l, int w, int h){ winDim = sf::IntRect(t, l, w, h); };

    void setName(std::string n){this->name = n;};

    /*void pushGameScreen(std::unique_ptr<GameScreen> s);/**/
    /*void popGameScreen(std::unique_ptr<GameScreen> s);/**/

    bool isRunning(){ return running; };

    sf::RenderWindow* getContext(){ return &window; };
private:
    bool running;
    bool isDebugMode = false;
    GamepadController gpcontroller;
    sf::IntRect winDim;//(0, 0, 720, 480);
    sf::RenderWindow window;
    std::string name = "New_Game";
    std::map <std::string, std::unique_ptr<GameScreen> > screens;
    //std::vector<std::unique_ptr<GameScreen>> scenes; // scene stack
    GameScreen* currScene = NULL;
    virtual void init(){}; // aka onStart
    //virtual void onStop(){};
    //virtual void onResume(){};
    virtual void onUpdate(float dt){};
    virtual bool onExit(){ return true; };
    void handleEvents();
    virtual void onEvent(){};
};

#endif
