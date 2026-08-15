#include "../Header/Game.hpp"

//Init
void Game::initVariables(){
    this->window = NULL;
    this->fullscreen = false;
    this->dt = 0.0f;
}

void Game::initWindow(){
    std::ifstream ifs("Config/window.ini");
    this->videoModes = sf::VideoMode::getFullscreenModes();

    std::string title="None";
    sf::VideoMode window_bounds = sf::VideoMode::getDesktopMode();
    bool fullscreen = false;
    unsigned framerate_limit;
    bool vertical_sync_enabled;
    unsigned antialiasing_level = 0;

    if(ifs.is_open()){
        std::getline(ifs, title);
        ifs >> window_bounds.width >> window_bounds.height;
        ifs>> fullscreen;
        ifs >> framerate_limit;
        ifs >> vertical_sync_enabled;
        ifs >> antialiasing_level;
    }

    this->fullscreen = fullscreen;
    this->windowSettings.antialiasingLevel = antialiasing_level;
    if(this->fullscreen)
        this->window = new sf::RenderWindow(window_bounds,
         title, sf::Style::Fullscreen , 
         this->windowSettings);
    else
        this->window = new sf::RenderWindow(window_bounds, title,
         sf::Style::Titlebar | sf::Style::Close, 
         this->windowSettings); 
    this->window->setFramerateLimit(framerate_limit);
    this->window->setVerticalSyncEnabled(vertical_sync_enabled);
}

void Game::initKeys(){

        std::ifstream ifs("Config/supportedKeys.ini");

    if(ifs.is_open()){
        std::string keyStr = "";
        int keyNum = 0;

        while(ifs >> keyStr >> keyNum){
            this->supportedKeys[keyStr] = keyNum;

        }
    }
    ifs.close();

    for(auto i : this->supportedKeys){
        //DEBUG REMOVE LATER
        std::cout << i.first << " " << i.second << std::endl;
    }

}

void Game::initState(){
    this->states.push(new MainMenuState(this->window, &this->supportedKeys, &this->states));
    //this->states.push(new GameState(this->window, &this->supportedKeys));
}

//Constructions & Destructions
Game::Game(){
    this->initWindow();
    this->initKeys(); //teclado deve ser interagido antes dos estatdos!
    this->initState();
}

Game::~Game(){
    delete this->window;

    while(!this->states.empty()){
        delete this->states.top();
        this->states.pop();
    }
}

//Functions

void Game::updateDt(){
    this->dt = this->dtClock.restart().asSeconds();
}

void Game::updateSFMLEvents(){

    while (this->window->pollEvent(this->event))
    {
        if (this->event.type == sf::Event::Closed)
            this->window->close();
    }
}

void Game::update(){
    this->updateSFMLEvents();

    if(!this->states.empty()){
        this->states.top()->update(this->dt);

        if(this->states.top()->getQuit()){
            this->states.top()->endState();
            delete this->states.top();
            this->states.pop();
        }
    }
    // app end
    else{
        this->window->close();
    }
}

void Game::render(){
    this->window->clear();
    //render Items
    if(!this->states.empty()){
        this->states.top()->render(this->window);
    }
    this->window->display();
}

void Game::run(){
    while (this->window->isOpen())
    {
        this->updateDt();
        this->update();
        this->render();
    }
}