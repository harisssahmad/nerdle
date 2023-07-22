/*
WordManager.h file
Contains:	DisplayWord class
			DisplayBlock class
			WordsBlock class
*/

#include <SFML/Graphics.hpp>

// Prototype for WordsBlock (so that it can be added as friend)
class WordsBlock;

class DisplayWord {
	string word;
	sf::Text text;
	sf::Font font;
	public:
		friend class WordsBlock;
		DisplayWord() {
			font.loadFromFile("fonts/FUTRFW.ttf");
			text.setFont(font);
			word = "\0";
			text.setString(word);
			text.setCharacterSize(65);
			text.setFillColor(sf::Color::White);
		}
		void clear_text() {
			word = "\0";
			text.setString(word);
		}
};

// class for the background color-filled block of each letter
class DisplayBlock {
	sf::RectangleShape *bg;
	sf::Color *color;
	public:
		friend class WordsBlock;
		DisplayBlock() {
			bg = new sf::RectangleShape[5];
			color = new sf::Color[5];
			for(int i = 0; i < 5; i++) {
				color[i] = sf::Color(99,102,106);
				bg[i].setFillColor(color[i]);
				bg[i].setSize(sf::Vector2f(60.f, 60.f));
				bg[i].setOutlineThickness(5.f);
				bg[i].setOutlineColor(sf::Color::White);
			}
		}
		~DisplayBlock() {
			delete [] bg;
			delete [] color;
		}
		void set_color(int index, sf::Color c) {
			color[index] = c;
			bg[index].setFillColor(color[index]);
		}
		void clear_bg() {
			for(int i = 0; i < 5; i++) {
				color[i] = sf::Color(99,102,106);
				bg[i].setFillColor(color[i]);
			}
		}
};

class WordsBlock {
	DisplayWord *word;
	DisplayBlock *background;
	int index;
	public:
		WordsBlock() {
			index = 0;
			// initializing words and background tiles
			word = new DisplayWord[6];
			background = new DisplayBlock[6];
			// setting positions for words and background tiles
			for(int i = 0; i < 6; i++) {
				word[i].text.setPosition(((SCREENW / 2) - (82 * 2)), ((SCREENH / 2) + 200 - (85 * (5 - i))));
				for(int j = 0; j < 5; j++) {
					background[i].bg[j].setPosition(((SCREENW / 2) - (82 * 2) + 3 + (j * 65)), ((SCREENH / 2) + 210 - (85 * (5 - i))));
				}
			}
		}
		~WordsBlock() {
			delete [] word;
			delete [] background;
		}
		
		bool check_word(int index, string word) {
			int count = 0;
			// first, check if the letter is in the CURR string
			for(int i = 0; i < 5; i++) {
				for(int j = 0; j < 5; j++) {
					if (word[i] == CURR[j]) {
						// change the color to yellow
						change_bg_color(index, i, sf::Color(213,195,6));
					}
				}
			}
			// second, check if the letter is in the correct POSITION with respect to the CURR string
			for(int i = 0; i < 5; i++) {
				if (word[i] == CURR[i]) {
					// change the color to green
					change_bg_color(index, i, sf::Color(99,162,95));
					count++;
				}
			}
			// third, check if the letter is correctly entered
			if(count == 5) {
				// stop the input process
				gm.set_is_started(false);
				// display success message
				gm.set_correct(1);
				// Tell score manager user won
				sm.gameResult(true);
				return true;
			} else {
				return false;
			}
		}
		// function that adds a valid character to the end of current string
		// or the beginning of next string
		void add(char letter) {
			if(word[index].word.length() < 5) {
				word[index].word.push_back(letter);
				word[index].text.setString(word[index].word);
				// if word is completely filled
				if(word[index].word.length() == 5) {
					// check if the word is correct (color code the background)
					bool correct = check_word(index, word[index].word);
					// check if all 6 attempts have been made
					if(index == 5 && correct == false) {
						gm.set_is_started(false);
						// Tell Score Manager user lost
						sm.gameResult(false);
						gm.set_correct(0);
					}
				}
			} else if(index < 5) {
				index++;
				word[index].word.push_back(letter);
				word[index].text.setString(word[index].word);
			}	
		}
		// function that removes the last letter from the current word when backspace is pressed
		void remove() {
			if(word[index].word.length() > 0 && word[index].word.length() < 5) {
        		word[index].word.pop_back();
            	word[index].text.setString(word[index].word);
			}
		}
		// function that clears everything
		void clear_block() {
			index = 0;
			for(int i = 0; i < 6; i++) {
				word[i].clear_text();
				background[i].clear_bg();
			}
			gm.set_is_started(true);
		}
		
		// function that changes background color of blocks
		void change_bg_color(int i, int j, sf::Color c) {
			background[i].color[j] = c;
			background[i].bg[j].setFillColor(c);
		}
		
		// function that draws game progress squares AFTER a game session
		void draw_squares(sf::RenderWindow &w) {
			if(gm.get_is_started() == false) {
				sf::RectangleShape square[6][5];
				int X_OFFSET = 5;
				int Y_OFFSET = 5;
				for(int i = 0; i < 6; i++) {
					for(int j = 0; j < 5; j++) {
						square[i][j].setSize(sf::Vector2f(25, 25));
						square[i][j].setOutlineThickness(4.75f);
						square[i][j].setOutlineColor(sf::Color(255, 255, 255));
						square[i][j].setFillColor( background[i].color[j] );
						square[i][j].setPosition((X_OFFSET + (j * 30)), (Y_OFFSET + (i * 30)));
						square[i][j].move(sf::Vector2f(225,500));
						w.draw(square[i][j]);
					}
				}
			}
		}
		
		// function that returns the text row for drawing in MAIN
		sf::Text get_text_block(int i) {
			return word[i].text;
		}
		
		// function that returns one block of background for drawing in MAIN
		sf::RectangleShape get_text_bg(int i, int j) {
			return background[i].bg[j];
		}
};
