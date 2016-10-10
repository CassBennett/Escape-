/*
	file:	MovingCritter.cpp
	Version:	3.0
	Date:	7th May 2015
	author:	Cassie Bennett

	Exposes:	implementation of the MovingCritter class.

	Description:
	This is an implementation of the MovingCritter class that models the group of critters with its movement and emitter, finding the path
	that the emitters will use to move across the room..
	* see Ghost.hpp for details.

	All of the string file names for the XASound objects within the class are set in the anonymous namespace to be used for initialising
*/

// System includes.
#include <Windows.h>
#include <math.h>
#include <XAudio2.h>
#include <X3DAudio.h>
#include <memory>
#include <string>
#include <sstream>
#include <iostream>
using std::string;

// Framework includes.
#include "XACore.hpp"
#include "XASound.hpp"
using AllanMilne::Audio::XACore;
using AllanMilne::Audio::XASound;

// Application includes
#include "MovingCritter.hpp"

// Anonymous namespace defining the sound files
namespace 
{
	const string BatSoundFile = "Sounds/BatsSound.wav";
	const string MiceSoundFile = "Sounds/MiceSound.wav";
}



// Function:		MovingCritter(Player) - Moving Critter constructor
// Description:		The MovingCritter constructor sets up the XASound objects for the emitter and creates the mAnimalEmitter.
//					It stores the pointer to the Player object passed so that it can be used in other functions, and initialises
//					the mNewCritterTimer game timer object.
// In:				Player *PlayerListener - This is the pointer to the player object
// Out:				N/a
MovingCritter::MovingCritter(Player *PlayerListener)
{
	// Create the XASound objects needed for this class using the strings from the anonymous namespace above
	mMiceSound = new XASound(BatSoundFile);
	mBatsSound = new XASound(MiceSoundFile);

	// Store the pointer to the player object so that it can be used in other functions
	mPlayerReference = PlayerListener;

	// Initialise the critter position vector to 0 for x, y and z
	mCritterPosition.x = 0.0f;
	mCritterPosition.y = 0.0f;
	mCritterPosition.z = 0.0f;

	// Initialise the critter direction vector to 0 for x, y and z
	mDirectionVector.x = 0.0f;
	mDirectionVector.y = 0.0f;
	mDirectionVector.z = 0.0f;

	// Set up the Moving Critter
	mInitialised = false;	// Critter not initialised
	mMoving = false;		// Critter not moving
	mCritterTargetX = 0;	// Initialise critter target to 0
	RoomSide = LEFT_SIDE;	// First side is LEFT_SIDE
	mCurrentType = BATS;	// First critter type is BATS

	// Create the mAnimalEmitter object
	mAnimalEmitter = new Emitter(mMiceSound, mCritterPosition, mPlayerReference->GetListenerStruct(), true);

	// Create the NewCritterTime and reset it so that it is inialised, this timer is used to countdown when the critters movement should occur
	mNewCritterTimer = new GameTimer();
	mNewCritterTimer->Reset();

	// Find the time for the next critter to appear
	GetNextTime();
}
// End MovingCritter constructor



// Function:		~MovingCritter() - Moving Critter destructor
// Description:		The destructor calls StopEmitter() on the mAnimalEmitter to stop the sounds from playing
// In:				N/a
// Out:				N/a
MovingCritter::~MovingCritter()
{
	mAnimalEmitter->StopEmitter();
} 
// end destructor.



// Function:		UpdateCritter() - Update Critter function
// Description:		This function updates the mNewCritterTimer every frame if the critter object has been initialised
//					and if the timer reaches above the NextCritterTime will set a new critter path, play the emitter and
//					move the critter along this path.
// In:				N/a
// Out:				N/a
void MovingCritter::UpdateCritter()
{
	// If the critter object has been initialised
	if (mInitialised)
	{
		// Update the game timer every frame
		mNewCritterTimer->Tick();
		// Find the elapsed time from the last Reset() of the game timer
		float elapsedtime = mNewCritterTimer->GetElapsedTime();
		// If the critter isn't currently moving and the elapsed time exceeds the NextCritterTime value
		if ((!mMoving) && (mNewCritterTimer->GetElapsedTime() > NextCritterTime))
  		{
			// Set the new critter path and play the sound from the emitter
			SetCritterPath();
			mAnimalEmitter->PlayEmitter();
			// Find the new NextCritterTime
			GetNextTime();
		}
	}
	// If the critter is moving, call the MoveCritter function to move the emitter dynamically
	if (mMoving)
	{
		MoveCritter();
	}
}
// End UpdateCritter function



// Function:		GetNextTime() - Get Next Time function
// Description:		This function finds the NextCritterTime based on a random number found between the upper and lower time limits
//					So that the time for the next critter is always slightly randomised and different
// In:				N/a
// Out:				N/a
void MovingCritter::GetNextTime()
{
	// Set the Lower and Upper time limits for the NextCritterTime
	int LowerTimeLimit = 15;
	int UpperTimeLimit = 25;

	// Get random time between these limits and set as the new NextCritterTime
	NextCritterTime = rand() % (UpperTimeLimit - 20) + LowerTimeLimit;
}
// End GetNextTime function



// Function:		InitialiseCritter() - Initialise Critter function
// Description:		This function resets the NewCritterTimer when it is called to reset the critter and sets mInitialised to true
//					so the class knows it has been initialised and won't initialise again.
// In:				N/a
// Out:				N/a
void MovingCritter::InitialiseCritter()
{
	// If the object hasn't been initialised
	if (!mInitialised)
	{
		// Reset the timer and set mInitialised to true so that the class knows it has been initialised
		mNewCritterTimer->Reset();
		mInitialised = true;
	}
}
// End InitialiseCritter function



// Function:		MoveCritter() - Move Critter function
// Description:		This function uses the Direction Vector set in SetCritterPath and moves the emitter along this path dynamically. 
//					If the critter reaches its destination target X, it then switches the side of the critter and the type so that they
//					alternate. Also pauses the emitter so that the sounds don't continue playing while the critters aren't active.
// In:				N/a
// Out:				N/a
void MovingCritter::MoveCritter()
{
	// Increment the critters position by the direction vector 
	mCritterPosition.x += mDirectionVector.x;
	mCritterPosition.y += mDirectionVector.y;
	mCritterPosition.z += mDirectionVector.z;

	// Set the position of the emitter to the new position and update the emitter to reflect these changes
	mAnimalEmitter->SetPosition(mCritterPosition);
	mAnimalEmitter->UpdateEmitter();

	// If the Critter is moving from the left side and it reaches its target X position
	if ((RoomSide == LEFT_SIDE) && (mCritterPosition.x > mCritterTargetX))
	{
		// Pause the emitter so it stops playing
		mAnimalEmitter->PauseEmitter();
		mMoving = false;			// Set moving to false so it stops its movement
		RoomSide = RIGHT_SIDE;		// Set the RoomSide to the RIGHT_SIDE to alternate sides

		// If the current type of the critter is Bats, alternate to Mice and change the sound for the emitter
		if (mCurrentType == BATS)
		{
			mCurrentType = MICE;
			mAnimalEmitter->ChangeEmitterSound(mMiceSound, mCritterPosition, true);
			mAnimalEmitter->PauseEmitter();
		}
		// Else if the current type of the critter is Mice, alternate to Bats and change the sound for the emitter
		else if (mCurrentType == MICE)
		{
			mCurrentType = BATS;
			mAnimalEmitter->ChangeEmitterSound(mBatsSound, mCritterPosition, true);
			mAnimalEmitter->PauseEmitter();
		}
		// Reset the new critter timer so that it will now count up to the NextCritterTime
		mNewCritterTimer->Reset();
	}

	// If the Critter is moving from the right side and it reaches its target X position
	if ((RoomSide == RIGHT_SIDE) && (mCritterPosition.x < mCritterTargetX))
	{
		// Pause the emitter so it stops playing
		mAnimalEmitter->PauseEmitter();
		mMoving = false;			// Set moving to false so it stops its movement
		RoomSide = LEFT_SIDE;		// Set the RoomSide to the LEFT_SIDE to alternate sides

		// If the current type of the critter is Bats, alternate to Mice and change the sound for the emitter
		if (mCurrentType == BATS)
		{
			mCurrentType = MICE;
			mAnimalEmitter->ChangeEmitterSound(mMiceSound, mCritterPosition, true);
			mAnimalEmitter->PauseEmitter();
		}
		// Else if the current type of the critter is Mice, alternate to Bats and change the sound for the emitter
		else if (mCurrentType == MICE)
		{
			mCurrentType = BATS;
			mAnimalEmitter->ChangeEmitterSound(mBatsSound, mCritterPosition, true);
			mAnimalEmitter->PauseEmitter();
		}
		// Reset the new critter timer so that it will now count up to the NextCritterTime
		mNewCritterTimer->Reset();
	}

}
// End MoveCritter() function



// Function:		SetCritterPath() - Set Critter Path function
// Description:		This function finds the target position of the critter based on the side it is assigned to (LEFT_SIDE or RIGHT_SIDE)
//					and its starting X position. The starting Y position depends on the type of critter it is, and the z position is 
//					randomly generated alongside the room's z plane so it is different every time.
//					The vector from this position to the player's position is then found, normalised, and multiplied by the movement speed
//					The MoveCritter function then uses this vector to move the critter along this path
// In:				N/a
// Out:				N/a
void MovingCritter::SetCritterPath()
{
	v3f CritterPosition;
	
	// Get critter positions and target positions based on their RoomSide
	if (RoomSide == LEFT_SIDE)
	{
		mCritterTargetX = RIGHT_X_LIMIT;
		CritterPosition.x = LEFT_X_LIMIT;
	}
	else if (RoomSide == RIGHT_SIDE)
	{
		mCritterTargetX = LEFT_X_LIMIT;
		CritterPosition.x = RIGHT_X_LIMIT;
	}

	// Set the Y position of the critter based on the type of critter they are
	if (mCurrentType == BATS)
	{
		CritterPosition.y = 1.0f;
	}
	else if (mCurrentType == MICE)
	{
		CritterPosition.y = 0.0f;
	}

	// Get Critter Start Z which is random between the height of the room value and 1
	CritterPosition.z = float((rand() % 16) + 1);
	
	// Find the player's position (discard the Y because only moving in the x and z plane)
	v3f PlayerPosition = { mPlayerReference->GetPosition().x, CritterPosition.y, mPlayerReference->GetPosition().z } ;
	// Find the direction vector between the critters position and the players, normalise it and multiply it by the movement speed
	mDirectionVector = PlayerPosition - CritterPosition;
	mDirectionVector.Normalise();
	mDirectionVector *= SPEED;
	// Set mMoving to true so that UpdateCritter will call MoveCritter to move the emitter along this path
	mMoving = true;
}
// End SetCritterPath function



// Function:		Reset() - Reset function
// Description:		Update the animal emitter so it reflects the settings of the player Listener
// In:				N/a
// Out:				N/a
void MovingCritter::Reset()
{
	// Update the animal emitter because the player will have a new position/orientation after reseting
	mAnimalEmitter->UpdateEmitter();
} 
// end Reset function.


// Function:		Stop() - Stop function
// Description:		Stop the Animal emitter from playing its sound
// In:				N/a
// Out:				N/a
void MovingCritter::Stop()
{
	mAnimalEmitter->StopEmitter();
}

// Function:		IsValid() - Is Valid function
// Description:		Returns true if the sounds have been set up correctly, false otherwise.
// In:				N/a
// Out:				bool - True if all sounds/objects created successfully, false if not.
bool MovingCritter::IsValid()
{
	// Check through all XASound objects in the critter class and check if they are valid
	for (auto CritterSound = mCritterSounds.begin(); CritterSound != mCritterSounds.end(); ++ CritterSound)
	{
		if (!(*CritterSound)->IsValid())
		{
			// Return false if any of the sounds are not valid
			return false;
		}
	}
	return true;
}
// End of IsValid function

// end of code.