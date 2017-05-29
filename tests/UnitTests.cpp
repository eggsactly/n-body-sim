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

#include "gtest/gtest.h"

#include "NBodyTypes.h"
#include "Particle.h"
#include "NBodySystem.h"

TEST(Basic, Initialize) {	
	NBodySim::ThreeVector <NBodySim::FloatingType> output;
	NBodySim::Particle <NBodySim::FloatingType> p;
	NBodySim::NBodySystem <NBodySim::FloatingType> sys;
	NBodySim::NBodySystem <NBodySim::FloatingType> sys2;
	
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
	
	
	EXPECT_DOUBLE_EQ(output.x, 1.00f);
	
	sys.addParticle(p);
	
	EXPECT_EQ(sys.numParticles(), 1);
	EXPECT_DOUBLE_EQ(sys.getParticle(0).getPos().x, 1.00f);
	
	p.setPosX(-1);
	p.setName("2");
	
	sys.addParticle(p);
	
	EXPECT_EQ(sys.numParticles(), 2);
	EXPECT_DOUBLE_EQ(sys.getParticle(0).getPos().x, 1.00f);
	EXPECT_DOUBLE_EQ(sys.getParticle(1).getPos().x, -1.00f);
	
	/* Test the result of letting two bodies move towards each other for one second */
	// Verifies Req FR.Calculate
	// Verifies Req NF.UsersProvideTime
	sys.step(1);
	// Verifies Req FR.Calculate
	// Verifies Req NF.UsersProvideTime
	double newXPos = (1 - (sys.getGravitation() * particleMass / pow(sys.getParticle(0).getPos().x - sys.getParticle(1).getPos().x, 2)));
	EXPECT_NEAR(sys.getParticle(0).getPos().x, newXPos, margin);
	
	// Verifies Req FR.Calculate
	// Verifies Req NF.UsersProvideTime
	newXPos = (sys.getGravitation() * particleMass / pow(sys.getParticle(0).getPos().x - sys.getParticle(1).getPos().x, 2)) - 1;
	EXPECT_NEAR(sys.getParticle(1).getPos().x, newXPos, margin);
	
	/* Test parsing of an xml file */
	// Verifies Req NF.UsersProvideFile
	// Verifies Req FR.Initiate
	EXPECT_EQ(sys2.parse(xmlString), 0);
	
	// Verifies Req NF.SystemsProvideG
	// Verifies Req FR.Initiate
	EXPECT_DOUBLE_EQ(sys2.getGravitation(), 1.00f);
	
	// Verifies Req FR.Initiate
	EXPECT_EQ(sys2.numParticles(), 3);
	
	// Verifies Req FR.Initiate
	EXPECT_EQ(sys2.getParticle(0).getName(), "Sun");

	// Verifies Req FR.Initiate
	EXPECT_EQ(sys2.getParticle(1).getName(), "Earth");

	// Verifies Req FR.Initiate
	EXPECT_EQ(sys2.getParticle(2).getName(), "Moon");
}

int main(int argc, char* argv[]){
	
	std::cout << "Running main()" << std::endl;
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
