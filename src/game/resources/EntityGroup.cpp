#include "components/EntityGroup.hpp"
#include <iostream>
void EntityGroup::init()
{   
    std::cout << characters.size() << std::endl;
    for(auto it = characters.begin(); it != characters.end(); it++){
        (*it)->init();
    } 
}
// Add a character entity
void EntityGroup::addCharacter(std::shared_ptr<Character> c)
{
    characters.push_back(std::move(c));
}
// convert our ordered set into an array
std::vector< std::shared_ptr<Character> > EntityGroup::getCharacters()
{
    return characters;
}

std::shared_ptr<Character> EntityGroup::getCharacter(int pnum)
{
    for(auto it = characters.begin(); it != characters.end(); it++){
        if ((*it)->player_number == pnum){
            return *it;
        }
    }
    return NULL;
}

// Update every entity
void EntityGroup::onUpdate(float dt)
{
    for(auto it = characters.begin(); it != characters.end(); it++){
        (*it)->update(dt);
    }
    // Sort our entities
    std::sort(characters.begin(), characters.end(), [](std::shared_ptr<GameObject> a, std::shared_ptr<GameObject> b){
        return a->z_index < b->z_index;
    });
}

// Draw every entity
void EntityGroup::onDraw(sf::RenderTarget& ctx, sf::RenderStates states) const
{
    for(auto it = characters.begin(); it != characters.end(); it++){
        ctx.draw(**it);
    }
}