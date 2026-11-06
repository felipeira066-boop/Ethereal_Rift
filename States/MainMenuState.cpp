#include "MainMenuState.hpp"
#include "Header/Palette.hpp"

//initializer Functions
void MainMenuState::initVariables(){

}

void MainMenuState::initBackground(){
    this->background.setSize(this->window->getView().getSize());
    this->background.setFillColor(theme::darkOliveGreen);
}

void MainMenuState::initKeybinds(){

    std::ifstream ifs("Config/MainMenuState_keybinds.ini");

    if(ifs.is_open()){
        std::string keyStr = "";
        std::string keyNum = "";

        while(ifs >> keyStr >> keyNum){
            this->keybinds[keyStr] = this->supportedKeys->at(keyNum);

        }
    }
    ifs.close();
    
}

void MainMenuState::initFonts(){
    if(!this->font.openFromFile("Fonts/slkscr.ttf")){
        throw("ERROR in the Font Loading!\n");
    }
}

void MainMenuState::initButton(){

}

//Constractures & Destroctures
MainMenuState::MainMenuState(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::stack<State*>* states) 
    : State(window, supportedKeys, states)
{
    this->initVariables();
    this->initBackground();
    this->initFonts();
    this->initButton();
    this->initKeybinds();

    constexpr float buttonWidth = 250.f;
    constexpr float buttonHeight = 100.f;
    constexpr float buttonSpacing = 50.f;
    constexpr float buttonCount = 3.f;
    const sf::Vector2f viewCenter = this->window->getView().getCenter();
    const float buttonX = viewCenter.x - buttonWidth / 2.f;
    const float firstButtonY =
        viewCenter.y - (buttonCount * buttonHeight + (buttonCount - 1.f) * buttonSpacing) / 2.f;

    //Botao de iniciar o jogo
    this->buttons["GAME_STATE_BTN"] = 
        new Button(buttonX, firstButtonY, buttonWidth, buttonHeight,
            &this->font, "New Game",
            theme::russet,
            theme::alloyOrange,
            theme::camel);

    //botão de configurações        
    this->buttons["SETTINGS_STATE_BTN"] = 
        new Button(buttonX, firstButtonY + buttonHeight + buttonSpacing, buttonWidth, buttonHeight,
            &this->font, "Settings",
            theme::russet,
            theme::alloyOrange,
            theme::camel);
            
    //botão de fechar o jogo
    this->buttons["EXIT_STATE_BTN"] =
        new Button(buttonX, firstButtonY + 2.f * (buttonHeight + buttonSpacing), buttonWidth, buttonHeight,
            &this->font, "Quit Game",
            theme::russet,
            theme::alloyOrange,
            theme::camel);

}
MainMenuState::~MainMenuState(){
    
    for(auto it =  this->buttons.begin(); it != this->buttons.end(); ++it){
        delete it->second;
    }
}

//Functions

void MainMenuState::updateInput(const float& dt){

}

void MainMenuState::update(const float& dt){
    this->updateMousePosition();
    this->updateInput(dt);

    this->updateButtons();
}

void MainMenuState::updateButtons(){
    /*update the all buttons in the states and the handles their function*/
    for(auto &it : this->buttons){
        it.second->update(this->mousePosView);
    }

    if(this->buttons["GAME_STATE_BTN"]->isPressed())
    {
        this->states->push(new setPerfilState(this->window, this->supportedKeys, this->states));
    } 

    //Quit Button
    if(this->buttons["EXIT_STATE_BTN"]->isPressed())
    {
        this->endState();
    }
}   

void MainMenuState::renderButtons(sf::RenderTarget* target){
    for(auto &it : this->buttons){
        it.second->render(target);
    }
}

void MainMenuState::render(sf::RenderTarget* target ){
    if(!target)
        target = this->window;

    target->draw(this->background);
    this->renderButtons(target);
    /*
    //Manutnção de posições
    sf::Text mouseText;
    mouseText.setPosition(this->mousePosView);
    mouseText.setFont(this->font);
    mouseText.setCharacterSize(12);
    std::stringstream ss;
    ss << this->mousePosView.x << " " << this->mousePosView.y;
    mouseText.setString(ss.str());

    target->draw(mouseText);
    */
}