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

void GameState::initInterface(){
    this->hudPanel.setPosition(sf::Vector2f(0.f, 0.f));
    this->hudPanel.setSize(sf::Vector2f(1920.f, 180.f));
    this->hudPanel.setFillColor(sf::Color(8, 16, 30, 235));
    this->hudPanel.setOutlineColor(sf::Color(80, 120, 150));
    this->hudPanel.setOutlineThickness(2.f);

    this->arenaPanel.setPosition(sf::Vector2f(40.f, 210.f));
    this->arenaPanel.setSize(sf::Vector2f(1840.f, 650.f));
    this->arenaPanel.setFillColor(sf::Color(20, 34, 48, 180));
    this->arenaPanel.setOutlineColor(sf::Color(80, 120, 150));
    this->arenaPanel.setOutlineThickness(2.f);

    this->playerHitbox.setPosition(sf::Vector2f(250.f, 500.f));
    this->playerHitbox.setSize(sf::Vector2f(90.f, 120.f));
    this->playerHitbox.setFillColor(sf::Color(60, 180, 100, 90));
    this->playerHitbox.setOutlineColor(sf::Color(100, 255, 150));
    this->playerHitbox.setOutlineThickness(2.f);

    this->monsterHitbox.setPosition(sf::Vector2f(1450.f, 470.f));
    this->monsterHitbox.setSize(sf::Vector2f(150.f, 180.f));
    this->monsterHitbox.setFillColor(sf::Color(190, 60, 70, 100));
    this->monsterHitbox.setOutlineColor(sf::Color(255, 110, 120));
    this->monsterHitbox.setOutlineThickness(3.f);

    this->arenaText.setString("COMBAT ARENA");
    this->arenaText.setCharacterSize(28);
    this->arenaText.setFillColor(sf::Color(220, 235, 245));
    this->arenaText.setPosition(sf::Vector2f(70.f, 230.f));

    this->monsterText.setString("MONSTER SPAWN");
    this->monsterText.setCharacterSize(20);
    this->monsterText.setFillColor(sf::Color(255, 170, 170));
    this->monsterText.setPosition(sf::Vector2f(1430.f, 430.f));

    this->hitboxText.setString("HITBOX DEBUG");
    this->hitboxText.setCharacterSize(18);
    this->hitboxText.setFillColor(sf::Color(255, 220, 120));
    this->hitboxText.setPosition(sf::Vector2f(1370.f, 680.f));

    this->buttons["ABILITY_1"] = new Button(
        500.f, 900.f, 170.f, 70.f, &this->font, "Ability 1",
        sf::Color(35, 90, 150), sf::Color(60, 130, 205), sf::Color(90, 170, 240));
    this->buttons["ABILITY_2"] = new Button(
        690.f, 900.f, 170.f, 70.f, &this->font, "Ability 2",
        sf::Color(35, 90, 150), sf::Color(60, 130, 205), sf::Color(90, 170, 240));
    this->buttons["ABILITY_3"] = new Button(
        880.f, 900.f, 170.f, 70.f, &this->font, "Ability 3",
        sf::Color(35, 90, 150), sf::Color(60, 130, 205), sf::Color(90, 170, 240));
    this->buttons["FLEE"] = new Button(
        1070.f, 900.f, 170.f, 70.f, &this->font, "Flee",
        sf::Color(130, 75, 35), sf::Color(190, 110, 45), sf::Color(230, 145, 70));
    this->buttons["INVENTORY"] = new Button(
        1260.f, 900.f, 190.f, 70.f, &this->font, "Inventory",
        sf::Color(45, 100, 75), sf::Color(65, 145, 100), sf::Color(90, 190, 130));
}

GameState::GameState(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys,
                     std::stack<State*>* states, const CharacterConfig& character)
    : State(window, supportedKeys, states),
      character(character),
      characterText(this->font, "", 28),
      statsText(this->font, "", 24),
      arenaText(this->font, "", 28),
      monsterText(this->font, "", 20),
      hitboxText(this->font, "", 18)
{
    this->initKeybinds();
    this->initTextures();
    this->initPlayer();
    if (!this->font.openFromFile("Fonts/GeistPixel-Regular-VariableFont_ELSH.ttf")) {
        throw "ERROR::GAME_STATE::INIT_FONT";
    }
    this->characterText.setString(
        this->character.name + " - " + this->character.race + " " + this->character.characterClass);
    this->characterText.setFillColor(sf::Color::White);
    this->characterText.setPosition(sf::Vector2f(80.f, 80.f));

    std::stringstream stats;
    stats << "VIG: " << this->character.stats[0] << "\n"
          << "DEF: " << this->character.stats[1] << "\n"
          << "VEL: " << this->character.stats[2] << "\n"
          << "FOR: " << this->character.stats[3] << "\n"
          << "INT: " << this->character.stats[4];
    this->statsText.setString(stats.str());
    this->statsText.setFillColor(sf::Color::White);
    this->statsText.setPosition(sf::Vector2f(80.f, 140.f));
    this->initInterface();
}
GameState::~GameState(){
    for (auto& button : this->buttons) {
        delete button.second;
    }
}

void GameState::updateInput(const float& dt){
    if(sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Scancode>(this->keybinds.at("MOVE_LEFT"))))
    {
        // movement left logic can be added here
    }

    if(sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Scancode>(this->keybinds.at("MOVE_RIGHT"))))
    {
        // movement right logic can be added here
    }

    if(sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Scancode>(this->keybinds.at("MOVE_UP"))))
    {
        // movement up logic can be added here
    }

    if(sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Scancode>(this->keybinds.at("MOVE_DOWN"))))
    {
        // movement down logic can be added here
    }

    if(sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Scancode>(this->keybinds.at("CLOSE"))))
        this->endState();
}

void GameState::update(const float& dt){
    this->updateMousePosition();

    this->updateInput(dt);
    this->updateButtons();
}

void GameState::updateButtons(){
    for (auto& button : this->buttons) {
        button.second->update(this->mousePosView);
    }
}

void GameState::renderInterface(sf::RenderTarget* target){
    target->draw(this->hudPanel);
    target->draw(this->arenaPanel);
    target->draw(this->arenaText);
    target->draw(this->playerHitbox);
    target->draw(this->monsterHitbox);
    target->draw(this->monsterText);
    target->draw(this->hitboxText);
    for (auto& button : this->buttons) {
        button.second->render(target);
    }
}

void GameState::render(sf::RenderTarget* target ){
    if(!target)
        target = this->window;

    target->draw(this->characterText);
    target->draw(this->statsText);
    this->renderInterface(target);
}