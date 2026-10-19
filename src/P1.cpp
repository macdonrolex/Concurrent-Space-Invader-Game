/**
 * @file P1.cpp - P1: My game
 * @author Chin Chi Kuo(Edward)
 * @see "Seattle University, CPSC 5042, Spring 2026"
 */

#include <iostream>
#include <future>
#include <barrier>
#include <random> // Required for mt19937 and random_device
#include "Terminal.h"
#include "BouncingBall.h" 
#include "SpaceShip.h"
#include "Manager.h"

using namespace std;

/**
 * Game logic
 * @return
 * @throws invalid_argument if the terminal is too small to play
 */
void playGame()
{
	const RGB BG_COLOR = RGB::BLACK;
    const int THROTTLE = 20;   // milliseconds of pause between generations
	bool allDestroyed = false; // Whether all enemies are destroyed
	int enemyNum = 10;         // Number of enemies

	// Set up the terminal and the pixel matrix feeding it
	Terminal* t = new Terminal(false);
	int rows, cols;
	t->getSize(rows, cols);
	PixelMatrix pxm(rows, cols, BG_COLOR);

	// Set up the synchronization 
	barrier<> meetup(2 + enemyNum); // main + space ship + enemies
	mutex lock;

	// Extra credit: Set up a space ship and enemies
	// Space ship object
	SpaceShip* spaceShip = new SpaceShip();
	ScreenManager* shipManager = new ScreenManager(spaceShip, &pxm, &lock, 
																	&meetup);
	// Enemy objects
	vector<Ball*> enemies;                // Store all enemies
	vector<ScreenManager*> enemyManagers; // Store all enemy managers
	// Random position
	// Extra credit: Each new game the enemies spawn in different position.
	// Make each game round feels different.
	random_device rd; 
	mt19937 gen(rd()); 
	uniform_int_distribution<> colDistr(0, 200); // Inclusive
	uniform_int_distribution<> rowDistr(10, 50); // Inclusive
	RGB ColorOptions[] = {RGB::RED, RGB::GREEN, RGB::WHITE};// Enemy colors
	for (int i = 0; i < enemyNum; ++i)
	{
		// Switch between 3 colors
		RGB color = ColorOptions[i % 3];

		// Create enemy
		Ball* enemy = new Ball(rowDistr(gen), colDistr(gen), color);
		enemies.push_back(enemy); 
		// Create enemy's screen manager
		ScreenManager* enemyManager = new ScreenManager(enemy, &pxm, &lock, 
																	&meetup);
		enemyManagers.push_back(enemyManager);
	}
	
	// Start all threads
	// Start space ship thread
	future<int> shipHandle = async(&ScreenManager::play, shipManager);
	// Start enemy threads
	vector<future<int>> enemyHandles;
	for (int i = 0; i < enemyNum; ++i)
	{
		enemyHandles.push_back(async(&ScreenManager::play, enemyManagers[i]));
	}

	// In the main thread loop till all enemies are destroyed
	int destroyCount = 0; // How many enemies has been destroyed 
	while(!allDestroyed)
	{
		// pause for human to see
		this_thread::sleep_for(chrono::milliseconds(THROTTLE));

		// Phase1: main thread waits for everyone been painted to canvas
		meetup.arrive_and_wait();
		// Paint the rendered pixel matrix, and then fresh the screen
		t->paint(pxm);
		pxm.paint(0, 0, rows, cols, BG_COLOR); 

		// Get player's input to move the ship here because phase 2 belongs 
		// to the main thread. If we do this in phase 1, the ship rendering 
		// might be cracked.
		// Only get the last key in the input buffer, because each frame can 
		// only consume one instruction. If we didn't do so and keep pressing 
		// the same key for a long time, the main loop will consume the  
		// remaining keys in the buffer queue, which make the control choppy. 
		int key = -1;
		// Drain the key buffer
		while(t->hasKey())
		{
			key = t->getKey(); // Keep overwriting, last one wins
		}
		if(key != -1)
		{
			switch(key)
			{
				case 'F':
				case 'f':
					spaceShip->moveLeft();
					break;
				case 'J':
				case 'j':
					spaceShip->moveRight();
					break;
				case ' ': // Space
					spaceShip->fire();
					break;
			}
		}

		// Check if there is any enemy got hit
		for (int i = 0; i < enemyNum; ++i)
		{
			Ball* e = enemies[i]; // Current enemy

			// Only check alive enemies.
			if (!e->isDestroyed())
			{
				// Each enemy, check all bullets
				for (Bullet* b : spaceShip->getBullets())
				{
					// The hit-box of current enemy is its row+3 and col+0~+10
					// If hit current enemy, stop the enemy and the bullet
					if (b->getBRow() <= e->getRow() + 3 &&
						b->getBCol() >= e->getCol() &&
						b->getBCol() <= e->getCol() + 10)
					{
						// Stop enemy
						enemyManagers[i]->stop.test_and_set();
						e->destroy();
						destroyCount++;

						// Stop bullet
						b->deactivate();
					}
				}
			}
		}

		// Phase2: now every object can move to next stage and paint to canvas
		meetup.arrive_and_wait();

		// Check if the game is over
		if (destroyCount == enemyNum)
		{
			allDestroyed = true;
		}
	}
	// Tell the ship to stop
    shipManager->stop.test_and_set();

	// After player hits the key, the play loop won't stop instantly and might 
	// be in the middle of the loop.
	meetup.arrive_and_wait(); // Release Phase1 meetup
    meetup.arrive_and_wait(); // Release Phase2 meetup
	
	//Destruction of the Terminal object returns the screen to normal shell mode
	delete t;
	

	// TODO Figure out why I need (void)join all threads and print out the result
	//  ignoring the return value is safe and intended?
	/*
		.get() waits for the thread to finish. You need that before deleting spaceShip and the managers, or a thread could use freed memory.
		(void) shows you are intentionally ignoring the ship’s return value.
		One tip: pass launch::async to async(...) explicitly. The barrier needs real concurrency, and without a policy the implementation is allowed to defer the call.
	*/
	(void)shipHandle.get();
	for (int i = 0; i < enemyNum; ++i)
	{
		// Each bouncing takes 4 cycles
		int bouncingCount = enemyHandles[i].get() / 4; 

		cout << "Enemy " << i + 1 << " survived " <<  bouncingCount 
														<< " bouncing." << endl;
	}

	// Delete instances on the heap
	delete spaceShip;
	delete shipManager;
	for (int i = 0; i < enemyNum; ++i)
	{
		delete enemies[i];
		delete enemyManagers[i];
	}
}

/**
 * main thread
 * @return
 * @throws invalid_argument if the terminal is too small to play
 */
int main()
{
	// Start message
	cout << "Destroy the Bouncing Invaders!\n"
		 << "Try shooting down all the enemies!\n\n" 
		 << "Move Left: [f]\n" 
		 << "Move Right: [j]\n"
		 << "Fire: [space]\n\n"
		 << "Turn your terminal window into fullscreen first"
		 << endl;

	char ready;
	cout << "Are you ready? [Y/n + enter]";
	cin >> ready;
	if (ready == 'Y' || ready == 'y')
	{
		playGame();
	}
	cout << "Good Bye!\n";
	
    return EXIT_SUCCESS;
}
