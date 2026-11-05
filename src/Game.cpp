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
    unsigned framerate_limit = 60;
    bool vertical_sync_enabled = false;
    unsigned antialiasing_level = 0;

    if(ifs.is_open()){
        std::getline(ifs, title);
        ifs >> window_bounds.size.x >> window_bounds.size.y;
        ifs >> fullscreen;
        ifs >> framerate_limit;
        ifs >> vertical_sync_enabled;
        ifs >> antialiasing_level;
    }

    this->fullscreen = fullscreen;
    if (this->fullscreen)
        window_bounds = sf::VideoMode::getDesktopMode();

    this->windowSettings.antiAliasingLevel = antialiasing_level;
    if(this->fullscreen)
        this->window = new sf::RenderWindow(window_bounds, title, sf::State::Fullscreen, this->windowSettings);
    else
        this->window = new sf::RenderWindow(window_bounds, title, sf::State::Windowed, this->windowSettings);

    this->updateView(this->window->getSize());
    this->window->setFramerateLimit(framerate_limit);
    this->window->setVerticalSyncEnabled(vertical_sync_enabled);
}

void Game::updateView(const sf::Vector2u& windowSize){
    constexpr float viewWidth = 1280.f;
    constexpr float viewHeight = 720.f;
    if (windowSize.x == 0 || windowSize.y == 0)
        return;

    sf::FloatRect viewport({0.f, 0.f}, {1.f, 1.f});
    const float windowRatio =
        static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);
    const float viewRatio = viewWidth / viewHeight;

    if (windowRatio > viewRatio) {
        viewport.size.x = viewRatio / windowRatio;
        viewport.position.x = (1.f - viewport.size.x) / 2.f;
    } else {
        viewport.size.y = windowRatio / viewRatio;
        viewport.position.y = (1.f - viewport.size.y) / 2.f;
    }

    this->view.setSize(sf::Vector2f(viewWidth, viewHeight));
    this->view.setCenter(sf::Vector2f(viewWidth / 2.f, viewHeight / 2.f));
    this->view.setViewport(viewport);
    this->window->setView(this->view);
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
    while (const std::optional<sf::Event> event = this->window->pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            this->window->close();

        if (const sf::Event::Resized* resized = event->getIf<sf::Event::Resized>())
            this->updateView(resized->size);

        if (!this->states.empty())
            this->states.top()->handleEvent(*event);
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
    if (!this->window->isOpen())
        return;

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