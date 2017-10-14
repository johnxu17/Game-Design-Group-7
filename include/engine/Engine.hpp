#ifndef ENGINE_EXPORT_HPP
#define ENGINE_EXPORT_HPP 
/////////////////////////////////////////////////
// Engine.hpp
// 
// Pretty much just exports all of the Engine's components at once
// so that they can be included in one file. I got the idea from 
// looking at how SFML does it.
//
// If you add a new engine component make sure to put it here as well!
//
// Next checkout include/engine/GameObject.hpp
////////////////////////////////////////////////

// Utilities
#include "engine/Interpolate.hpp"
#include "engine/Joystick.hpp"
#include "engine/Random.hpp"

// Game creation
#include "engine/GameObject.hpp"
#include "engine/Events.hpp"
#include "engine/GameScreen.hpp"
#include "engine/GameEngine.hpp"

#endif