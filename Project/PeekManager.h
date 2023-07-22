#include <SFML/Graphics.hpp>

// PeekManager class that handles the word displayed when user wants to peek at the correct word

class PeekManager {
	string curr_word;
	sf::Clock e_time; // elapsed time
	sf::Time p_time; // peek time
	sf::Text p_text;
	sf::Font font;
	bool peek;
	public:
		PeekManager() {
			peek = false;
			curr_word = "\n";
			// You will be able to peek at the word for 0.5 seconds!
			p_time = sf::seconds(0.5f);
			font.loadFromFile("fonts/FUTRFW.ttf");
			p_text.setFont(font);
			p_text.setString("NERDL"); // placeholder word for debugging, gets changed later
			p_text.setCharacterSize(25);
			p_text.setFillColor(sf::Color::White);
			p_text.setPosition(SCREENW / 2 - (25.0 * 2.5), 140);
		}
		
		// starts the peeking process that goes on for p_time seconds
		void start(string current) {
			e_time.restart();
			peek = true;
			curr_word = current;
			p_text.setString(curr_word);
		}
		
		// checks the clock time against p_time seconds
		bool isOver() {
			return e_time.getElapsedTime().asSeconds() > p_time.asSeconds();
		}
		
		// draw function that draws the peek word onto the screen
		void draw(sf::RenderWindow &w) {
			if(peek) {
				if(isOver() == false) {
					// draw the word
					w.draw(p_text);
				} else if(isOver() == true) {
					peek = false;
				}
			}
		}
};
