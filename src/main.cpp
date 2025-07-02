#include "Manager/store/store.h"
#include "Common/macros.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/Window.hpp>
#include <SFML/Window/WindowEnums.hpp>
int main()
{
    sf::RenderWindow window(
        sf::VideoMode(sf::Vector2u(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT)),
        DEFAULT_WINDOW_TITLE,
        sf::Style::Default);
    Store store = Store();
    store.Game(window);
    return 0;
}