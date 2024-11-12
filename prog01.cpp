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
        if (!farmTexture.loadFromFile("farm.jpg") || 
            !houseTexture.loadFromFile("farmhouse.png") ||
            !animalTexture.loadFromFile("animal.png") ||
            !marketIconTexture.loadFromFile("market.png") ||
            !rectangleText.loadFromFile("farmer.png"))
            std::cerr << "Error loading textures!" << std::endl;

        

        // Market button setup
        marketButton.setSize(sf::Vector2f(50, 50));
        marketButton.setPosition(750, 550);
        marketButton.setFillColor(sf::Color(255, 255, 255, 0)); // Fully transparent

        marketIcon.setTexture(marketIconTexture);
        marketIcon.setScale(0.085f, 0.085f); // Smaller scale to fit the button
        marketIcon.setPosition(755, 555); 
        //farmer
        rectangle.setTexture(rectangleText);
        rectangle.setScale(0.35f, 0.35f); // Smaller scale to fit the button
        rectangle.setPosition(400, 300);
         // Farm background setup (fill the entire screen)
        farmbackground.setTexture(farmTexture);  // Apply the farm texture to the sprite
        farmbackground.setPosition(0, 0); // Position the farm image at the top-left corner
        farmbackground.setTextureRect(sf::IntRect(0, 0, 800, 600)); // Ensure it covers the whole screen
        
        houseSprite.setTexture(houseTexture);

        // Scale the house to make sure it fits within the screen
        // For example, scale to 50% of its original size (you can adjust the scale factor as needed)
        float scaleFactorX = 0.4f;
        float scaleFactorY = 0.4f;
        houseSprite.setScale(scaleFactorX, scaleFactorY);

        // Set position to (500, 100)
        houseSprite.setPosition(500, 100);

        // Move the house to the left and upwards
        float offsetX = -50;  // Move 50 units to the left
        float offsetY = -30;  // Move 30 units upwards

        // Apply the offset to the house's position
        houseSprite.setPosition(houseSprite.getPosition().x + offsetX, houseSprite.getPosition().y + offsetY);

        // Ensure the house does not exceed the right boundary (800px width)
        sf::FloatRect houseBounds = houseSprite.getGlobalBounds();
        if (houseBounds.left + houseBounds.width > window.getSize().x) {
            houseSprite.setPosition(window.getSize().x - houseBounds.width, houseSprite.getPosition().y);
        }


        // // Sky setup
        // sky.setTexture(skyTexture);
        // sky.setPosition(0, 0);
        // sky.setTextureRect(sf::IntRect(0, 0, 800, 210));

        //set farm backround
        farmbackground.setTexture(farmTexture);  // Apply the farm texture to the sprite
        farmbackground.setPosition(0, 0); // Position the farm image at the top-left corner

    

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
        // rectangle.setSize(sf::Vector2f(50, 50));
        // rectangle.setFillColor(sf::Color::Red);
        // rectangle.setPosition(375, 275);
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
    sf::Texture farmTexture, animalTexture, marketIconTexture, houseTexture, rectangleText;
    // sf::Texture skyTexture;
    // sf::Sprite sky;
    sf::Sprite houseSprite;
    sf::Sprite farmbackground;
    sf::Sprite rectangle;
    //sf::RectangleShape rectangle;
    std::vector<sf::Sprite> grassSprites, animalSprites;
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
    // Player movement and animal movement logic
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        rectangle.move(-0.15f, 0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        rectangle.move(0.15f, 0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        rectangle.move(0, -0.15f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        rectangle.move(0, 0.15f);
    }
    
    // Left boundary
    if (rectangle.getPosition().x < 0) {
        rectangle.setPosition(1, rectangle.getPosition().y);  // Set slightly inside the screen
        rectangle.move(2, 0);  // Vibrate to the right
    }

    // Right boundary (Fixed using getGlobalBounds().width)
    if (rectangle.getPosition().x + rectangle.getGlobalBounds().width > 800) {
        rectangle.setPosition(798 - rectangle.getGlobalBounds().width, rectangle.getPosition().y);  // Set slightly inside the screen
        rectangle.move(-2, 0);  // Vibrate to the left
    }

    // Top boundary
    if (rectangle.getPosition().y < 0) {
        rectangle.setPosition(rectangle.getPosition().x, 1);  // Set slightly inside the screen
        rectangle.move(0, 2);  // Vibrate downwards
    }

    // Bottom boundary (Fixed using getGlobalBounds().height)
    if (rectangle.getPosition().y + rectangle.getGlobalBounds().height > 600) {
        rectangle.setPosition(rectangle.getPosition().x, 598 - rectangle.getGlobalBounds().height);  // Set slightly inside the screen
        rectangle.move(0, -2);  // Vibrate upwards
    }

    // Scale the farm background to fit the entire window (800x600)
    farmbackground.setScale(
        window.getSize().x / farmbackground.getLocalBounds().width, 
        window.getSize().y / farmbackground.getLocalBounds().height
    );

    // Animal movement logic (same as before)
    for (auto& animal : animalSprites) {
        int direction = rand() % 10; // Randomly pick a direction
        float moveSpeed = 0.15f; // Movement speed for animals

        // Move animal in the chosen direction while respecting window boundaries
        switch (direction) {
            case 0: // Move up
                if (animal.getPosition().y > 210) {
                    animal.move(0, -moveSpeed);
                }
                break;
            case 1: // Move down
                if (animal.getPosition().y + animal.getGlobalBounds().height < window.getSize().y) {
                    animal.move(0, moveSpeed);
                }
                break;
            case 2: // Move left
                if (animal.getPosition().x > 0) {
                    animal.move(-moveSpeed, 0);
                }
                break;
            case 3: // Move right
                if (animal.getPosition().x + animal.getGlobalBounds().width < window.getSize().x) {
                    animal.move(moveSpeed, 0);
                }
                break;
        }
    }
}

    void render() {
        window.clear();
        // window.draw(sky);
        window.draw(farmbackground); 
        window.draw(houseSprite);

      
        for (auto& animal : animalSprites) {
            window.draw(animal);
        }
        window.draw(marketButton);
        window.draw(marketIcon);
        window.draw(rectangle);


        window.display();
    }
};

int main() {
    FarmGame farmGame;
    farmGame.run();
    return 0;
}
