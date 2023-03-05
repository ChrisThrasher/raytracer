#include <SFML/Graphics.hpp>

#include <array>
#include <memory>

int main()
{
    constexpr auto length = 256ull;

    auto window = sf::RenderWindow(sf::VideoMode({ length, length }), "Raytracer");
    window.setFramerateLimit(30);

    // Heap allocate to accomodate systems with small (<1MB) stack sizes
    const auto pixels_allocation = std::make_unique<std::array<std::array<sf::Color, length>, length>>();
    auto& pixels = *pixels_allocation;

    for (size_t i = 0; i < length; ++i) {
        for (size_t j = 0; j < length; ++j) {
            const auto r = double(j) / (length - 1);
            const auto g = double(length - i) / (length - 1);
            const auto b = 0.25;

            const auto ir = uint8_t(255.999 * r);
            const auto ig = uint8_t(255.999 * g);
            const auto ib = uint8_t(255.999 * b);

            pixels[i][j] = { ir, ig, ib };
        }
    }

    auto image = sf::Image();
    image.create({ length, length }, reinterpret_cast<uint8_t*>(pixels.data()));
    auto texture = sf::Texture();
    if (!texture.loadFromImage(image))
        throw std::runtime_error("Failed to load texture");
    auto sprite = sf::Sprite(texture);

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
        window.draw(sprite);
        window.display();
    }
}
