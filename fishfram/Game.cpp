#include "Game.h"
#include <string>
//Private functions.

void Game::initVars()
{
	this->window = nullptr;
	this->http.setHost("http://localhost",8000);
	this->deltaTime = 0.f;
}
void Game::initWindow()
{
	this->videoMode.height = 768;
	this->videoMode.width = 1366;
	//get desktop mode for user res.
	this->window = new sf::RenderWindow(this->videoMode, "Fish Farm", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
	this->window->setFramerateLimit(60);
}
void Game::initFish()
{
	this->fishTexture.loadFromFile("assets/goldfish.png");
	Animation temp(&fishTexture, sf::Vector2u(16, 1), 0.2f);
	this->fishAnimation = temp;
	this->fishShape.setSize(sf::Vector2f(470.f, 280.f));
	this->fishShape.setPosition(100.f, 100.f);
	this->fishShape.setScale(sf::Vector2f(0.5f, 0.5f));
	this->fishShape.setTexture(&fishTexture);

}
void Game::initTank()
{
	sf::Http::Request request("/logi/reqtank/", sf::Http::Request::Get);
	sf::Http::Response response = http.sendRequest(request);
	std::string str = response.getBody();
	this->json = nlohmann::json::parse(str.substr(1, str.length() - 2));

	this->wallPaperTexture.loadFromFile("assets/"+ json["wallpaper"].get<std::string>());
	this->tankWallpaper.setTexture(wallPaperTexture);
	this->toolBarTexture.loadFromFile("assets/toolbartexture.png");
	this->toolBar.setTexture(toolBarTexture);
	this->toolBar.setPosition(sf::Vector2f(1224.f, 659.f));
	this->gravelTexture.loadFromFile("assets/"+json["gravel"].get<std::string>());
	this->gravel.setTexture(gravelTexture);
}


//Constructors
Game::Game()
{
	this->initVars();
	this->initWindow();
	this->initTank();
	this->initFish();
}
Game::~Game()
{
	delete this->window;
}

//Accsesors

const bool Game::running() const
{
	return this->window->isOpen();
}

//Functions.
void Game::update()
{
	this->pollEvents();

	this->deltaTime = this->clock.restart().asSeconds();
	this->fishAnimation.update(0,this->deltaTime);
	this->fishShape.setTextureRect(this->fishAnimation.uvRect);
}
void Game::render() 
{
	this->window->clear(sf::Color::Black);
	// Renders the game Objects
	this->window->draw(this->tankWallpaper);
	this->window->draw(this->gravel);
	this->window->draw(this->toolBar);
	this->window->draw(this->fishShape);

	//Draw Game
	this->window->display();
}

void Game::pollEvents()
{
	//Event polling event controlling
	while (this->window->pollEvent(this->event)) {
		switch (this->event.type)
		{
		case sf::Event::Closed:
			this->window->close();
			break;

		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Escape)
			{
				this->window->close();
			}
		}
	}
}

