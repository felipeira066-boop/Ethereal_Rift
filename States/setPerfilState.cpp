#include "setPerfilState.h"

void setPerfilState::initVariables(){
    this->characterName = "Hero";
    this->selectedRace = "Human";
    this->selectedClass = "Warrior";
    this->updateSelectedStats();
}

void setPerfilState::initBackground(){
    this->background.setSize(sf::Vector2f(static_cast<float>(this->window->getSize().x), static_cast<float>(this->window->getSize().y)));
    if (!this->bgTexture.loadFromFile("Resource_Files/Images/Backgrounds/bg1.png")) {
        this->background.setFillColor(sf::Color(15, 23, 42));
        return;
    }
    this->background.setTexture(&this->bgTexture);
}
void setPerfilState::initFonts(){
    if (!this->font.openFromFile("Fonts/GeistPixel-Regular-VariableFont_ELSH.ttf")) {
        throw "ERROR SETPERFILSTATE LOADING FONT";
    }
}
void setPerfilState::initKeybinds(){
}
void setPerfilState::initButton(){

}

sf::Vector2f setPerfilState::radarPoint(float value, float radius) const{
    constexpr float pi = 3.14159265358979323846f;
    constexpr float centerX = 1200.f;
    constexpr float centerY = 560.f;
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
    this->radarGrid.setOutlineColor(sf::Color(190, 210, 230, 180));
    this->radarGrid.setOutlineThickness(2.f);

    this->radarValues.setPointCount(5);
    this->radarValues.setFillColor(sf::Color(40, 180, 210, 130));
    this->radarValues.setOutlineColor(sf::Color(130, 240, 255));
    this->radarValues.setOutlineThickness(3.f);

    const std::array<std::string, 5> names = {
        "VIG", "DEF", "VEL", "FOR", "INT"
    };
    this->radarLabels.clear();
    for (std::size_t index = 0; index < names.size(); ++index) {
        this->radarLabels.emplace_back(this->font, names[index], 22);
        this->radarLabels[index].setFillColor(sf::Color::White);
        const sf::Vector2f labelPoint = this->radarPoint(static_cast<float>(index), 255.f);
        this->radarLabels[index].setPosition(labelPoint);
    }

    this->updateRadarChart();
}

void setPerfilState::updateRadarChart(){
    constexpr float maxStat = 10.f;
    constexpr float radius = 200.f;

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
      instructionsText(this->font, "", 20)
{
    this->initVariables();
    this->initBackground();
    this->initFonts();
    this->initButton();
    this->initKeybinds();
    this->initRadarChart();

    this->buttons["RETURN_STATE"] =
        new Button(150, 940, 150, 75,
            &this->font, "Return",
            sf::Color(0, 128, 128, 100),
            sf::Color(255, 99, 71, 100),
            sf::Color(255, 105, 180, 100));

    this->buttons["NEXT_STATE"] =
        new Button(1800, 940, 150, 75,
            &this->font, "Next",
            sf::Color(0, 128, 128, 100),
            sf::Color(255, 99, 71, 100),
            sf::Color(255, 105, 180, 100));

    this->buttons["HUMAN_RACE"] =
        new Button(250, 450, 220, 80,
            &this->font, "Human",
            sf::Color(0, 128, 128, 100),
            sf::Color(255, 99, 71, 100),
            sf::Color(255, 105, 180, 100));

    this->buttons["ELF_RACE"] =
        new Button(250, 560, 220, 80,
            &this->font, "Elf",
            sf::Color(0, 128, 128, 100),
            sf::Color(255, 99, 71, 100),
            sf::Color(255, 105, 180, 100));

    this->buttons["DWARF_RACE"] =
        new Button(250, 670, 220, 80,
            &this->font, "Dwarf",
            sf::Color(0, 128, 128, 100),
            sf::Color(255, 99, 71, 100),
            sf::Color(255, 105, 180, 100));

    this->buttons["WARRIOR_CLASS"] =
        new Button(500, 450, 220, 80,
            &this->font, "Warrior",
            sf::Color(0, 128, 128, 100),
            sf::Color(255, 99, 71, 100),
            sf::Color(255, 105, 180, 100));

    this->buttons["HUNTER_CLASS"] =
        new Button(500, 560, 220, 80,
            &this->font, "Hunter",
            sf::Color(0, 128, 128, 100),
            sf::Color(255, 99, 71, 100),
            sf::Color(255, 105, 180, 100));

    this->buttons["MAGE_CLASS"] =
        new Button(500, 670, 220, 80,
            &this->font, "Mage",
            sf::Color(0, 128, 128, 100),
            sf::Color(255, 99, 71, 100),
            sf::Color(255, 105, 180, 100));

    this->buttons["CLERIC_CLASS"] =
        new Button(500, 780, 220, 80,
            &this->font, "Cleric",
            sf::Color(0, 128, 128, 100),
            sf::Color(255, 99, 71, 100),
            sf::Color(255, 105, 180, 100));

    this->selectedRaceText.setFont(this->font);
    this->selectedRaceText.setString("Race: Human");
    this->selectedRaceText.setCharacterSize(28);
    this->selectedRaceText.setFillColor(sf::Color::White);
    this->selectedRaceText.setPosition(sf::Vector2f(700.f, 250.f));

    this->selectedClassText.setFont(this->font);
    this->selectedClassText.setString("Class: Warrior");
    this->selectedClassText.setCharacterSize(28);
    this->selectedClassText.setFillColor(sf::Color::White);
    this->selectedClassText.setPosition(sf::Vector2f(700.f, 295.f));

    this->nameInputBox.setPosition(sf::Vector2f(250.f, 270.f));
    this->nameInputBox.setSize(sf::Vector2f(470.f, 60.f));
    this->nameInputBox.setFillColor(sf::Color(10, 20, 35, 210));
    this->nameInputBox.setOutlineColor(sf::Color(130, 220, 240));
    this->nameInputBox.setOutlineThickness(2.f);

    this->nameText.setString(this->characterName);
    this->nameText.setFillColor(sf::Color::White);
    this->nameText.setPosition(sf::Vector2f(270.f, 284.f));

    this->instructionsText.setString("Name (type here) | Select race and class");
    this->instructionsText.setFillColor(sf::Color(220, 230, 240));
    this->instructionsText.setPosition(sf::Vector2f(250.f, 210.f));
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
        this->states->push(new GameState(this->window, this->supportedKeys, this->states,
                                          this->buildCharacterConfig()));
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