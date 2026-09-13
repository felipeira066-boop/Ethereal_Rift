#include "MainMenuState.hpp"

//initializer Functions
void MainMenuState::initVariables(){

}

void MainMenuState::initBackground(){
    this->background.setSize(sf::Vector2f(static_cast<float>(this->window->getSize().x), static_cast<float>(this->window->getSize().y)));

    if(!this->bgTexture.loadFromFile("Resource_Files/Images/Backgrounds/bg1.png")){
        throw "ERROR::MAIN_MENU_STATE TO LOADING THE BACKGROUND ONE!";
    }

    this->background.setTexture(&this->bgTexture);
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
    if(!this->font.openFromFile("Fonts/GeistPixel-Regular-VariableFont_ELSH.ttf")){
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

    //Botao de iniciar o jogo
    this->buttons["GAME_STATE_BTN"] = 
        new Button(250, 450, 250, 100,
            &this->font, "New Game",
            sf::Color(0, 128, 128, 100),
            sf::Color(255, 99, 71, 100),
            sf::Color(255, 105, 180, 100));

    //botão de configurações        
    this->buttons["SETTINGS_STATE_BTN"] = 
        new Button(250, 600, 250, 100,
            &this->font, "Settings",
            sf::Color(0, 128, 128, 100),
            sf::Color(255, 99, 71, 100),
            sf::Color(255, 105, 180, 100));
            
    //botão de fechar o jogo
    this->buttons["EXIT_STATE_BTN"] =
        new Button(250, 750, 250, 100,
            &this->font, "End Game",
            sf::Color(0, 128, 128, 100),
            sf::Color(255, 99, 71, 100),
            sf::Color(255, 105, 180, 100));

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