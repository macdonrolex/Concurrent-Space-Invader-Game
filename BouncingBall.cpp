/**
 * @file BouncingBall.cpp
 * @author Chin Chi Kuo
 * @see "Seattle University, CPSC 5042, Spring 2026"
 */

#include "BouncingBall.h"

void Ball::render(PixelMatrix &pxm) const 
{
	const int N = 16;  // 16 pixels for case A, B, D
	const int NC = 11; // 11 pixels for case C
	
	int rows[N], cols[N]; // Arrays to store coordinates
	int c = column;
	switch(state)
	{
		case A: // Full ball
			// Row coordinates
			rows[3] = rows[4] = rows[5]= row - 3; // upper half
			rows[2] = rows[6] = row - 2; // upper half
			rows[1] = rows[7] = row - 1; // upper half 
			rows[0] = rows[8] = row; // middle
			rows[15] = rows[9] = row + 1; // lower half
			rows[14] = rows[10] = row + 2; // lower half
			rows[13] = rows[12] = rows[11] = row + 3; // lower half

			// Col coordinates
			cols[1] = cols[0] = cols[15] = column + 2; // left half
			cols[2] = cols[14] = column + 3; // left half
			cols[3] = cols[13] = column + 4; // left half
			cols[4] = cols[12] = column + 5; // middle
			cols[5] = cols[11] = column + 6; // right half
			cols[6] = cols[10] = column + 7; // right half
			cols[7] = cols[8] = cols[9] = column + 8; // right half

			for (int i = 0; i < N; i++)
			{
				pxm.paint(rows[i], cols[i], COLOR);
			}
			break;
		case B:
		case D:
			// Row coordinates
			rows[3] = rows[4] = rows[5]= row - 2; // upper half
			rows[1] = rows[2] = rows[6] = rows[7] = row - 1; // upper half
			rows[0] = rows[8] = row; // middle
			rows[15] = rows[14] = rows[10] = rows[9] = row + 1; // lower half
			rows[13] = rows[12] = rows[11] = row + 2; // lower half

			// Col coordinates
			cols[0] = column + 1; // left half
			cols[1] = cols[15] = column + 2; // left half
			cols[2] = cols[14] = column + 3; // left half
			cols[3] = cols[13] = column + 4; // left half
			cols[4] = cols[12] = column + 5; // middle
			cols[5] = cols[11] = column + 6; // right half
			cols[6] = cols[10] = column + 7; // right half
			cols[7] = cols[9] = column + 8; // right half
			cols[8] = column + 9; // right half

			for (int i = 0; i < N; i++)
			{
				pxm.paint(rows[i], cols[i], COLOR);
			}
			break;
		case C:
			for (int i = 0; i < N; ++i)
			{
				rows[i] = row; // All pixels on the same row(10)
				cols[i] = c++; // Each pixel one column to the right
			}
			for (int i = 0; i < NC; i++)
			{
				pxm.paint(rows[i], cols[i], COLOR);
			}
			break;
	}
}
void Ball::destroy()
{
	destroyed = true;
}
bool Ball::isDestroyed() const
{
	return destroyed;
}
