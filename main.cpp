#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
using namespace std;
using namespace sf;
int width = 1000, height = 1000;
int wSemafor = 30, hSemafor = 100;
int wCar = 70, hCar = 50;
float rCircle = 13.0;
chrono::time_point<chrono::system_clock> start, stop;
RenderWindow window(VideoMode(width, height), "Trafic de masini", Style::Default);
RectangleShape car(Vector2f(wCar, hCar));
RectangleShape semafor(Vector2f(wSemafor, hSemafor));
CircleShape circle(rCircle, 30U);
struct node
{
	float posX, posY;
	float speedCar, accCar;
	node* next;
	node* prev;
};
bool isEmpty(node* head)
{
	if (head == NULL)
	{
		return true;
	}
	else
	{
		return false;
	}
}
void Put(node*& head, node*& tail)
{
	node* p = new node;
	p->posX = 40;
	p->speedCar = 5;
	p->posY = height / 2 - hCar / 2;
	p->prev = NULL;
	p->next = NULL;
	if (isEmpty(head))
	{
		head = p;
	}
	else
	{
		if (tail == NULL)
		{
			p->next = head;
			head->prev = p;
			tail = p;
		}
		else
		{
			p->next = tail;
			tail->prev = p;
			tail = p;
		}
	}
}
void Get(node*& head, node*& tail)
{
	node* p = head;
	if (head != NULL)
	{
		if (head->prev != NULL)
		{
			head = head->prev;
			head->next = NULL;
			delete p;
		}
		else
		{
			head = NULL;
			tail = NULL;
			delete p;
		}
	}
}
void drawCars(node*& head, node*& tail)
{
	node* p = new node;
	if (head != NULL)
	{
		if (tail != NULL)
		{
			p = tail;
		}
		else
		{
			p = head;
		}
		while (p->next != NULL)
		{
			car.setFillColor(Color::Blue);
			car.setPosition(p->posX, p->posY);
			window.draw(car);
			if (p->next->speedCar == 0)
			{
				if (p->next->posX - p->posX - wCar < 40)
				{
					p->speedCar = 0;
				}
				else
				{
					p->speedCar = 5;
				}
			}
			else
			{
				p->speedCar = 5;
			}
			if (p->posX + wCar - circle.getPosition().x < 10 && p->posX + wCar - circle.getPosition().x > 0)
			{
				if (circle.getFillColor() == Color::Red)
				{
					p->speedCar = 0;
				}
				else
				{
					p->speedCar = 5;
				}
			}
			p->posX += p->speedCar;
			p = p->next;
		}
		car.setFillColor(Color::Blue);
		car.setPosition(p->posX, p->posY);
		window.draw(car);
		p->posX += p->speedCar;
		if (p->posX + wCar - circle.getPosition().x < 10 && p->posX + wCar - circle.getPosition().x > 0)
		{
			if(circle.getFillColor() == Color::Red || circle.getFillColor() == Color::Yellow)
			{
				p->speedCar = 0;
			}
			else if(circle.getFillColor() == Color::Green)
			{
				p->speedCar = 5;
			}
		}
	}
}
int main()
{
	node* head = NULL;
	node* tail = NULL;
	int wRoad = 80;
	window.setFramerateLimit(30);
	semafor.setFillColor(Color::Magenta);
	circle.setFillColor(Color::Red);
	circle.setPosition(width - wSemafor  - wCar + 2, height / 2 - wRoad - hSemafor + 2);
	semafor.setPosition(width - wSemafor - wCar, height / 2 - wRoad - hSemafor);
	start = chrono::system_clock::now();
	int col = 0;
	bool power = false;
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::EventType::Closed)
			{
				window.close();
				start = chrono::system_clock::now();
			}
			else
			{
				break;
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::P) || power)
		{
			power = true;
			if (head != NULL && head->prev != NULL)
			{
				if (head->prev == NULL)
				{
					if (head->posX > 60)
					{
						Put(head, tail);
					}
				}
				else
				{
					if (tail->posX > wCar + 60)
					{
						Put(head, tail);
					}

				}
			}
			else
			{
				Put(head, tail);
			}
			if (Keyboard::isKeyPressed(Keyboard::O))
			{
				power = false;
			}
		}
		stop = chrono::system_clock::now();
		int interval;
		interval = chrono::duration_cast<chrono::seconds>(stop - start).count();
		window.clear();
		if (interval == 2 && circle.getFillColor() == Color::Green)
		{
			circle.setPosition(circle.getPosition().x, circle.getPosition().y - rCircle - 2);
			circle.setFillColor(Color::Yellow);
			col = 1;
			start = stop;
		}
		if (interval == 3 && circle.getFillColor() == Color::Red)
		{
			circle.setPosition(circle.getPosition().x, circle.getPosition().y + rCircle + 2);
			circle.setFillColor(Color::Yellow);
			col = -1;
			start = stop;
		}
		if (interval == 1 && circle.getFillColor() == Color::Yellow)
		{
			if (col == -1)
			{
				circle.setPosition(circle.getPosition().x, circle.getPosition().y + rCircle + 2);
				circle.setFillColor(Color::Green);
			}
			else if (col == 1)
			{
				circle.setPosition(circle.getPosition().x, circle.getPosition().y - rCircle - 2);
				circle.setFillColor(Color::Red);
			}
			start = stop;
		}
		if (!isEmpty(head))
		{
			drawCars(head, tail);
			if (head->posX > width - wCar)
			{
				Get(head, tail);
			}
		}
		window.draw(semafor);
		window.draw(circle);
		window.display();
			
	}
	return 0;
}