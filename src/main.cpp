/*
 * Copyright (c) 2016-2017 W.A. Garrett Weaver
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
#include <sstream>
#include <fstream>
#include <streambuf>
#include <getopt.h>
#ifdef _WIN32
	#include <SDL.h>
	// Keep redeclaration of timespec
	#define HAVE_STRUCT_TIMESPEC
	// Undefine main because of sdl
	#undef main
#else
	// Mac OS X and Linux
	#include <SDL2/SDL.h>
#endif
#include <fcntl.h>
#include <sys/stat.h>
#include <cmath>
#include <errno.h>

#include <boost/thread.hpp>
#include <boost/functional.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/io.hpp>

#include "NBodyTypes.h"
#include "Particle.h"
#include "NBodySystem.h"

const static std::string programName = "n-body-sim";

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
	COULD_NOT_CREATE_RENDERER,
	/**
	 * Failed to create surface
	 */
	COULD_NOT_CREATE_SURFACE,
	/**
	 * Could not create texture
	 */
	COULD_NOT_CREATE_TEXTURE,
	/**
	 * Time warp buttons null
	 */
	TIME_WARP_BUTTONS_NULL
} guiInitErrors;

// Copied and pasted from Lazyfoo.net's SDL mouse events tutorial
enum LButtonSprite
{
	BUTTON_SPRITE_MOUSE_OUT = 0,
	BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
	BUTTON_SPRITE_MOUSE_DOWN = 2,
	BUTTON_SPRITE_MOUSE_UP = 3,
	BUTTON_SPRITE_TOTAL = 4
};
//The mouse button
class LButton
{
public:
	//Initializes internal variables
	LButton();
	
	//Sets top left position
	void setPosition( int x, int y );
	
	void setHeightWidth(int x, int y);
	
	//Handles mouse event
	bool handleEvent( SDL_Event* e );
	
private:
	//Top left position
	SDL_Point mPosition;
	SDL_Point heightWidth;
	
	//Currently used global sprite
	LButtonSprite mCurrentSprite;
};

LButton::LButton()
{
	mPosition.x = 0;
	mPosition.y = 0;
	heightWidth.x = 0;
	heightWidth.y = 0;
	
	mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
}

void LButton::setPosition( int x, int y )
{
	mPosition.x = x;
	mPosition.y = y;
}

void LButton::setHeightWidth( int x, int y )
{
	heightWidth.x = x;
	heightWidth.y = y;
}

bool LButton::handleEvent( SDL_Event* e)
{
	//If mouse event happened
	if( (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP) )
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState( &x, &y );
		
		//Check if mouse is in button
		bool inside = true;
		
		//Mouse is left of the button
		if( x < mPosition.x )
		{
			inside = false;
		}
		//Mouse is right of the button
		else if( x > mPosition.x + heightWidth.x )
		{
			inside = false;
		}
		//Mouse above the button
		else if( y < mPosition.y )
		{
			inside = false;
		}
		//Mouse below the button
		else if( y > mPosition.y + heightWidth.y )
		{
			inside = false;
		}
		
		//Mouse is outside button
		if( !inside )
		{
			return false;
		}
		//Mouse is inside button
		else
		{
			//Set mouse over sprite
			switch( e->type )
			{
				case SDL_MOUSEMOTION:
					return false;
					break;
					
				case SDL_MOUSEBUTTONDOWN:
					return false;
					break;
					
				case SDL_MOUSEBUTTONUP:
					return true;
					break;
					
				default:
					return false;
					break;
			}
		}
	}
	return false;
}

/**
 * @brief guiInitErrorsToString converts the guiInitErrors to a std::string
 *
 * @param error error code
 * @return string representation of error code
 */
std::string guiInitErrorsToString(guiInitErrors error);

typedef struct {
	NBodySim::FloatingType interval;
	unsigned numSems;
	boost::interprocess::interprocess_semaphore ** timingSems;
	volatile bool * quitTiming;
} timingFunctionStruct;

/**
 * @brief timingFunction pulls a semaphore high every interval for as long as quitTiming is false
 *
 * @param inputParams A pointer to the timingFunctionStruct
 * @return A null pointer
 */
void * timingFunction(void * inputParams);

typedef struct {
	NBodySim::FloatingType stepSize;
	boost::interprocess::interprocess_semaphore * timingSem;
	volatile bool * quitTiming;
	NBodySim::NBodySystem<NBodySim::FloatingType> * solarSystem;
	volatile size_t * stepsPerTime;
} workThreadStruct;

/**
 * @brief workThread calculates new positions and velocities for the particle vector till program close
 * The Proletariat will rise and overthrow the Bourgeoisie.
 *
 * @param inputParams A pointer to the workerThreadStruct
 * @return A null pointer
 */
void * workThread(void * inputParams);

/**
 * @brief drawTrianle draws a triangle at position x, y with height and width, users can fill in the triangle too
 * @param gRenderer place to draw the trianlge
 * @param x location in the x plane
 * @param y location in the y plane
 * @param height of triangle
 * @param width of triangle
 * @param whether the triangle should be filled in
 */
void drawTriangle(SDL_Renderer * gRenderer, int x, int y, int height, int width, unsigned char fillIn);


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
		std::cerr << programName << ": Error: Could not open file :" << fileName << std::endl;
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
		case COULD_NOT_CREATE_SURFACE: return "could not create surface"; break;
		case COULD_NOT_CREATE_TEXTURE: return "could not create texture"; break;
		case TIME_WARP_BUTTONS_NULL: return "timw warp buttons null"; break;
		default: return "unknown error"; break;
	}
}

guiInitErrors guiInit(SDL_Window ** gWindow, SDL_Renderer ** gRenderer, SDL_Surface ** timeAccelSurf, SDL_Texture ** timeAccelTex, LButton ** timeAccelButtons, int height, int width, unsigned numTimeWarpFactors, unsigned triangleMargin, unsigned triangleWidth, unsigned triangleHeight)
{
	Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif
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
			else{
				*timeAccelSurf = SDL_CreateRGBSurface(0, width, height, 32, rmask, gmask, bmask, amask);
				if(*timeAccelSurf == NULL){
					return COULD_NOT_CREATE_SURFACE;
				}
				else {
					*timeAccelTex = SDL_CreateTextureFromSurface( *gRenderer, *timeAccelSurf );
					if(*timeAccelTex == NULL){
						return COULD_NOT_CREATE_TEXTURE;
					}
					else {
						if(timeAccelButtons == NULL){
							return TIME_WARP_BUTTONS_NULL;
						}
						else{/*
							// Create the time acceleration buttons
							for(int i = 0; i < numTimeWarpFactors; i++){
								timeAccelButtons[i]->setPosition(triangleMargin + (triangleWidth + triangleMargin) * i, triangleMargin);
								timeAccelButtons[i]->setHeightWidth(triangleWidth, triangleHeight);
							}*/
						}
					}
				}
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
		return NULL;
	}
	
	timingFunctionStruct * inputParamStruct = reinterpret_cast<timingFunctionStruct *>(inputParams);
	
	NBodySim::FloatingType interval = inputParamStruct->interval;
	unsigned numSems = inputParamStruct->numSems;
	boost::interprocess::interprocess_semaphore ** timingSems = inputParamStruct->timingSems;
	volatile bool * quitTiming = inputParamStruct->quitTiming;
	
	if(timingSems == NULL){
		return NULL;
	}
	for(unsigned i = 0; i < numSems; i++){
		if(timingSems[i] == NULL){
			return NULL;
		}
	}
	if(quitTiming == NULL){
		return NULL;
	}
	
	// When each interval passes, raise up all the semaphores passed to this function.
	while(!(*quitTiming)){
		SDL_Delay(interval);
		for(unsigned i = 0; i < numSems; i++){
			timingSems[i]->post();
		}
	}

	return NULL;
}

void * workThread(void * inputParams){
	if(inputParams == NULL){
		return NULL;
	}
	
	workThreadStruct * inputParamStruct = reinterpret_cast<workThreadStruct *>(inputParams);
	
	NBodySim::FloatingType stepSize = inputParamStruct->stepSize;
	boost::interprocess::interprocess_semaphore * timingSem = inputParamStruct->timingSem;
	volatile bool * quitTiming = inputParamStruct->quitTiming;
	NBodySim::NBodySystem<NBodySim::FloatingType> * solarSystem = inputParamStruct->solarSystem;
	volatile size_t * stepsPerTime = inputParamStruct->stepsPerTime;
	
	if(timingSem == NULL){
		return NULL;
	}
	if(quitTiming == NULL){
		return NULL;
	}
	if(solarSystem == NULL){
		return NULL;
	}
	if(stepsPerTime == NULL){
		return NULL;
	}
	
	// For each iteration, wait on a semaphore
	while(!(*quitTiming)){
		timingSem->wait();
		// Implements Req FR.Calculate
		for(size_t i = 0; i < *stepsPerTime; i++){
			solarSystem->step(stepSize);
		}
	}

	return NULL;

}

// Source: http://www.crystalclearsoftware.com/cgi-bin/boost_wiki/wiki.pl?LU_Matrix_Inversion
template<class T> 
bool InvertMatrix (const boost::numeric::ublas::matrix<T>& input, boost::numeric::ublas::matrix<T>& inverse) { 
    typedef  boost::numeric::ublas::permutation_matrix<std::size_t> pmatrix; 
    // create a working copy of the input 
    boost::numeric::ublas::matrix<T> A(input); 
    // create a permutation matrix for the LU-factorization 
    pmatrix pm(A.size1()); 
    // perform LU-factorization 
    int res = lu_factorize(A,pm); 
    if( res != 0 )
        return false; 
    // create identity matrix of "inverse" 
    inverse.assign(boost::numeric::ublas::identity_matrix<T>(A.size1())); 
    // backsubstitute to get the inverse 
    boost::numeric::ublas::lu_substitute(A, pm, inverse); 
    return true; 
}

void drawTriangle(SDL_Renderer * gRenderer, int x, int y, int height, int width, unsigned char fillIn){
	int calculatedWidth = static_cast<int>(static_cast<double>(width) * sqrt(3.00f)/2.00f);
	int slope = height / width;
	SDL_Point triangle[4] = {{x, y}, {x + calculatedWidth, y + height/2}, {x, y + height}, {x, y}};
	
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	
	for(int i = 0; fillIn && i < height/2; i++){
		for(int j = 0; j < slope * i + 1; j++){
			SDL_RenderDrawPoint(gRenderer, j + x, i + y);
		}
	}
	for(int i = 0; fillIn && i < height/2; i++){
		for(int j = (slope * (height/2 - i)) + 1; j >= 0; j--){
			SDL_RenderDrawPoint(gRenderer, j + x, i + y + height/2);
		}
	}
	// Set color for fill in
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	// Draw time acceleration arrows
	SDL_RenderDrawLines(gRenderer, triangle, 4);
}

int main(int argc, char* argv[]){
	// KSP style time warp factors
	const size_t timeWarpFactors[] = {1, 2, 3, 4, 5, 10, 50, 1000, 10000, 100000};
	const int triangleWidth = 10;
	const int triangleHeight = 10;
	const int triangleMargin = 5;
	const NBodySim::FloatingType thetaChangePerSecond = M_PI; // In radians
	const NBodySim::FloatingType phiChangePerSecond = M_PI; // In radians
	unsigned timeWarpLevel = 0;
	//Buttons objects
	LButton * gButtons;
	gButtons = new LButton[ sizeof(timeWarpFactors) / sizeof(const size_t) ];
	if(gButtons == NULL){
		std::cerr << programName << ": Failed to allocate gButtons." << std::endl;
		return EXIT_FAILURE;
	}
	else {
		for(int i = 0; i < sizeof(timeWarpFactors) / sizeof(const size_t); i++){
			gButtons[i].setPosition(triangleMargin + (triangleWidth + triangleMargin) * i, triangleMargin);
			gButtons[i].setHeightWidth(triangleWidth, triangleHeight);
		}
	}
	const unsigned numTimingSems = 2;
	argsList inputArgs = parseArgs(argc, argv);
	std::string inputScenario;
	NBodySim::NBodySystem<NBodySim::FloatingType> solarSystem;
	NBodySim::NBodySystemSpace::error solarSystemParseResult;
	volatile bool quit;
	volatile size_t stepsPerTime = timeWarpFactors[timeWarpLevel];
	SDL_Event e;
	boost::interprocess::interprocess_semaphore ** timingSemaphores;
	timingSemaphores = new boost::interprocess::interprocess_semaphore*[numTimingSems];
	if(timingSemaphores == NULL){
		std::cerr << programName << ": Error: Could not allocate semaphore array." << std::endl;
		return EXIT_FAILURE;
	}
	// Range: 0 <= Theta < 2 * PI
	NBodySim::FloatingType theta = 0;
	// Range: 0 <= Phi <= PI
	NBodySim::FloatingType phi = 0;
	boost::numeric::ublas::matrix<NBodySim::FloatingType> a (3, 3);
	boost::numeric::ublas::matrix<NBodySim::FloatingType> A (3, 3);
	boost::numeric::ublas::vector<NBodySim::FloatingType> projectedPoints(3);
	boost::numeric::ublas::vector<NBodySim::FloatingType> particlePoints(3);
	
	Uint32 startTime = SDL_GetTicks();
	Uint32 stopTime = SDL_GetTicks();
	Uint32 ticksPerFrame;
	
	timingFunctionStruct timingStruct;
	workThreadStruct workerStruct;
	
	// Initialize the semaphores
	for(unsigned i = 0; i < numTimingSems; i++){
		timingSemaphores[i] = new boost::interprocess::interprocess_semaphore(0);
	}
	
	timingStruct.interval = 1000 * inputArgs.stepSize;
	timingStruct.numSems = numTimingSems;
	timingStruct.timingSems = timingSemaphores;
	timingStruct.quitTiming = &quit;

	workerStruct.stepSize = inputArgs.stepSize;
	workerStruct.timingSem = timingSemaphores[1];
	workerStruct.quitTiming = &quit;
	workerStruct.solarSystem = &solarSystem;
	workerStruct.stepsPerTime = &stepsPerTime;
	
	//The window we'll be rendering to
	SDL_Window* gWindow = NULL;
	guiInitErrors guiErrorReturn;
	SDL_Renderer* gRenderer = NULL;
	SDL_Surface* timeAccelSurf = NULL;
	SDL_Texture * timeAccelTex = NULL;
	
	if(inputArgs.help){
		std::cout << "Command line flags: " << std::endl;
		std::cout << "\t-i, --input-file [Filename]: Input xml file with initial conditions" << std::endl;
		std::cout << "\t-s, --step-size  [float]   : Simulation step size in seconds" << std::endl;
		std::cout << "\t-r, --resolution [float]   : Scale in meters per pixel" << std::endl;
		std::cout << "\t-l, --length     [int]     : Length of window in pixels" << std::endl;
		std::cout << "\t-w, --width      [int]     : Width of window in pixels" << std::endl;
		std::cout << "\t-h, --help                 : Shows this help option" << std::endl;
		return EXIT_SUCCESS;
	}

	if(inputArgs.fileName.length() == 0){
		inputScenario = "<?xml version=\"1.0\"?><system G=\"5.483e-10\"><particle posX=\"0\" posY=\"0\" posZ=\"0\" velX=\"0\" velY=\"0\" velZ=\"0\" mass=\"1e10\" name=\"Sun\"/><particle posX=\"0\" posY=\"-10\" posZ=\"0\" velX=\"-0.5\" velY=\"0\" velZ=\"0\" mass=\"100\" name=\"Comet1\"/><particle posX=\"7\" posY=\"0\" posZ=\"0\" velX=\"0\" velY=\"-0.55\" velZ=\"0\" mass=\"100\" name=\"Comet2\"/><particle posX=\"-6\" posY=\"0\" posZ=\"0\" velX=\"0\" velY=\"0.6\" velZ=\"0\" mass=\"400\" name=\"Comet3\"/><particle posX=\"-3.5\" posY=\"3.6\" posZ=\"0\" velX=\"0.7\" velY=\"0.7\" velZ=\"0\" mass=\"900\" name=\"Comet4\"/><particle posX=\"0\" posY=\"-5\" posZ=\"0\" velX=\"-1\" velY=\"0\" velZ=\"0\" mass=\"300\" name=\"Commet5\"/><particle posX=\"-5.2\" posY=\"3\" posZ=\"0\" velX=\"0.6\" velY=\"0.9\" velZ=\"0\" mass=\"700\" name=\"Commet6\"/><particle posX=\"5.2\" posY=\"3\" posZ=\"0\" velX=\"0.3\" velY=\"-0.7\" velZ=\"0\" mass=\"500\" name=\"Commet7\"/><particle posX=\"0\" posY=\"8\" posZ=\"0\" velX=\"0.4\" velY=\"0\" velZ=\"0\" mass=\"300\" name=\"Commet8\"/><particle posX=\"7\" posY=\"-7\" posZ=\"0\" velX=\"-0.4\" velY=\"-0.5\" velZ=\"0\" mass=\"200\" name=\"Commet9\"/><particle posX=\"-1\" posY=\"-1\" posZ=\"0\" velX=\"-1.6\" velY=\"1.8\" velZ=\"0\" mass=\"200\" name=\"Commet10\"/><particle posX=\"-8.5\" posY=\"-8.5\" posZ=\"0\" velX=\"-0.3\" velY=\"0.3\" velZ=\"0\" mass=\"700\" name=\"Commet11\"/><particle posX=\"0.7\" posY=\"0.7\" posZ=\"0\" velX=\"2\" velY=\"-2\" velZ=\"0\" mass=\"100\" name=\"Commet12\"/><particle posX=\"1.1\" posY=\"0\" posZ=\"0\" velX=\"0\" velY=\"-2.2\" velZ=\"0\" mass=\"100\" name=\"Commet13\"/><particle posX=\"2.7\" posY=\"2.7\" posZ=\"0\" velX=\"0.9\" velY=\"-0.8\" velZ=\"0\" mass=\"100\" name=\"Commet14\"/><particle posX=\"8\" posY=\"8\" posZ=\"0\" velX=\"0.4\" velY=\"-0.4\" velZ=\"0\" mass=\"150\" name=\"Commet15\"/><particle posX=\"20\" posY=\"0\" posZ=\"0\" velX=\"0\" velY=\"-0.3\" velZ=\"0\" mass=\"750\" name=\"Commet16\"/><particle posX=\"1\" posY=\"-1\" posZ=\"0\" velX=\"-1.7\" velY=\"-1.9\" velZ=\"0\" mass=\"450\" name=\"Commet17\"/><particle posX=\"-18\" posY=\"0\" posZ=\"0\" velX=\"0\" velY=\"0.35\" velZ=\"0\" mass=\"750\" name=\"Commet18\"/><particle posX=\"14\" posY=\"-14\" posZ=\"0\" velX=\"-0.13\" velY=\"-0.11\" velZ=\"0\" mass=\"150\" name=\"Commet19\"/><particle posX=\"0\" posY=\"25\" posZ=\"0\" velX=\"0.22\" velY=\"-0\" velZ=\"0\" mass=\"1000\" name=\"Commet20\"/></system>";
	}
	else {
		inputScenario = readFile(inputArgs.fileName);
	}
	
	// Implements Req FR.Initiate
	solarSystemParseResult = solarSystem.parse(inputScenario);
	if(solarSystemParseResult != NBodySim::NBodySystemSpace::SUCCESS){
		std::cerr << programName << ": Error: " << NBodySim::NBodySystem<NBodySim::FloatingType>::errorToString(solarSystemParseResult) << std::endl;
		return EXIT_FAILURE;
	}
	
	// Start the GUI
	guiErrorReturn = guiInit(&gWindow, &gRenderer, &timeAccelSurf, &timeAccelTex, &gButtons, inputArgs.length, inputArgs.width, sizeof(timeWarpFactors) / sizeof(const size_t),  triangleMargin, triangleWidth, triangleHeight);
	if(guiErrorReturn != SUCCESS){
		std::cerr << programName << ": Error: " << guiInitErrorsToString(guiErrorReturn) << std::endl;
		return EXIT_FAILURE;
	}
	
	quit = false;
	// Create a thread for the timer
	boost::thread timingThread(timingFunction, reinterpret_cast<void *>(&timingStruct));
	// Create a thread for the worker
	boost::thread workerThread(workThread, reinterpret_cast<void *>(&workerStruct));
	
	//While application is running
	while( !quit )
	{
		stopTime = SDL_GetTicks();
		ticksPerFrame = stopTime - startTime;
		startTime = stopTime;
		if(ticksPerFrame <= 0){
			ticksPerFrame = 1;
		}
		//Handle events on queue
		while( SDL_PollEvent( &e ) != 0 )
		{
			//User requests quit
			if( e.type == SDL_QUIT )
			{
				quit = true;
			}
			//User presses a key
			else if( e.type == SDL_KEYDOWN )
			{
				switch( e.key.keysym.sym )
				{
					case SDLK_UP:
						phi -= (phiChangePerSecond * ticksPerFrame) / 1000.0f;
						if(phi < 0){
							phi = 0.0f;
						}
						break;
					
					case SDLK_DOWN:
						phi += (phiChangePerSecond * ticksPerFrame) / 1000.0f;
						if(phi > M_PI){
							phi = M_PI;
						}
						break;
						
					default:
						// Do nothing
						break;
				}
				switch( e.key.keysym.sym )
				{
					case SDLK_LEFT:
						theta += (thetaChangePerSecond * ticksPerFrame) / 1000.0f;
						if(theta >= (2.0f * M_PI)){
							theta = theta - (2.0f * M_PI);
						}
						break;
						
					case SDLK_RIGHT:
						theta -= (thetaChangePerSecond * ticksPerFrame) / 1000.0f;
						if(theta < 0.0f){
							theta = (2 * M_PI) + theta;
						}
						break;
						
						default:
						// Do nothing
						break;
				}
			}
			//Handle button events
			for( unsigned i = 0; i < sizeof(timeWarpFactors)/sizeof(const size_t); i++ )
			{
				if(gButtons[i].handleEvent(&e)){
					timeWarpLevel = i;
				}
			}
		}
		
		// Calculate the graphics matrix
		a(0, 0) = cos(theta);
		a(0, 1) = -1.0f * sin(theta) * cos(phi);
		a(0, 2) = -1.0f * sin(theta) * sin(phi);
		a(1, 0) = sin(theta);
		a(1, 1) = cos(theta) * cos(phi);
		a(1, 2) = cos(theta) * sin(phi);
		a(2, 0) = 0.0f;
		a(2, 1) = sin(phi);
		a(2, 2) = -1.0f * cos(phi);
		
		// Invert the graphics matrix
		InvertMatrix (a, A);
		
		// Clear Screen
		SDL_RenderClear( gRenderer );
		
		//Render texture to screen
		SDL_RenderCopy( gRenderer, timeAccelTex, NULL, NULL );
		
		// Set the color of all the arrows to white
		SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
		
		// Draw time acceleration arrows
		for(unsigned i = 0; i < sizeof(timeWarpFactors)/sizeof(const size_t); i++){
			drawTriangle(gRenderer, triangleMargin + (triangleWidth + triangleMargin) * i, triangleMargin, triangleHeight, triangleWidth, timeWarpLevel >= i);
		}
		
		SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
		// Draw all the particles as points
		for(unsigned i = 0; i < solarSystem.numParticles(); i++){
			particlePoints(0) = solarSystem.getParticle(i).getPos().x;
			particlePoints(1) = solarSystem.getParticle(i).getPos().y;
			particlePoints(2) = solarSystem.getParticle(i).getPos().z;
			
			// Calculate the projected points of the particles onto the plane
			boost::numeric::ublas::axpy_prod(A, particlePoints, projectedPoints, true);
			
			SDL_RenderDrawPoint(gRenderer, (projectedPoints(0)/inputArgs.resolution) + (inputArgs.width/2), (projectedPoints(1)/inputArgs.resolution) + (inputArgs.length/2));
		}
		SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0x00 );
		
		SDL_RenderPresent( gRenderer );
		
		stepsPerTime = timeWarpFactors[timeWarpLevel];
	}
	
	workerThread.join();
	timingThread.join();
	
	close(gWindow, gRenderer);
	delete [] gButtons;
	return EXIT_SUCCESS;
}
