#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>

class FarmGame {
public:
    FarmGame() {
        // Create the main window
        window.create(sf::VideoMode(800, 600), "Farm Game");

        // Load textures
        if (!grassTexture.loadFromFile("grass.jpg") || !cropTexture.loadFromFile("crop.png") || 
            !skyTexture.loadFromFile("sky.jpg") || !animalTexture.loadFromFile("animal.png") ||
            !marketIconTexture.loadFromFile("market.png")) {
            std::cerr << "Error loading textures!" << std::endl;
        }

        // Market button setup
        marketButton.setSize(sf::Vector2f(50, 50));
        marketButton.setPosition(750, 550);
        marketButton.setFillColor(sf::Color(255, 255, 255, 0)); // Fully transparent

        marketIcon.setTexture(marketIconTexture);
        marketIcon.setScale(0.085f, 0.085f); // Smaller scale to fit the button
        marketIcon.setPosition(755, 555); 

        // Sky setup
        sky.setTexture(skyTexture);
        sky.setPosition(0, 0);
        sky.setTextureRect(sf::IntRect(0, 0, 800, 210));

        // Grass tiles setup
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 6; ++j) {
                sf::Sprite grassSprite;
                grassSprite.setTexture(grassTexture);
                grassSprite.setPosition(i * 80, 210 + j * 80);
                grassSprites.push_back(grassSprite);
            }
        }

        // Crop setup
        float cropX = 250;
        float cropY = 240;
        for (int i = 0; i < 6; ++i) {
            for (int j = 0; j < 4; ++j) {
                sf::Sprite cropSprite;
                cropSprite.setTexture(cropTexture);
                cropSprite.setPosition(cropX + (i * 40), cropY + (j * 40));
                cropSprite.setScale(0.2f, 0.2f);
                cropSprites.push_back(cropSprite);
            }
        }

        // Animal setup
        srand(time(0));
        for (int i = 0; i < 2; ++i) { 
            sf::Sprite animalSprite;
            animalSprite.setTexture(animalTexture);
            animalSprite.setScale(0.25f, 0.25f);
            float x = rand() % 200 + 600;
            float y = rand() % 100 + 420;
            animalSprite.setPosition(x, y);
            animalSprites.push_back(animalSprite);
        }
        for (int i = 0; i < 3; ++i) { 
            sf::Sprite animalSprite;
            animalSprite.setTexture(animalTexture);
            animalSprite.setScale(0.25f, 0.25f);
            float x = rand() % 200;
            float y = rand() % 100 + 420;
            animalSprite.setPosition(x, y);
            animalSprites.push_back(animalSprite);
        }

        // Movable rectangle setup
        rectangle.setSize(sf::Vector2f(50, 50));
        rectangle.setFillColor(sf::Color::Red);
        rectangle.setPosition(375, 275);
    }

    void run() {
        while (window.isOpen()) {
            handleEvents();
            update();
            render();
        }
    }

private:
    sf::RenderWindow window;
    sf::RenderWindow marketWindow;
    sf::Texture grassTexture, cropTexture, skyTexture, animalTexture, marketIconTexture;
    sf::Sprite sky;
    sf::RectangleShape rectangle;
    std::vector<sf::Sprite> grassSprites, cropSprites, animalSprites;
    sf::RectangleShape marketButton;
    sf::Sprite marketIcon;
    bool isMarketOpen = false;

    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (marketButton.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
                    isMarketOpen = true;
                }
            }
        }

        // Open market window if button is pressed
        if (isMarketOpen) {
            openMarketWindow();
        }
    }

    void openMarketWindow() {
        int marketWidth = static_cast<int>(window.getSize().x * 0.75);
        int marketHeight = static_cast<int>(window.getSize().y * 0.75);
        int marketX = window.getPosition().x + (window.getSize().x - marketWidth) / 2;
        int marketY = window.getPosition().y + (window.getSize().y - marketHeight) / 2;

        sf::RenderWindow marketWindow(sf::VideoMode(marketWidth, marketHeight), "Market", sf::Style::Titlebar | sf::Style::Close);
        marketWindow.setPosition(sf::Vector2i(marketX, marketY));

        // Create buttons for market window
        std::vector<sf::RectangleShape> buttons(3);
        for (int i = 0; i < 3; ++i) {
            buttons[i].setSize(sf::Vector2f(100, 40));
            buttons[i].setFillColor(sf::Color::Blue);
            buttons[i].setPosition(marketWidth / 2 - 50, (marketHeight / 2 - 60) + i * 60);
        }

        // Display the market window until closed
        while (marketWindow.isOpen()) {
            sf::Event marketEvent;
            while (marketWindow.pollEvent(marketEvent)) {
                if (marketEvent.type == sf::Event::Closed) {
                    marketWindow.close();
                    isMarketOpen = false;
                }
                if (marketEvent.type == sf::Event::MouseButtonPressed) {
                    for (int i = 0; i < 3; ++i) {
                        if (buttons[i].getGlobalBounds().contains(sf::Mouse::getPosition(marketWindow).x, sf::Mouse::getPosition(marketWindow).y)) {
                            openSubWindow(i);  // Open new window based on button
                        }
                    }
                }
            }

            marketWindow.clear(sf::Color(245, 245, 220)); // Beige color
            for (auto& button : buttons) {
                marketWindow.draw(button);
            }
            marketWindow.display();
        }
    }

    void openSubWindow(int buttonIndex) {
        sf::RenderWindow subWindow(sf::VideoMode(400, 300), "Sub Window", sf::Style::Titlebar | sf::Style::Close);
        while (subWindow.isOpen()) {
            sf::Event subEvent;
            while (subWindow.pollEvent(subEvent)) {
                if (subEvent.type == sf::Event::Closed) {
                    subWindow.close();
                }
            }
            subWindow.clear(sf::Color::White);
            subWindow.display();
        }
    }

    void update() {
        // Player movement and animal logic remains as before
    }

    void render() {
        window.clear();
        window.draw(sky);
        for (auto& grass : grassSprites) {
            window.draw(grass);
        }
        for (auto& crop : cropSprites) {
            window.draw(crop);
        }
        for (auto& animal : animalSprites) {
            window.draw(animal);
        }
        window.draw(rectangle);
        window.draw(marketButton);
        window.draw(marketIcon);
        window.display();
    }
};

int main() {
    FarmGame farmGame;
    farmGame.run();
    return 0;
}
