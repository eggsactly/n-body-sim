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

#ifndef N_BODY_SYSTEM_H
#define N_BODY_SYSTEM_H

#include <string>
#include <vector>

#include "NBodyTypes.h"
#include "Particle.h"

namespace NBodySim {
	class NBodySystem;
}

/**
 * @brief Contains a vector of type Particle and is used to calculate each new step of the simulation.
 *
 * @author W.A. Garrett Weaver
 * @see Particle
 */
class NBodySim::NBodySystem {
private:
	
protected:
	/**
	 * system is set of all particles, where the order doesn't matter, but a vector was used anyways.
	 */
	std::vector<NBodySim::Particle> system;
public:
	/**
	 * Default constructor
	 */
	NBodySystem(void);
	/**
	 * Destructor
	 */
	virtual ~NBodySystem(void);
	
	/**
	 * addParticle adds a particle to the sim
	 *
	 * @param p is a particle to be added to the system vector
	 */
	void addParticle(NBodySim::Particle p);
	
	/**
	 * getParticle returns a particle based on its index
	 *
	 * @param index the index of the particle to return
	 * @return the particle specified by the index
	 */
	NBodySim::Particle getParticle(NBodySim::UnsignedType index);
	
	/**
	 * numParticles returns the number of particles in the simulation
	 *
	 * @return an unsigned number representing the number of particles in the simulation
	 */
	NBodySim::UnsignedType numParticles(void);
	
	/**
	 * removeParticle removes a particle from the simulation based on its index
	 *
	 * @param index is the index of the particle to be removed
	 */
	void removeParticle(NBodySim::UnsignedType index);
	
	/**
	 * step calculates new positions and velocities of the particles in the system
	 *
	 * @param deltaT is a floating point number of the amount of time that passes till the next step
	 */
	void step(NBodySim::FloatingType deltaT);
	
	/**
	 * parse takes in a string containing xml text of a system scenario and creates particle instances in this class
	 *
	 * @param xmlText is a string containing valid xml
	 * @return 0 on success, 1 on failure
	 */
	unsigned char parse(std::string xmlText);
};

#endif // N_BODY_SYSTEM_H