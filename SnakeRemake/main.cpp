
#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <list>
#include <algorithm>

using namespace std;

const int BASE_SPEED = 480;//amount of counters triggered before move
//const sf::Vector2f ORIGINAL_DIRECTION(0, 30);

class Snake_Block : public sf::RectangleShape {
public:
	sf::Vector2f direction;
	sf::Vector2f PastPosition;
	Snake_Block(sf::Vector2f origin) : RectangleShape(origin) {}

};

void reverse(list<Snake_Block>* snake);
void intializeSnake(list<Snake_Block>* pSnake, int xStartPos);
bool intialize();



int main(int, char const**)
{
	srand(time(NULL));

	//GROWTH CUBE
	sf::RectangleShape holder(sf::Vector2f(20, 20));
	holder.setOrigin(10, 10);
	holder.setFillColor(sf::Color::White);
	holder.setPosition(400, 400);
	int cube_counter = 0;
	int speed = 1;
	
	//Head 1 and Head 2
	list<Snake_Block>::iterator head;
	list<Snake_Block>::iterator head2;


	// Create the main window
	sf::RenderWindow window(sf::VideoMode(800, 800), "SFML window");

	//DIRECTION AND COUNTER VARIABLES
	int counter = 0;
	sf::Vector2f direction(0, 30);
	sf::Vector2f direction2(0, 30);
	sf::Vector2f olddirection(0, 30);
	sf::Vector2f aheadposition(0, 0);

	//PAUSE TEXT
	sf::Font font;
	//font.loadFromFile("Deutsch.ttf");

	sf::Text text("Pause", font, 70);
	text.setStyle(sf::Text::Bold);
	text.setFillColor(sf::Color::White);
	text.setPosition(317, 310);

	//GAME OVER TEXT
	bool game_over = false;
	bool yellow_win = false;
	bool blue_win = false;

	sf::Text ywt("YELLOW WINS", font, 70);
	ywt.setStyle(sf::Text::Bold);
	ywt.setFillColor(sf::Color::Yellow);
	ywt.setPosition(135, 300);

	sf::Text bwt("BLUE WINS", font, 70);
	bwt.setStyle(sf::Text::Bold);
	bwt.setFillColor(sf::Color::Blue);
	bwt.setPosition(200, 300);

	//Score Text
	sf::Text yellow("YELLOW", font, 20);
	yellow.setFillColor(sf::Color::Yellow);
	yellow.setPosition(650, 0);

	sf::Text yellow_score("15", font, 20);
	yellow_score.setFillColor(sf::Color::Yellow);
	yellow_score.setPosition(685, 20);

	sf::Text blue("BLUE", font, 20);
	blue.setFillColor(sf::Color::Blue);
	blue.setPosition(50, 0);

	sf::Text blue_score("15", font, 20);
	blue_score.setFillColor(sf::Color::Blue);
	blue_score.setPosition(65, 20);

	//SOUND
	/*sf::SoundBuffer buffer;
	if (!buffer.loadFromFile("bop.wav"))
		return -1;
	sf::Sound bad;
	bad.setBuffer(buffer);*/
	//SNAKE CREATION
	list <Snake_Block> snake;
	list <Snake_Block> snake2;
	intializeSnake(&snake, 10);
	intializeSnake(&snake2, 790);
	int number = 10;

	bool pause = false;
	while (window.isOpen())
	{

		//calculate speed based on length
		int length = max(snake.size(), snake2.size());
		if (length < 20)
			speed = 1;
		if (length < 40 && length >= 20)
			speed = 2;
		if (length < 60 && length >= 40)
			speed = 3;
		if (length < 80 && length >= 60)
			speed = 4;
		if (length < 110 && length >= 80)
			speed = 5;

		//Speed of snakes
		if (!pause && !game_over) {
			counter++;
			if (speed >= 2)
				counter++;
			if (speed >= 3)
				counter++;
			if (speed >= 4)
				counter++;
			if (speed >= 5)
				counter++;
		}


		//RESET COLOR FOR SNAKE
		if (counter % 120 == 0) {
			for (list<Snake_Block>::iterator it = snake.begin(); it != snake.end(); ++it)
				it->setFillColor(sf::Color::Blue);
		}
		//RESET COLOR FOR SNAKE2
		if (counter % 120 == 0) {
			for (list<Snake_Block>::iterator it = snake2.begin(); it != snake2.end(); ++it)
				it->setFillColor(sf::Color::Yellow);
		}
		// Process events
		sf::Event event;
		while (window.pollEvent(event)) {
			// Close window: exit
			if (event.type == sf::Event::Closed)
				window.close();

			// KEY PRESSED IF
			if (event.type == sf::Event::KeyPressed) {
				// IF ESCAPE
				//    movCommand = true;
				switch (event.key.code) {
					//ESCAPE KEY PRESSED
				case sf::Keyboard::Escape:
					window.close();
					break;

					//MOVEMENT KEY PRESSED FOR SNAKE 1
				case sf::Keyboard::A:
					if (direction != sf::Vector2f(30, 0))
						direction = sf::Vector2f(-30, 0);
					break;
				case sf::Keyboard::W:
					if (direction != sf::Vector2f(0, 30))
						direction = sf::Vector2f(0, -30);
					break;
				case sf::Keyboard::D:
					if (direction != sf::Vector2f(-30, 0))
						direction = sf::Vector2f(30, 0);
					break;
				case sf::Keyboard::S:
					if (direction != sf::Vector2f(0, -30))
						direction = sf::Vector2f(0, 30);
					break;
					//MOVEMENT KEY PRESSED FOR SNAKE 2
				case sf::Keyboard::Left:
					if (direction2 != sf::Vector2f(30, 0))
						direction2 = sf::Vector2f(-30, 0);
					break;
				case sf::Keyboard::Up:
					if (direction2 != sf::Vector2f(0, 30))
						direction2 = sf::Vector2f(0, -30);
					break;
				case sf::Keyboard::Right:
					if (direction2 != sf::Vector2f(-30, 0))
						direction2 = sf::Vector2f(30, 0);
					break;
				case sf::Keyboard::Down:
					if (direction2 != sf::Vector2f(0, -30))
						direction2 = sf::Vector2f(0, 30);
					break;
				case sf::Keyboard::P:
					if (!pause)
						pause = true;
					else
						pause = false;
					break;
				case sf::Keyboard::Space:
					if (!pause)
						pause = true;
					else
						pause = false;
					break;
				}
			}
		}

		//MOVEMENT FOR SNAKE1	
		if (!pause && !game_over) {
			if (counter % BASE_SPEED == 0) {
				for (list<Snake_Block>::iterator it = snake.begin(); it != snake.end(); it++) {
					it->PastPosition = it->getPosition();
					if (it == snake.begin()) {//first block moves based on direction
						it->move(direction);
						//TELEPORTATION
						if (it->getPosition().y > 800)
							it->setPosition(it->getPosition().x, 10);
						if (it->getPosition().y < 0)
							it->setPosition(it->getPosition().x, 790);
						if (it->getPosition().x > 800)
							it->setPosition(10, it->getPosition().y);
						if (it->getPosition().x < 0)
							it->setPosition(790, it->getPosition().y);
					}
					else//successive blocks move based on previous position of block ahead of it
						it->setPosition(prev(it, 1)->PastPosition);
				}
			}

			//MOVEMENT FOR SNAKE2
			if (counter % BASE_SPEED == 0) {
				for (list<Snake_Block>::iterator it = snake2.begin(); it != snake2.end(); it++) {
					it->PastPosition = it->getPosition();
					if (it == snake2.begin()) {
						it->move(direction2);
						//TELEPORTATION
						if (it->getPosition().y > 800)
							it->setPosition(it->getPosition().x, 10);
						if (it->getPosition().y < 0)
							it->setPosition(it->getPosition().x, 790);
						if (it->getPosition().x > 800)
							it->setPosition(10, it->getPosition().y);
						if (it->getPosition().x < 0)
							it->setPosition(790, it->getPosition().y);

					}
					else
						it->setPosition(prev(it, 1)->PastPosition);
				}
			}       //  box.move(0, -10);
			//sf::Vector2f temp = snake[0].getPosition();
		//cout << "The position of the head is: " << temp.x << ", " << temp.y << endl; 
		}


		//CLASH DETECTION - is snake1 hitting snake2
		head = snake.begin();
		for (list<Snake_Block>::iterator it = snake2.begin(); it != snake2.end(); ++it) {
			if (it == snake2.begin()) {
				if (it->getPosition() == head->getPosition()) {
					reverse(&snake);
					reverse(&snake2);
					if (snake.size() == 1) {
						yellow_win = true;
						game_over = true;
					}
					if (snake2.size() == 1) {
						blue_win = true;
						game_over = true;
					}
					it->setFillColor(sf::Color::Red);
					head->setFillColor(sf::Color::Red);
					//bad.play();
				}
			}
			else {
				if (it->getPosition() == head->getPosition()) {
					reverse(&snake);
					if (snake.size() == 1) {
						yellow_win = true;
						game_over = true;
					}
					head->setFillColor(sf::Color::Red);
					//bad.play();
				}
			}
		}

		//CLASH DETECTION - is snake2 hitting snake1
		head2 = snake2.begin();
		for (list<Snake_Block>::iterator it = snake.begin(); it != snake.end(); ++it) {
			if (it == snake.begin()) {
				if (it->getPosition() == head2->getPosition()) {
					reverse(&snake);
					reverse(&snake2);
					if (snake.size() == 1) {
						yellow_win = true;
						game_over = true;
					}
					if (snake2.size() == 1) {
						blue_win = true;
						game_over = true;
					}
					it->setFillColor(sf::Color::Red);
					head2->setFillColor(sf::Color::Red);
					//bad.play();
				}
			}
			else {
				if (it->getPosition() == head2->getPosition()) {
					reverse(&snake2);
					if (snake2.size() == 1) {
						blue_win = true;
						game_over = true;
					}
					head2->setFillColor(sf::Color::Red);
					//bad.play();
				}
			}
		}

		//OVERLAP COLOR FOR SNAKE
		for (list<Snake_Block>::iterator it = snake.begin(); it != snake.end(); ++it) {
			for (list<Snake_Block>::iterator it2 = snake.begin(); it2 != snake.end(); ++it2) {
				if (it != it2 && it->getPosition() == it2->getPosition()) {
					if (it2->getFillColor() != sf::Color::Red)
						it2->setFillColor(sf::Color::Magenta);
					if (it->getFillColor() != sf::Color::Red)
						it->setFillColor(sf::Color::Magenta);
				}
			}
		}

		//OVERLAP COLOR FOR SNAKE2
		for (list<Snake_Block>::iterator it = snake2.begin(); it != snake2.end(); ++it) {
			for (list<Snake_Block>::iterator it2 = snake2.begin(); it2 != snake2.end(); ++it2) {
				if (it != it2 && it->getPosition() == it2->getPosition()) {
					if (it2->getFillColor() != sf::Color::Red)
						it2->setFillColor(sf::Color::Green);
					if (it->getFillColor() != sf::Color::Red)
						it->setFillColor(sf::Color::Green);
				}
			}
		}

		//GROWTH CUBE CONTACT
		if (head->getPosition() == holder.getPosition()) {
			Snake_Block box(sf::Vector2f(20, 20));
			box.setFillColor(sf::Color::Blue);
			box.setOrigin(10, 10);
			box.setPosition((snake.end()--)->PastPosition);
			box.direction = olddirection;
			snake.push_back(box);
			snake.push_back(box);
			snake.push_back(box);
			snake.push_back(box);
			snake.push_back(box);
			if (snake.size() >= 100) {
				blue_win = true;
				game_over = true;
			}
			holder.setPosition(900, 900);
		}

		if (head2->getPosition() == holder.getPosition()) {
			Snake_Block box(sf::Vector2f(20, 20));
			box.setFillColor(sf::Color::Yellow);
			box.setOrigin(10, 10);
			box.setPosition((snake2.end()--)->PastPosition);
			box.direction = olddirection;
			snake2.push_back(box);
			snake2.push_back(box);
			snake2.push_back(box);
			snake2.push_back(box);
			snake2.push_back(box);
			if (snake2.size() >= 100) {
				yellow_win = true;
				game_over = true;
			}
			holder.setPosition(900, 900);
		}

		//GROWTH CUBE
		if (counter % 2000 == 0 && holder.getPosition() == sf::Vector2f(900, 900)) {
			bool overlap = true;
			while (overlap) {
				overlap = false;
				holder.setPosition(sf::Vector2f(((rand() % 27) * 30) + 10, ((rand() % 27) * 30) + 10));
				holder.setFillColor(sf::Color::White);
				for (list<Snake_Block>::iterator it = snake2.begin(); it != snake2.end(); ++it) {
					if (holder.getPosition() == it->getPosition())
						overlap = true;
				}
				for (list<Snake_Block>::iterator it = snake.begin(); it != snake.end(); ++it) {
					if (holder.getPosition() == it->getPosition())
						overlap = true;
				}
			}
			cube_counter++;
		}

		//SCORE UPDATE
		yellow_score.setString(to_string(snake2.size()));
		blue_score.setString(to_string(snake.size()));
		// Clear screen
		window.clear();

		//Draw
		for (std::list<Snake_Block>::iterator it = snake.begin(); it != snake.end(); ++it) {
			window.draw(*it);
		}
		for (std::list<Snake_Block>::iterator it = snake2.begin(); it != snake2.end(); ++it) {
			window.draw(*it);
		}
		if (pause)
			window.draw(text);
		if (yellow_win)
			window.draw(ywt);
		if (blue_win)
			window.draw(bwt);
		window.draw(holder);
		window.draw(yellow);
		window.draw(blue);
		window.draw(yellow_score);
		window.draw(blue_score);



		// Update the window
		window.display();

	}
	return EXIT_SUCCESS;

}

bool intialize() {

	return true;
}


void intializeSnake(list<Snake_Block>* pSnake, int xStartPos) {
	int number = 10;
	sf::Vector2f direction(0, 30);

	for (int x = 0; x < number; x++) {
		Snake_Block box(sf::Vector2f(20, 20));
		box.setFillColor(sf::Color::Blue);
		box.setOrigin(10, 10);
		box.setPosition(xStartPos, (100 - (30 * x)));
		box.direction = direction;
		pSnake->push_back(box);
	}

	return;
}

void reverse(list<Snake_Block>* snake) {
	for (list<Snake_Block>::iterator it = snake->begin(); it != (snake->end()--); ++it)
		it->setPosition(next(it, 1)->getPosition());
	snake->pop_back();
	return;
}


