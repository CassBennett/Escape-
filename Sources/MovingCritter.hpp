/*
	file:	MovingCritter.hpp
	Version:	2.5
	Date:	6th May 2015
	Author:	Cassie Bennett

	Exposes:	MovingCritter
	Requires:	XASound, Player, Emitter, GameTimer, StuVector3

	Description:
	This class models a group of critters that move dynamically across the room. The class alternates between two types of critters for
	the same object: Bats and Mice. This is to give the room more personality and add to the reality. 
	The critters are initialised with a new start and end position and a path to move across after an amount of time

	This class exposes behaviour to:
	*	Creation of an emitter for the critters
	*	Dynamic movement of the emitter 
	*	Randomly generate the start position of the critter however create its path towards the players position
	*	Alternate between bats and mice critters in the scene
	
*/


/*
	file:	Ghost.hpp
	Version:	2.0
	Date:		30th May 2015
	Author:	Cassie Bennett

	Exposes:	Ghost
	Requires:	XASound, Player, Emitter, StuVector

	Description:
	This class models the ghost within the game that has a position and is rendered in audio. 
	The ghost has an Emitter object for its own sounds, and an Emitter object for the events/objects the ghost interacts with
	The ghost has 4 different stages and in each stage a different sound is used for the event emitter
	For the ghost emitter, sounds such as laughter, pain, and breathing are used to tell the player where it is in the room
	as well as give the player audio feedback on if they have affected the ghost or not.
	The class holds all of the sounds related to the ghost and the objects it interacts with. 

	The class exposes behaviour to:
	*	Set up the ghost object with a Ghost emitter and an Event emitter
	*	Set up the positions of the objects within the room that the ghost interacts with
	*	Selects a new random stage out of the four possible stages for the ghost so that each playthrough is different
	*	When the ghost selects a new stage, dynamically moves the emitter from its original position to the next stages position
	*	Plays the appropriate ghost sounds for the game and the events
*/

// Define the class
#ifndef MOVING_CRITTER_HPP
#define MOVING_CRITTER_HPP

// System includes.
#include <Windows.h>
#include <X3DAudio.h>
#include <memory>
#include <list>
using std::list;

// Framework includes.
#include "XASound.hpp"
using AllanMilne::Audio::XASound;

// Application includes.
#include "Player.hpp"
#include "Emitter.hpp"
#include "Framework\GameTimer.h"
#include "StuVector3.hpp"
using namespace stu;

// Definitions for the Left and Right side target/starting values
#define LEFT_X_LIMIT -10
#define RIGHT_X_LIMIT 31

// Definitions used for identifying which side the critter starts on
#define LEFT_SIDE 1
#define RIGHT_SIDE 2

// The movement speed value for the emitter movement
#define SPEED 0.00005

// The MovingCritter class
class MovingCritter
{

// Public Member Functions and Variables
public:
	// Function:		MovingCritter(Player) - Moving Critter constructor
	// Description:		The MovingCritter constructor sets up the XASound objects for the emitter and creates the mAnimalEmitter.
	//					It stores the pointer to the Player object passed so that it can be used in other functions, and initialises
	//					the mNewCritterTimer game timer object.
	// In:				Player *PlayerListener - This is the pointer to the player object
	// Out:				N/a
	MovingCritter(Player *PlayerListener);

	// Function:		UpdateEmitter() - Update Emitter function
	// Description:		This calls the UpdateEmitter function on the mAnimalEmitter
	// In:				N/a
	// Out:				N/a
	inline void UpdateEmitter() const { mAnimalEmitter->UpdateEmitter(); };

	// Function:		~MovingCritter() - Moving Critter destructor
	// Description:		The destructor calls StopEmitter() on the mAnimalEmitter to stop the sounds from playing
	// In:				N/a
	// Out:				N/a
	~MovingCritter();

	// Function:		UpdateCritter() - Update Critter function
	// Description:		This function updates the mNewCritterTimer every frame if the critter object has been initialised
	//					and if the timer reaches above the NextCritterTime will set a new critter path, play the emitter and
	//					move the critter along this path.
	// In:				N/a
	// Out:				N/a
	void UpdateCritter();

	// Function:		InitialiseCritter() - Initialise Critter function
	// Description:		This function resets the NewCritterTimer when it is called to reset the critter and sets mInitialised to true
	//					so the class knows it has been initialised and won't initialise again.
	// In:				N/a
	// Out:				N/a
	void InitialiseCritter();

	// Function:		Reset() - Reset function
	// Description:		Update the animal emitter so it reflects the settings of the player Listener
	// In:				N/a
	// Out:				N/a
	void Reset();

	// Function:		Stop() - Stop function
	// Description:		Stop the Animal emitter from playing its sound
	// In:				N/a
	// Out:				N/a
	void Stop();

	// Function:		IsValid() - Is Valid function
	// Description:		Returns true if the sounds have been set up correctly, false otherwise.
	// In:				N/a
	// Out:				bool - True if all sounds/objects created successfully, false if not.
	bool IsValid();

	// Bool signifying if the object has been initialised or not
	bool mInitialised;

// Private Member Functions and Variables
private:


	// Function:		GetNextTime() - Get Next Time function
	// Description:		This function finds the NextCritterTime based on a random number found between the upper and lower time limits
	//					So that the time for the next critter is always slightly randomised and different
	// In:				N/a
	// Out:				N/a
	void GetNextTime();

	// Function:		MoveCritter() - Move Critter function
	// Description:		This function uses the Direction Vector set in SetCritterPath and moves the emitter along this path dynamically. 
	//					If the critter reaches its destination target X, it then switches the side of the critter and the type so that they
	//					alternate. Also pauses the emitter so that the sounds don't continue playing while the critters aren't active.
	// In:				N/a
	// Out:				N/a
	void MoveCritter();

	// Function:		SetCritterPath() - Set Critter Path function
	// Description:		This function finds the target position of the critter based on the side it is assigned to (LEFT_SIDE or RIGHT_SIDE)
	//					and its starting X position. The starting Y position depends on the type of critter it is, and the z position is 
	//					randomly generated alongside the room's z plane so it is different every time.
	//					The vector from this position to the player's position is then found, normalised, and multiplied by the movement speed
	//					The MoveCritter function then uses this vector to move the critter along this path
	// In:				N/a
	// Out:				N/a
	void SetCritterPath();

	// Store the pointer to the player object
	Player *mPlayerReference;
	
	// The Animal Emitter object
	Emitter *mAnimalEmitter; 

	// Enumerator for the type of critter
	enum AnimalType { BATS, MICE };
	AnimalType mCurrentType;

	// Sound files used for this class
	XASound *mMiceSound;
	XASound *mBatsSound;

	// Pointer list of all XASound objects in the player class so they can be interacted with
	list<XASound*> mCritterSounds;

	// Variables for the critters current state
	bool mMoving;
	bool mCritterActive;
	int RoomSide;
	int NextCritterTime; 

	// Holds the critters position
	X3DAUDIO_VECTOR mCritterPosition;
	// Holds the critters direction
	v3f mDirectionVector;
	// Holds the current target X for the critter 
	float mCritterTargetX;

	// GameTimer object to count the time between a critter movement finishes and the time when the critter should move again
	GameTimer *mNewCritterTimer;
};
// End MovingCritter class

#endif