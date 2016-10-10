/*
	--Original--
	file:	Room.hpp
	Version:	1.0
	Date:	20th March 2014.
	Author:	Allan c. Milne.

	--Edit--
	Edited by: Cassie Bennett
	Version:	3.0
	Date:	5th May 2015

	Exposes:	Room.
	Requires:	Player, Ghost, MovingCritter, Stuvector3, XASound.

	Description:
	This room class is based on the room class provided in the RoomEscape solution and has been edited for this applications purpose
	The class models a 16 by 16 unit square room with a door at the center of the bottom of the room. There is an emitter behind the door
	with outside noises which is muffled through a filter while the doors are closed, and the filter is removed when the door opens.
	The room sets up the Player and Ghost objects for the game, initialising them after the intro sound has finished playing. 
	
	The player can be moved around the room:
	*	movement is in 1 step increments in the current player direction;
	*	turns are by 90 degrees;
	*	cannot move past a wall, or obstacle
	*	can only move out the door if the ghost has been defeated by the player
	*	all the above actions have relevant audio feedback.

	The player has to find the ghost in the room based on the position of the ghosts sounds, and press space in order to try and
	damage/capture the ghost, which will then move to a new event or it will be defeated.

	When the ghost is defeated, it disappears and the doors can be heard opening. The filter is then removed from the outside noise
	emitter and the player can then exit through the door and win the game.

	The room is a fixed layout of 16 by 16 units. It uses a 2 dimensional grid array to store the positions of the walls/obstacles
	based on the values used in those positions on the grid, and the player checks this grid before moving to check if they bump into
	an obstacle or not, so the appropriate reaction and noise can be carried out.

	The class calls the update functions for the Ghost, Player and Moving Critter objects.

	All sound files needed for this class are created in an anonymous namespace within the .cpp file.
*/

// Define the class
#ifndef __ROOM_HPP__
#define __ROOM_HPP__

// System includes.
#include <Windows.h>
#include <X3DAudio.h>
#include <memory>
#include <stdlib.h>
#include <time.h>
#include <list>
using std::list;
using std::unique_ptr;

// Framework includes.
#include "XASound.hpp"
using AllanMilne::Audio::XASound;

// Application includes.
#include "Player.hpp"
#include "Emitter.hpp"
#include "MovingCritter.hpp"
#include "StuVector3.hpp"
#include "Ghost.hpp"
using namespace stu;

// Define the room dimensions
#define ROOM_X_SIZE 16
#define ROOM_Z_SIZE 16

// Define the values to be used within the grid to identify what is within the maps space
#define NORMAL_FLOOR_VALUE 0
#define WALL_VALUE 1
#define DOOR_VALUE 2
#define TABLE_VALUE 3
#define PIANO_VALUE 4
#define RADIO_VALUE 5
#define TYPEWRITER_VALUE 6

// The amount of obstacles in the scene
#define OBJECT_COUNT 4

// Struct for the collidable objects to hold the information needed to place them in the grid array
struct CollidableObject
{
	int ObjectType;						// To recognise the type of the object
	X3DAUDIO_VECTOR ObjectPosition;		// So the position can be set within the grid
	float ObjectWidth;					// Dimensions needed so that appropriate grid cells are filled with the object's value
	float ObjectHeight;
};

// The Room class
class Room 
{

// Public Member functions and variables
public:

	// Function:		Room() - Room Constructor
	// Description:		Initialises sounds/objects and emitters for the game
	// In:				N/a
	// Out:				N/a
	Room ();

	// Function:		~Room() - Room Destructor
	// Description:		Destructor stops the sounds playing in the scene
	// In:				N/a
	// Out:				N/a
	virtual ~Room();

	// Function:		UpdateRoom() - Update Room function
	// Description:		Update the room based on the current state of the application, and updates the Player, Ghost and
	//					MovingCritter objects. 
	// In:				N/a
	// Out:				N/a
	void UpdateRoom();
		
	// Function:		Reset() - Reset function
	// Description:		Reset the player to the starting position and update the emitters in the scene again to reflect the players new position/orientation
	// In:				N/a	
	// Out:				N/a
	void Reset();

	// Function:		Stop() - Stop function
	// Description:		Stop playing the emitters and the ghost sounds
	// In:				N/a
	// Out:				N/a
	void Stop();

	// These functions affect the position and orientation of the Listener object within the Player class

	// Function:		MovePlayer() - Move Player function
	// Description:		Move by 1 step in the direction the player is currently facing based on the listeners
	//					current orientation. Calls HitWallOrDoor to check if the player can move in this direction
	//					or should be stopped, and plays the appropriate sound either way. 
	//					Requires recalculation and application of DSP settings to the emitters within the application
	// In:				N/a
	// Out:				N/a
	void MovePlayer();

	// Function:		TurnPlayerLeft() - Turn Player Left function
	// Description:		Calls TurnCounterClockwise twice on the player's Listener object so that it will turn 90 degrees
	//					Also updates the emitters in the application so that their new relativity to the player's position
	//					and orientation is updated
	// In:				N/a
	// Out:				N/a
	void TurnPlayerLeft();

	// Function:		TurnPlayerRight() - Turn Player Right function
	// Description:		Calls TurnClockwise twice on the player's Listener object so that it will turn 90 degrees
	//					Also updates the emitters in the application so that their new relativity to the player's position
	//					and orientation is updated
	// In:				N/a
	// Out:				N/a
	void TurnPlayerRight();

	// Function:		TryToCaptureGhost() - Try To Capture Ghost function
	// Description:		This function is called when the player presses the space bar. 
	//					Checks the distance between the player and the ghost, and carries out appropriate reaction if player is in/out of range
	//					If in range, the ghost is damaged and will move to the next stage, if out of range, the ghost will laugh
	// In:				N/a
	// Out:				N/a
	void TryToCaptureGhost();
	
	// Function:		IsValied() - Is Valid function
	// Description:		Returns true if the player, ghost, critters and other sounds have been set up correctly, false otherwise.
	//					This is because the game can't properly run without all of these initialisations working correctly therefore
	//					its safer to check beforehand.
	// In:				N/a
	// Out:				bool - True or False depending on if the objects have been initialised properly
	// 
	inline bool IsValid() const { return (mPlayer->IsValid() && (mGhost->IsValid()) && (mCritters->IsValid()) 
								&& mHitWallSound->IsValid() && mForestSound->IsValid()); }

	// Function:		DoorExited() - Door Exited function
	// Description:		Indicates if the player has exited through the door.
	// In:				N/a
	// Out:				bool - True or false depending on if the player has exited the door

	inline bool DoorExited () const { return mDoorExited; }


// Private member functions and variables
private:
	// Function:		SetUpRoom() - Set Up Room Function
	// Description:		Set up the map/grid of the room with the walls, obstacles, and the spaces that are free for the player to walk
	// In:				N/a
	// Out:				N/a
	void SetUpRoom();
	
	// Function:		InitialiseRoomObjects()
	// Description:		Initialises the Ghost and Critter objects when the intro sounds are finished playing
	// In:				N/a
	// Out:				N/a
	void InitialiseRoomObjects();

	// Function:		GetDistance() - Get Distance function
	// Description:		Finds the distance between the player's position and the ghost's in order to find if the player is within range
	//					Sets this distance to the member variable mGhostDistance so that the distance can be checked in the TryToCaptureGhost function
	// In:				N/a
	// Out:				N/a
	void GetDistance();

	// Function:		OpenDoor() - Open Door function
	// Description:		Plays the sound for opening the door and sets mDoorOpen to true so that player can then leave the room
	// In:				N/a
	// Out:				N/a
	void OpenDoor();

	// Function:		HitWallOrDoor() - Hit Wall or Door function
	// Description:		Return true if one step forward will hit a wall/obstacle or exit the door, if the latter then the mDoorExited flag is set.
	//					Otherwise the collision bools for each type of collision (e.g. collision with the piano) are set to true and the appropriate sound 
	//					is played.
	// In:				N/a
	// Out:				Bool - true or false based on if the player has hit a wall or an object (true)
	bool HitWallOrDoor();

	// The 2D Array grid for the room's layout
	float mRoomBase[ROOM_X_SIZE][ROOM_Z_SIZE];

	// The player object, which has the X3DAudio listener object.
	Player *mPlayer;

	// The ghost object, which handles the behaviour/sounds of the ghost in the room using emitters
	Ghost *mGhost;

	// The MovingCritter object, which handles the dynamic movement of the rats and bats within the scene
	MovingCritter *mCritters;

	// Positions needed for the class
	X3DAUDIO_VECTOR mDoorPosition;
	X3DAUDIO_VECTOR mOutsideSoundPosition;

	// Collidable Objects needed for placing the obstacles within the grid
	CollidableObject mPianoObject;
	CollidableObject mTypewriterObject;
	CollidableObject mTableObject;
	CollidableObject mRadioObject;

	// Bool to check if the room has been initialised
	bool mInitialised;

	// True when the player has exited through the door.
	bool mDoorExited;

	// True if player is free to exit the door 
	bool mPlayerFree;

	// Bool used when door can't be exited yet
	bool mDoorTry;
	
	// Tell Player when they have bumped into specific objects
	bool mPianoCollision;
	bool mRadioCollision;
	bool mTypeWriterCollision;
	bool mTableCollision;

	// XASound objects for all sounds needed for the game
	XASound *mHitWallSound;
	XASound *mHitDoorSound;
	XASound *mAmbientMusic;
	XASound *mDoorOpenSound;
	XASound *mTableCollisionSound;
	XASound *mPianoCollisionSound;
	XASound *mRadioCollisionSound;
	XASound *mTypeWriterCollisionSound;
	XASound *mForestSound;

	// Pointer list of all XASound objects in the room so they can be interacted with
	list<XASound*> mRoomSounds;

	// Emitter for sound outside of the room
	Emitter *mOutdoorEmitter;

	// True when the door is open
	bool mDoorOpen;

	// The distance value between the player and the ghost
	float mGhostDistance;
}; 
// end Room class.

#endif
