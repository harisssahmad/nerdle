#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>
// ScoreManager class that handles the user scores

class ScoreManager {
	int wins; // number of wins
	int losses; // number of losses
	float winPerc; // win percentage
	sf::Clock time; // to keep track of time
	sf::Time totalTime; // to keep track of total time
	sf::Time winTime; // to keep track of win time
	sf::Time avgWinTime; // calculate average win time
	sf::Time avgTime; // calculate average time
	sf::Time gameTime;
	// FOR DISPLAY
	sf::Text curr_text; // if the player loses, show the correct word
	sf::Text disp_text; // stats text (shows wins, percentage etc)
	sf::Text disp_heading; // prompt for win/lose
	sf::Text disp_restart_text; // press enter to restart text
	sf::Font font; // font for all the texts
	sf::Font timer_font; // font for timer text
	sf::Text timer_text; // timer text
	sf::RectangleShape background; // transparent background for stats
	sf::RectangleShape timer_background; // black background for timer
	public:
		ScoreManager() {
			font.loadFromFile("fonts/Helvetica-Bold.ttf");
			disp_heading.setFont(font);
			disp_heading.setString("YOU WON!");
			disp_heading.setCharacterSize(55);
			disp_heading.setFillColor(sf::Color::Green);
			disp_heading.setPosition(100, 50);
			disp_text.setFont(font);
			disp_text.setString("HELLO WORKING");
			disp_text.setCharacterSize(25);
			disp_text.setFillColor(sf::Color::White);
			disp_text.setPosition(100, 150);
			
			disp_restart_text.setFont(font);
			disp_restart_text.setString("Press ENTER to restart");
			disp_restart_text.setCharacterSize(25);
			disp_restart_text.setFillColor(sf::Color::Yellow);
			disp_restart_text.setPosition(155, 450);
			
			curr_text.setFont(font);
			curr_text.setString("\n");
			curr_text.setCharacterSize(25);
			curr_text.setFillColor(sf::Color::Yellow);
			curr_text.setPosition(170, SCREENH - 100);
			
			timer_font.loadFromFile("fonts/Helvetica-Bold.ttf");
			timer_text.setFont(timer_font);
			timer_text.setString(to_string(int(time.getElapsedTime().asSeconds())));
			timer_text.setCharacterSize(25);
			timer_text.setFillColor(sf::Color::White);
			timer_text.setPosition(SCREENW / 2 - 60, SCREENH - 100);
			timer_background.setPosition(0, SCREENH - 110);
			timer_background.setSize(sf::Vector2f(SCREENW, 50.f));
			timer_background.setFillColor(sf::Color(0, 0, 0));
			
			// load stats from file
			if( load_progress() == false ) {
				// if file does not exist (user is playing for the first time)
				// Setting all time values as ZERO
				wins = losses = 0;
				winPerc = 0;
				totalTime = sf::seconds(0.0f);
				avgWinTime = sf::seconds(0.0f);
				avgTime = sf::seconds(0.0f);
				gameTime = sf::seconds(0.0f);
			}
			
			// Display initializations:
			background.setSize(sf::Vector2f(SCREENW, SCREENH));
			background.setPosition(0, 0);
			background.setFillColor(sf::Color(0, 0, 0, 200));
		}
		
		// draw function
		// draws all the text at appropriate situations
		// curr is the current word that was in play
		
		void draw(sf::RenderWindow &w, string curr) {
			// setting timer to the elapsed time (as seconds only, that's why int casting)
			timer_text.setString("TIMER: " + to_string(int(time.getElapsedTime().asSeconds())));
			
			// if game is not started (user has either won or lost)
			if(gm.get_is_started() == false) {
				
				// Default: we assume player has won
				disp_heading.setString("YOU WON!");
				disp_heading.setFillColor(sf::Color::Green);
				
				// Draw the background
				w.draw(background);
				
				// If player has lost
				if(gm.get_correct() == 0) {
					// show prompt of loss
					disp_heading.setString("YOU LOST!");
					disp_heading.setFillColor(sf::Color::Red);
					curr_text.setString("Correct word: " + curr);
					// also show the correct word
					w.draw(curr_text);
				}
				
				// draw the heading, stats and restart texts
				w.draw(disp_heading);
				w.draw(disp_text);
				w.draw(disp_restart_text);
			}
			// if the game is still in play, display timer background, then timer text
			if(gm.get_is_started() == true) {
				w.draw(timer_background);
				w.draw(timer_text);
			}
		}
		
		// function that gets the game result and adjusts the stats accordingly
		void gameResult(bool win) {
			// get the game time
			gameTime = time.restart();
			// add it to total time
			totalTime += gameTime;
			// if player won
			if(win) {
				wins++; // increase wins
				winTime += gameTime; // increase win time
				avgWinTime = sf::seconds(float(winTime.asSeconds()) / float(wins)); // calculate new average win time
			}
			// if player lost
			else {
				losses++; // increase losses
			}
			// in either case: calculate average time, win percentage and restart the game timer
			avgTime = sf::seconds(float(totalTime.asSeconds() / (float(wins) + float(losses))));
			
			winPerc = float(wins) / (float(wins) + float(losses));
			time.restart();
		}
		
		// function that can be used in main to restart the timer
		void restart_timer() {
			time.restart();
		}
		
		// function that updates the display text
		void update_disp_text() {
			string text =	"TIME TAKEN = " + to_string(gameTime.asSeconds()) + "\n\n" +
							"WINS = " + to_string(wins) + "\n\n" +
							"LOSSES = " + to_string(losses) + "\n\n" +
							"AVERAGE TIME = " + to_string(avgTime.asSeconds()) + "\n\n" +
							"AVERAGE WIN TIME = " + to_string(avgWinTime.asSeconds()) + "\n\n" +
							"WIN PERCENTAGE = " + to_string(int(winPerc * 100)) + "%\n\n";
			disp_text.setString(text);
		}
		
		// function that writes the stats to file
		void save_progress() {
			ofstream write("files/in_game/game_data.txt");
			
			if(write.is_open()) {
				write << totalTime.asSeconds() << endl;
				write << winTime.asSeconds() << endl;
				write << avgWinTime.asSeconds() << endl;
				write << avgTime.asSeconds() << endl;
				write << gameTime.asSeconds() << endl;
				write << wins << endl;
				write << losses << endl;
				write << winPerc << endl;
			}
			
			write.close();
		}
		
		// function that saves zeroes to the game data file, so that stats are cleared
		void load_default() {
			ofstream write("files/in_game/game_data.txt");
			
			if(write.is_open()) {
				for(int i = 0; i < 8; i++)
					write << 0 << endl;
			}
			
			write.close();
			
			load_progress();
		}
		
		// function that if a file exists, loads all the game data and returns true
		// otherwise returns false
		bool load_progress() {
			ifstream read("files/in_game/game_data.txt");
			string text;
			
			if(read.is_open()) {
				getline(read, text);
				totalTime = sf::seconds( stof(text) );
				getline(read, text);
				winTime = sf::seconds( stof(text) );
				getline(read, text);
				avgWinTime = sf::seconds( stof(text) );
				getline(read, text);
				avgTime = sf::seconds( stof(text) );
				getline(read, text);
				gameTime = sf::seconds( stof(text) );
				getline(read, text);
				wins = stoi(text);
				getline(read, text);
				losses = stoi(text);
				getline(read, text);
				winPerc = stof(text);
				
				read.close();
				return true;
			} else {
				return false;
			}
		}
};
