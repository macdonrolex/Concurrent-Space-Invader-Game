/**
 * @file SpaceShip.h - A space ship
 * @author Chin Chi Kuo(Edward)
 * @see "Seattle University, CPSC 5042, Spring 2026"
 */

 #pragma once
 #include <vector>
 #include "Object.h"

/** 
 * @struct Bullet 
 * Fired by the space ship. Contained by the space ship
*/
struct Bullet
{
public:
	// Spawn a bullet from the central top of current ship 
	Bullet(int row, int col) : bRow(row - 3), bCol(col + 5){}; 

	void moveForward(); // Bullet moves forward 1 pixel
	int getBRow() const;
	int getBCol() const;
	bool isActive() const;
	void deactivate();  // Stop the bullet
	

private:
	bool active = true;        // Whether the bullet is active
	int bRow, bCol;     // Coordinate of the bullet
};

/**
 * @class SpaceShip - A space ship
 * 
 * Rendered a space ship. It can fire bullets toward enemies. 
 * See more details in README.
 */
class SpaceShip : public Object
{
public:
	// Space ship starts out in state A, default color is pink, default position
	// is (60, 0)
	SpaceShip();
	~SpaceShip() override;

	/**
	 * Render the space ship and all fired bullets to the canvas.
	 */
	void render(PixelMatrix &pxm) const override;

	/**
	 * Move the space ship left or right by changing the column coordinate.
	 * Boundary is column: 0 ~ 200
	 */ 
	void moveLeft();  // Move Left 2 pixels 
	void moveRight(); // Move right 2 pixels

	/** 
	 * Fire a bullet
	 * Create a bullet instance and store it into the bullets vector.
	 */
	void fire(); 

	/**
	 * Space ship's move is used to update all bullets' positions.
	 * Loop through all created bullets, move them forward 1 pixel by adding 
	 * the row coordinate of the bullet by 1. If the bullet hit the boarder of
	 * the window, deactivate it.
	 */ 
	void move() override;  

	/**
	 * Getter of the bullets vector
	 * @return a readonly vector reference with all bullets in it
	 */ 
	const std::vector<Bullet*>& getBullets() const;

	/**
	 * Remove all deactivated bullets in the list with an iterator
	 */ 
	void removeDeactivated(); 

private:
	// Extra credit: each bullet is an object.
	std::vector<Bullet*> bullets; // Stores all bullets
};