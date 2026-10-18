/**
 * @file SpaceShip.cpp - A space ship
 * @author Chin Chi Kuo(Edward)
 * @see "Seattle University, CPSC 5042, Spring 2026"
 */

#include "SpaceShip.h"

SpaceShip::SpaceShip() : Object(60, 0, RGB::BLUE){}
SpaceShip::~SpaceShip()
{
	for (Bullet* b : bullets)
	{
		delete b;
	}
}

void SpaceShip::render(PixelMatrix &pxm) const
{
	const int N = 20;  // 20 pixels for case A

	int rows[N], cols[N]; // Arrays to store coordinates
	// Row coordinates
	rows[13] = rows[2] = rows[3] = rows[4] = rows[15] = row + 2;
	rows[12] = rows[1] = rows[5] = rows[14] = row + 1;
	rows[0] = rows[6] = row;
	rows[16] = rows[11] = rows[7] = rows[18] = row - 1;
	rows[17] = rows[10] = rows[9] = rows[8] = rows[19] = row - 2;

	// Col coordinates
	cols[13] = cols[17] = column + 2;
	cols[12] = cols[16] = column + 3;
	cols[2] = cols[1] = cols[0] = cols[11] = cols[10] = column + 4;
	cols[3] = cols[9] = column + 5;
	cols[4] = cols[5] = cols[6] = cols[7] = cols[8] = column + 6;
	cols[14] = cols[18] = column + 7;
	cols[15] = cols[19] = column + 8;

	for (int i = 0; i < N; i++)
	{
		pxm.paint(rows[i], cols[i], COLOR);
	}

	// Render all active bullets
	for (Bullet* b : bullets)
	{
		if (b->isActive())
		{
			pxm.paint(b->getBRow(), b->getBCol(), RGB::YELLOW);
		}
	}
}

void SpaceShip::moveLeft()
{
	if (column > 0)
	{
		column -= 2;
	}
}
void SpaceShip::moveRight()
{
	if (column < 200 )
	{
		column += 2;
	}
}

void SpaceShip::fire()
{
	Bullet* b = new Bullet(row, column);
	bullets.push_back(b);
}
void SpaceShip::move()
{
	// Move the bullet
	for (Bullet* b : bullets)
	{
		b->moveForward();
		if (b->getBRow() <= 0)
		{
			b->deactivate();
		}
	}

	// Remove and delete bullet hit the celling. 
	removeDeactivated();
}
const std::vector<Bullet*>& SpaceShip::getBullets() const
{
	return bullets;
}

void SpaceShip::removeDeactivated()
{
	auto it = bullets.begin();
	while (it != bullets.end())
	{
		if (!(*it)->isActive())
		{
			delete *it;
			it = bullets.erase(it); // erase returns next valid iterator
		}
		else
		{
			it++;
		}
	}
}

// Bullet
void Bullet::moveForward()
{
	bRow--;
}
int Bullet::getBRow() const
{
	return bRow;
}
int Bullet::getBCol() const
{
	return bCol;
}
bool Bullet::isActive() const
{
	return active;
}
void Bullet::deactivate()
{
	active = false;
}

