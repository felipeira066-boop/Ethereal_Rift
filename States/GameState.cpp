#include "GameState.hpp"
#include "Header/Palette.hpp"

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
    this->hudPanel.setPosition(sf::Vector2f(32.f, 24.f));
    this->hudPanel.setSize(sf::Vector2f(1216.f, 128.f));
    this->hudPanel.setFillColor(theme::darkOliveGreen);
    this->hudPanel.setOutlineColor(theme::camel);
    this->hudPanel.setOutlineThickness(2.f);

    this->arenaPanel.setPosition(sf::Vector2f(32.f, 176.f));
    this->arenaPanel.setSize(sf::Vector2f(1216.f, 420.f));
    this->arenaPanel.setFillColor(sf::Color(theme::laurelGreen.r,
                                            theme::laurelGreen.g,
                                            theme::laurelGreen.b, 220));
    this->arenaPanel.setOutlineColor(theme::camel);
    this->arenaPanel.setOutlineThickness(2.f);

    this->playerHitbox.setPosition(sf::Vector2f(180.f, 330.f));
    this->playerHitbox.setSize(sf::Vector2f(80.f, 105.f));
    this->playerHitbox.setFillColor(sf::Color(theme::darkOliveGreen.r,
                                              theme::darkOliveGreen.g,
                                              theme::darkOliveGreen.b, 100));
    this->playerHitbox.setOutlineColor(theme::lemonMeringue);
    this->playerHitbox.setOutlineThickness(2.f);

    this->monsterHitbox.setPosition(sf::Vector2f(1000.f, 320.f));
    this->monsterHitbox.setSize(sf::Vector2f(120.f, 145.f));
    this->monsterHitbox.setFillColor(sf::Color(theme::russet.r,
                                               theme::russet.g,
                                               theme::russet.b, 140));
    this->monsterHitbox.setOutlineColor(theme::alloyOrange);
    this->monsterHitbox.setOutlineThickness(3.f);

    this->arenaText.setString("COMBAT ARENA");
    this->arenaText.setCharacterSize(22);
    this->arenaText.setFillColor(theme::russet);
    this->arenaText.setPosition(sf::Vector2f(64.f, 190.f));

    this->monsterText.setString("MONSTER SPAWN");
    this->monsterText.setCharacterSize(16);
    this->monsterText.setFillColor(theme::russet);
    this->monsterText.setPosition(sf::Vector2f(980.f, 280.f));

    this->hitboxText.setString("HITBOX DEBUG");
    this->hitboxText.setCharacterSize(14);
    this->hitboxText.setFillColor(theme::russet);
    this->hitboxText.setPosition(sf::Vector2f(930.f, 520.f));

    this->buttons["ABILITY_1"] = new Button(
        202.f, 630.f, 160.f, 56.f, &this->font, "Ability 1",
        theme::russet, theme::alloyOrange, theme::camel);
    this->buttons["ABILITY_2"] = new Button(
        376.f, 630.f, 160.f, 56.f, &this->font, "Ability 2",
        theme::russet, theme::alloyOrange, theme::camel);
    this->buttons["ABILITY_3"] = new Button(
        550.f, 630.f, 160.f, 56.f, &this->font, "Ability 3",
        theme::russet, theme::alloyOrange, theme::camel);
    this->buttons["FLEE"] = new Button(
        724.f, 630.f, 140.f, 56.f, &this->font, "Flee",
        theme::russet, theme::alloyOrange, theme::camel);
    this->buttons["INVENTORY"] = new Button(
        878.f, 630.f, 200.f, 56.f, &this->font, "Inventory",
        theme::darkOliveGreen, theme::laurelGreen, theme::camel);
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
    this->characterText.setCharacterSize(22);
    this->characterText.setFillColor(theme::cornsilk);
    this->characterText.setPosition(sf::Vector2f(64.f, 54.f));

    std::stringstream stats;
    stats << "VIG: " << this->character.stats[0] << "\n"
          << "DEF: " << this->character.stats[1] << "\n"
          << "VEL: " << this->character.stats[2] << "\n"
          << "FOR: " << this->character.stats[3] << "\n"
          << "INT: " << this->character.stats[4];
    this->statsText.setString(stats.str());
    this->statsText.setCharacterSize(18);
    this->statsText.setFillColor(theme::cornsilk);
    this->statsText.setPosition(sf::Vector2f(640.f, 42.f));
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