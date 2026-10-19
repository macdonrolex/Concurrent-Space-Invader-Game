/**
 * @file BouncingBall.h - A bouncing ball (as the enemy in my game)
 * @author Chin Chi Kuo(Edward)
 * @see "Seattle University, CPSC 5042, Spring 2026"
 */

 #pragma once
 #include "Object.h"

/**
 * @class  Ball - a Ball that keep bouncing.
 * 
 * Rendered as a bouncing pink ball. The ball keep squishing it self to a flat
 * line and then recovering back and forth. State A is the full ball, B and D 
 * are squished ball, and C is a flat line. State A, B, and D have 16 pixels. 
 * State C has only 11 pixels. See more details in README.
 */
class Ball : public Object
{
public:
	/**
	 *  Ball starts out in state A, default color is pink, default position is
	 * 	(10, 20)
	 * @param r the row coordinate
	 * @param c the column coordinate
	 * @param color the color of the ball
	 */ 
	Ball(int r = 10, int c = 20, const RGB &color = RGB(0xFF, 0x00, 0xFF)) :
														Object(r, c, color){}

	/**
	 * Render the ball to the canvas.
	 */
	void render(PixelMatrix &pxm) const override;

	/**
	 * Label the enemy as destroyed
	 */
	void destroy();

	/**
	 * Destroyed status getter
	 */
	bool isDestroyed() const; 
	

private:
	bool destroyed = false; // Whether the ball enemy is destroyed
};