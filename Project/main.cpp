#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

// Custom header file for Game Manager:
#include "GameManager.h"
// Custom header file for Peek Manager:
#include "PeekManager.h"

// Game Manager Object
GameManager gm;

// Custom header file for Score Manager:
#include "ScoreManager.h" // scoremanager requires gm object

// Current word being assigned
string CURR = gm.restart(0);

ScoreManager sm;

// Custom header file for Word Manager classes:
#include "WordManager.h" // wordmanager requires CURR string and sm object

PeekManager pm;

// help function that opens a new window that just displays the help.jpg file on screen
// function is called when the user presses F1
void help() {
	sf::RenderWindow window(sf::VideoMode(732, 740), "NERDLE HELP");
	window.setFramerateLimit(60);
	// set up texture
	sf::Texture texture;
	texture.loadFromFile("files/textures/help.jpg");
	sf::Sprite bg;
	bg.setTexture(texture);
    // run the program as long as the window is open
    while (window.isOpen()) {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event)) {
            // "close requested" or "any key pressed" event: we close the window
            if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed) {
            	window.close();
			}
			
			if (event.type == sf::Event::Resized) {
            	window.setSize(sf::Vector2u(732, 740));
			}
        }
        
        if(window.hasFocus() == false)
        	window.close();
        
        window.clear();
        window.draw(bg);
        window.display();
    }

    return;
}

int main() {
	// for the slightly blue background
	sf::RectangleShape bg(sf::Vector2f(SCREENW, SCREENH));
	bg.setFillColor(sf::Color(7, 41, 57));
	
	// Set up SFML window
    sf::RenderWindow window(sf::VideoMode(SCREENW, SCREENH), "NERDLE");
    window.setFramerateLimit(60);

	WordsBlock *block = new WordsBlock;

	char input[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
					'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch(event.type){
                case sf::Event::Closed:
                	// save score progress
                	sm.save_progress();
                    window.close();
                    break;
                // Change window size back to normal when it is changed
                case sf::Event::Resized:
                    window.setSize(sf::Vector2u(SCREENW, SCREENH));
                    break;
                // Check for keyboard input
                case sf::Event::KeyPressed:
                	// if the input is between A-Z;
                	for(int i = 0; i < 26; i++) {
                		// SFML has enum for 'A' to 'Z' as 0-26:
		            	if(event.key.code == i && gm.get_is_started() == true) {
		            		// add it to the block
		            		block->add(input[i]);
		            		break;
						}
					}
					// if the user presses backspace, remove one letter
					if(event.key.code == sf::Keyboard::BackSpace && gm.get_is_started() == true)
                        block->remove();
                    
                    // Open help window if user presses F1
                    if(event.key.code == sf::Keyboard::F1) {
                    	help();
					}
					
                    // Start the peek word process if user presses F2
                    if(event.key.code == sf::Keyboard::F2)
                    	pm.start(CURR);
                    
                    // Reset all values if user presses F3
                    if(event.key.code == sf::Keyboard::F3) {
                    	gm.loadDefault();
                    	sm.load_default();
					}
                    
                    // If the user presses the enter key and the game is not started (win or lose)
                    // then, Restart!
					if(event.key.code == sf::Keyboard::Return) {
						if(gm.get_is_started() == false) {
                    		gm.set_is_started(true);
                    		// clear all the blocks
                    		block->clear_block();
                    		// get the next word
                    		CURR = gm.restart( gm.get_correct() );
                    		gm.set_correct(-1);
                    		// save progress
                    		gm.saveTree();
                    		// restart the game timer
                    		sm.restart_timer();
						}
					}
                    break;
                default:
                    break;
            }
    	}
    	
    	// keep updating stats text
    	sm.update_disp_text();

        window.clear();
        // DRAW STARTS HERE
        // draw the blue background first
        window.draw(bg);
        
		for(int i = 0; i < 6; i++) {
			for(int j = 0; j < 5; j++) {
				// DRAWING EACH SQUARE ON THE SCREEN
				window.draw(block->get_text_bg(i, j));
			}
			// DRAWING EACH WORD ON THE SCREEN
			window.draw(block->get_text_block(i));
		}
		
		// calling game manager draw
		gm.draw(window);
		// calling peek manager draw
		pm.draw(window);
		// calling score manager draw
		sm.draw(window, CURR);
		
		block->draw_squares(window);
		// DRAW ENDS HERE
        window.display();
    }
	
	delete block;
    return 0;
}
