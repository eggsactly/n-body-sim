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

#include <string>

#include "NBodyTypes.h"
#include "Particle.h"

NBodySim::Particle::Particle(void){
	particle(0, 0, 0, 0, 0, 0, 0, "");
}

NBodySim::Particle::Particle(NBodySim::FloatingType xPos, NBodySim::FloatingType yPos, NBodySim::FloatingType zPos, NBodySim::FloatingType xVel, NBodySim::FloatingType yVel, NBodySim::FloatingType zVel, NBodySim::FloatingType massIn, std::string nameIn){
	particle(xPos, yPos, zPos, xVel, yVel, zVel, massIn, nameIn);
}

NBodySim::Particle::Particle(NBodySim::ThreeVector <NBodySim::FloatingType> pos, NBodySim::ThreeVector <NBodySim::FloatingType> vel, NBodySim::FloatingType massIn, std::string nameIn){
	particle(pos.x, pos.y, pos.z, vel.x, vel.y, vel.z, massIn, nameIn);
}

void NBodySim::Particle::particle(NBodySim::FloatingType xPos, NBodySim::FloatingType yPos, NBodySim::FloatingType zPos, NBodySim::FloatingType xVel, NBodySim::FloatingType yVel, NBodySim::FloatingType zVel, NBodySim::FloatingType massIn, std::string nameIn){
	position.x = xPos;
	position.y = yPos;
	position.z = zPos;
	velocity.x = xVel;
	velocity.y = yVel;
	velocity.z = zVel;
	mass = massIn;
	name = nameIn;
}

NBodySim::Particle::~Particle(void){
	// Do nothing
}

NBodySim::ThreeVector <NBodySim::FloatingType> NBodySim::Particle::getPos(void){
	return position;
}

NBodySim::ThreeVector <NBodySim::FloatingType> NBodySim::Particle::getVel(void){
	return velocity;
}

NBodySim::FloatingType NBodySim::Particle::getMass(void){
	return mass;
}

std::string NBodySim::Particle::getName(void){
	return name;
}

void NBodySim::Particle::setPos(NBodySim::ThreeVector <NBodySim::FloatingType> newPosition){
	position = newPosition;
}

void NBodySim::Particle::setVel(NBodySim::ThreeVector <NBodySim::FloatingType> newVelocity){
	velocity = newVelocity;
}

void NBodySim::Particle::setMass(NBodySim::FloatingType newMass){
	mass = newMass;
}

void NBodySim::Particle::setPosX(NBodySim::FloatingType newPosition){
	position.x = newPosition;
}

void NBodySim::Particle::setPosY(NBodySim::FloatingType newPosition){
	position.y = newPosition;
}

void NBodySim::Particle::setPosZ(NBodySim::FloatingType newPosition){
	position.z = newPosition;
}

void NBodySim::Particle::setVelX(NBodySim::FloatingType newVelocity){
	velocity.x = newVelocity;
}

void NBodySim::Particle::setVelY(NBodySim::FloatingType newVelocity){
	velocity.y = newVelocity;
}

void NBodySim::Particle::setVelZ(NBodySim::FloatingType newVelocity){
	velocity.z = newVelocity;
}
