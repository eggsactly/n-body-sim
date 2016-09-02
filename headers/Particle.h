/*
 * Copyright (c) 2016 Garrett Weaver
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
 * A particle is an entity with a name, mass, instantaneous position and velocity in three dimensional space.
 *
 * @author: Garrett Weaver
 */
class NBodySim::Particle {
private:
	
protected:
	virtual void particle(NBodySim::FloatingType xPos, NBodySim::FloatingType yPos, NBodySim::FloatingType zPos, NBodySim::FloatingType xVel, NBodySim::FloatingType yVel, NBodySim::FloatingType zVel, NBodySim::FloatingType massIn, std::string nameIn);
	NBodySim::ThreeVector <NBodySim::FloatingType> position;
	NBodySim::ThreeVector <NBodySim::FloatingType> velocity;
	NBodySim::FloatingType mass;
	std::string name;
public:
	Particle(void);
	Particle(NBodySim::FloatingType xPos, NBodySim::FloatingType yPos, NBodySim::FloatingType zPos, NBodySim::FloatingType xVel, NBodySim::FloatingType yVel, NBodySim::FloatingType zVel, NBodySim::FloatingType massIn, std::string nameIn);
	Particle(NBodySim::ThreeVector <NBodySim::FloatingType> pos, NBodySim::ThreeVector <NBodySim::FloatingType> vel, NBodySim::FloatingType massIn, std::string nameIn);
	virtual ~Particle(void);
	NBodySim::ThreeVector <NBodySim::FloatingType> getPos(void);
	NBodySim::ThreeVector <NBodySim::FloatingType> getVel(void);
	NBodySim::FloatingType getMass(void);
	std::string getName(void);
	void setPos(NBodySim::ThreeVector <NBodySim::FloatingType> newPosition);
	void setVel(NBodySim::ThreeVector <NBodySim::FloatingType> newVelocity);
	void setMass(NBodySim::FloatingType newMass);
	void setPosX(NBodySim::FloatingType newPosition);
	void setPosY(NBodySim::FloatingType newPosition);
	void setPosZ(NBodySim::FloatingType newPosition);
	void setVelX(NBodySim::FloatingType newPosition);
	void setVelY(NBodySim::FloatingType newPosition);
	void setVelZ(NBodySim::FloatingType newPosition);
};

#endif //PARTICLE_H
