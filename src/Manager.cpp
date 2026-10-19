/**
 * @file Manager.cpp - Manager for all kinds of object
 * @author Chin Chi Kuo(Edward)
 * @see "Seattle University, CPSC 5042, Spring 2026"
 */
#include "Manager.h"

int ScreenManager::play()
{
	int generations = 0;

		while(!stop.test())
		{
			generations++;

			// paint the object on canvas
			lock->lock();
			obj->render(*pxm);
			lock->unlock();

			// Once we've all met up, all the rendering is done; wait for 
			// resumption point
			meetup->arrive_and_wait();

			// Once the previous generation has been painted (in main), move to 
			// next state. Without this wait, the the rendered content on canvas 
			// may be cleaned up by the pxm.clear() in main.
			meetup->arrive_and_wait();
			obj->move();
		}
		meetup->arrive_and_drop();

		return generations;
}