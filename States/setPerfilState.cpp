#include "setPerfilState.h"
#include "WorldState.hpp"
#include "Header/Palette.hpp"

void setPerfilState::initVariables(){
    this->characterName = "Hero";
    this->selectedRace = "Human";
    this->selectedClass = "Warrior";
    this->updateSelectedStats();
}

void setPerfilState::initBackground(){
    this->background.setSize(sf::Vector2f(1280.f, 720.f));
    this->background.setFillColor(theme::darkOliveGreen);
}
void setPerfilState::initFonts(){
    if (!this->font.openFromFile("Fonts/slkscr.ttf")) {
        throw "ERROR SETPERFILSTATE LOADING FONT";
    }
    if (!this->boldFont.openFromFile("Fonts/slkscrb.ttf")) {
        throw "ERROR SETPERFILSTATE LOADING BOLD FONT";
    }
}
void setPerfilState::initKeybinds(){
}
void setPerfilState::initButton(){

}

sf::Vector2f setPerfilState::radarPoint(float value, float radius) const{
    constexpr float pi = 3.14159265358979323846f;
    constexpr float centerX = 900.f;
    constexpr float centerY = 385.f;
    constexpr float startAngle = -pi / 2.f;
    const float angle = startAngle + value * (2.f * pi / 5.f);
    return sf::Vector2f(centerX + std::cos(angle) * radius,
                        centerY + std::sin(angle) * radius);
}

CharacterConfig setPerfilState::buildCharacterConfig() const{
    CharacterConfig config;
    config.name = "Hero";
    config.name = this->characterName.empty() ? "Hero" : this->characterName;
    config.race = this->selectedRace;
    config.characterClass = this->selectedClass;
    config.stats = this->selectedStats;
    return config;
}

void setPerfilState::updateSelectedStats(){
    std::array<float, 5> raceStats = {2.f, 2.f, 2.f, 3.f, 2.f};
    if (this->selectedRace == "Elf") {
        raceStats = {3.f, 1.f, 2.f, 2.f, 3.f};
    } else if (this->selectedRace == "Dwarf") {
        raceStats = {5.f, 4.f, 0.f, 2.f, 1.f};
    }

    std::array<float, 5> classStats = {2.f, 2.f, 0.f, 3.f, 2.f};
    if (this->selectedClass == "Hunter") {
        classStats = {0.f, 0.f, 3.f, 2.f, 0.f};
    } else if (this->selectedClass == "Mage") {
        classStats = {1.f, 0.f, 0.f, 0.f, 4.f};
    } else if (this->selectedClass == "Cleric") {
        classStats = {4.f, 0.f, 0.f, 0.f, 1.f};
    }

    for (std::size_t index = 0; index < this->selectedStats.size(); ++index) {
        this->selectedStats[index] = raceStats[index] + classStats[index];
    }
}

void setPerfilState::initRadarChart(){
    this->radarGrid.setPointCount(5);
    this->radarGrid.setFillColor(sf::Color::Transparent);
    this->radarGrid.setOutlineColor(sf::Color(theme::lemonMeringue.r,
                                               theme::lemonMeringue.g,
                                               theme::lemonMeringue.b, 150));
    this->radarGrid.setOutlineThickness(2.f);

    this->radarValues.setPointCount(5);
    this->radarValues.setFillColor(sf::Color(theme::alloyOrange.r,
                                              theme::alloyOrange.g,
                                              theme::alloyOrange.b, 130));
    this->radarValues.setOutlineColor(theme::lemonMeringue);
    this->radarValues.setOutlineThickness(3.f);

    const std::array<std::string, 5> names = {
        "VIT", "DEF", "SPD", "STR", "INT"
    };
    this->radarLabels.clear();
    for (std::size_t index = 0; index < names.size(); ++index) {
        this->radarLabels.emplace_back(this->font, names[index], 18);
        this->radarLabels[index].setFillColor(theme::cornsilk);
        const sf::Vector2f labelPoint = this->radarPoint(static_cast<float>(index), 158.f);
        this->radarLabels[index].setPosition(labelPoint);
    }

    this->updateRadarChart();
}

void setPerfilState::updateRadarChart(){
    constexpr float maxStat = 10.f;
    constexpr float radius = 125.f;

    for (std::size_t index = 0; index < 5; ++index) {
        this->radarGrid.setPoint(index, this->radarPoint(static_cast<float>(index), radius));
        const float normalized = std::max(0.f, std::min(this->selectedStats[index] / maxStat, 1.f));
        this->radarValues.setPoint(index, this->radarPoint(static_cast<float>(index), radius * normalized));
    }
}

setPerfilState::setPerfilState(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::stack<State*>* states)
    : State(window, supportedKeys, states),
      selectedRaceText(this->font, "", 28),
      selectedClassText(this->font, "", 28),
      nameText(this->font, "", 24),
      instructionsText(this->boldFont, "", 20)
{
    this->initVariables();
    this->initBackground();
    this->initFonts();
    this->initButton();
    this->initKeybinds();
    this->initRadarChart();

    this->buttons["RETURN_STATE"] =
        new Button(64, 640, 150, 52,
            &this->font, "Return",
            theme::russet,
            theme::alloyOrange,
            theme::camel);

    this->buttons["NEXT_STATE"] =
        new Button(1066, 640, 150, 52,
            &this->font, "Next",
            theme::russet,
            theme::alloyOrange,
            theme::camel);

    this->buttons["HUMAN_RACE"] =
        new Button(64, 205, 170, 56,
            &this->font, "Human",
            theme::russet,
            theme::alloyOrange,
            theme::camel);

    this->buttons["ELF_RACE"] =
        new Button(64, 275, 170, 56,
            &this->font, "Elf",
            theme::russet,
            theme::alloyOrange,
            theme::camel);

    this->buttons["DWARF_RACE"] =
        new Button(64, 345, 170, 56,
            &this->font, "Dwarf",
            theme::russet,
            theme::alloyOrange,
            theme::camel);

    this->buttons["WARRIOR_CLASS"] =
        new Button(260, 205, 170, 56,
            &this->font, "Warrior",
            theme::russet,
            theme::alloyOrange,
            theme::camel);

    this->buttons["HUNTER_CLASS"] =
        new Button(260, 275, 170, 56,
            &this->font, "Hunter",
            theme::russet,
            theme::alloyOrange,
            theme::camel);

    this->buttons["MAGE_CLASS"] =
        new Button(260, 345, 170, 56,
            &this->font, "Mage",
            theme::russet,
            theme::alloyOrange,
            theme::camel);

    this->buttons["CLERIC_CLASS"] =
        new Button(260, 415, 170, 56,
            &this->font, "Cleric",
            theme::russet,
            theme::alloyOrange,
            theme::camel);

    this->selectedRaceText.setFont(this->font);
    this->selectedRaceText.setString("Race: Human");
    this->selectedRaceText.setCharacterSize(22);
    this->selectedRaceText.setFillColor(theme::cornsilk);
    this->selectedRaceText.setPosition(sf::Vector2f(470.f, 205.f));

    this->selectedClassText.setFont(this->font);
    this->selectedClassText.setString("Class: Warrior");
    this->selectedClassText.setCharacterSize(22);
    this->selectedClassText.setFillColor(theme::cornsilk);
    this->selectedClassText.setPosition(sf::Vector2f(470.f, 245.f));

    this->nameInputBox.setPosition(sf::Vector2f(64.f, 92.f));
    this->nameInputBox.setSize(sf::Vector2f(366.f, 48.f));
    this->nameInputBox.setFillColor(theme::laurelGreen);
    this->nameInputBox.setOutlineColor(theme::alloyOrange);
    this->nameInputBox.setOutlineThickness(2.f);

    this->nameText.setString(this->characterName);
    this->nameText.setFillColor(theme::russet);
    this->nameText.setCharacterSize(20);
    this->nameText.setPosition(sf::Vector2f(80.f, 101.f));

    this->instructionsText.setString("Name (type here) | Select race and class");
    this->instructionsText.setCharacterSize(18);
    this->instructionsText.setFillColor(theme::lemonMeringue);
    this->instructionsText.setPosition(sf::Vector2f(64.f, 40.f));
}

setPerfilState::~setPerfilState()
{
    for(auto it =  this->buttons.begin(); it != this->buttons.end(); ++it){
        delete it->second;
    }
}

void setPerfilState::update(const float& dt){
    this->updateMousePosition();
    this->updateInput(dt);

    this->updateButton();
} 

void setPerfilState::handleEvent(const sf::Event& event){
    if (const sf::Event::TextEntered* textEntered = event.getIf<sf::Event::TextEntered>()) {
        if (textEntered->unicode >= 32 && textEntered->unicode <= 126 &&
            this->characterName.size() < 18) {
            this->characterName += static_cast<char>(textEntered->unicode);
            this->nameText.setString(this->characterName);
        }
    }

    if (const sf::Event::KeyPressed* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->code == sf::Keyboard::Key::Backspace &&
            !this->characterName.empty()) {
            this->characterName.pop_back();
            this->nameText.setString(this->characterName);
        }
    }
}
void setPerfilState::updateButton(){
    for(auto &it : this->buttons){
        it.second->update(this->mousePosView);
    }

    if(this->buttons["HUMAN_RACE"]->isPressed())
    {
        this->selectedRace = "Human";
        this->updateSelectedStats();
        this->selectedRaceText.setString("Race: Human");
        this->updateRadarChart();
    }
    if(this->buttons["ELF_RACE"]->isPressed())
    {
        this->selectedRace = "Elf";
        this->updateSelectedStats();
        this->selectedRaceText.setString("Race: Elf");
        this->updateRadarChart();
    }
    if(this->buttons["DWARF_RACE"]->isPressed())
    {
        this->selectedRace = "Dwarf";
        this->updateSelectedStats();
        this->selectedRaceText.setString("Race: Dwarf");
        this->updateRadarChart();
    }

    if(this->buttons["WARRIOR_CLASS"]->isPressed())
    {
        this->selectedClass = "Warrior";
        this->updateSelectedStats();
        this->selectedClassText.setString("Class: Warrior");
        this->updateRadarChart();
    }
    if(this->buttons["HUNTER_CLASS"]->isPressed())
    {
        this->selectedClass = "Hunter";
        this->updateSelectedStats();
        this->selectedClassText.setString("Class: Hunter");
        this->updateRadarChart();
    }
    if(this->buttons["MAGE_CLASS"]->isPressed())
    {
        this->selectedClass = "Mage";
        this->updateSelectedStats();
        this->selectedClassText.setString("Class: Mage");
        this->updateRadarChart();
    }
    if(this->buttons["CLERIC_CLASS"]->isPressed())
    {
        this->selectedClass = "Cleric";
        this->updateSelectedStats();
        this->selectedClassText.setString("Class: Cleric");
        this->updateRadarChart();
    }

    if(this->buttons["NEXT_STATE"]->isPressed())
    {
        const CharacterConfig character = this->buildCharacterConfig();
        auto* battle = new BattleState(
            this->window, this->supportedKeys, this->states, character);
        this->states->push(battle);
        this->states->push(new WorldState(
            this->window, this->supportedKeys, this->states, character, *battle));
    }

    if(this->buttons["RETURN_STATE"]->isPressed())
    {
        this->endState();
    }
}
void setPerfilState::renderButton(sf::RenderTarget* target){
    for(auto &it : this->buttons){
        it.second->render(target);
    }
}
void setPerfilState::updateInput(const float& dt){

}
void setPerfilState::render(sf::RenderTarget* target){
    if(!target)
        target = this->window;

    target->draw(this->background);
    target->draw(this->instructionsText);
    target->draw(this->nameInputBox);
    target->draw(this->nameText);
    target->draw(this->selectedRaceText);
    target->draw(this->selectedClassText);
    target->draw(this->radarGrid);
    target->draw(this->radarValues);
    for (const sf::Text& label : this->radarLabels) {
        target->draw(label);
    }
    this->renderButton(target);
}