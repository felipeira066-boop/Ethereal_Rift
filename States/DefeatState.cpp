#include "DefeatState.hpp"
#include "setPerfilState.h"
#include "Header/Palette.hpp"

DefeatState::DefeatState(sf::RenderWindow* window,
                         std::map<std::string, int>* supportedKeys,
                         std::stack<State*>* states)
    : State(window, supportedKeys, states),
      titleText(this->boldFont, "You were defeated!", 48)
{
    if (!this->font.openFromFile("Fonts/slkscr.ttf")) {
        throw "ERROR::DEFEAT_STATE::INIT_FONT";
    }
    if (!this->boldFont.openFromFile("Fonts/slkscrb.ttf")) {
        throw "ERROR::DEFEAT_STATE::INIT_BOLD_FONT";
    }

    this->initKeybinds();

    this->background.setSize(sf::Vector2f(1280.f, 720.f));
    this->background.setFillColor(theme::darkOliveGreen);

    this->panel.setPosition(sf::Vector2f(300.f, 190.f));
    this->panel.setSize(sf::Vector2f(680.f, 340.f));
    this->panel.setFillColor(theme::laurelGreen);
    this->panel.setOutlineColor(theme::camel);
    this->panel.setOutlineThickness(4.f);

    this->titleText.setFillColor(theme::russet);
    this->titleText.setPosition(sf::Vector2f(
        640.f - this->titleText.getGlobalBounds().size.x / 2.f, 255.f));

    this->buttons["EXIT"] = new Button(
        400.f, 405.f, 210.f, 64.f, &this->font, "Exit",
        theme::russet, theme::alloyOrange, theme::camel);
    this->buttons["RETRY"] = new Button(
        670.f, 405.f, 210.f, 64.f, &this->font, "Play Again",
        theme::russet, theme::alloyOrange, theme::camel);
}

DefeatState::~DefeatState()
{
    for (auto& button : this->buttons) {
        delete button.second;
    }
}

void DefeatState::initKeybinds()
{
}

void DefeatState::updateInput(const float&)
{
}

void DefeatState::update(const float& dt)
{
    this->updateMousePosition();
    this->updateInput(dt);
    this->updateButtons();
}

void DefeatState::updateButtons()
{
    for (auto& button : this->buttons) {
        button.second->update(this->mousePosView);
    }

    if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        this->actionButtonLatched = false;
    }
    if (this->actionButtonLatched) {
        return;
    }

    if (this->buttons["EXIT"]->isPressed()) {
        this->actionButtonLatched = true;
        this->window->close();
        this->endState();
    } else if (this->buttons["RETRY"]->isPressed()) {
        this->actionButtonLatched = true;
        this->restartFromProfile();
    }
}

void DefeatState::restartFromProfile()
{
    this->states->pop();
    while (this->states->size() > 1) {
        delete this->states->top();
        this->states->pop();
    }

    State* defeatState = this;
    this->states->push(new setPerfilState(
        this->window, this->supportedKeys, this->states));
    this->states->push(defeatState);
    this->endState();
}

void DefeatState::render(sf::RenderTarget* target)
{
    if (!target) {
        target = this->window;
    }

    target->draw(this->background);
    target->draw(this->panel);
    target->draw(this->titleText);
    for (auto& button : this->buttons) {
        button.second->render(target);
    }
}
