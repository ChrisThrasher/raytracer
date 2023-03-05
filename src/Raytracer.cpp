#include <SFML/Graphics.hpp>

#include <memory>
#include <array>

int main()
{
    constexpr auto length = size_t(600);

    // Heap allocate to accomodate systems with small (<1MB) stack sizes
    const auto pixels_allocation = std::make_unique<std::array<std::array<sf::Color, length>, length>>();
    auto& pixels = *pixels_allocation;

    auto window = sf::RenderWindow(sf::VideoMode({ length, length }), "Raytracer");
    window.setFramerateLimit(30);

    while (window.isOpen()) {
        for (auto event = sf::Event(); window.pollEvent(event);) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            default:
                break;
            }
        }

        window.clear();
        window.display();
    }
}
