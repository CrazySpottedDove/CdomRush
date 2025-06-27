#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include "gui/spriteManager.h"
int main()
{
    // sf::RenderWindow window(sf::VideoMode(sf::Vector2u(1600, 1200)), "SFML Window");
    // sf::Texture      texture;
    // //    纹理：图片资源
    // if (!texture.loadFromFile("assets/images/screen_map-1.png")) {
    //     return -1;
    // }

    // //    精灵：纹理的实例化
    // sf::Sprite sprite(texture);

    // while (window.isOpen()) {
    //     while (const std::optional event = window.pollEvent()) {   // process event...
    //         if (event->is<sf::Event::Closed>()) {
    //             window.close();
    //         }
    //         window.clear();
    //         window.draw(sprite);
    //         window.display();
    //     }
    // }
    SpriteManager sprite_manager;
    sprite_manager.LoadResources("assets/images/enemies/go_enemies_acaroth.lua");
    sprite_manager.UnloadSpecificResources();
    return 0;
}