/*
 * Copyright (c) 2016 W.A. Garrett Weaver
 *
 * This file is part of n-body-sim.
 *
 * n-body-sim is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * n-body-sim is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with n-body-sim.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifdef _WIN32
#include <SDL.h>
	// Keep redeclaration of timespec
	#define HAVE_STRUCT_TIMESPEC
	// Undefine main because of sdl
	#undef main
#else
	// Mac OS X and Linux
	#include <SDL2/SDL.h>
#endif

#include "threads.h"

void * timingFunction(NBodySim::FloatingType interval, unsigned numSems, boost::interprocess::interprocess_semaphore ** timingSems, volatile bool * quitTiming){
	if(timingSems == NULL){
		return NULL;
	}
	for(unsigned i = 0; i < numSems; i++){
		if(timingSems[i] == NULL){
			return NULL;
		}
	}
	if(quitTiming == NULL){
		return NULL;
	}
	
	// When each interval passes, raise up all the semaphores passed to this function.
	while(!(*quitTiming)){
		SDL_Delay(interval);
		for(unsigned i = 0; i < numSems; i++){
			timingSems[i]->post();
		}
	}
	
	return NULL;
}

void * workThread(NBodySim::FloatingType stepSize, boost::interprocess::interprocess_semaphore * timingSem, volatile bool * quitTiming, NBodySim::NBodySystem<NBodySim::FloatingType> * solarSystem, volatile size_t * stepsPerTime){
	
	if(timingSem == NULL){
		return NULL;
	}
	if(quitTiming == NULL){
		return NULL;
	}
	if(solarSystem == NULL){
		return NULL;
	}
	if(stepsPerTime == NULL){
		return NULL;
	}
	
	// For each iteration, wait on a semaphore
	while(!(*quitTiming)){
		timingSem->wait();
		// Implements Req FR.Calculate
		for(size_t i = 0; i < *stepsPerTime; i++){
			solarSystem->step(stepSize);
		}
	}
	
	return NULL;
	
}
