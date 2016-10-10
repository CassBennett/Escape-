/*
	--Original--
	File:	Player.hpp
	Version:	1.0
	Date:	19th March 2014.
	Author:	Allan c. Milne.

	--Edit--
	Edited by: Cassie Bennett
	Version:	3.0
	Date:	25th April 2015

	Exposes:	Player.
	Requires:	XASound, Listener, GameTimer.

	Description:
	This class models a player within a 3D game that is rendered in audio.
	The class is based on the Player class provided by Allan Milne in the RoomEscape solution and expanded/edited for this application
	The class holds all of the sounds that are related to the Player and also a Listener object which is changed through the player class.

	The class exposes behaviour to 
	*	Update/Change settings of the Listener object within the Player Class
	*	Access attributes of the listener;
	*	Provide audio feedback on listener movement or rotation.
	*	Provide audio feedback on the current state of the Player in the game
	*	Find the next position of the player based on the listener's orientation in order to detect collisions within Room.cpp

	Although 3D audio is utilised, this listener assumes it is on the X-Z horizontal plane and therefore only rotation around the Y-axis is supported(yaw in 3D)
	rotation about the X-axis (pitch) and rotation about the Z axis (roll) are not supported.

	Different sounds are played on moving forward or rotating. In addition sounds are played based on the stage of the game such
	as the intro or the outro, as well as the constant sounds such as breathing and the heartbeat.
	If the sounds are not created successfully then the Player behaviour will still work, except without audio feedback.
*/

#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__

// System includes.
#include <Windows.h>
#include <memory>
#include <string>
#include <stdlib.h>
#include <X3DAudio.h>
#include <list>
using std::list;
using std::unique_ptr;
using std::string;;

// Framework includes.
#include "Listener.hpp"
#include "GameTimer.h"
#include "XASound.hpp"
using AllanMilne::Audio::XASound;

// Define the integer ID for each footstep type
#define DEFAULT_FOOTSTEPS 1
#define FOOTSTEPS_2_VARIANT 2
#define FOOTSTEPS_3_VARIANT 3

// The Player Class
class Player 
{

// Public Member functions and variables
public:
	// Function:		Player() - Player Constructor
	// Description:		The constructor for the Player class creates the Listener object and sets the object with its start position/orientation
	//					Initialises all variables to their defaults at the start of the game and creates the XASound objects with the string
	//					file names in the anonymous namespace within Player.cpp.
	// In:				N/a
	// Out:				N/a
	Player ();

	// Function:		IsValid() - Is Valid function
	// Description:		This function checks to see if the sounds needed for the player have been created successfully
	//					and if the Listener has been created successfully.
	// In:				N/a
	// Out:				bool - True if all sounds/objects created successfully, false if not.
	bool IsValid();

	// Function:		GetNextPosition() - Get Next Position function
	// Description:		This function uses the players current position and the listeners orientation and adds this orientation to the 
	//					current position in order to find the position the player would be in if they were to move in this direction.
	//					This is used in the Room.cpp file to check for collisions within the room and it is called every frame
	// In:				N/a
	// Out:				XA3DAUDIO_VECTOR NextPosition - the vector for the next position the player will be in 
	X3DAUDIO_VECTOR GetNextPosition ();

	// Function:		GetPosition() - Get Position Function
	// Description:		This function returns the current position of the player's Listener object
	// In:				N/a
	// Out:				XA3DAUDIO_VECTOR Position - The current position of the Listener
	inline X3DAUDIO_VECTOR GetPosition () const { return mPlayerListener->mListener.Position; }

	// Function:		GetListenerStruct() - Get Listener Struct function
	// Description:		This function returns the struct of the Player's listener object so that it can be interacted with outside the player class
	// In:				N/a
	// Out:				X3DAUDIO_LISTENER mListener - The listener struct
	inline X3DAUDIO_LISTENER* GetListenerStruct () { return &mPlayerListener->mListener; }

	// Function:		GetDirection() - Get Direction function
	// Description:		This function returns the Direction enum value of the current orientation of the Listener
	// In:				N/a
	// Out:				Listener::Direction - The enum value of the Listeners current orientation
	inline Listener::Direction GetDirection() const { return mPlayerListener->mBodyOrientation; }

	// Function:		ResetPlayer() - Reset Player function
	// Description:		This player calls the Listeners Reset() function to reset the position and orientation of the Listener
	// In:				N/a
	// Out:				N/a
	inline void ResetPlayer() const { mPlayerListener->Reset(); }

	// Function:		Stop() - Stop function
	// Description:		Stop the Player sounds from playing
	// In:				N/a
	// Out:				N/a
	void Stop();

	// Function:		UpdatePlayer(float, float) - Update Player function
	// Description:		This function plays sounds given the current state of the player. For example, if the Listener is moving, 
	//					this function checks the mListenerMoving bool and plays the footsteps sound. It stores the distance from the 
	//					player to the ghost for use in the UpdateHeartbeat function and calls Tick for the gametimers in the class every frame.
	//					Plays the footsteps, turning and pain sounds based on their booleans.
	// In:				N/a
	// Out:				N/a
	void UpdatePlayer(float GhostDistance, bool PlayerFree);

	// Function:		SetPlayerActive() - Set Player Active function
	// Description:		Plays the breathing and heartbeat files once the player is active 
	// In:				N/a
	// Out:				N/a
	void SetPlayerActive();

	// Function:		CaptureGhost() - Capture Ghost function
	// Description:		Is called when the player presses the Spacebar from the TryToCaptureGhost function in Room.cpp
	//					Plays the mCaptureSound XASound object
	// In:				N/a
	// Out:				N/a
	void CaptureGhost();

	// The listenere object for the player
	Listener *mPlayerListener;	

	// Bools for sound reactions for the player
	bool mPlayerActive;	
	bool mPlayerCanMove;
	bool mPlayerCanTurn;
	bool mInPain;
private:
	// Function:		UpdateHeartbeat() - Update Heartbeat function
	// Description:		If the player is active, this function updates the Heartbeat and Breathing sound files based on the distance of
	//					the player from the ghost. The volume of both sounds increases the closer the player is to the ghost's position
	//					and the pitch of the heartbeat file increase based on the distance, therefore plays faster and acts as though
	//					the player's heartbeat races faster the closer they are. 
	// In:				N/a
	// Out:				N/a
	void UpdateHeartbeat();


	// Function:		SetFootstepsSound() - Set Footsteps Sound function
	// Description:		This function is called whenever the footsteps sound is played when the player moves in order to randomly
	//					select the next footsteps sound. There are 3 different footstep sound files and one is randomly selected to give the 
	//					room diversity by making the floor sound different in different places
	// In:				N/a
	// Out:				N/a
	void SetFootstepsSound();

	// Function:		SetPainsound() - Set Pain Sound function
	// Description:		This function is called when the player collides with a wall or an object in order to randomly select the next
	//					pain sound file to use. There are 3 different pain sound files and one is randomly selected to give the player 
	//					more variety in how they sound when they collide with an object.
	// In:				N/a
	// Out:				N/a
	void SetPainSound();

	// All Sounds used for the Player 
	XASound *mFootsteps;
	XASound *mNormalFootsteps;
	XASound *mCreakingFootsteps;
	XASound *mVariant1Footsteps;
	XASound *mTurning;
	XASound *mCaptureSound;
	XASound *mBreathing;
	XASound *mHeartBeat;
	XASound *mIntroSound;
	XASound *mPainSound1;
	XASound *mPainSound2;
	XASound *mPainSound3;
	XASound *mCurrentPainSound;
	XASound *mOutroSound;

	// Pointer list of all XASound objects in the player class so they can be interacted with
	list<XASound*> mPlayerSounds;

	// Game Timers to limit the amount of times the player can move forward and turn each time the buttons are pressed
	GameTimer *mFootstepTimer;
	GameTimer *mTurningTimer;

	// Bool used to signify if the outro sound for the player has been played
	bool mOutroPlayed;

	// Stores the distance from the player to the ghost to use in the UpdateHeartbeat function
	float mDistanceFromGhost;
}; 
// end Player class.

#endif
