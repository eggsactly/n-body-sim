/*
 * Copyright (c) 2016 - 2017 W.A. Garrett Weaver
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
	template <class T> class NBodySystem;
	namespace NBodySystemSpace {
		const unsigned particleAttributeListLength = 8;
		const char particleAttributeList [][NBodySim::NBodySystemSpace::particleAttributeListLength] = {"posX", "posY", "posZ", "velX", "velY", "velZ", "mass", "name"};
		/**
		 * Values for the particle attribute list
		 */
		typedef enum {
			/**
			 * Index of POSX
			 */
			POSX = 0,
			/**
			 * Index of POSY
			 */
			POSY = 1,
			/**
			 * Index of POSZ
			 */
			POSZ = 2,
			/**
			 * Index of VELX
			 */
			VELX = 3,
			/**
			 * Index of VELY
			 */
			VELY = 4,
			/**
			 * Index of VELZ
			 */
			VELZ = 5,
			/**
			 * Index of MASS
			 */
			MASS = 6,
			/**
			 * Index of NAME
			 */
			NAME = 7
		} particleAttributeIndexes;
		/**
		 * List of errors the parse method can have
		 */
		typedef enum {
			/**
			 * Successful parse
			 */
			SUCCESS = 0,
			/*
			 * Failed to allocate memory
			 */
			FAILED_TO_ALLOCATE_MEMORY,
			/*
			 * More than one system in XML file
			 */
			MORE_THAN_ONE_SYSTEM,
			/*
			 * No system in XML file
			 */
			NO_SYSTEM,
			/*
			 * No particles in the system
			 */
			NO_PARTICLES,
			/*
			 * Index of particleAttributeListLength exceeded
			 */
			INDEX_EXCEEDED,
			/**
			 * No POSX attribute provided
			 */
			NO_POSX,
			/**
			 * No POSY attribute provided
			 */
			NO_POSY,
			/**
			 * No POSX attribute provided
			 */
			NO_POSZ,
			/**
			 * No VELX attribute provided
			 */
			NO_VELX,
			/**
			 * No VELY attribute provided
			 */
			NO_VELY,
			/**
			 * No VELZ attribute provided
			 */
			NO_VELZ,
			/**
			 * No MASS attribute provided
			 */
			NO_MASS,
			/**
			 * No NAME attribute provided
			 */
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
template <class T>
class NBodySim::NBodySystem {
private:
	
protected:
	/**
	 * system is set of all particles, where the order doesn't matter, but a vector was used anyways.
	 */
	std::vector<NBodySim::Particle<T> > system;
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
	void addParticle(NBodySim::Particle<T> p);
	
	/**
	 * getParticle returns a particle based on its index
	 *
	 * @param index the index of the particle to return
	 * @return the particle specified by the index
	 */
	NBodySim::Particle<T> getParticle(size_t index);
	
	/**
	 * numParticles returns the number of particles in the simulation
	 *
	 * @return an unsigned number representing the number of particles in the simulation
	 */
	size_t numParticles(void);
	
	/**
	 * removeParticle removes a particle from the simulation based on its index
	 *
	 * @param index is the index of the particle to be removed
	 */
	void removeParticle(size_t index);
	
	/**
	 * step calculates new positions and velocities of the particles in the system
	 *
	 * @param deltaT is a floating point number of the amount of time that passes till the next step
	 */
	void step(T deltaT);
	
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
	void setGravitation(T gravitationConstant);
	
	/**
	 * getGravitation returns the systems gravitation constant
	 *
	 * @return the systems gravitation constant
	 */
	T getGravitation(void);
	
	/**
	 * errorToString takes an error code and returns it in human readable format
	 *
	 * @param errorCode is the error code sent out by a method in this class
	 * @return the human readable error code for the given error
	 */
	static std::string errorToString(NBodySim::NBodySystemSpace::error errorCode);
};

#endif // N_BODY_SYSTEM_H
