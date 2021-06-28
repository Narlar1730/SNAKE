#include <SFML/Graphics.hpp>
#include <iostream>
#include "piece.h"
using namespace std;

bool MouseDown        = false;
char CurrentSelection = ' ';

//Board Can be optimized, make binary tree maybe?
char Board [784] = {};

void initBoard()
{
	for(int i = 0; i < 784; i++)
	{
		Board[i] = ' ';
	}
}
//Function to round down to nearest 50
int roundNearest50(int value)
{
	int outVal = -200;
	int RoundedHundred = ((value + 50) / 100 * 100);
	if (value - RoundedHundred > 0)
	{
		outVal = RoundedHundred; 
	}
	else
	{
		outVal = RoundedHundred-50;
	}

	return outVal;
}

void drawButtonPictures(sf::RenderWindow* window, int index, int bX, int bY)
{
	switch(index)
	{
		case 0 :
			{	
			sf::RectangleShape r1(sf::Vector2f(10.f, 80.f));
			r1.setPosition(bX + 198/2 - 5, bY + 98/2 - 40);
			r1.setFillColor(sf::Color(255, 0, 0));
			sf::RectangleShape r3(sf::Vector2f(14.f, 84.f));
			r3.setPosition(bX + 198/2 - 5-2, bY + 98/2 - 40-2);
			r3.setFillColor(sf::Color(0, 0, 0));
			
			
			sf::RectangleShape r2(sf::Vector2f(80.f, 10.f));
			r2.setPosition(bX + 198/2 - 40, bY + 98/2 - 5);
			r2.setFillColor(sf::Color(255, 0, 0));
			sf::RectangleShape r4(sf::Vector2f(84.f, 14.f));
			r4.setPosition(bX + 198/2 - 40-2, bY + 98/2 - 5-2);
			r4.setFillColor(sf::Color(0, 0, 0));

			window->draw(r4);
			window->draw(r3);
			window->draw(r1);
			window->draw(r2);
			break;
			}
		case 27 :
			{
			sf::RectangleShape r1(sf::Vector2f(154.f, 54.f));
			r1.setPosition(bX+22, bY+22);
			r1.setFillColor(sf::Color(255, 192, 203));
			window->draw(r1);
			break;
			}
	}
}

//Draws Buttons and handles button input
//Buttons Implemented so far:
//button 0 -> Wire
//button 27 -> Clear
void drawButtons(sf::RenderWindow* window, int MouseX, int MouseY, bool MouseReleased)
{
	int inc = 0;
	for(int i = 0; i < 2; i++)
	{
		for(int j = 0; j < 14; j++)
		{
		
			sf::RectangleShape b1(sf::Vector2f(198.f, 98.f));
			b1.setPosition(1401 + i * 200, 1 + j * 100);
			if(b1.getGlobalBounds().contains(MouseX, MouseY))
			{
				if(MouseDown)
				{
					b1.setFillColor(sf::Color(100, 100, 100));
				}
				else if(MouseReleased)
				{
					cout << inc <<"\n";
					switch(inc)
					{
						case 0 : 
							CurrentSelection = 'w';
							break;
						case  27 :
							CurrentSelection = ' ';
							break;

					}

				}
				else
				{
					b1.setFillColor(sf::Color(120, 120, 120));
				}
			}
			else
			{
				b1.setFillColor(sf::Color(80, 80, 80));
			}
			window->draw(b1);
			drawButtonPictures(window, inc, 1401+i*200, i+j*100);
			inc++;
		}

	}
}

void drawBoardPictures(sf::RenderWindow* window, int x, int y, int inc)
{
	switch(Board[inc])
	{
		case 'w' :
			sf::RectangleShape r1(sf::Vector2f(6.f, 48.f));
			r1.setPosition(x + 21, y);
			r1.setFillColor(sf::Color(255, 0, 0));
			sf::RectangleShape r2(sf::Vector2f(48.f, 6.f));
			r2.setPosition(x, y+21);
			r2.setFillColor(sf::Color(255, 0, 0));
			window->draw(r2);
			window->draw(r1);
			break;
	}
}

void drawBoard(sf::RenderWindow* window, int MouseX, int MouseY, bool MouseReleased)
{
	int inc = 0;
	for (float i = 1; i<1400; i+=50)
	{
		for (float j = 1; j<1400; j+=50)
		{
			sf::RectangleShape rectangle(sf::Vector2f(48.f, 48.f));
			rectangle.setPosition(i, j);
				
			rectangle.setFillColor(sf::Color(100, 100, 100));
			if(rectangle.getGlobalBounds().contains(MouseX, MouseY))
			{
				rectangle.setFillColor(sf::Color(120, 120, 120));
				if(MouseDown)
				{
					rectangle.setFillColor(sf::Color(140, 140, 140));
					switch(CurrentSelection)
					{
						case 'w' :
							Board[inc] = 'w';
							break;
						case ' ' :
							Board[inc] = ' ';
							break;
					}
				}
			}
			
			window->draw(rectangle);
			drawBoardPictures(window, i, j, inc);
			inc++;
		}
	}

}

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
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::C))
		{
			initBoard();
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			CurrentSelection = 'w';
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::E))
		{
			CurrentSelection = ' ';
		}
		// Pass
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		cout << "User Quit Program\n";
		exit(0);
	}

}

// Thread handling graphics and drawing
void renderingThread(sf::RenderWindow* window)
{
	window->setActive(true);

	while (window->isOpen())
	{
		sf::Event event;
		// Pointer to window works to get window.
		// This makes sense if I think about it really hard.
		// I haven't thought about it really hard
		sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);
		
		// Handle Events
		int  MouseX        = pixelPos.x;
		int  MouseY        = pixelPos.y;
		bool MouseReleased = false;
		
		while (window->pollEvent(event))
		{
			if(event.type == sf::Event::MouseButtonReleased)
			{
				cout << "Mouse Clicked at " << MouseX << " " << MouseY << "\n";
				MouseDown = false;
				MouseReleased = true;
			}
			else if(event.type == sf::Event::MouseButtonPressed)
			{
				MouseDown = true;
			}
		}
		drawBoard(window, MouseX, MouseY, MouseReleased);
		drawButtons(window, MouseX, MouseY, MouseReleased);
		performKeyPress();
		window->display();
		window->clear();
	}
}

int main()
{
    	initBoard();
	sf::RenderWindow window(sf::VideoMode(1800, 1800), "Computer");
	window.setActive(false);

    	//Launches Graphics in new thread
    	sf::Thread thread(&renderingThread, &window);
    	thread.launch();

    	while (window.isOpen())
    	{
    	}

    	return 0;
}
