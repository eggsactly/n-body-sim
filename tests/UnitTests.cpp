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


#include <iostream>
#include <cstdlib>

#include "NBodyTypes.h"
#include "Particle.h"
#include "NBodySystem.h"

int main(){
	
	NBodySim::ThreeVector <NBodySim::FloatingType> output;
	NBodySim::Particle p;
	p.setPosX(1);
	p.setPosY(0);
	p.setPosZ(0);
	p.setVelX(0);
	p.setVelY(0);
	p.setVelZ(0);
	p.setMass(1);
	p.setName("1");
	
	output = p.getPos();
	
	if(output.x != 1){
		std::cout << "Assertion 1 failed." << std::endl;
		return EXIT_FAILURE;
	}
	
	std::cout << "Tests passed." << std::endl;
	return EXIT_SUCCESS;
}