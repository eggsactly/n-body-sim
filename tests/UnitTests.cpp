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

#include <boost/thread.hpp>
#include <boost/functional.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>

#include "gtest/gtest.h"

#include "NBodyTypes.h"
#include "Particle.h"
#include "NBodySystem.h"
#include "threads.h"


TEST(FR_Initiate, EarthMoonSun) {
	std::string xmlString = "<?xml version=\"1.0\"?>\n<system G=\"1.00\">\n\t<particle posX=\"0\" posY=\"0\" posZ=\"0\" velX=\"0\" velY=\"0\" velZ=\"0\" mass=\"1.988500e30\" name=\"Sun\"/>\n\t<particle posX=\"0\" posY=\"1.5210e11\" posZ=\"0\" velX=\"-2.929e4\" velY=\"0\" velZ=\"0\" mass=\"5.972e24\" name=\"Earth\"/>\n\t<particle posX=\"4.054e8\" posY=\"1.5210e11\" posZ=\"0\" velX=\"-2.929e4\" velY=\"-964.0f\" velZ=\"0\" mass=\"7.34767309e22\" name=\"Moon\"/>\t</system>";
	NBodySim::NBodySystem <NBodySim::FloatingType> sys;
	
	EXPECT_EQ(sys.parse(xmlString), 0);
	EXPECT_EQ(sys.numParticles(), 3);
	EXPECT_EQ(sys.getParticle(0).getName(), "Sun");
	EXPECT_EQ(sys.getParticle(1).getName(), "Earth");
	EXPECT_EQ(sys.getParticle(2).getName(), "Moon");
}

TEST(FR_Calculate, TwoMassCollide){
	NBodySim::Particle <NBodySim::FloatingType> p;
	NBodySim::NBodySystem <NBodySim::FloatingType> sys;
	NBodySim::FloatingType newXPos;
	NBodySim::FloatingType particleMass = 1000000;
	NBodySim::FloatingType margin = 0.00001;
	
	p.setPosX(1);
	p.setPosY(0);
	p.setPosZ(0);
	p.setVelX(0);
	p.setVelY(0);
	p.setVelZ(0);
	p.setMass(particleMass);
	p.setName("1");
	
	sys.addParticle(p);
	
	p.setPosX(-1);
	p.setName("2");
	
	sys.addParticle(p);
	
	EXPECT_EQ(sys.numParticles(), 2);
	EXPECT_DOUBLE_EQ(sys.getParticle(0).getPos().x, 1.00f);
	EXPECT_DOUBLE_EQ(sys.getParticle(1).getPos().x, -1.00f);
	
	sys.step(1);
	newXPos = (1 - (sys.getGravitation() * particleMass / pow(sys.getParticle(0).getPos().x - sys.getParticle(1).getPos().x, 2)));
	EXPECT_NEAR(sys.getParticle(0).getPos().x, newXPos, margin);
	
	newXPos = (sys.getGravitation() * particleMass / pow(sys.getParticle(0).getPos().x - sys.getParticle(1).getPos().x, 2)) - 1;
	EXPECT_NEAR(sys.getParticle(1).getPos().x, newXPos, margin);
}

TEST(NF_SystemsProvideG, GetGofOne){
	std::string xmlString = "<?xml version=\"1.0\"?>\n<system G=\"1.00\">\n\t<particle posX=\"0\" posY=\"0\" posZ=\"0\" velX=\"0\" velY=\"0\" velZ=\"0\" mass=\"1.988500e30\" name=\"Sun\"/>\n\t<particle posX=\"0\" posY=\"1.5210e11\" posZ=\"0\" velX=\"-2.929e4\" velY=\"0\" velZ=\"0\" mass=\"5.972e24\" name=\"Earth\"/>\n\t<particle posX=\"4.054e8\" posY=\"1.5210e11\" posZ=\"0\" velX=\"-2.929e4\" velY=\"-964.0f\" velZ=\"0\" mass=\"7.34767309e22\" name=\"Moon\"/>\t</system>";
	NBodySim::NBodySystem <NBodySim::FloatingType> sys;
	
	EXPECT_EQ(sys.parse(xmlString), 0);
	EXPECT_EQ(sys.getGravitation(), 1.0f);
}

TEST(NF_UsersProvideTime, TestPointFiveStepSize){
	NBodySim::Particle <NBodySim::FloatingType> p;
	NBodySim::NBodySystem <NBodySim::FloatingType> sys;
	NBodySim::FloatingType newXPos;
	NBodySim::FloatingType particleMass = 1000000;
	NBodySim::FloatingType margin = 0.00001;
	NBodySim::FloatingType stepSize = 0.5;
	
	p.setPosX(1);
	p.setPosY(0);
	p.setPosZ(0);
	p.setVelX(0);
	p.setVelY(0);
	p.setVelZ(0);
	p.setMass(particleMass);
	p.setName("1");
	
	sys.addParticle(p);
	
	p.setPosX(-1);
	p.setName("2");
	
	sys.addParticle(p);
	
	EXPECT_EQ(sys.numParticles(), 2);
	EXPECT_DOUBLE_EQ(sys.getParticle(0).getPos().x, 1.00f);
	EXPECT_DOUBLE_EQ(sys.getParticle(1).getPos().x, -1.00f);
	
	sys.step(stepSize);
	newXPos = (1 - (stepSize * sys.getGravitation() * particleMass / pow(sys.getParticle(0).getPos().x - sys.getParticle(1).getPos().x, 2)));
	EXPECT_NEAR(sys.getParticle(0).getPos().x, newXPos, margin);
	
	newXPos = (stepSize * sys.getGravitation() * particleMass / pow(sys.getParticle(0).getPos().x - sys.getParticle(1).getPos().x, 2)) - 1;
	EXPECT_NEAR(sys.getParticle(1).getPos().x, newXPos, margin);
}

TEST(FR_TimeAccelerate, TenStepsTest){
	/* For the time acceleration test we shall use one paticle moving at 1 m/s and run it 
	 * for 10 steps and test to see if it moved 10 meters.
	 */
	NBodySim::Particle <NBodySim::FloatingType> p;
	NBodySim::NBodySystem <NBodySim::FloatingType> sys;
	NBodySim::FloatingType newXPos;
	NBodySim::FloatingType particleMass = 1000000;
	NBodySim::FloatingType margin = 0.00001;
	NBodySim::FloatingType stepSize = 1;
	size_t numTimingSems = 1;
	boost::interprocess::interprocess_semaphore * timingSemaphore = new boost::interprocess::interprocess_semaphore(0);
	volatile bool quit = false;
	volatile size_t stepsPerTime = 10;
	NBodySim::FloatingType yVel = 1;
	size_t numIterations = 1;
	
	p.setPosX(0);
	p.setPosY(0);
	p.setPosZ(0);
	p.setVelX(0);
	p.setVelY(yVel);
	p.setVelZ(0);
	p.setMass(particleMass);
	p.setName("1");
	
	sys.addParticle(p);
	
	// Create a thread for the worker
	boost::thread workerThread(workThread, stepSize, timingSemaphore, &quit, &sys, &stepsPerTime);
	
	for(size_t i = 0; i < numIterations; i++){
		timingSemaphore->post();
		// Give the thread plenty of time to complete
		sleep(1);
	}
	quit = true;
	// Post the semaphore to force the thread to exit
	timingSemaphore->post();
	
	workerThread.join();
	
	EXPECT_DOUBLE_EQ(sys.getParticle(0).getPos().y, stepSize * stepsPerTime * numIterations * yVel);
}

TEST(NF_DynamicTimeAccelerate, FifteenStepsTest){
	/* For the time acceleration test we shall use one paticle moving at 1 m/s and run it 
	 * for 15 steps and test to see if it moved 15 meters.
	 */
	NBodySim::Particle <NBodySim::FloatingType> p;
	NBodySim::NBodySystem <NBodySim::FloatingType> sys;
	NBodySim::FloatingType newXPos;
	NBodySim::FloatingType particleMass = 1000000;
	NBodySim::FloatingType margin = 0.00001;
	NBodySim::FloatingType stepSize = 1;
	size_t numTimingSems = 1;
	boost::interprocess::interprocess_semaphore * timingSemaphore = new boost::interprocess::interprocess_semaphore(0);
	volatile bool quit = false;
	volatile size_t stepsPerTime[] = {5, 10};
	volatile size_t stepsToThread;
	NBodySim::FloatingType yVel = 1;
	size_t numIterations[sizeof(stepsPerTime)/sizeof(size_t)] = {1, 1};
	size_t totalSteps = 0;
	
	p.setPosX(0);
	p.setPosY(0);
	p.setPosZ(0);
	p.setVelX(0);
	p.setVelY(yVel);
	p.setVelZ(0);
	p.setMass(particleMass);
	p.setName("1");
	
	sys.addParticle(p);
	
	// Create a thread for the worker
	boost::thread workerThread(workThread, stepSize, timingSemaphore, &quit, &sys, &stepsToThread);
	
	// This loop simulates the user changing the time acceleration rate between timing sem posts
	for(size_t i = 0; i < sizeof(stepsPerTime)/sizeof(size_t); i++){
		stepsToThread = stepsPerTime[i];
		for(size_t j = 0; j < numIterations[i]; j++){
			timingSemaphore->post();
		}
		// Give the thread plenty of time to complete
		sleep(1);
	}
	quit = true;
	// Post the semaphore to force the thread to exit
	timingSemaphore->post();
	
	workerThread.join();
	
	for(size_t i; i < sizeof(stepsPerTime)/sizeof(size_t); i++){
		totalSteps += stepsPerTime[i] * numIterations[i];
	}
	
	EXPECT_DOUBLE_EQ(sys.getParticle(0).getPos().y, stepSize * totalSteps * yVel);
}

int main(int argc, char* argv[]){
	
	std::cout << "Running main()" << std::endl;
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
