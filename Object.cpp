/**
 * @file Object.cpp - Base class for all objects
 * @author Chin Chi Kuo(Edward)
 * @see "Seattle University, CPSC 5042, Spring 2026"
 */
#include "Object.h"

void Object::move()
{
	switch(state)
	{
		case A:
			state = B;
			break;
		case B:
			state = C;
			break;
		case C:
			state = D;
			break;
		case D:
			state = A;
			break;
	}
}

int Object::getRow() const
{
	return row;
}
int Object::getCol() const
{
	return column;
}