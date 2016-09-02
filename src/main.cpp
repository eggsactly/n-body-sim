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

#include <cstdlib>
#include <iostream>
#include <string.h>

#include "NBodyTypes.h"
#include "Particle.h"
#include "NBodySystem.h"

int main(int argc, char* argv[]){
	NBodySim::ThreeVector <NBodySim::FloatingType> pos;
	NBodySim::ThreeVector <NBodySim::FloatingType> vel;
	NBodySim::FloatingType mass;
	
	pos.x = 0;
	pos.y = 0;
	pos.z = 0;
	vel.x = 0;
	vel.y = 0;
	vel.z = 0;
	mass = 1.988500e30;
	NBodySim::Particle sun(pos, vel, mass, "Sun");
	
	pos.x = 0;
	pos.y = 1.5210e11;
	pos.z = 0;
	vel.x = -2.929e4;
	vel.y = 0;
	vel.z = 0;
	mass = 5.972e24;
	NBodySim::Particle earth(pos, vel, mass, "Earth");
	
	pos.x = 4.054e8;
	pos.y = 1.5210e11;
	pos.z = 0;
	vel.x = -2.929e4;
	vel.y = -964.0f;
	vel.z = 0;
	mass = 7.34767309e22;
	NBodySim::Particle::Particle moon(pos, vel, mass, "Moon");
	
	NBodySim::NBodySystem solarSystem;
	
	solarSystem.addParticle(sun);
	solarSystem.addParticle(earth);
	solarSystem.addParticle(moon);
	
	solarSystem.step(1.00f);
	
	return EXIT_SUCCESS;
}