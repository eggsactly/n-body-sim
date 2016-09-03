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

#ifndef PARTICLE_H
#define PARTICLE_H

#include <string>

#include "NBodyTypes.h"


namespace NBodySim{
	class Particle;
}
/**
 * @brief An entity with a name, mass, instantaneous position and velocity in three dimensional space.
 *
 * @author: W.A. Garrett Weaver
 * @see ThreeVector
 */
class NBodySim::Particle {
private:
	
protected:
	/**
	 *	position holds the floating point three dimensional position of the particle in meters
	 */
	NBodySim::ThreeVector <NBodySim::FloatingType> position;
	
	/** 
	 * velocity holds the floating point three dimensional velocity in meters per second
	 */
	NBodySim::ThreeVector <NBodySim::FloatingType> velocity;
	
	/**
	 * mass holds the floating point mass of a particle in kilograms
	 */
	NBodySim::FloatingType mass;
	
	/**
	 * name is the string that holds what the particle is named
	 */
	std::string name;
	
	/**
	 * particle is a method that all the versions of the constructors call to maintain consistancy
	 *
	 * @param xPos is the position of the particle, in meters, in the x direction
	 * @param yPos is the position of the particle, in meters, in the y direction
	 * @param zPos is the position of the particle, in meters, in the z direction
	 * @param xVel is the velocity of the particle, in meters per second, in the x direction
	 * @param yVel is the velocity of the particle, in meters per second, in the y direction
	 * @param zVel is the velocity of the particle, in meters per second, in the z direction
	 * @param massIn is the mass of the particle in kilograms
	 * @param nameIn is the name of the particle
 	 */
	virtual void particle(NBodySim::FloatingType xPos, NBodySim::FloatingType yPos, NBodySim::FloatingType zPos, NBodySim::FloatingType xVel, NBodySim::FloatingType yVel, NBodySim::FloatingType zVel, NBodySim::FloatingType massIn, std::string nameIn);

public:
	/**
	 * Default constructor
	 */
	Particle(void);
	
	/**
	 * constructor which takes in floating types
	 *
	 * @param xPos is the position of the particle, in meters, in the x direction
	 * @param yPos is the position of the particle, in meters, in the y direction
	 * @param zPos is the position of the particle, in meters, in the z direction
	 * @param xVel is the velocity of the particle, in meters per second, in the x direction
	 * @param yVel is the velocity of the particle, in meters per second, in the y direction
	 * @param zVel is the velocity of the particle, in meters per second, in the z direction
	 * @param massIn is the mass of the particle in kilograms
	 * @param nameIn is the name of the particle
	 */
	Particle(NBodySim::FloatingType xPos, NBodySim::FloatingType yPos, NBodySim::FloatingType zPos, NBodySim::FloatingType xVel, NBodySim::FloatingType yVel, NBodySim::FloatingType zVel, NBodySim::FloatingType massIn, std::string nameIn);
	
	/**
	 * constructor which takes in Three vectors where possible
	 *
	 * @param pos is the position of the particle, in meters, in the x, y and z direction
	 * @param vel is the velocity of the particle, in meters per second, in the x, y, and z direction
	 * @param massIn is the mass of the particle in kilograms
	 * @param nameIn is the name of the particle
	 */
	Particle(NBodySim::ThreeVector <NBodySim::FloatingType> pos, NBodySim::ThreeVector <NBodySim::FloatingType> vel, NBodySim::FloatingType massIn, std::string nameIn);
	
	/**
	 * Destructor
	 */
	virtual ~Particle(void);
	
	/**
	 * Returns of the position of the particle as a ThreeVector
	 *
	 * @return the position of the particle as a ThreeVector
	 */
	NBodySim::ThreeVector <NBodySim::FloatingType> getPos(void);
	
	/**
	* Returns of the velocity of the particle as a ThreeVector
	*
	* @return the position of the particle as a ThreeVector
	*/
	NBodySim::ThreeVector <NBodySim::FloatingType> getVel(void);
	
	/**
	* Returns of the mass of the particle as a ThreeVector
	*
	* @return the position of the particle as a ThreeVector
	*/
	NBodySim::FloatingType getMass(void);
	
	/**
	 * Returns of the name of the particle as a string
	 *
	 * @return the name of the particle as a string
	 */
	std::string getName(void);
	
	/**
	 * Sets the position of the particle with ThreeVector
	 *
	 * @param newPosition is the new position of the particle
	 */
	void setPos(NBodySim::ThreeVector <NBodySim::FloatingType> newPosition);
	
	/**
	 * Sets the velocity of the particle with ThreeVector
	 *
	 * @param newVelocity is the new velocity of the particle
	 */
	void setVel(NBodySim::ThreeVector <NBodySim::FloatingType> newVelocity);
	
	/**
	 * Sets the mass of the particle with ThreeVector
	 *
	 * @param newMass is the new mass of the particle
	 */
	void setMass(NBodySim::FloatingType newMass);
	
	/**
	 * Sets the x position of the particle with a floating point number
	 *
	 * @param newPosition is the new x position of the particle
	 */
	void setPosX(NBodySim::FloatingType newPosition);
	
	/**
	 * Sets the y position of the particle with a floating point number
	 *
	 * @param newPosition is the new y position of the particle
	 */
	void setPosY(NBodySim::FloatingType newPosition);
	
	/**
	 * Sets the z position of the particle with a floating point number
	 *
	 * @param newPosition is the new z position of the particle
	 */
	void setPosZ(NBodySim::FloatingType newPosition);
	
	/**
	 * Sets the x velocity of the particle with a floating point number
	 *
	 * @param newVelocity is the new x position of the particle
	 */
	void setVelX(NBodySim::FloatingType newVelocity);
	
	/**
	 * Sets the y velocity of the particle with a floating point number
	 *
	 * @param newVelocity is the new y position of the particle
	 */
	void setVelY(NBodySim::FloatingType newVelocity);
	
	/**
	 * Sets the z velocity of the particle with a floating point number
	 *
	 * @param newVelocity is the new z position of the particle
	 */
	void setVelZ(NBodySim::FloatingType newVelocity);
};

#endif //PARTICLE_H
