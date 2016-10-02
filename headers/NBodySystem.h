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
	namespace NBodySystemSpace {
		const unsigned particleAttributeListLength = 8;
		const char particleAttributeList [][NBodySim::NBodySystemSpace::particleAttributeListLength] = {"posX", "posY", "posZ", "velX", "velY", "velZ", "mass", "name"};
		typedef enum {
			POSX = 0,
			POSY = 1,
			POSZ = 2,
			VELX = 3,
			VELY = 4,
			VELZ = 5,
			MASS = 6,
			NAME = 7
		} particleAttributeIndexes;
		typedef enum {
			SUCCESS = 0,
			FAILED_TO_ALLOCATE_MEMORY,
			MORE_THAN_ONE_SYSTEM,
			NO_SYSTEM,
			NO_PARTICLES,
			INDEX_EXCEEDED,
			NO_POSX,
			NO_POSY,
			NO_POSZ,
			NO_VELX,
			NO_VELY,
			NO_VELZ,
			NO_MASS,
			NO_NAME
		} error;
	}
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
	/**
	 * G is the gravitation constant for the objects system
	 */
	FloatingType G;
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
	 * @return 0 on success
	 */
	NBodySim::NBodySystemSpace::error parse(std::string xmlText);
	
	/**
	 * setGravitation constant sets the systems gravitation constant
	 *
	 * @param gravitationConstant is the gravitation constant the system shall be set to
	 */
	void setGravitation(NBodySim::FloatingType gravitationConstant);
	
	/**
	 * getGravitation returns the systems gravitation constant
	 *
	 * @return the systems gravitation constant
	 */
	NBodySim::FloatingType getGravitation(void);
	
	/**
	 * errorToString takes an error code and returns it in human readable format
	 *
	 * @param errorCode is the error code sent out by a method in this class
	 * @return the human readable error code for the given error
	 */
	static std::string errorToString(NBodySim::NBodySystemSpace::error errorCode);
};

#endif // N_BODY_SYSTEM_H
