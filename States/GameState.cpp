#include "GameState.hpp"

void GameState::initKeybinds(){

    std::ifstream ifs("Config/GameStates_keybinds.ini");

    if(ifs.is_open()){
        std::string keyStr = "";
        std::string keyNum = "";

        while(ifs >> keyStr >> keyNum){
            this->keybinds[keyStr] = this->supportedKeys->at(keyNum);

        }
    }
    ifs.close();

}

void GameState::initTextures(){
    if(!this->textures["PLAYER_IDLE"].loadFromFile("Resource_Files/Images/Sprites/Player/player.png")){
        throw "ERROR::GAME_STATE::INIT_TEXTURE::LOADING_PLAYER";
    }
}

void GameState::initPlayer(){
}

GameState::GameState(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::stack<State*>* states) 
    : State(window, supportedKeys, states)
{
    this->initKeybinds();
    this->initTextures();
    this->initPlayer();
}
GameState::~GameState(){

}

void GameState::updateInput(const float& dt){
    //update input

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_LEFT"))))

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_RIGHT"))))

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_UP"))))

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_DOWN"))))
    
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CLOSE"))))
        this->endState();
}

void GameState::update(const float& dt){
    this->updateMousePosition();

    this->updateInput(dt);
    
    
}
void GameState::render(sf::RenderTarget* target ){
    if(!target)
        target = this->window;

}