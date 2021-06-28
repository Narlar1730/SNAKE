#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <X11/Xlib.h>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <stdlib.h>
#include <tuple>
#include <string>
#include <sstream>
#include <time.h>
//#include "board.h"


using namespace std;

bool isApple = false;
int AppleX = 0;
int AppleY = 0;

sf::Font font;

int Score = 0;

bool MouseDown = false;

char direction = 'd';
vector<int> snakex = { 15, 16, 17};
vector<int> snakey = { 15, 15, 15};
int snakeHeadx = 18;
int snakeHeady = 15;

void performKeyPress()
{

	// Handle Key Press
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
	{
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::C))
		{
			cout << "User Quit Program\n";
			exit(0);
		}
	}
	else
	{
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			direction = 'w';
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			direction = 'a';
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			direction = 's';
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			direction = 'd';
		}
		
		//cout << "User Pressed Key: " << CurrentSelection << "\n";
		// Pass
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		cout << "User Quit Program\n";
		exit(0);
	}

}

bool updateGame()
{
	bool outVal = true;

	int curHeadX = snakeHeadx;
	int curHeadY = snakeHeady;
	
	int len = snakex.size();
	tuple<int, int> HeadPos (curHeadX, curHeadY);
	if(curHeadX > 27 || curHeadY > 27 || curHeadX < 0 || curHeadY < 0)
	{
		outVal = false;
		return outVal;
	}
	for(int i = 0; i < len; i++)
	{
		int snaX = snakex[i];
		int snaY = snakey[i];
		tuple<int, int> Pos (snaX, snaY);
		if(HeadPos == Pos)
		{
			cout << "GAMEOVER\n";
			outVal = false;
		}
	}

	snakex.push_back(curHeadX);
	snakey.push_back(curHeadY);
		
	if(isApple)
	{
		tuple<int, int> snakeTuple (curHeadX, curHeadY);
		tuple<int, int> appleTup (AppleX, AppleY);
		if(appleTup == snakeTuple)
		{
			snakex.push_back(curHeadX);
			snakey.push_back(curHeadY);
			isApple = false;
			Score += 100;
			cout << "Score: " << Score << "\n";
		}
	}	

	while(!isApple)
	{
		int xapp = rand() % 28;
		int yapp = rand() % 28;
	
		int len = snakex.size();
		vector<tuple<int, int>> AllPos;
		tuple<int, int> ApplePos (xapp, yapp);
		for(int i = 0; i < len; i++)
		{
			int snaX = snakex[i];
			int snaY = snakey[i];
			tuple<int, int> P1 (snaX, snaY);
			AllPos.push_back(P1);
		}	
		
		if(find(AllPos.begin(), AllPos.end(), ApplePos) != AllPos.end())
		{
			cout << "Apple Couldn't spawn at x: " << xapp << " Y pos: " << yapp << "\n";
		}
		else
		{
			AppleX = xapp;
			AppleY = yapp;
			isApple = true;		
		}
		

	}


	snakex.erase(snakex.begin());
	snakey.erase(snakey.begin());
	switch(direction)
	{
		case 'w' :
			snakeHeady -= 1;
			break;
		case 'a' :
			snakeHeadx -= 1;
			break;
		case 's' :
			snakeHeady += 1;
			break;
		case 'd' :
			snakeHeadx += 1;
			break;
		default :
			cout << "ERR, undefined user input: " << direction << "\n";
			break;
	}

	return outVal;

}

void drawBoard(sf::RenderWindow* window)
{
	//Draw Board
	for (float i = 201; i<1600; i+=50)
	{
		for (float j = 201; j<1600; j+=50)
		{
			sf::RectangleShape rectangle(sf::Vector2f(48.f, 48.f));
			rectangle.setPosition(i, j);
				
			rectangle.setFillColor(sf::Color(170, 170, 170));
			
			window->draw(rectangle);
		}
	}

	//Draw Apple
	sf::CircleShape apple;
	apple.setRadius(22);
	apple.setOutlineColor(sf::Color::Black);
	apple.setFillColor(sf::Color::Red);
	apple.setPosition(AppleX*50 + 202, AppleY*50 + 202);
	window ->draw(apple);

	//Draw Snake
	int Pieces = snakex.size();
	for (int i = 0; i < Pieces; i++){
		int curX = snakex[i];
		int curY = snakey[i];
		sf::RectangleShape rectangle(sf::Vector2f(42.f, 42.f));
		rectangle.setPosition(curX*50+204, curY*50+204);
		rectangle.setFillColor(sf::Color(100, 180, 100));
		window->draw(rectangle);
		int prevX, prevY, nextX, nextY;
		//Get prev and next piece
		if(i == 0)
		{
			prevX = 0;
			prevY = 0;
			nextX = snakex[i+1];
			nextY = snakey[i+1];
		}
		else if(i == Pieces - 1)
		{
			prevX = snakex[i-1];
			prevY = snakey[i-1];
			nextX = snakeHeadx;
			nextY = snakeHeady;
		}
		else
		{
			prevX = snakex[i-1];
			prevY = snakey[i-1];
			nextX = snakex[i+1];
			nextY = snakey[i+1];
		}

		//draw straights
		if(prevX == nextX)
		{
			sf::RectangleShape wall1(sf::Vector2f(4.f, 42.f));
			wall1.setPosition(curX*50+201, curY*50+201);
			wall1.setFillColor(sf::Color(100, 220, 220));
			window->draw(wall1);
			sf::RectangleShape wall2(sf::Vector2f(4.f, 42.f));
			wall2.setPosition(curX*50+246, curY*50+201);
			wall2.setFillColor(sf::Color(100, 220, 220));
			window->draw(wall2);

		}
		else if(prevY == nextY)
		{
			sf::RectangleShape wall1(sf::Vector2f(42.f, 4.f));
			wall1.setPosition(curX*50+201, curY*50+201);
			wall1.setFillColor(sf::Color(100, 220, 220));
			window->draw(wall1);
			sf::RectangleShape wall2(sf::Vector2f(42.f, 4.f));
			wall2.setPosition(curX*50+201, curY*50+246);
			wall2.setFillColor(sf::Color(100, 220, 220));
			window->draw(wall2);

		}
		//draw tail
		else if(prevX == 0){
			if (nextX == curX)
			{
				sf::RectangleShape wall1(sf::Vector2f(4.f, 42.f));
				wall1.setPosition(curX*50+201, curY*50+201);
				wall1.setFillColor(sf::Color(100, 220, 220));
				window->draw(wall1);
				sf::RectangleShape wall2(sf::Vector2f(4.f, 42.f));
				wall2.setPosition(curX*50+246, curY*50+201);
				wall2.setFillColor(sf::Color(100, 220, 220));
				window->draw(wall2);
				if (nextY < curY)
				{
				
					sf::RectangleShape wall3(sf::Vector2f(48.f, 4.f));
					wall3.setPosition(curX*50+201, curY*50+245);
					wall3.setFillColor(sf::Color(100, 220, 220));
					window->draw(wall3);
				}
				else
				{
					sf::RectangleShape wall3(sf::Vector2f(48.f, 4.f));
					wall3.setPosition(curX*50+201, curY*50+201);
					wall3.setFillColor(sf::Color(100, 220, 220));
					window->draw(wall3);
				
				}
			}
			else
			{
				sf::RectangleShape wall1(sf::Vector2f(42.f, 4.f));
				wall1.setPosition(curX*50+201, curY*50+201);
				wall1.setFillColor(sf::Color(100, 220, 220));
				window->draw(wall1);
				sf::RectangleShape wall2(sf::Vector2f(42.f, 4.f));
				wall2.setPosition(curX*50+201, curY*50+246);
				wall2.setFillColor(sf::Color(100, 220, 220));
				window->draw(wall2);
				if(nextX < curX)
				{
					sf::RectangleShape wall3(sf::Vector2f(4.f, 48.f));
					wall3.setPosition(curX*50+245, curY*50+201);
					wall3.setFillColor(sf::Color(100, 220, 220));
					window->draw(wall3);
				}
				else
				{
					sf::RectangleShape wall3(sf::Vector2f(4.f, 48.f));
					wall3.setPosition(curX*50+201, curY*50+201);
					wall3.setFillColor(sf::Color(100, 220, 220));
					window->draw(wall3);
				}

			}
		}
		//draw turns
		else
		{
			if(prevX < nextX)
			{
				if(prevY < nextY)
				{
					if(curX < nextX)
					{
						sf::RectangleShape wall1(sf::Vector2f(4.f, 48.f));
						wall1.setPosition(curX*50+201, curY*50+201);
						wall1.setFillColor(sf::Color(100, 220, 220));
						window->draw(wall1);

						sf::RectangleShape wall2(sf::Vector2f(48.f, 4.f));
						wall2.setPosition(curX*50+201, curY*50+245);
						wall2.setFillColor(sf::Color(100, 220, 220));
						window->draw(wall2);
					}
					else
					{
						sf::RectangleShape wall1(sf::Vector2f(4.f, 48.f));
						wall1.setPosition(curX*50+245, curY*50+201);
						wall1.setFillColor(sf::Color(100, 220, 220));
						window->draw(wall1);

						sf::RectangleShape wall2(sf::Vector2f(48.f, 4.f));
						wall2.setPosition(curX*50+201, curY*50+201);
						wall2.setFillColor(sf::Color(100, 220, 220));
						window->draw(wall2);
					}
				}
				else
				{
					if(curX < nextX)
					{
						sf::RectangleShape wall1(sf::Vector2f(4.f, 48.f));
						wall1.setPosition(curX*50+201, curY*50+201);
						wall1.setFillColor(sf::Color(100, 220, 220));
						window->draw(wall1);

						sf::RectangleShape wall2(sf::Vector2f(48.f, 4.f));
						wall2.setPosition(curX*50+201, curY*50+201);
						wall2.setFillColor(sf::Color(100, 220, 220));
						window->draw(wall2);
					}
					else
					{
						sf::RectangleShape wall1(sf::Vector2f(4.f, 48.f));
						wall1.setPosition(curX*50+245, curY*50+201);
						wall1.setFillColor(sf::Color(100, 220, 220));
						window->draw(wall1);

						sf::RectangleShape wall2(sf::Vector2f(48.f, 4.f));
						wall2.setPosition(curX*50+201, curY*50+245);
						wall2.setFillColor(sf::Color(100, 220, 220));
						window->draw(wall2);
					}
				}
			}
			else
			{
				if(prevY < nextY)
				{
					if(curX == nextX)
					{
						sf::RectangleShape wall1(sf::Vector2f(4.f, 48.f));
						wall1.setPosition(curX*50+201, curY*50+201);
						wall1.setFillColor(sf::Color(100, 220, 220));
						window->draw(wall1);

						sf::RectangleShape wall2(sf::Vector2f(48.f, 4.f));
						wall2.setPosition(curX*50+201, curY*50+201);
						wall2.setFillColor(sf::Color(100, 220, 220));
						window->draw(wall2);
					}
					else
					{
						sf::RectangleShape wall1(sf::Vector2f(4.f, 48.f));
						wall1.setPosition(curX*50+245, curY*50+201);
						wall1.setFillColor(sf::Color(100, 220, 220));
						window->draw(wall1);

						sf::RectangleShape wall2(sf::Vector2f(48.f, 4.f));
						wall2.setPosition(curX*50+201, curY*50+245);
						wall2.setFillColor(sf::Color(100, 220, 220));
						window->draw(wall2);
					}
				}
				else
				{
					if(curX == nextX)
					{
						sf::RectangleShape wall1(sf::Vector2f(4.f, 48.f));
						wall1.setPosition(curX*50+201, curY*50+201);
						wall1.setFillColor(sf::Color(100, 220, 220));
						window->draw(wall1);

						sf::RectangleShape wall2(sf::Vector2f(48.f, 4.f));
						wall2.setPosition(curX*50+201, curY*50+245);
						wall2.setFillColor(sf::Color(100, 220, 220));
						window->draw(wall2);
					}
					else
					{
						sf::RectangleShape wall1(sf::Vector2f(4.f, 48.f));
						wall1.setPosition(curX*50+245, curY*50+201);
						wall1.setFillColor(sf::Color(100, 220, 220));
						window->draw(wall1);

						sf::RectangleShape wall2(sf::Vector2f(48.f, 4.f));
						wall2.setPosition(curX*50+201, curY*50+201);
						wall2.setFillColor(sf::Color(100, 220, 220));
						window->draw(wall2);
					}
				}
			}
		}

	}

	sf::RectangleShape head(sf::Vector2f(48.f, 48.f));
	head.setPosition(snakeHeadx*50+201, snakeHeady*50+201);
	head.setFillColor(sf::Color(50, 250, 50));
	window->draw(head);

	//Draw Score
	sf::Text text;
	text.setFont(font);
	text.setString("Score: " + std::to_string(Score) + "\n");
	text.setCharacterSize(40);
	text.setFillColor(sf::Color::Red);
	text.setStyle(sf::Text::Bold | sf::Text::Underlined);
	window->draw(text);

	
}

void runGame(sf::RenderWindow* window){
	bool gameRunning = true;

	while (window->isOpen() && gameRunning)
	{
		sf::Event event;
		// Pointer to window works to get window.
		// This makes sense if I think about it really hard.
		// I haven't thought about it really hard
		sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);
		
		// Handle Events
		int  MouseX        = pixelPos.x;
		int  MouseY        = pixelPos.y;
		//bool MouseReleased;
		
		while (window->pollEvent(event))
		{
			if(event.type == sf::Event::MouseButtonReleased)
			{
				cout << "Mouse Clicked at " << MouseX << " " << MouseY << "\n";
				MouseDown = false;
				//MouseReleased = true;
			}
			else if(event.type == sf::Event::MouseButtonPressed)
			{
				MouseDown = true;
			}
		}

//		drawBoard(window, MouseX, MouseY, MouseReleased);
//		drawButtons(window, MouseX, MouseY, MouseReleased);
		gameRunning = updateGame();
		drawBoard(window);
		window->display();
		window->clear();
		unsigned int microsecond = 1000000;
		usleep(0.2 * microsecond);
	}

}

void resetGame()
{
	isApple = false;
	AppleX = 0;
	AppleY = 0;

	Score = 0;


	direction = 'd';
	snakex = { 15, 16, 17};
	snakey = { 15, 15, 15};
	snakeHeadx = 18;
	snakeHeady = 15;

}

void drawGameOver(sf::RenderWindow* window)
{
	bool running = true;
	while(running){

		sf::Event event;
		sf::RectangleShape backBox(sf::Vector2f(800.f, 500.f));
		backBox.setPosition(300, 200);
		backBox.setFillColor(sf::Color(190, 190, 190));
		window->draw(backBox);
	
		sf::Text gameOverText;
		gameOverText.setFont(font);
		gameOverText.setString("Game Over, better luck next time");
		gameOverText.setCharacterSize(40);
		gameOverText.setFillColor(sf::Color::Red);
		gameOverText.setStyle(sf::Text::Bold | sf::Text::Underlined);
		gameOverText.setPosition(300, 250);
		window->draw(gameOverText);

		sf::Text text;
		text.setFont(font);
		text.setString("Score: " + std::to_string(Score) + "\n");
		text.setCharacterSize(40);
		text.setFillColor(sf::Color::Red);
		text.setStyle(sf::Text::Bold | sf::Text::Underlined);
		text.setPosition(300, 300);
		window->draw(text);
		
		sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);

		int MouseX = pixelPos.x;
		int MouseY = pixelPos.y;
		bool MouseReleased = false;

		while(window->pollEvent(event))
		{
			if(event.type == sf::Event::MouseButtonReleased)
			{
				cout << "Mouse Released at " << MouseX << " " << MouseY << "\n";
				MouseReleased = true;
			}
		}




		sf::RectangleShape lbox(sf::Vector2f(300.f, 200.f));
		lbox.setPosition(350, 400);
		lbox.setFillColor(sf::Color(100, 100, 100));
		lbox.setOutlineColor(sf::Color(0, 0, 0));
		lbox.setOutlineThickness(4);

		sf::RectangleShape rbox(sf::Vector2f(300.f, 200.f));
		rbox.setPosition(750, 400);
		rbox.setFillColor(sf::Color(100, 100, 100));
		rbox.setOutlineColor(sf::Color(0, 0, 0));
		rbox.setOutlineThickness(4);
		
		if(lbox.getGlobalBounds().contains(MouseX, MouseY))
		{
			lbox.setFillColor(sf::Color(120, 120, 120));
			if(MouseReleased)
			{
				resetGame();
				running = false;
			}
		}
		else if(rbox.getGlobalBounds().contains(MouseX, MouseY))
		{
			if(MouseReleased)
			{
				exit(0);
			}
			rbox.setFillColor(sf::Color(120, 120, 120));
		}

		window->draw(lbox);
		window->draw(rbox);
		
		sf::Text quitText;
		quitText.setFont(font);
		quitText.setString("QUIT?");
		quitText.setCharacterSize(40);
		quitText.setFillColor(sf::Color(0, 0, 0));
		quitText.setPosition(820, 410);
		window->draw(quitText);

		sf::Text restartText;
		restartText.setFont(font);
		restartText.setString("Restart?");
		restartText.setCharacterSize(40);
		restartText.setFillColor(sf::Color(0, 0, 0));
		restartText.setPosition(420, 410);
		window->draw(restartText);

		window->display();
		window->clear();
	}
}


// Thread handling graphics and drawing
void renderingThread(sf::RenderWindow* window)
{
	window->setActive(true);
	while(window->isOpen())
	{
	runGame(window);
	drawGameOver(window);	
	}
}


int main()
{
	// Seed random
	srand ( time(NULL));
	// Start threads
	XInitThreads();
	// Setup window
	sf::RenderWindow window(sf::VideoMode(1800, 1800), "SNAKE");
	window.setActive(false);
	//Loads font
	if(!font.loadFromFile("arial.ttf")){
		cout << "ERR, couldn't load font file\n";
		return 0;
	}

    	//Launches Graphics in new thread
    	sf::Thread thread(&renderingThread, &window);
    	thread.launch();

	//sf::Thread keyPresses(&performKeyPress);
	//keyPresses.launch();

    	while (window.isOpen())
    	{
		performKeyPress();
    	}

    	return 0;
}

