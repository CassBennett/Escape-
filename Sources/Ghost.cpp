/*
	file:	Ghost.cpp
	Version:	3.0
	Date:	5th May 2015
	author:	Cassie Bennett

	Exposes:	implementation of the Ghost class.

	Description:
	This is an implementation of the Ghost class that models the ghost with its movement, emitter and event emitter.
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

// Application includes.
#include "Ghost.hpp"

// Anonymous namespace defining the sound files
namespace
{
	const string RadioFile = "Sounds/RadioSound.wav";
	const string GhostIntroFile = "Sounds/GhostIntro.wav";
	const string GhostLaugh1File = "Sounds/GhostLaugh1.wav";
	const string GhostLaugh2File = "Sounds/GhostLaugh2.wav";
	const string GhostLaugh3File = "Sounds/GhostLaugh3.wav";
	const string DefaultGhostFile = "Sounds/DefaultGhostNoise.wav";
	const string TypeWriterFile = "Sounds/typewriter.wav";
	const string PianoFile = "Sounds/PianoMusic.wav";
	const string KnockingFile = "Sounds/KnockingSound.wav";
	const string GhostYellFile = "Sounds/GhostYell.wav";
	const string GhostDeathFile = "Sounds/GhostDeath.wav";
}



// Function:		Ghost(Player) - Ghost Constructor
// Description:		The Ghost constructor sets up all of the XASound objects needed for the ghost as well as the Ghost Emitter and the
//					Event emitter. Also calls SetUpGhost to set up the positions/settings for objects the ghost can interact with
// In:				Player* - Pointer to the player object so that the Player's listener can be used to create the emitters and to update 
//					them. 
// Out:				N/a
Ghost::Ghost (Player *PlayerListener)
{
	// Create the XASound objects with the strings for their file names
	mGhostDefaultSound = new XASound(DefaultGhostFile);		// Default ghost breathing sound
	mGhostSounds.push_back(mGhostDefaultSound);

	mTypeWriterSound = new XASound(TypeWriterFile);			// Typewriter sound
	mGhostSounds.push_back(mTypeWriterSound);

	mPianoSound = new XASound(PianoFile);					// Piano sound
	mGhostSounds.push_back(mPianoSound);

	mDoorSound = new XASound(KnockingFile);					// Door Sound
	mGhostSounds.push_back(mDoorSound);

	mGhostYellSound = new XASound(GhostYellFile);			// Ghost Yell Sound
	mGhostSounds.push_back(mGhostYellSound);

	mGhostLaughSound = new XASound(GhostLaugh2File);		// Ghost Laugh Sound
	mGhostSounds.push_back(mGhostLaughSound);	

	mGhostLaugh1 = new XASound(GhostLaugh1File);			// Ghost Laugh 1 Sound
	mGhostSounds.push_back(mGhostLaugh1);

	mGhostLaugh2 = new XASound(GhostLaugh2File);			// Ghost Laugh 2 Sound
	mGhostSounds.push_back(mGhostLaugh2);

	mGhostLaugh3 = new XASound(GhostLaugh3File);			// Ghost Laugh 3 Sound
	mGhostSounds.push_back(mGhostLaugh3);

	mVoiceIntroSound = new XASound(GhostIntroFile);			// Voice Intro sound
	mGhostSounds.push_back(mVoiceIntroSound);

	mRadioSound = new XASound(RadioFile);					// Radio Sound
	mGhostSounds.push_back(mRadioSound);

	mGhostDeathSound = new XASound(GhostDeathFile);			// Ghost Death Sound
	mGhostSounds.push_back(mGhostDeathSound);

	// Store the pointer to the Player object
	mPlayerReference = PlayerListener;

	// Position of the ghost in the scene
	mGhostPosition.x = 8.0f;
	mGhostPosition.y = 0.0f;
	mGhostPosition.z = -1.0f; 

	// Initialise the Ghost Path vector to 0s
	mGhostPath.x = 0.0f;
	mGhostPath.y = 0.0f;
	mGhostPath.z = 0.0f;

	// Create the emitters with their initial sounds, positions and the player listener, and if they loop or not
	mGhostEmitter = new Emitter(mVoiceIntroSound, mGhostPosition, PlayerListener->GetListenerStruct(), false);
	mEventEmitter = new Emitter(mGhostYellSound, mGhostPosition, PlayerListener->GetListenerStruct(), true);
	
	// Ghost Bool Initialisations
	mGhostStart = true;		// Ghost is created 
	mMoving = false;		// Ghost is not currently moving
	mGhostActive = false;	// Ghost is not yet active
	mInitialised = false;	// Not yet initialised
	mGhostDefeated = false;	// Not yet defeated
	mAtStage = false;		// Not yet at the state
	mGhostLaughing = false; // Ghost not laughing

	// The amount of stages the ghost has been through is currently 0
	mStageCount = 0;

	// Call SetUpGhost to set up the events of the scene
	SetUpGhost();
} 
// end Room constructor.



// Function:		~Ghost() - Ghost Destructor
// Description:		The ghost destructor calls StopEmitter on both of the emitters within the class to stop playing their sounds
// In:				N/a
// Out:				N/a
Ghost::~Ghost()
{
	// Stop both emitters
	mGhostEmitter->StopEmitter();
	mEventEmitter->StopEmitter();
} 
// end ghost destructor.
	


// Function:		IsValid() - Is Valid function
// Description:		Returns true if the sounds have been set up correctly, false otherwise.
// In:				N/a
// Out:				bool - True if all sounds/objects created successfully, false if not.
bool Ghost::IsValid()
{
	// Check through all XASound objects in the ghost class and check if they are valid
	for (auto GhostSound = mGhostSounds.begin(); GhostSound != mGhostSounds.end(); ++ GhostSound)
	{
		if (!(*GhostSound)->IsValid())
		{
			// Return false if any of the sounds are not valid
			return false;
		}
	}
	return true;
}
// End of IsValid function



// Function:		SetUpGhost() - Set Up Ghost function
// Description:		This function sets the positions of the objects that the ghost can interact with in the room, and also initialises
//					the bools to signify these stages are available to true so that the ghost can interact with them
// In:				N/a
// Out:				N/a
void Ghost::SetUpGhost()
{
	// Set up the door position
	mDoorPosition.x = 8;
	mDoorPosition.y = 0;
	mDoorPosition.z = 0;

	// Set up the piano position
	mPianoPosition.x = 2;
	mPianoPosition.y = 0;
	mPianoPosition.z = 14;

	// Set up the typewriter position
	mTypeWriterPosition.x = 2;
	mTypeWriterPosition.y = 0;
	mTypeWriterPosition.z = 2;

	// Set up the radio position
	mRadioPosition.x = 13;
	mRadioPosition.y = 0;
	mRadioPosition.z = 9;

	// Initialise Ghost Target vector to 0 for x, y and z 
	mGhostTargetPosition.x = 0;
	mGhostTargetPosition.y = 0;
	mGhostTargetPosition.z = 0;

	// Initialise Ghost Path vector to 0 for x, y and z
	mGhostPath.x = 0;
	mGhostPath.y = 0;
	mGhostPath.z = 0;

	// Set the 'Available' bools to true
	// These bools are to signify that the stages haven't been visited before, therefore they are available
	mPianoAvailable = true;
	mTypewriterAvailable = true;
	mDoorAvailable = true;
	mRadioAvailable = true;
}
// End SetUpGhost function



// Function:		InitialiseGhost() - Initialise Ghost function
// Description:		This function is called from the InitialiseRoomObjects function in the Room class after the intro sounds are finished.
//					This function sets the ghost to active, and waits until the ghosts intro sound has finished to set the new stage and
//					move the ghost to the new stage.
// In:				N/a
// Out:				N/a
void Ghost::InitialiseGhost()
{
	// If the ghost hasn't been completely initialised
	if (!mInitialised)
	{
		// If the ghost isn't currently active, set the ghost to active
		if (!mGhostActive)
		{
			SetGhostActive();
		}

		// If the ghost is active and the emitter is no longer active (the ghost intro sound has finished) and the Ghost is in its starting movement
		if ((mGhostActive) && (!mGhostEmitter->EmitterActive()) && (mGhostStart))
		{
			// Set the new stage of the ghost and set the new path
			SetNewStage();
			// Let the ghost object know it has been initialised
			mInitialised = true;
		}
	}
}
// End InitialiseGhost function


// Function:		UpdateGhost() - Update Ghost function
// Description:		This function carries out the frame updates for the ghost. The only case where this is needed is if the ghost has
//					a new target and needs to move to that target, meaning mMoving will be set to true. This then calls 
//					MoveGhostToNewPosition() each frame until the ghost is no longer moving. 
// In:				N/a
// Out:				N/a
void Ghost::UpdateGhost()
{
	// If the ghost is moving from a stage to another, call this function in order to move the ghost until it reaches it's target
	if (mMoving)
	{
		MoveGhostToNewPosition();
	}

	// If the ghost is set to laughing
	if (mGhostLaughing)
	{
		// If the ghost's laugh sound is no longer playing
		if (mGhostEmitter->EmitterActive() == false)
		{
			// Change the emitter sound back to the mGhostDefaultSound and loop it, and set GhostLaughing to false
			mGhostEmitter->ChangeEmitterSound(mGhostDefaultSound, mGhostPosition, true);
			mGhostLaughing = false;
		}
	} 
}
// End UpdateGhost function



// Function:		SetGhostActive() - Set Ghost Active function
// Description:		This function updates and plays the ghost emitter after the player's intro has finished 
// In:				N/a
// Out:				N/a
void Ghost::SetGhostActive()
{
	// Update and Play the Ghost Emitter
	mGhostEmitter->UpdateEmitter();
	mGhostEmitter->PlayEmitter();
	// Let the ghost object know it is now active
	mGhostActive = true;
}
// End SetGhostActive function



// Function:		Reset() - Reset function
// Description:		Update the Ghost and Event emitter so that they reflect the settings of the reseted player Listener
// In:				N/a
// Out:				N/a
void Ghost::Reset()
{
	mGhostEmitter->UpdateEmitter();
	mEventEmitter->UpdateEmitter();
} 
// end Reset function.




// Function:		Stop() - Stop function
// Description:		Stop the Ghost and Event emitters from playing their sounds
// In:				N/a
// Out:				N/a
void Ghost::Stop()
{
	// Call StopEmitter for each emitter which stops the sound playing for that emitter
	mGhostEmitter->StopEmitter();
	mEventEmitter->StopEmitter();
} 
// end Stop function.



// Function:		SetGhostPath() - Set Ghost Path function
// Description:		Finds the vector between the ghost starting position and the target position from one ghost stage to the next
//					This vector is then used to move the ghost dynamically along that path in the MoveGhostToNewPosition function
// In:				N/a
// Out:				N/a
void Ghost::SetGhostPath()
{
	// Get the vector for the start position of the ghost
	v3f StartingPosition = {mGhostPosition.x, mGhostPosition.y, mGhostPosition.z};
	// Get the vector for the target position of the ghost
	// This will depend on which stage the ghost is moving to next
	v3f TargetPosition = {mGhostTargetPosition.x, mGhostTargetPosition.y, mGhostTargetPosition.z};

	// Create the Path vector (y stays at 0 as ghost only moves along the x and z plane)
	X3DAUDIO_VECTOR PathVector;
	// The path vector is the direction vector from the start position to the target position (therefore = target position - starting position)
	PathVector.x = TargetPosition.x - StartingPosition.x;
	PathVector.y = 0;
	PathVector.z = TargetPosition.z - StartingPosition.z;

	// Normalise this vector and multiply it by the speed of the ghosts movement defined in Ghost.hpp
	float Length = sqrt((PathVector.x * PathVector.x) + (PathVector.y * PathVector.y) + (PathVector.z * PathVector.z));

	PathVector.x = (PathVector.x / Length) * SPEED;
	PathVector.y = (PathVector.y / Length) * SPEED;
	PathVector.z = (PathVector.z / Length) * SPEED;

	// Set mMoving to true so that the ghost will update the position in the UpdateGhost function
	mMoving = true;

	// Store the path vector needed to move this ghost along the path
	mGhostPath = PathVector;
}
// End SetGhostPath function



	// Function:		MoveGhostToNewPosition() - Move Ghost To New Position function
	// Description:		Move the ghost along the path vector found in SetGhostPath until it reaches its target position
	//					Then changes the sound used for the emitter to the sound for the current state
	// In:				N/a
	// Out:				N/a
void Ghost::MoveGhostToNewPosition()
{
	// Set the ghost position to the current position plus the ghost path vector to move the ghost along the path
	mGhostPosition.x += mGhostPath.x;
	mGhostPosition.y += mGhostPath.y;
	mGhostPosition.z += mGhostPath.z;

	// Find the distance between the ghost and its target position
	v3f GhostPos = {mGhostPosition.x, mGhostPosition.y, mGhostPosition.z };
	v3f TargetPos = {mGhostTargetPosition.x, mGhostTargetPosition.y, mGhostTargetPosition.z };
	float Distance = v3f::Distance(GhostPos, TargetPos);

	// Set the new position of the emitter and update it to change the settings to reflect the position change
	mGhostEmitter->SetPosition(mGhostPosition);
	mGhostEmitter->UpdateEmitter();

	// If the ghost is within the HIT_TARGET_RANGE of its target position, then set the ghost to its target position and change the emitter
	if (Distance < HIT_TARGET_RANGE)
	{
		// Ghost is no longer moving
		mMoving = false;
		// Set the ghost position to its actual target position and then update the emitter position and play it
		mGhostPosition = mGhostTargetPosition;
		mGhostEmitter->SetPosition(mGhostPosition);
		mEventEmitter->PlayEmitter();

		// Let class know it is currently at a stage and therefore can be captured by the player at this stage
 		mAtStage = true;

		// Check the state of the ghost to see what sound should be played for the emitter
		// Change the sound using the ChangeEmitterSound function on the EventEmitter object and pass in the appropriate sound
		if (mCurrentState == PIANO)
		{
			mEventEmitter->ChangeEmitterSound(mPianoSound, mGhostTargetPosition, true);
		}
		else if (mCurrentState == TYPEWRITER)
		{
			mEventEmitter->ChangeEmitterSound(mTypeWriterSound, mGhostTargetPosition, true);
		}
		else if (mCurrentState == KNOCKING)
		{
			mEventEmitter->ChangeEmitterSound(mDoorSound, mGhostTargetPosition, true);
		}
		else if (mCurrentState == RADIO)
		{
			mEventEmitter->ChangeEmitterSound(mRadioSound, mGhostTargetPosition, true);
		}
		// Play normal ghost breathing noise
		mGhostEmitter->ChangeEmitterSound(mGhostDefaultSound, mGhostPosition, true);
	}
}
// End MoveGhostToNewPosition function



// Function:		DefeatGhost() - Defeat Ghost function
// Description:		Changes the Ghost emitter sound toplay the ghost death sound, and sets the mGhostDefeated bool to true to 
//					let the class know the ghost has been defeated
// In:				N/a
// Out:				N/a
void Ghost::DefeatGhost()
{
	// Set the sound for the ghost emitter to mGhostDeathSound and don't loop the sound
	mGhostEmitter->ChangeEmitterSound(mGhostDeathSound, mGhostPosition, false);
	mGhostDefeated = true;		// Set this bool to true so that the class knows the ghost has been defeated
}
// End DefeatGhost function




// Function:		SetNewStage() - Set New Stage function
// Description:		This function is called when the ghost needs to change its stage, and if the ghost hasn't reached its stage
//					limit, will randomly select a stage to carry out as long as that stage is available. This ensures each play
//					of the game is different and randomised. It then sets the ghost state, target position and path to the new stage.
//					The Ghost yell sound is played if it is not the first movement to a stage, and the event emitter is stopped.
// In:				N/a
// Out:				N/a
void Ghost::SetNewStage()
{
	// Stop playing the previous stages event
	mEventEmitter->PauseEmitter();
	mAtStage = false;		// is no longer at a stage so the player can't capture the ghost as it is moving

	// If the ghost has reached the max amount of stages, the ghost has been defeated
	if (mStageCount == STAGE_LIMIT)
	{
		DefeatGhost();
		return;
	}

	bool NewStageSet = false;
	
	// Check until the new stage is set
	while (!NewStageSet)
	{
		// Find a random number between 0 and 4 which corresponds to the definitions for each stage in the Ghost.hpp file
		int NewStage = rand() % 4;
		
		// For each stage, if the stage is available, it will change the mCurrentState variable to the new state, it will set the ghost
		// target position to the stages position, it will call SetGhostPath and increase the stage count for the ghost.
		// In addition, it will make that stage no longer available, so that it can't be carried out again, and breaks out of the while loop
		// by setting NewStageSet to true.

		// If the new stage is the PIANO stage
		if (NewStage == PIANO)
		{
			if (mPianoAvailable)
			{
				mCurrentState = PIANO;
				mGhostTargetPosition = mPianoPosition;
				SetGhostPath();
				mStageCount++;
				mPianoAvailable = false;
				NewStageSet = true;
			}
		}
		// If the new stage is the KNOCKING stage
		if (NewStage == KNOCKING)
		{
			if (mDoorAvailable)
			{
				mCurrentState = KNOCKING;
				mGhostTargetPosition = mDoorPosition;
				SetGhostPath();
				mStageCount++;
				mDoorAvailable = false;
				NewStageSet = true;
			}
		}
		// If the new stage is the TYPEWRITER stage
		if (NewStage == TYPEWRITER)
		{
			if (mTypewriterAvailable)
			{
				mCurrentState = TYPEWRITER;
				mGhostTargetPosition = mTypeWriterPosition;
				SetGhostPath();
				mStageCount++;
				mTypewriterAvailable = false;
				NewStageSet = true;
			}
		}
		// If the new stage is the RADIO stage
		if (NewStage == RADIO)
		{
			if (mRadioAvailable)
			{
				mCurrentState = RADIO;
				mGhostTargetPosition = mRadioPosition;
				SetGhostPath();
				mStageCount++;
				mRadioAvailable = false;
				NewStageSet = true;

			}
		}
	}

	// If this is the first movement of the ghost from its starting position to the first stage, play the ghost laugh sound
	if (mGhostStart)
	{
		mGhostEmitter->ChangeEmitterSound(mGhostLaugh1, mGhostPosition, false);
		mGhostStart = false;
	}
	// Else if the ghost is moving it is because the player has damaged it, therefore play the mGhostYellSound for the Ghost Emitter
	else 
	{
		mEventEmitter->StopEmitter();
		mGhostEmitter->ChangeEmitterSound(mGhostYellSound, mGhostPosition, false);
	}
}
// End SetNewStage function



// Function:		GhostLaugh() - Ghost Laugh function
// Description:		This function is called in the TryToCaptureGhost function in Room.cpp if the player tries to capture the ghost
//					but isn't within the detection range. The function calls SetGhostLaugh which randomly selects one of the three
//					laugh sounds used for the Ghost and then changes the sound of the GhostEmitter to that laugh. In the Ghost
//					Update function there is a check when the laugh finishes, and the the ghost emitter's sound is set back to its 
//					default sound.
// In:				N/a
// Out:				N/a
void Ghost::GhostLaugh()
{
	mGhostLaughing = true;
	// Choose the new ghost laugh
	SetGhostLaugh();
	// Set the sound for the ghost emitter to that Ghost laugh sound object and do not loop it
	mGhostEmitter->ChangeEmitterSound(mGhostLaughSound, mGhostPosition, false);
}
// End GhostLaugh function



// Function:		SetGhostLaugh() - Set Ghost Laugh function
// Description:		Randomly selects one of the three ghost laugh sounds and sets it to the mGhostLaughSound used when the sound
//					is played, so that the ghost doesn't use the same laugh each time and makes it more realistic.
// In:				N/a
// Out:				N/a
void Ghost::SetGhostLaugh()
{
	// Find a random number between 1 and 3 and use this as the ID for the different laugh sound objects
	int GhostLaughID = rand() % 3 + 1;

	if (GhostLaughID == 1)
	{
		mGhostLaughSound = mGhostLaugh1;
	}
	if (GhostLaughID == 2)
	{
 		mGhostLaughSound = mGhostLaugh2;
	}
	if (GhostLaughID == 3)
	{
		mGhostLaughSound = mGhostLaugh3;
	}
}
// End SetGhostLaugh function

// end of code.