#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/Window.hpp>
#include <SFML/Window/WindowEnums.hpp>
#include "Manager/resourceManager/animationManager.h"
#include "Manager/store/store.h"
int main()
{
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(1920, 1080)),"CdomRush",sf::Style::Default);
    Store store = Store();
    store.Game(window);
    return 0;
}