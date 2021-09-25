#include <SFML/Graphics.hpp>
#include <thread>

const static size_t WIDTH = 720;
const static size_t HEIGHT = 480;

int main()
{
	int x = 0;
	int y = 0;

	int cellSize = 10;

	std::vector <sf::RectangleShape> snake;
	sf::Vector2f SPRITE_SIZE{ 10.f, 10.f };

	snake.emplace_back(SPRITE_SIZE);
	snake.emplace_back(SPRITE_SIZE);
	snake.emplace_back(SPRITE_SIZE);
	snake.emplace_back(SPRITE_SIZE);

	sf::RectangleShape food(SPRITE_SIZE);
	
	sf::VideoMode vm({ WIDTH, HEIGHT });
	sf::RenderWindow w(vm, "Snake game by @abay11");

	enum direction { left, right, up, down };

	direction cdir = right;

	float start_pos_x = WIDTH / 2;
	float start_pos_y = HEIGHT / 2;
	snake[0].setPosition(start_pos_x, start_pos_y);
	snake[1].setPosition(start_pos_x - 10, start_pos_y);
	snake[2].setPosition(start_pos_x - 20, start_pos_y);
	snake[3].setPosition(start_pos_x - 30, start_pos_y);

	bool foodEaten = true;
	bool foodColor = 0;

	size_t score = -1;
	sf::Font font;
	font.loadFromFile("C:/Windows/Fonts/Arial.ttf");
	sf::Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(14u);
	scoreText.setPosition(20.f, 20.f);
	scoreText.setFillColor(sf::Color::White);

	sf::Text message;
	message.setFont(font);
	message.setCharacterSize(44u);
	message.setFillColor(sf::Color::White);
	message.setString("Game over! Press Enter to try again");

	auto msgRect = message.getLocalBounds();
	message.setOrigin(msgRect.width / 2.f + msgRect.left, msgRect.height / 2.f + msgRect.top);
	message.setPosition(WIDTH / 2, HEIGHT / 2);

	bool gameover = false;

	sf::Clock clock;
	while (w.isOpen())
	{
		clock.restart();

		// process input
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			w.close();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
			gameover = false;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			cdir = left;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			cdir = right;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			cdir = up;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			cdir = down;

		// update
		auto headPos = snake[0].getPosition();

		switch (cdir)
		{
		case left:
			headPos -= sf::Vector2f(10, 0);
			break;
		case right:
			headPos += sf::Vector2f(10, 0);
			break;
		case up:
			headPos -= sf::Vector2f(0, 10);
			break;
		case down:
			headPos += sf::Vector2f(0, 10);
			break;
		default:
			break;
		}

		auto tmp = snake[0].getPosition();
		snake[0].setPosition(headPos);

		for (size_t i = 1; i < snake.size(); ++i)
		{
			auto prev = snake[i].getPosition();
			snake[i].setPosition(tmp);
			tmp = prev;
		}

		// check collision
		for (size_t i = 0; i < snake.size(); ++i)
		{
			if (gameover)
				break;

			for (size_t j = i + 1; j < snake.size(); ++j)
			{
				if (snake[i].getPosition() == snake[j].getPosition())
				{
					gameover = true;
					break;
				}
			}
		}

		// check if user out of borders
		auto pos = snake[0].getPosition();
		if (pos.x >= WIDTH)
		{
			pos.x = 10;
			snake[0].setPosition(pos);
			cdir = right;
		}
		else if (pos.x < 0)
		{
			pos.x = WIDTH - 10;
			snake[0].setPosition(pos);
			cdir = left;
		}
		else if (pos.y >= HEIGHT)
		{
			pos.y = 10;
			snake[0].setPosition(pos);
			cdir = down;
		}
		else if (pos.y < 0)
		{
			pos.y = HEIGHT - 10;
			snake[0].setPosition(pos);
			cdir = up;
		}

		if (foodEaten)
		{
			float x = std::rand() % (WIDTH / 10) * 10;
			float y = std::rand() % (HEIGHT / 10) * 10;
			food.setPosition(x, y);
			foodEaten = false;

			sf::RectangleShape tail(SPRITE_SIZE);
			tail.setPosition(tmp);
			snake.push_back(std::move(tail));
			
			++score;
		}

		foodEaten = snake[0].getPosition() == food.getPosition();
		foodColor = !foodColor;
		food.setFillColor(foodColor ? sf::Color::White : sf::Color::Black); // flickering

		scoreText.setString("Score: " + std::to_string(score));
		
		// render
		w.clear();

		if (gameover)
		{
			score = 0;
			snake.resize(4);
			float x = std::rand() % (WIDTH / 10) * 10;
			float y = std::rand() % (HEIGHT / 10) * 10;
			snake[0].setPosition(x, y);
			snake[1].setPosition(x + 10, y);
			snake[2].setPosition(x + 20, y);
			snake[3].setPosition(x + 30, y);
			cdir = left;
			w.draw(message);
		}
		else
		{
			for (auto s : snake)
			{
				w.draw(s);
			}
			w.draw(food);
			w.draw(scoreText);
		}
		
		w.display();

		auto dt = sf::milliseconds(33) - clock.restart();
		std::this_thread::sleep_for(std::chrono::milliseconds(dt.asMilliseconds()));
	}

	return 0;
}