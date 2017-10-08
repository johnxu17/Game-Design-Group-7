#include "HouseHaunters.hpp"

////////////////////////
// HouseHaunters.cpp
//
// This is where we perform our game setup. You can do things like add game screens
// create 
//
// Next check out the file include/game/screens/GameplayScreen.hpp
///////////////////////

void HouseHauntersGame::init()
{
    // Setup the window position and dimensions
    this->setWindowRect(100, 100, 720, 480);
    // Initialize the game screen
    screen_gameplay = std::unique_ptr<GameScreen>(new GameplayScreen());
    // Add the game screen
    // since it's a unique pointer we have to move it first
    this->changeGameScreen(std::move(screen_gameplay));
}