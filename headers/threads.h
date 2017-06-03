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

#ifndef THREADS_H
#define THREADS_H

#include <boost/thread.hpp>
#include <boost/functional.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/io.hpp>

#include "NBodyTypes.h"
#include "Particle.h"
#include "NBodySystem.h"

/**
 * @brief timingFunction pulls a semaphore high every interval for as long as quitTiming is false
 *
 * @param interval simulation step size
 * @param numSems number of semaphores to post
 * @param timingSems an array of sempahores that shall be posted when the interval has transpired
 * @param quiTiming a boolean used to indicate if the timing should continue
 * @return A null pointer
 */
void * timingFunction(NBodySim::FloatingType interval, unsigned numSems, boost::interprocess::interprocess_semaphore ** timingSems, volatile bool * quitTiming);

/**
 * @brief workThread calculates new positions and velocities for the particle vector till program close
 * The Proletariat will rise and overthrow the Bourgeoisie.
 *
 * @param stepSize the amount of time for each simulation step
 * @param timingSem a pointer to a semaphore used to tell the function when to procede with the next step
 * @param quitTiming a bool used to indicate if the thread should continue
 * @param solarSystem a pointer to the system containing all the particles
 * @param stepsPerTime a pointer to an int indicating how many time steps should occur per timingSem post
 * @return A null pointer
 */
void * workThread(NBodySim::FloatingType stepSize, boost::interprocess::interprocess_semaphore * timingSem, volatile bool * quitTiming, NBodySim::NBodySystem<NBodySim::FloatingType> * solarSystem, volatile size_t * stepsPerTime);

#endif //THREADS_H
