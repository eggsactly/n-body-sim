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
#include <cmath>

#include "NBodyTypes.h"
#include "Particle.h"
#include "NBodySystem.h"

int main(){
	
	NBodySim::ThreeVector <NBodySim::FloatingType> output;
	NBodySim::Particle p;
	NBodySim::NBodySystem sys;
	double partileMass = 1000000;
	p.setPosX(1);
	p.setPosY(0);
	p.setPosZ(0);
	p.setVelX(0);
	p.setVelY(0);
	p.setVelZ(0);
	p.setMass(partileMass);
	p.setName("1");
	double margin = 0.001;
	
	output = p.getPos();
	
	if(output.x != 1){
		std::cout << "Assertion 1 failed, output.x is " << output.x << " not 1." << std::endl;
		return EXIT_FAILURE;
	}
	
	sys.addParticle(p);
	
	if(sys.numParticles() != 1){
		std::cout << "Assertion 2 failed, sys.numParticles() is " << sys.numParticles() << " not 1." << std::endl;
		return EXIT_FAILURE;
	}
	
	if(sys.getParticle(0).getPos().x != 1){
		std::cout << "Assertion 3 failed, sys.getParticle(0).getPos().x is " << sys.getParticle(0).getPos().x << " not 1." << std::endl;
		return EXIT_FAILURE;
	}
	
	p.setPosX(-1);
	p.setName("2");
	
	sys.addParticle(p);
	
	if(sys.numParticles() != 2){
		std::cout << "Assertion 4 failed, sys.numParticles() is " << sys.numParticles() << " not 2." << std::endl;
		return EXIT_FAILURE;
	}
	if(sys.getParticle(0).getPos().x != 1){
		std::cout << "Assertion 5 failed, sys.getParticle(0).getPos().x is " << sys.getParticle(0).getPos().x << " not 1." << std::endl;
		return EXIT_FAILURE;
	}
	if(sys.getParticle(1).getPos().x != -1){
		std::cout << "Assertion 6 failed, sys.getParticle(1).getPos().x is " << sys.getParticle(1).getPos().x << " not -1." << std::endl;
		return EXIT_FAILURE;
	}
	
	sys.step(1);
	
	if(sys.getParticle(0).getPos().x != 1){
		std::cout << "Assertion 7 failed, sys.getParticle(0).getPos().x is " << sys.getParticle(0).getPos().x << " not 1." << std::endl;
		return EXIT_FAILURE;
	}
	if(sys.getParticle(1).getPos().x != -1){
		std::cout << "Assertion 8 failed, sys.getParticle(1).getPos().x is " << sys.getParticle(1).getPos().x << " not -1." << std::endl;
		return EXIT_FAILURE;
	}
	
	sys.step(1);
	double newXPos = (1 - (NBodySim::G * partileMass / pow(sys.getParticle(0).getPos().x - sys.getParticle(1).getPos().x, 2)));
	if(sys.getParticle(0).getPos().x > (newXPos + margin) || sys.getParticle(0).getPos().x < (newXPos - margin)){
		std::cout << "Assertion 9 failed, sys.getParticle(0).getPos().x is " << sys.getParticle(0).getPos().x << " not " << newXPos << "." << std::endl;
		return EXIT_FAILURE;
	}
	newXPos = (NBodySim::G * partileMass / pow(sys.getParticle(0).getPos().x - sys.getParticle(1).getPos().x, 2)) - 1;
	if(sys.getParticle(1).getPos().x > (newXPos + margin) || sys.getParticle(0).getPos().x < (newXPos - margin)){
		std::cout << "Assertion 10 failed, sys.getParticle(1).getPos().x is " << sys.getParticle(1).getPos().x << " not " << newXPos << "." << std::endl;
		return EXIT_FAILURE;
	}
	
	std::cout << "Tests passed!" << std::endl;
	return EXIT_SUCCESS;
}