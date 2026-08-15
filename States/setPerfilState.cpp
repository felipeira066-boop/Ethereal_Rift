#include "setPerfilState.h"

void setPerfilState::initVariables(){

}

void setPerfilState::initBackground(){
    this->background.setSize(sf::Vector2f(static_cast<float>(this->window->getSize().x), static_cast<float>(this->window->getSize().y)));
    if (!this->bgTexture.loadFromFile("")) {
        throw "ERROR::SETPERFILSTATE TO LOADING THE BACKGROUND ONE";
    }
}
void setPerfilState::initFonts(){
    if (!this->font.loadFromFile("../Fonts/GeistPixel-Regular-VariableFont_ELSH.ttf")) {
        throw "ERROR SETPERFILSTATE LOADING FONT";
    }
}
void setPerfilState::initKeybinds(){
}
void setPerfilState::initButton(){

}

setPerfilState::setPerfilState(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::stack<State*>* states)
    : State(window, supportedKeys, states)
{

    this->initVariables();
    this->initBackground();
    this->initFonts();
    this->initButton();
    this->initKeybinds();

    this->buttons["RETURN_STATE"] = 
        new Button( 150, 940, 150, 75,
            &this->font, "Return",
            sf::Color(0, 128, 128, 100),
            sf::Color(255, 99, 71, 100),
            sf::Color(255, 105, 180, 100));

    //botão de configurações        
    this->buttons["NEXT_STATE"] = 
        new Button(1800, 940, 150, 75,
            &this->font, "Next",
            sf::Color(0, 128, 128, 100),
            sf::Color(255, 99, 71, 100),
            sf::Color(255, 105, 180, 100));
            
    //botão de fechar o jogo
    this->buttons["HUMAN_RACE"] =
        new Button(250, 750, 150, 75,
            &this->font, "Human",
            sf::Color(0, 128, 128, 100),
            sf::Color(255, 99, 71, 100),
            sf::Color(255, 105, 180, 100));

    this->buttons["ELF_RACE"] =
        new Button(250, 825, 150, 75,
            &this->font, "Elf",
            sf::Color(0, 128, 128, 100),
            sf::Color(255, 99, 71, 100),
            sf::Color(255, 105, 180, 100));

    this->buttons["DWARF_RACE"] =
        new Button(250, 900, 150, 75,
            &this->font, "Dwarf",
            sf::Color(0, 128, 128, 100),
            sf::Color(255, 99, 71, 100),
            sf::Color(255, 105, 180, 100));

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
void setPerfilState::updateButton(){
    for(auto &it : this->buttons){
        it.second->update(this->mousePosView);
    }

    if(this->buttons["NEXT_STATE"]->isPressed())
    {
        this->states->push(new GameState(this->window, this->supportedKeys, this->states));
    } 

    //Quit Button
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
    this->renderButton(target);
}