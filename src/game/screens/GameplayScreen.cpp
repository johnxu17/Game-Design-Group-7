#include "game/screens/GameplayScreen.hpp"
#include "engine/Random.hpp"
#include "engine/ClueReader.hpp"
#include "game/characters/Character.hpp"
#include "game/characters/Villain.hpp"
#include "game/objects/Clue.hpp"
#include "game/objects/Clue.hpp"
#include <iostream>

void GameplayScreen::init()
{
    hunt.setBuffer(*ResourceManager::getSoundBuffer("../resources/music/start.ogg"));

    clock.restart();
    this->views.clear();
    entity_group = EntityGroup();
    switch(num_players){
        case 1:
        group.generateRoomGrid(20);
        break;
        case 2:
        group.generateRoomGrid(40);
        break;
        case 3:
        group.generateRoomGrid(60);
        break;
        case 4:
        group.generateRoomGrid(100);
        break;
    }

    this->createClues();
    num_players = config->num_players;
    // If we let the playerview set its own viewport
    // then we end up running the same code over and over inside PlayerView#init
    this->createViews(num_players);
    entity_group.init();
    // Create the ghost (this could easily be another function)
    // this->createVillain()
    // ghost = std::make_shared<Villain>();
    // ghost->setPlayerNumber(-1);
    // ghost->setRoomGroup(&group);
    // ghost->setEntities(&entity_group);
    // entity_group.addCharacter(std::move(ghost));
    Events::addEventListener("player_died", [=](base_event_type e){
      // We'll need to delete this listener in the destructor or we'll have segfaults/undefined behavior eventually
      // Cast to gamepad event
      if(--num_players == 0){
        std::cout << "All players died" << std::endl;
        auto event = std::make_shared< Event<std::string> >("GameEnd");
        Events::clearAll("gamepad_event");
        Events::queueEvent("change_screen", event);
      };


    });
    // std::cout << group.rooms.size() << std::endl;
}

void GameplayScreen::createClues()
{
    PlantSeeds(-1); // for random in clue reader
    reader.readFile("../resources/items.xml");
    reader.selectItems();
    // std::cout << group.rooms.size() << std::endl;
    for(auto it = group.rooms.begin(); it != group.rooms.end(); it++){
        std::shared_ptr<Room> r = *it;
        // std::cout << r->room_setup << std::endl;
        // std::cout << r->clueCoordinates.size() << std:: endl;
        for(auto j = r->clueCoordinates.begin(); j != r->clueCoordinates.end(); j++){
            clue = std::make_shared<Clue>();
            clue->setRoomGroup(&group);
            clue->setEntities(&entity_group);
            // std::cout << "success" << std::endl;
            hiLow = rand() % 2;
            clue->clueJackpot = reader.getCluesJackpot()[hiLow];
            clue->clueSpec = reader.getCluesSpec()[hiLow];
            clue->clueVague = reader.getCluesVague()[hiLow];
            clue->clueWorthless = reader.getCluesWorthless()[hiLow];
            clue->highLow = hiLow;
            int randint = rand() % 100;
            if (randint <= 50){
                clue->setClue = clue->clueWorthless;
            }
            else if(randint <= 80){
                clue->setClue = clue->clueVague;
            }
            else if(randint <= 95){
                clue->setClue = clue->clueSpec;
            }
            else{
                clue->setClue = clue->clueJackpot;
                clue->activatedItem = false;
            }
            int x = (*it)->rect.getPosition().x + (32 * (*j));
            // std::cout << x;
            // std::cout << " ";
            j++;
            int y = (*it)->rect.getPosition().y + (32 * (*j));
            // std::cout << y;
            // std::cout << " ";
            j++;
            int width = 32 * (*j);
            // std::cout << width;
            // std::cout << " ";
            j++;
            int height = 32 * (*j);
            // std::cout << height << std::endl;
            clue->setCoordinates(x, y, width, height);
            clue->init();
            entity_group.addClue(std::move(clue));
        }
    }
    for(int i = 0; i < reader.getCluesSpec().size(); i++){

        // add clues to our clue entity group

    }
}

void GameplayScreen::createViews(int numPlayers)
{
    double ratio_w = 1.0;
    double ratio_h = 1.0;
    double gutter  = 5.0; // space between player views in pixels
    double gutterx = gutter / 720.0 / 2;
    double guttery = gutter / 480.0 / 2;

    if(numPlayers >= 3){
        ratio_w /= 2;
        ratio_h /= 2;
    }
    else if(numPlayers == 2){
        ratio_w /= 2;
    }
    // We don't need to store these in our class definition
    // we won't use these variables after we're done here
    // so they shouldn't be members
    std::unique_ptr<PlayerView> view;



    // Maybe turn this object into an "EntitiesGroup" obj
    std::shared_ptr<Character> character;

    for(int i=0; i < numPlayers; i++)
    {
        // Map i to a 2d array [2][2]
        int x = i % 2;
        int y = i / 2;
        int playernum = i + 1; // Player Numbers start at 1
        // Find the gamepad by the player number
        int gamepad_index = -1;
        for(auto it = config->player_map.begin(); it != config->player_map.end(); it++){
            if( it->second == playernum ){
                gamepad_index = it->first;
                break;
            }
        }

        view = std::unique_ptr<PlayerView>(new PlayerView());
        view->setRoomGroup(&group);
        // Quick hack
        view->numPlayers = numPlayers;
        // Define player view (using math)
        view->setView(
            sf::FloatRect(0, 0, 720 * ratio_w, 480 * ratio_h),
            sf::FloatRect((ratio_w + gutterx) * x, (ratio_h  + guttery) * y, ratio_w - gutterx * (1.0 - x), ratio_h - guttery * (1.0 - y))
        );
        // Create a new character
        character = std::make_shared<Character>();
        character->setRoomGroup(&group);
        character->setPlayerNumber(playernum);
        character->setEntities(&entity_group);
        // Hope that it isn't possible for this to throw an error!
        character->setCharacter(config->char_map[playernum]);
        character->setGamepadIndex(gamepad_index);
        // Add player to our entities map
        entity_group.addCharacter(std::move(character));
        view->setEntities(&entity_group);
        view->setEntityNumber(playernum);
        view->init();
        // Add child to this view
        this->views.push_back(std::move(view));
    }

}

void GameplayScreen::onUpdate(float dt)
{
    for(auto it = views.begin(); it != views.end(); it++)
        (*it)->update(dt);
    // Update the rooms (not really necessary though)
    group.update(dt);
    entity_group.update(dt);

    // std::cout << clock.getElapsedTime().asSeconds() << std::endl;
    if (clock.getElapsedTime().asSeconds() >= config -> time_Per_Phase) {
        if(phase == 1){
            hunt.play();

            std::cout << "phase ends" << std::endl;
            ghost = std::make_shared<Villain>();
            ghost->setPlayerNumber(-1);
            ghost->setRoomGroup(&group);
            ghost->setEntities(&entity_group);
            ghost->init();
            entity_group.addCharacter(std::move(ghost));
            phase++;
        }
        clock.restart();
    }

}

void GameplayScreen::onDraw(sf::RenderTarget& ctx, sf::RenderStates states) const
{
    for(auto it = views.begin(); it != views.end(); it++)
        ctx.draw(**it);
}
