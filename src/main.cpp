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
#include <SDL2/SDL.h>
#include <pthread.h>
#include <semaphore.h>

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
	NBodySim::FloatingType resolution; /**< resolution in meters per pixel */
	unsigned width; /**< width of window in pixels */
	unsigned length; /**< length of window in pixels */
} argsList;

/**
 * @brief parseArgs parses input arguments from the user
 *
 * @param argc the number of space separated words in the command
 * @param argv an array of words in the input
 * @return a list of arguments selected by the user
 */
argsList parseArgs(int argc, char* argv[]);

/**
 * List of Gui initialization errors
 */
typedef enum {
	/**
	 * Successful Gui initialization
	 */
	SUCCESS = 0,
	/**
	 * SDL could not be initialized
	 */
	COULD_NOT_INITIALIZE,
	/**
	 * Failed to create a window
	 */
	COULD_NOT_CREATE_WINDOW,
	/**
	 * Failed to create a renderer
	 */
	COULD_NOT_CREATE_RENDERER
} guiInitErrors;

/**
 * @brief guiInitErrorsToString converts the guiInitErrors to a std::string
 *
 * @param error error code
 * @return string representation of error code
 */
std::string guiInitErrorsToString(guiInitErrors error);

typedef struct {
	NBodySim::FloatingType interval;
	sem_t * timingSem;
	volatile unsigned char continueTiming;
} timingFunctionStruct;

/**
 * @brief timingFunction pulls a semaphore high every interval for as long as continueTiming is non zero
 *
 * @param inputParams A pointer to the timingFunctionStruct
 * @return A null pointer
 */
void * timingFunction(void * inputParams);

std::string readFile(std::string fileName){
	std::string scenarioText;
	std::ifstream scenarioFile(fileName.c_str());
	
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
		{"resolution",  required_argument, 0, 'r'},
		{"width",       required_argument, 0, 'w'},
		{"length",      required_argument, 0, 'l'},
		{0, 0, 0, 0}
	};
	argsList output;
	
	output.help = false;
	output.fileName = "";
	output.stepSize = 0.033;
	output.resolution = 0.1;
	output.length = 480;
	output.width = 640;
	
	while ((c = getopt_long(argc, argv, "hi:s:r:w:l:", long_options, &option_index)) != -1){
		switch (c)
		{
			case 'h':
				output.help = true;
				break;
			// Implements NF.UsersProvideFile
			case 'i':
				output.fileName = optarg;
				break;
			// Implements NF.UsersProvideTime
			case 's':
				output.stepSize = atof(optarg);
				break;
			case 'r':
				output.resolution = atof(optarg);
				break;
			case 'l':
				output.length = atoi(optarg);
				break;
			case 'w':
				output.width = atoi(optarg);
				break;
			default:
				abort ();
				break;
		}
	}
	return output;
}

std::string guiInitErrorsToString(guiInitErrors error){
	switch(error){
		case SUCCESS: return "success"; break;
		case COULD_NOT_INITIALIZE: return "SDL could not initialize"; break;
		case COULD_NOT_CREATE_WINDOW: return "window could not be created"; break;
		case COULD_NOT_CREATE_RENDERER: return "could not create renderer"; break;
		default: return "unknown error"; break;
	}
}

guiInitErrors guiInit(SDL_Window ** gWindow, SDL_Renderer ** gRenderer, int height, int width)
{
	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		return COULD_NOT_INITIALIZE;
	}
	else
	{
		//Create window
		*gWindow = SDL_CreateWindow( "N Body Sim", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN );
		if(*gWindow == NULL )
		{
			return COULD_NOT_CREATE_WINDOW;
		}
		else
		{
			//Get window surface
			*gRenderer = SDL_CreateRenderer( *gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if(*gRenderer == NULL){
				return COULD_NOT_CREATE_RENDERER;
			}
		}
	}
	
	return SUCCESS;
}

void close(SDL_Window * gWindow, SDL_Renderer * gRenderer)
{
	SDL_DestroyRenderer( gRenderer );
	//Destroy window
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;
	
	//Quit SDL subsystems
	SDL_Quit();
}

void * timingFunction(void * inputParams){

	if(inputParams == NULL){
		pthread_exit(NULL);
	}
	
	timingFunctionStruct * inputParamStruct = reinterpret_cast<timingFunctionStruct *>(inputParams);
	
	NBodySim::FloatingType interval = timingFunctionStruct->interval;
	sem_t * timingSem = timingFunctionStruct->timingSem;
	volatile unsigned char * continueTiming = timingFunctionStruct->continueTiming;
	
	if(timingSem == NULL){
		pthread_exit(NULL);
	}
	if(continueTiming == NULL){
		pthread_exit(NULL);
	}
	
	while((*continueTiming) != 0){
		SDL_Delay(interval);
		sem_post(timingSem);
	}
	pthread_exit(NULL);
}

int main(int argc, char* argv[]){
	argsList inputArgs = parseArgs(argc, argv);
	std::string inputScenario;
	NBodySim::NBodySystem solarSystem;
	NBodySim::NBodySystemSpace::error solarSystemParseResult;
	bool quit;
	SDL_Event e;
	sem_t timingSemaphore;
	pthread_t timingThread;
	int pthreadResponse;
	timingFunctionStruct timingStruct;
	
	//The window we'll be rendering to
	SDL_Window* gWindow = NULL;
	guiInitErrors guiErrorReturn;
	SDL_Renderer* gRenderer = NULL;
	
	if(inputArgs.help){
		std::cout << "Command line flags: " << std::endl;
		std::cout << "\t-i, --input-file [Filename]: Input xml file with initial conditions" << std::endl;
		std::cout << "\t-s, --step-size  [float]   : Simulation step size in seconds" << std::endl;
		std::cout << "\t-r, --resolution [float]   : Scale in meters per pixel" << std::endl;
		std::cout << "\t-l, --length     [int]     : Length of window in pixels" << std::endl;
		std::cout << "\t-w, --width      [int]     : Width of window in pixels" << std::endl;
		std::cout << "\t-h, --help                 : Shows this help option" << std::endl;
	}
	else {
		if(inputArgs.fileName.length() == 0){
			inputScenario = "<?xml version=\"1.0\"?><system G=\"5.483e-10\"><particle posX=\"0\" posY=\"0\" posZ=\"0\" velX=\"0\" velY=\"0\" velZ=\"0\" mass=\"1e10\" name=\"Sun\"/><particle posX=\"0\" posY=\"-10\" posZ=\"0\" velX=\"-0.5\" velY=\"0\" velZ=\"0\" mass=\"100\" name=\"Comet1\"/><particle posX=\"7\" posY=\"0\" posZ=\"0\" velX=\"0\" velY=\"-0.55\" velZ=\"0\" mass=\"100\" name=\"Comet2\"/><particle posX=\"-6\" posY=\"0\" posZ=\"0\" velX=\"0\" velY=\"0.6\" velZ=\"0\" mass=\"400\" name=\"Comet3\"/><particle posX=\"-3.5\" posY=\"3.6\" posZ=\"0\" velX=\"0.7\" velY=\"0.7\" velZ=\"0\" mass=\"900\" name=\"Comet4\"/><particle posX=\"0\" posY=\"-5\" posZ=\"0\" velX=\"-1\" velY=\"0\" velZ=\"0\" mass=\"300\" name=\"Commet5\"/><particle posX=\"-5.2\" posY=\"3\" posZ=\"0\" velX=\"0.6\" velY=\"0.9\" velZ=\"0\" mass=\"700\" name=\"Commet6\"/><particle posX=\"5.2\" posY=\"3\" posZ=\"0\" velX=\"0.3\" velY=\"-0.7\" velZ=\"0\" mass=\"500\" name=\"Commet7\"/><particle posX=\"0\" posY=\"8\" posZ=\"0\" velX=\"0.4\" velY=\"0\" velZ=\"0\" mass=\"300\" name=\"Commet8\"/><particle posX=\"7\" posY=\"-7\" posZ=\"0\" velX=\"-0.4\" velY=\"-0.5\" velZ=\"0\" mass=\"200\" name=\"Commet9\"/><particle posX=\"-1\" posY=\"-1\" posZ=\"0\" velX=\"-1.6\" velY=\"1.8\" velZ=\"0\" mass=\"200\" name=\"Commet10\"/><particle posX=\"-8.5\" posY=\"-8.5\" posZ=\"0\" velX=\"-0.3\" velY=\"0.3\" velZ=\"0\" mass=\"700\" name=\"Commet11\"/><particle posX=\"0.7\" posY=\"0.7\" posZ=\"0\" velX=\"2\" velY=\"-2\" velZ=\"0\" mass=\"100\" name=\"Commet12\"/><particle posX=\"1.1\" posY=\"0\" posZ=\"0\" velX=\"0\" velY=\"-2.2\" velZ=\"0\" mass=\"100\" name=\"Commet13\"/><particle posX=\"2.7\" posY=\"2.7\" posZ=\"0\" velX=\"0.9\" velY=\"-0.8\" velZ=\"0\" mass=\"100\" name=\"Commet14\"/><particle posX=\"8\" posY=\"8\" posZ=\"0\" velX=\"0.4\" velY=\"-0.4\" velZ=\"0\" mass=\"150\" name=\"Commet15\"/><particle posX=\"20\" posY=\"0\" posZ=\"0\" velX=\"0\" velY=\"-0.3\" velZ=\"0\" mass=\"750\" name=\"Commet16\"/><particle posX=\"1\" posY=\"-1\" posZ=\"0\" velX=\"-1.7\" velY=\"-1.9\" velZ=\"0\" mass=\"450\" name=\"Commet17\"/><particle posX=\"-18\" posY=\"0\" posZ=\"0\" velX=\"0\" velY=\"0.35\" velZ=\"0\" mass=\"750\" name=\"Commet18\"/><particle posX=\"14\" posY=\"-14\" posZ=\"0\" velX=\"-0.13\" velY=\"-0.11\" velZ=\"0\" mass=\"150\" name=\"Commet19\"/><particle posX=\"0\" posY=\"25\" posZ=\"0\" velX=\"0.22\" velY=\"-0\" velZ=\"0\" mass=\"1000\" name=\"Commet20\"/></system>";
		}
		else {
			inputScenario = readFile(inputArgs.fileName);
		}
		// Implements Req FR.Initiate
		solarSystemParseResult = solarSystem.parse(inputScenario);
		if(solarSystemParseResult == NBodySim::NBodySystemSpace::SUCCESS){
			guiErrorReturn = guiInit(&gWindow, &gRenderer, inputArgs.length, inputArgs.width);
			if(guiErrorReturn == SUCCESS){
				// Create a thread for the timer
				pthreadResponse = pthread_create(&timingSemaphore, NULL, timingFunction, reinterpret_cast<void *>(&timingStruct));
				quit = false;
				if(!pthreadResponse){
					//While application is running
					while( !quit )
					{
						//Handle events on queue
						while( SDL_PollEvent( &e ) != 0 )
						{
							//User requests quit
							if( e.type == SDL_QUIT )
							{
								quit = true;
							}
						}
					
						// Implements Req FR.Calculate
						solarSystem.step(inputArgs.stepSize);
					
						// Clear Screen
						SDL_RenderClear( gRenderer );
						// Draw all the particles as points
						for(unsigned i = 0; i < solarSystem.numParticles(); i++){
							SDL_RenderDrawPoint(gRenderer, (solarSystem.getParticle(i).getPos().x/inputArgs.resolution) + (inputArgs.width/2), (solarSystem.getParticle(i).getPos().y/inputArgs.resolution) + (inputArgs.length/2));
						}
					
						SDL_RenderPresent( gRenderer );
					
						SDL_Delay(1000 * inputArgs.stepSize);
					}
				}
				else {
					std::cout << "Error: return code from pthread: " << pthreadResponse << std::endl;
					return EXIT_FAILURE;
				}
			}
			else{
				std::cout << "Error: " << guiInitErrorsToString(guiErrorReturn) << std::endl;
				return EXIT_FAILURE;
			}
		}
		else{
			std::cout << "Error: " << NBodySim::NBodySystem::errorToString(solarSystemParseResult) << std::endl;
			return EXIT_FAILURE;
		}
	}
	
	close(gWindow, gRenderer);
	return EXIT_SUCCESS;
}
