#include "Button.hpp"
#include "Header/Palette.hpp"

Button::Button(
                float x, float y,
                float width, float height,
                sf::Font* font, std::string text,
                sf::Color idleColor,
                sf::Color hoverColor,
                sf::Color activeColor
                )
    : buttonStates(BTN_IDLE),
      font(font),
      text(*font, "", 30),
      idleColor(idleColor),
      hoverColor(hoverColor),
      activeColor(activeColor)
{
    this->shape.setPosition(sf::Vector2f(x, y));
    this->shape.setSize(sf::Vector2f(width, height));

    this->text.setFont(*this->font);
    this->text.setString(text);
    this->text.setFillColor(theme::cornsilk);
    this->text.setCharacterSize(24);
    const sf::FloatRect bounds = this->shape.getGlobalBounds();
    this->text.setPosition(sf::Vector2f(
        this->shape.getPosition().x + (bounds.size.x / 2.f) - (this->text.getGlobalBounds().size.x / 2.f),
        this->shape.getPosition().y + (bounds.size.y / 2.f) - (this->text.getGlobalBounds().size.y / 2.f)
    ));

    this->shape.setFillColor(this->idleColor);
}

Button::~Button(){

}

void Button::update(const sf::Vector2f mousePos){
    /*Update the booleans for hover and pressed*/

    //idle
    this->buttonStates = BTN_IDLE;
    
    //hover
    if(this->shape.getGlobalBounds().contains(mousePos)){
        this->buttonStates = BTN_HOVER;

        //pressed
        if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
            this->buttonStates = BTN_ACTIVE;
        }
    }

    switch(this->buttonStates){
        case BTN_IDLE:
            this->shape.setFillColor(this->idleColor);
            break;
        case BTN_HOVER:
            this->shape.setFillColor(this->hoverColor);
            break;
        case BTN_ACTIVE:
            this->shape.setFillColor(this->activeColor);
            break;
        default:
            this->shape.setFillColor(sf::Color::Red);
            break;
    }
}

const bool Button::isPressed() const{

    if(this->buttonStates == BTN_ACTIVE)
        return true;

    return false;
}

void Button::render(sf::RenderTarget* target){
    target->draw(this->shape);
    target->draw(this->text);
}