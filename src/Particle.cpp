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

template <class T>
NBodySim::Particle<T>::Particle(void){
	particle(0, 0, 0, 0, 0, 0, 0, "");
}

template <class T>
NBodySim::Particle<T>::Particle(T xPos, T yPos, T zPos, T xVel, T yVel, T zVel, T massIn, std::string nameIn){
	particle(xPos, yPos, zPos, xVel, yVel, zVel, massIn, nameIn);
}

template <class T>
NBodySim::Particle<T>::Particle(NBodySim::ThreeVector <T> pos, NBodySim::ThreeVector <T> vel, T massIn, std::string nameIn){
	particle(pos.x, pos.y, pos.z, vel.x, vel.y, vel.z, massIn, nameIn);
}

template <class T>
void NBodySim::Particle<T>::particle(T xPos, T yPos, T zPos, T xVel, T yVel, T zVel, T massIn, std::string nameIn){
	position.x = xPos;
	position.y = yPos;
	position.z = zPos;
	velocity.x = xVel;
	velocity.y = yVel;
	velocity.z = zVel;
	mass = massIn;
	name = nameIn;
}

template <class T>
NBodySim::Particle<T>::~Particle(void){
	// Do nothing
}

template <class T>
NBodySim::ThreeVector <T> NBodySim::Particle<T>::getPos(void){
	return position;
}

template <class T>
NBodySim::ThreeVector<T> NBodySim::Particle<T>::getVel(void){
	return velocity;
}

template <class T>
T NBodySim::Particle<T>::getMass(void){
	return mass;
}

template <class T>
std::string NBodySim::Particle<T>::getName(void){
	return name;
}

template <class T>
void NBodySim::Particle<T>::setPos(NBodySim::ThreeVector <T> newPosition){
	position = newPosition;
}

template <class T>
void NBodySim::Particle<T>::setVel(NBodySim::ThreeVector <T> newVelocity){
	velocity = newVelocity;
}

template <class T>
void NBodySim::Particle<T>::setMass(T newMass){
	mass = newMass;
}

template <class T>
void NBodySim::Particle<T>::setPosX(T newPosition){
	position.x = newPosition;
}

template <class T>
void NBodySim::Particle<T>::setPosY(T newPosition){
	position.y = newPosition;
}

template <class T>
void NBodySim::Particle<T>::setPosZ(T newPosition){
	position.z = newPosition;
}

template <class T>
void NBodySim::Particle<T>::setVelX(T newVelocity){
	velocity.x = newVelocity;
}

template <class T>
void NBodySim::Particle<T>::setVelY(T newVelocity){
	velocity.y = newVelocity;
}

template <class T>
void NBodySim::Particle<T>::setVelZ(T newVelocity){
	velocity.z = newVelocity;
}

template <class T>
void NBodySim::Particle<T>::setName(std::string nameIn){
	name = nameIn;
}