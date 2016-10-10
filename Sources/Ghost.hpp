/*
	file:	Ghost.hpp
	Version:	2.0
	Date:		1st May 2015
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
#ifndef __GHOST_HPP__
#define __GHOST_HPP__

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
#include "Emitter.hpp"
#include "Player.hpp"
#include "StuVector3.hpp"
using namespace stu;

// Define the detection range which is the distance the player must be within in order to affect the ghost
#define DETECTION_RANGE 3.5

// Define the hit target range which is used when moving the ghost to detect if it is within range of its next stage
#define HIT_TARGET_RANGE 2.0

// Define the speed of the movement for the ghost
#define SPEED 0.00005

// Define the amount of stages the ghost has (max = 4)
#define STAGE_LIMIT 4

// The Ghost Class
class Ghost 
{

// Public Member Functions and Variables
public:
	// Function:		Ghost(Player) - Ghost Constructor
	// Description:		The Ghost constructor sets up all of the XASound objects needed for the ghost as well as the Ghost Emitter and the
	//					Event emitter. Also calls SetUpGhost to set up the positions/settings for objects the ghost can interact with
	// In:				Player* - Pointer to the player object so that the Player's listener can be used to create the emitters and to update 
	//					them. 
	// Out:				N/a
	Ghost (Player *PlayerListener);

	// Function:		~Ghost() - Ghost Destructor
	// Description:		The ghost destructor calls StopEmitter on both of the emitters within the class to stop playing their sounds
	// In:				N/a
	// Out:				N/a
	virtual ~Ghost();

	// Function:		IsValid() - Is Valid function
	// Description:		Returns true if the sounds have been set up correctly, false otherwise.
	// In:				N/a
	// Out:				bool - True if all sounds/objects created successfully, false if not.
	bool IsValid();

	// Function:		InitialiseGhost() - Initialise Ghost function
	// Description:		This function is called from the InitialiseRoomObjects function in the Room class after the intro sounds are finished.
	//					This function sets the ghost to active, and waits until the ghosts intro sound has finished to set the new stage and
	//					move the ghost to the new stage.
	// In:				N/a
	// Out:				N/a
	void InitialiseGhost();

	// Function:		UpdateGhost() - Update Ghost function
	// Description:		This function carries out the frame updates for the ghost. The only case where this is needed is if the ghost has
	//					a new target and needs to move to that target, meaning mMoving will be set to true. This then calls 
	//					MoveGhostToNewPosition() each frame until the ghost is no longer moving.
	// In:				N/a
	// Out:				N/a
	void UpdateGhost();

	// Function:		Reset() - Reset function
	// Description:		Update the Ghost and Event emitter so that they reflect the settings of the reseted player Listener
	// In:				N/a
	// Out:				N/a
	void Reset();

	// Function:		Stop() - Stop function
	// Description:		Stop the Ghost and Event emitters from playing their sounds
	// In:				N/a
	// Out:				N/a
	void Stop();

	// Function:		GetGhostPosition() - Get Ghost Position function
	// Description:		Returns the current GhostPosition so that it can be checked in other classes 
	// In:				N/a
	// Out:				X3DAUDIO_VECTOR mGhostPosition - the current position of the ghost
	inline X3DAUDIO_VECTOR Ghost::GetGhostPosition() const { return mGhostPosition; };

	// Function:		UpdateEmitter() - Update Emitter function
	// Description:		Inline function that calls UpdateEmitter for both of the emitter objects in this class
	// In:				N/a
	// Out:				N/a
	inline void UpdateEmitter() const { mGhostEmitter->UpdateEmitter(); mEventEmitter->UpdateEmitter(); };

	// Function:		SetNewStage() - Set New Stage function
	// Description:		This function is called when the ghost needs to change its stage, and if the ghost hasn't reached its stage
	//					limit, will randomly select a stage to carry out as long as that stage is available. This ensures each play
	//					of the game is different and randomised. It then sets the ghost state, target position and path to the new stage.
	//					The Ghost yell sound is played if it is not the first movement to a stage, and the event emitter is stopped.
	// In:				N/a
	// Out:				N/a
	void SetNewStage();

	// Function:		GhostLaugh() - Ghost Laugh function
	// Description:		This function is called in the TryToCaptureGhost function in Room.cpp if the player tries to capture the ghost
	//					but isn't within the detection range. The function calls SetGhostLaugh which randomly selects one of the three
	//					laugh sounds used for the Ghost and then changes the sound of the GhostEmitter to that laugh. When the laugh
	//					finishes, the ghost emitter's sound is set back to its default sound.
	// In:				N/a
	// Out:				N/a
	void GhostLaugh();

	// The Emitter object for the Ghost emitter
	Emitter *mGhostEmitter;

	// X3DAUDIO_VECTOR positions for each stage the ghost can move to
	X3DAUDIO_VECTOR mDoorPosition;
	X3DAUDIO_VECTOR mPianoPosition;
	X3DAUDIO_VECTOR mTypeWriterPosition;
	X3DAUDIO_VECTOR mRadioPosition;

	// Bools used for the ghost class
	bool mGhostDefeated;
	bool mInitialised;
	bool mGhostActive;
	bool mAtStage;

// Private Member Functions and Variables
private:
	// Function:		SetUpGhost() - Set Up Ghost function
	// Description:		This function sets the positions of the objects that the ghost can interact with in the room, and also initialises
	//					the bools to signify these stages are available to true so that the ghost can interact with them
	// In:				N/a
	// Out:				N/a
	void SetUpGhost();

	// Function:		SetGhostActive() - Set Ghost Active function
	// Description:		This function updates and plays the ghost emitter after the player's intro has finished 
	// In:				N/a
	// Out:				N/a
	void SetGhostActive();

	// Function:		SetGhostPath() - Set Ghost Path function
	// Description:		Finds the vector between the ghost starting position and the target position from one ghost stage to the next
	//					This vector is then used to move the ghost dynamically along that path in the MoveGhostToNewPosition function
	// In:				N/a
	// Out:				N/a
	void SetGhostPath();

	// Function:		MoveGhostToNewPosition() - Move Ghost To New Position function
	// Description:		Move the ghost along the path vector found in SetGhostPath until it reaches its target position
	//					Then changes the sound used for the emitter to the sound for the current state
	// In:				N/a
	// Out:				N/a
	void MoveGhostToNewPosition();

	// Function:		DefeatGhost() - Defeat Ghost function
	// Description:		Changes the Ghost emitter sound toplay the ghost death sound, and sets the mGhostDefeated bool to true to 
	//					let the class know the ghost has been defeated
	// In:				N/a
	// Out:				N/a
	void DefeatGhost();

	// Function:		SetGhostLaugh() - Set Ghost Laugh function
	// Description:		Randomly selects one of the three ghost laugh sounds and sets it to the mGhostLaughSound used when the sound
	//					is played, so that the ghost doesn't use the same laugh each time and makes it more realistic.
	// In:				N/a
	// Out:				N/a
	void SetGhostLaugh();

	// All XASound objects for the ghost class
	XASound *mGhostDefaultSound;
	XASound *mGhostYellSound;
	XASound *mGhostLaughSound;
	XASound *mPianoSound;
	XASound *mTypeWriterSound;
	XASound *mDoorSound;
	XASound *mRadioSound;
	XASound *mVoiceIntroSound;
	XASound *mGhostLaugh1;
	XASound *mGhostLaugh2;
	XASound *mGhostLaugh3;
	XASound *mGhostDeathSound;

	// Pointer list of all XASound objects in the player class so they can be interacted with
	list<XASound*> mGhostSounds;

	// The Event Emitter object
	Emitter *mEventEmitter;

	// Enumerator for the current state of the ghost
	enum GhostState { KNOCKING, PIANO, TYPEWRITER, RADIO};
	GhostState mCurrentState;		// Holds the current state

	// Holds the reference to the Player object
	Player *mPlayerReference;

	// Holds the current ghost position, target position, and path vector
	X3DAUDIO_VECTOR mGhostPosition;
	X3DAUDIO_VECTOR mGhostTargetPosition;
	X3DAUDIO_VECTOR mGhostPath;

	// Bools for the state of the ghost class
	bool mGhostStart;
	bool mMoving;
	bool mPianoAvailable;
	bool mTypewriterAvailable;
	bool mDoorAvailable;
	bool mRadioAvailable;
	bool mGhostLaughing;

	// Counts the amount of stages the ghost has completed
	int mStageCount;
}; 
// end Ghost class.

#endif
