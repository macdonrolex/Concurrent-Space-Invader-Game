/**
 * @file Manager.h - Manager for all kinds of object
 * @author Chin Chi Kuo(Edward)
 * @see "Seattle University, CPSC 5042, Spring 2026"
 */

#pragma once
#include <future>
#include <barrier>
#include "Object.h"

/**
 * @class ScreenManager handles the game play, synchronizing with the other threads
 */
class ScreenManager
{
public:
	std::atomic_flag stop;

	ScreenManager(Object* o, PixelMatrix* p, std::mutex* m, std::barrier<>* b) :
		obj(o),
		pxm(p),
		lock(m),
		meetup(b){}

	/**
	 * This is the method running in the threads.
	 * @return number of generations drawn
	 */
	int play();

private:
	Object* obj; // Polymorphism for different types of object
	PixelMatrix* pxm;
	std::mutex* lock;
	std::barrier<> *meetup;
};