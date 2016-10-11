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

template <typename T>
unsigned char assert(T val1, T val2, T margin){
	if(val1 > (val2 + margin) || val1 < (val2 - margin)){
		return 1;
	}
	else{
		return 0;
	}
}

int main(){
	
	NBodySim::ThreeVector <NBodySim::FloatingType> output;
	NBodySim::Particle p;
	NBodySim::NBodySystem sys;
	NBodySim::NBodySystem sys2;
	
	double particleMass = 1000000;
	p.setPosX(1);
	p.setPosY(0);
	p.setPosZ(0);
	p.setVelX(0);
	p.setVelY(0);
	p.setVelZ(0);
	p.setMass(particleMass);
	p.setName("1");
	double margin = 0.00001;
	
	std::string xmlString = "<?xml version=\"1.0\"?>\n<system G=\"1.00\">\n\t<particle posX=\"0\" posY=\"0\" posZ=\"0\" velX=\"0\" velY=\"0\" velZ=\"0\" mass=\"1.988500e30\" name=\"Sun\"/>\n\t<particle posX=\"0\" posY=\"1.5210e11\" posZ=\"0\" velX=\"-2.929e4\" velY=\"0\" velZ=\"0\" mass=\"5.972e24\" name=\"Earth\"/>\n\t<particle posX=\"4.054e8\" posY=\"1.5210e11\" posZ=\"0\" velX=\"-2.929e4\" velY=\"-964.0f\" velZ=\"0\" mass=\"7.34767309e22\" name=\"Moon\"/>\t</system>";
	
	output = p.getPos();
	
	if(assert<NBodySim::FloatingType>(output.x, 1, 0)){
		std::cout << "Assertion 1 failed, output.x is " << output.x << " not 1." << std::endl;
		return EXIT_FAILURE;
	}
	
	sys.addParticle(p);
	
	if(assert<NBodySim::FloatingType>(sys.numParticles(), 1, 0)){
		std::cout << "Assertion 2 failed, sys.numParticles() is " << sys.numParticles() << " not 1." << std::endl;
		return EXIT_FAILURE;
	}
	
	if(assert<NBodySim::FloatingType>(sys.getParticle(0).getPos().x, 1, 0)){
		std::cout << "Assertion 3 failed, sys.getParticle(0).getPos().x is " << sys.getParticle(0).getPos().x << " not 1." << std::endl;
		return EXIT_FAILURE;
	}
	
	p.setPosX(-1);
	p.setName("2");
	
	sys.addParticle(p);
	
	if(assert<NBodySim::FloatingType>(sys.numParticles(), 2, 0)){
		std::cout << "Assertion 4 failed, sys.numParticles() is " << sys.numParticles() << " not 2." << std::endl;
		return EXIT_FAILURE;
	}
	if(assert<NBodySim::FloatingType>(sys.getParticle(0).getPos().x, 1, 0)){
		std::cout << "Assertion 5 failed, sys.getParticle(0).getPos().x is " << sys.getParticle(0).getPos().x << " not 1." << std::endl;
		return EXIT_FAILURE;
	}
	if(assert<NBodySim::FloatingType>(sys.getParticle(1).getPos().x, -1, 0)){
		std::cout << "Assertion 6 failed, sys.getParticle(1).getPos().x is " << sys.getParticle(1).getPos().x << " not -1." << std::endl;
		return EXIT_FAILURE;
	}
	
	/* Test the result of letting two bodies move towards each other for one second */
	// Verifies Req FR.Calculate
	// Verifies Req NF.UsersProvideTime
	sys.step(1);
	// Verifies Req FR.Calculate
	// Verifies Req NF.UsersProvideTime
	double newXPos = (1 - (sys.getGravitation() * particleMass / pow(sys.getParticle(0).getPos().x - sys.getParticle(1).getPos().x, 2)));
	if(assert<NBodySim::FloatingType>(sys.getParticle(0).getPos().x, newXPos, margin)){
		std::cout << "Assertion 7 failed, sys.getParticle(0).getPos().x is " << sys.getParticle(0).getPos().x << " not " << newXPos << "." << std::endl;
		return EXIT_FAILURE;
	}
	// Verifies Req FR.Calculate
	// Verifies Req NF.UsersProvideTime
	newXPos = (sys.getGravitation() * particleMass / pow(sys.getParticle(0).getPos().x - sys.getParticle(1).getPos().x, 2)) - 1;
	if(assert<NBodySim::FloatingType>(sys.getParticle(1).getPos().x, newXPos, margin)){
		std::cout << "Assertion 7 failed, sys.getParticle(1).getPos().x is " << sys.getParticle(1).getPos().x << " not " << newXPos << "." << std::endl;
		return EXIT_FAILURE;
	}
	
	/* Test parsing of an xml file */
	// Verifies Req NF.UsersProvideFile
	// Verifies Req FR.Initiate
	if(assert<unsigned char>(sys2.parse(xmlString), 0, 0)){
		std::cout << "Assertion 9 failed, sys2.parse(xmlString) is " << 1 << " not " << 0 << "." << std::endl;
		return EXIT_FAILURE;
	}
	
	// Verifies Req NF.SystemsProvideG
	// Verifies Req FR.Initiate
	if(assert<NBodySim::FloatingType>(sys2.getGravitation(), 1.00f, 0.0)){
		std::cout << "Assertion 10 failed, sys2.getGravitation() " << sys2.getGravitation() << " not " << 1.00 << "." << std::endl;
		return EXIT_FAILURE;
	}
	
	// Verifies Req FR.Initiate
	if(assert<NBodySim::UnsignedType>(sys2.numParticles(), 3, 0)){
		std::cout << "Assertion 11 failed, sys2.numParticles() " << sys2.numParticles() << " not " << 3 << "." << std::endl;
		return EXIT_FAILURE;
	}
	// Verifies Req FR.Initiate
	if(sys2.getParticle(0).getName() != "Sun"){
		std::cout << "Assertion 12 failed, sys2.getParticle(0).getName() " << sys2.getParticle(0).getName() << " not " << "Sun" << "." << std::endl;
		return EXIT_FAILURE;
	}
	// Verifies Req FR.Initiate
	if(sys2.getParticle(1).getName() != "Earth"){
		std::cout << "Assertion 13 failed, sys2.getParticle(1).getName() " << sys2.getParticle(1).getName() << " not " << "Earth" << "." << std::endl;
		return EXIT_FAILURE;
	}
	// Verifies Req FR.Initiate
	if(sys2.getParticle(2).getName() != "Moon"){
		std::cout << "Assertion 14 failed, sys2.getParticle(2).getName() " << sys2.getParticle(2).getName() << " not " << "Moon" << "." << std::endl;
		return EXIT_FAILURE;
	}
	
	std::cout << "Tests passed!" << std::endl;
	return EXIT_SUCCESS;
}
