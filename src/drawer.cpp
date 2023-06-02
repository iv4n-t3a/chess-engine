#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "attack.h"
#include "base.h"
#include "bitboard.h"
#include "position.h"

#include "config.h"

#include "drawer.h"


Drawer::Drawer(sf::RenderWindow& w, PosAdapter const& p, Config c) : window(w), pos(p), cfg(c) {
	sf::Vector2u size = w.getSize();
	square_size = std::min(size.x, size.y) / 8;
	for (int i = 0; i < SQ_COND_COUNT; i++) {
		if (i == EMPTY) continue;
		textures[i].loadFromFile(  cfg.get_texture_path( (SquareContent)i )  );
	}
	redraw();
}
void Drawer::redraw() {
	window.clear( to_sf_color(cfg.background_color) );

	for (Square s = 0; s < 64; s++) {
		SquareContent sc = pos.at(s);
		sf::RectangleShape shape(sf::Vector2f(square_size, square_size));

		if (sc != EMPTY)
			shape.setTexture( &textures[sc] );

		Bitboard WHITE_SQUARES = 0xaa55'aa55'aa55'aa55;
		if (getbit(bordered, s))
			shape.setFillColor( to_sf_color(cfg.inbordered_color) );
		else if (getbit(WHITE_SQUARES, s))
			shape.setFillColor( to_sf_color(cfg.square_color[WHITE]) );
		else
			shape.setFillColor( to_sf_color(cfg.square_color[BLACK]) );

		shape.setPosition(s%8 * square_size, s/8 * square_size);
		window.draw(shape);
	}

	window.display();
}
Square Drawer::pick_square() {
	sf::Event event;
	while (window.isOpen() and window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			window.close();
			exit(0);
		}
		if (event.type != sf::Event::MouseButtonPressed)
			continue;

		int x = event.mouseButton.x,
			y = event.mouseButton.y;

		if (x >= square_size*8 or y >= square_size*8)
			return NONE_SQUARE;

		Square sq = x/square_size + 8*(y/square_size);
		unborder_all();
		border(sq);
		redraw();
		return sq;
	}
	return NONE_SQUARE;
}

sf::Color Drawer::to_sf_color(Color c) {
	return sf::Color(c.r, c.g, c.b);
}
