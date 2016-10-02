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

#include <cstdlib>
#include <iostream>
#include <string.h>
#include <fstream>
#include <streambuf>
#include <getopt.h>

#include "NBodyTypes.h"
#include "Particle.h"
#include "NBodySystem.h"


/**
 * @brief main is the root function for the program
 *
 * @param argc the number of space separated words in the command
 * @param argv an array of words in the input
 * @return 0 on exit
 */
int main(int argc, char* argv[]);

/**
 * @brief readFile reads a file and outputs the contents of the file
 *
 * @param fileName is a std::string containing a path to a file to read
 * @return the contents of the file specified by fileName
 */
std::string readFile(std::string fileName);

/**
 * @brief This structure contains a list of options a user can control on the command line
 */
typedef struct {
	bool help;                       /**< Indicates whether to print the list of command line options to the user */
	std::string fileName;            /**< Indicates path to input file */
	NBodySim::FloatingType stepSize; /**< Size of simulation steps in seconds */
} argsList;

/**
 * @brief parseArgs parses input arguments from the user
 *
 * @param argc the number of space separated words in the command
 * @param argv an array of words in the input
 * @return a list of arguments selected by the user
 */

argsList parseArgs(int argc, char* argv[]);


std::string readFile(std::string fileName){
	std::string scenarioText;
	std::ifstream scenarioFile(fileName);
	
	if (scenarioFile.is_open())
	{
		scenarioFile.seekg(0, std::ios::end);
		scenarioText.reserve(scenarioFile.tellg());
		scenarioFile.seekg(0, std::ios::beg);
		scenarioText.assign((std::istreambuf_iterator<char>(scenarioFile)), std::istreambuf_iterator<char>());
		scenarioFile.close();
		return scenarioText;
	}
	else {
		std::cout << "Could not open file " << fileName << std::endl;
		return "";
	}
}

argsList parseArgs(int argc, char* argv[]){
	int c;
	int option_index = 0;
	static struct option long_options[] =
	{
		{"help",        no_argument,       0, 'h'},
		{"input-file",  required_argument, 0, 'i'},
		{"step-size",   required_argument, 0, 's'},
		{0, 0, 0, 0}
	};
	argsList output;
	
	output.help = false;
	output.fileName = "";
	output.stepSize = 1.0f;
	
	while ((c = getopt_long(argc, argv, "hi:s:", long_options, &option_index)) != -1){
		switch (c)
		{
			case 'h':
				output.help = true;
				break;
			case 'i':
				output.fileName = optarg;
				break;
			case 's':
				output.stepSize = atof(optarg);
				break;
			default:
				abort ();
				break;
		}
	}
	return output;
}

int main(int argc, char* argv[]){
	argsList inputArgs = parseArgs(argc, argv);
	std::string inputScenario;
	NBodySim::NBodySystem solarSystem;
	NBodySim::NBodySystemSpace::error solarSystemParseResult;
	
	if(inputArgs.help){
		std::cout << "Command line flags: " << std::endl;
		std::cout << "\t-i, --input-file [Filename]: Input xml file with initial conditions" << std::endl;
		std::cout << "\t-s, --step-size  [float]   : Simulation step size in seconds" << std::endl;
		std::cout << "\t-h, --help                 : Shows this help option" << std::endl;
	}
	else {
		if(inputArgs.fileName.length() == 0){
			inputScenario = readFile("inputs/SolarSystem.xml");
		}
		else {
			inputScenario = readFile(inputArgs.fileName);
		}
		
		solarSystemParseResult = solarSystem.parse(inputScenario);
		if(solarSystemParseResult == NBodySim::NBodySystemSpace::SUCCESS){
			solarSystem.step(inputArgs.stepSize);
		}
		else{
			std::cout << "Error: " << NBodySim::NBodySystem::errorToString(solarSystemParseResult) << std::endl;
		}
	}
	
	return EXIT_SUCCESS;
}
