/**
 * @file Object.h - Base class for all objects
 * @author Chin Chi Kuo(Edward)
 * @see "Seattle University, CPSC 5042, Spring 2026"
 */

#pragma once
#include "PixelMatrix.h"

/**
 * @class Object - the base class for all objects
 *  
 */
class Object
{
public:
	/**
	 *  Object constructor
	 * @param r the row coordinate
	 * @param c the column coordinate
	 * @param color the color of the object
	 */ 
	Object(int r, int c, const RGB &color) : 
		row(r),
		column(c), 
		COLOR(color),
		state(A){};

	// Set default destructor because auto generated destructor is not virtual	
	virtual ~Object() = default;

	/**
	 * Move the object to its next state
	 */ 
	virtual void move();

	/**
	 * Render the object to the canvas
	 * = 0 means pure virtual function
	 */
	virtual void render(PixelMatrix &pxm) const = 0; 

	// Getters
	int getRow() const;
	int getCol() const;

protected:
	int row, column;         // Initial position of the object
	const RGB COLOR;         // Object's color
	enum State {A, B, C, D}; // Four states for each object
	State state; 			 // Object's state
};