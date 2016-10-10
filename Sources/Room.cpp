/*
	--Original--
	file:	Room.cpp
	Version:	1.0
	Date:	20th March 2014.
	Author:	Allan c. Milne.

	--Edit--
	Edited by: Cassie Bennett
	Version:	3.0		(Previous ver 2.9)
	Date:	5th May 2015

	Exposes:	Implementation of the Room class.
	Requires:	Player, XACore, XASound, stu::v3f.

	Description:
	Models a square room on the X-Z plane with a door through which can be heard the outside sounds, appropriately oriented.
	The room class sets up the ghost and player objects so that the game can begin, and opens the door when the player has
	successfully defeated the ghost. 
	*	See Room.hpp for details.

	This version has a fixed room layout;
	This layout is defined by the definitions in Room.hpp and the positions are set in the initialisation functions

	Although the Room is entirely on the X-Z horizontal plane, the stu::v3f vector and X3DAUDIO_VECTOR representation is used.

	All sound files needed for this class are created in the anonymous namespace below
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
using std::unique_ptr;
using std::string;

// Framework includes.
#include "XACore.hpp"
#include "XASound.hpp"
using AllanMilne::Audio::XACore;
using AllanMilne::Audio::XASound;

// Application includes. (All other file includes are within Room.hpp)
#include "Room.hpp"



// Anonymous namespace defining the sound files
namespace 
{
	// .wav files used for various sound effects for the room
	const string AmbientFile = "Sounds/AmbientMusic.wav";
	const string DoorOpenFile = "Sounds/DoorOpen.wav";
	const string HitWallFile = "Sounds/HitWallNoise.wav";
	const string LockedDoorFile = "Sounds/LockedDoor.wav";
	const string OutsideFile = "Sounds/OutdoorSound.wav";
	const string PianoCollisionFile = "Sounds/PianoCollision.wav";
	const string RadioCollisionFile = "Sounds/RadioCollision.wav";
	const string TableCollisionFile = "Sounds/TableCollision.wav";
	const string TypewriterCollisionFile = "Sounds/TypewriterCollision.wav";
} 
// end anonymous namespace.



// Function:		Room() - Room Constructor
// Description:		Initialises sounds/objects and emitters for the game
// In:				N/a
// Out:				N/a
Room::Room ()
{
	// Initialise all XASound objects for the room using the file names from the anonymous namespace
	mHitWallSound = new XASound(HitWallFile);			// Sound used for when player collides with wall
	mRoomSounds.push_back(mHitWallSound);

	mForestSound = new XASound(OutsideFile, true);		// Sound used for the OutdoorEmitter
	mRoomSounds.push_back(mForestSound);

	mHitDoorSound = new XASound(LockedDoorFile);		// Sound used for when player collides with the locked door
	mRoomSounds.push_back(mHitDoorSound);

	mAmbientMusic = new XASound(AmbientFile);			// Sound used for background ambient music
	mRoomSounds.push_back(mAmbientMusic);

	mDoorOpenSound = new XASound(DoorOpenFile);			// Sound used for when the door opens
	mRoomSounds.push_back(mDoorOpenSound);

	mTableCollisionSound = new XASound(TableCollisionFile);	// Sound used when player collides with the table
	mRoomSounds.push_back(mTableCollisionSound);

	mPianoCollisionSound = new XASound(PianoCollisionFile);	// Sound used when player collides with the piano
	mRoomSounds.push_back(mPianoCollisionSound);

	mRadioCollisionSound = new XASound(RadioCollisionFile);	// Sound used when player collides with the radio
	mRoomSounds.push_back(mRadioCollisionSound);

	mTypeWriterCollisionSound = new XASound(TypewriterCollisionFile);	// Sound used when player collides with the typewriter
	mRoomSounds.push_back(mTypeWriterCollisionSound);

	// Seed the random generator with the current time
	srand (time(NULL));

	// Create the Player, Ghost and Critter objects for the room
	mPlayer = new Player();
	mGhost = new Ghost(mPlayer);
	mCritters = new MovingCritter(mPlayer);

	// Set initial bool states for the room
	mInitialised = false;		// The room has not been initialised
	mDoorExited = false;		// Door is not exited
	mDoorTry = false;			// Player is not trying to open the door 
	mPlayerFree = false;		// The player is not free to exit the room
	mDoorOpen = false;			// The door is not open

	// Initialise the ghost distance value to 0
	mGhostDistance = 0;

	// Position of the door in the scene
	mDoorPosition.x = 8.0f;
	mDoorPosition.y = 0.0f;
	mDoorPosition.z = 0.0f; 

	// Position of outside sound; in this case 1.5 units perpindicular to middle of door so that the noise is positioned just behind it
	mOutsideSoundPosition.x = mDoorPosition.x;
	mOutsideSoundPosition.y = 0.0f;
	mOutsideSoundPosition.z = mDoorPosition.z - 1.5f;

	// Get stu::v3f objects of the X3DAUDIO_VECTOR objects so they can be used to initialise the emitters
	v3f SoundPosition = {mOutsideSoundPosition.x, mOutsideSoundPosition.y, mOutsideSoundPosition.z};
	v3f DoorPosition = {mDoorPosition.x, mDoorPosition.y, mDoorPosition.z};

	// Create the Outdoor emitter with a cone using the mForestSound and setting it to looped
	mOutdoorEmitter = new Emitter(mForestSound, mOutsideSoundPosition, mPlayer->GetListenerStruct(), &SoundPosition, &DoorPosition, true);
	// Set the filter for the Outdoor emitter to a Low Pass Filter so it sounds muffled while the door is closed
	mOutdoorEmitter->SetEmitterFilter(LowPassFilter, 550.0f, 1.2);

	// Call the SetUpRoom function to fill the 2D room grid with the correct values placing the walls, door and obstacles
	SetUpRoom();
} 
// end Room constructor.
	


// Function:		~Room() - Room Destructor
// Description:		Destructor stops the sounds playing in the scene
// In:				N/a
// Out:				N/a
Room::~Room()
{
	// Stop the outdoor emitter sound playing and delete the cone
	mOutdoorEmitter->StopEmitter();
	delete mOutdoorEmitter->mEmitter.pCone;

	// Check through all XASound objects in the room and stop them
	for (auto RoomSound = mRoomSounds.begin(); RoomSound != mRoomSounds.end(); ++ RoomSound)
	{
		if ((*RoomSound)->IsValid())
		{
			(*RoomSound)->Stop();
		}
	}

	mPlayer->Stop();
	mGhost->Stop();
	mCritters->Stop();
} 
// end Room destructor.



// Function:		SetUpRoom() - Set Up Room Function
// Description:		Set up the map/grid of the room with the walls, obstacles, and the spaces that are free for the player to walk
// In:				N/a
// Out:				N/a
void Room::SetUpRoom()
{
	// Set up the room objects with their type, position, and dimensions
	// Some positions are already set within the Ghost class

	// Piano Object
	mPianoObject.ObjectType = PIANO_VALUE;
	mPianoObject.ObjectPosition = mGhost->mPianoPosition;
	mPianoObject.ObjectWidth = 4.0f;
	mPianoObject.ObjectHeight = 4.0f;

	// Typewriter Object
	mTypewriterObject.ObjectType = TYPEWRITER_VALUE;
	mTypewriterObject.ObjectPosition = mGhost->mTypeWriterPosition;
	mTypewriterObject.ObjectWidth = 2.0f;
	mTypewriterObject.ObjectHeight = 2.0f;

	// Table object
	mTableObject.ObjectType = TABLE_VALUE;
	mTableObject.ObjectPosition.x = 8.0f;
	mTableObject.ObjectPosition.y = 0.0f;
	mTableObject.ObjectPosition.z = 9.0f;
	mTableObject.ObjectWidth = 2.0f;
	mTableObject.ObjectHeight = 4.0f;

	// Radio Object
	mRadioObject.ObjectType = RADIO_VALUE;
	mRadioObject.ObjectPosition = mGhost->mRadioPosition;
	mRadioObject.ObjectWidth = 2.0f;
	mRadioObject.ObjectHeight = 2.0f;

	// Set the collision variables to false, these are true when the player collides with these objects to play the appropriate sound
	mPianoCollision = false;
	mRadioCollision = false;
	mTypeWriterCollision = false;
	mTableCollision = false;

	// Each cell in the grid represents 1.0 on the map
	// Check through the grid and set the cells to their appropriate values based on if they are a free space,
	// a door, or a specific type of obstacle
	for (int i = 0; i < ROOM_X_SIZE; i ++)
	{
		for (int j = 0; j < ROOM_Z_SIZE; j ++)
		{
			// Set the values of the cells at the outer edges of the grid to the WALL_VALUE
			if ((i == 0) || (j == 0) || (i == ROOM_X_SIZE - 1) || (j == ROOM_Z_SIZE - 1))
			{
				// These are the walls around the room
				mRoomBase[i][j] = WALL_VALUE;
			}
			else
			{
				// Else set the cells to the NORMAL_FLOOR_VALUE to indicate it is free space
				mRoomBase[i][j] = NORMAL_FLOOR_VALUE;
			}

			// Set the values for the door in the room based on the doors position, and + 1 and - 1 in the x axis so that the door is
			// 3 cells wide
			if (((i == mDoorPosition.x) || (i == mDoorPosition.x - 1) || (i == mDoorPosition.x + 1)) 
				&& (j == mDoorPosition.z))
			{
				// Fill in values for the door
				mRoomBase[i][j] = DOOR_VALUE;
			}
		}
	}

	// Fill room with collidable objects
	for (int i = 0; i < OBJECT_COUNT; i++)
	{
		CollidableObject CurrentObject;
		// Set the current object to the piano object
		if (i == 0)
		{
			CurrentObject = mPianoObject;
		}
		// Set the current object to the typewriter object
		if (i == 1)
		{
			CurrentObject = mTypewriterObject;
		}
		// Set the current object to the radio object
		if (i == 2)
		{
			CurrentObject = mRadioObject;
		}
		// Set the current object to the table object
		if (i == 3)
		{
			CurrentObject = mTableObject;
		}

		// Find the half height and width dimension values to calculate the bottom left and top right positions
		float HalfHeight = int(0.5 * CurrentObject.ObjectHeight);
		float HalfWidth = int(0.5 * CurrentObject.ObjectWidth);

		// Fill in the X Axis from the left of the obstacle to the right
		for (int i = CurrentObject.ObjectPosition.x - HalfWidth; i <= (CurrentObject.ObjectPosition.x + HalfWidth) - 1; i++)
		{
			// Fill in the Z Axis from the bottom of the obstacle to the top
			for (int j = CurrentObject.ObjectPosition.z - HalfHeight; j <= (CurrentObject.ObjectPosition.z + HalfHeight) - 1; j++)
			{
				// Set the cell value to the current objects ObjectType value, indicating what kind of obstacle is in this position
				mRoomBase[i][j] = CurrentObject.ObjectType;
	 		}
		}
	}

	// Set up ambient music
	mAmbientMusic->SetLooped(true);
	mAmbientMusic->SetVolume(-25.0f);		// Reduce the volume of the ambient music so that it doesn't overpower other sounds
	mAmbientMusic->Play();
}
// End SetUpRoom function



// Function:		InitialiseRoomObjects()
// Description:		Initialises the Ghost and Critter objects when the intro sounds are finished playing
// In:				N/a
// Out:				N/a
void Room::InitialiseRoomObjects()
{
	// This is called when the intro sounds of the player have finished playing in order to initialise the ghost and critters

	// Initialise the ghost
	mGhost->InitialiseGhost();
	// Initialise the critters
	mCritters->InitialiseCritter();

	// When both have been successfully initialised, set the mInitialised bool for the room to true so that it knows not to continue
	// calling this function
	if ((mGhost->mInitialised == true) && (mCritters->mInitialised == true))
	{
		mInitialised = true;	
	}
}
// End InitialiseRoomObjects function



// Function:		UpdateRoom() - Update Room function
// Description:		Update the room based on the current state of the application, and updates the Player, Ghost and
//					MovingCritter objects
// In:				N/a
// Out:				N/a
void Room::UpdateRoom()
{
	// Updates

	// If the Room has not been fully initialised, and the player is active (after the intro has played) call InitialiseRoomObjects
	if ((!mInitialised) && (mPlayer->mPlayerActive))
	{
		// mInitialised is set to true in here when the room is fully initialised
		InitialiseRoomObjects();
	}

	// If the ghost has been defeated
	if (mGhost->mGhostDefeated)
	{
		// When the ghost has finished its death sound and the door isn't currently open, open the door with the OpenDoor function
		if ((!mGhost->mGhostEmitter->EmitterActive()) && (!mDoorOpen))
		{
			// This function plays the door open sound and sets the bools to true for allowing the player to escape
			// The mDoorOpen bool is set to true in here when it has been called
			OpenDoor();
		}
		// When the door is open and the door open sound has finished playing, set the mPlayerFree bool to true
		if ((mDoorOpen) && (!mDoorOpenSound->IsPlaying()))
		{
			// This is used in the Player's Update function to let the player know to play the PlayerOutro sound file
			mPlayerFree = true;
		}
	}
	
	// Find the distance between the player and the ghost every frame, which is needed for finding if the player is within range in the 
	// TryToCaptureGhost function
	GetDistance();

	// Update the player, ghost and critter objects every frame
	mPlayer->UpdatePlayer(mGhostDistance, mPlayerFree);
	mGhost->UpdateGhost();
	mCritters->UpdateCritter();
}
// End UpdateRoom function


// Function:		GetDistance() - Get Distance function
// Description:		Finds the distance between the player's position and the ghost's in order to find if the player is within range
//					Sets this distance to the member variable mGhostDistance so that the distance can be checked in the TryToCaptureGhost function
// In:				N/a
// Out:				N/a
void Room::GetDistance()
{
	// Use stu::v3f objects of the Player Position and Ghost Position in order to find the distance between them
	X3DAUDIO_VECTOR PlayerPosition = mPlayer->GetPosition();
	v3f PlayerPos = {PlayerPosition.x, PlayerPosition.y, PlayerPosition.z };
	v3f GhostPos = {mGhost->GetGhostPosition().x, mGhost->GetGhostPosition().y, mGhost->GetGhostPosition().z };

	// Set the distance to the value returned from the stu::v3f::Distance function between the Player and Ghost position
	// This distance is then used in the TryToCaptureGhost function to check if the player is within range
	mGhostDistance = stu::v3f::Distance(PlayerPos, GhostPos);
} 
// End GetDistance function


// Function:		Reset() - Reset function
// Description:		Reset the player to the starting position and update the emitters in the scene again to reflect the players new 
//					position/orientation
// In:				N/a	
// Out:				N/a
void Room::Reset()
{
	// Reset the Player's listener object.
	mPlayer->ResetPlayer();
	// Reset the Ghost and Critter objects to update their emitters for the new player position/orientation
	mGhost->Reset();
	mCritters->Reset();
	// Reset the Outdoor emitter
	mOutdoorEmitter->ResetEmitter(true);
} 
// end Reset function.


// Function:		Stop() - Stop function
// Description:		Stop playing the emitters and the player/ghost/critter sounds
// In:				N/a
// Out:				N/a
void Room::Stop()
{
	mOutdoorEmitter->StopEmitter();
	mPlayer->Stop();
	mGhost->Stop();
	mCritters->Stop();
} 
// end Stop function.



// Function:		OpenDoor() - Open Door function
// Description:		Plays the sound for opening the door and sets mDoorOpen to true so that player can then leave the room
// In:				N/a
// Out:				N/a
void Room::OpenDoor()
{
	// Play the sound to show the doors have opened
	mDoorOpenSound->Play();
	// Change the filter for the OutdoorEmitter to a HighPassFilter with the frequency being 0.0, effectively removing the previous LowPassFilter
	// This is so that the sound no longer sounds as if it is muffled behind the door, and the player can follow it to escape the room
	mOutdoorEmitter->SetEmitterFilter(HighPassFilter, 0.0f, 0.0);
	// Set the door open bool to true so that the player can then escape
	mDoorOpen = true;
}



// Function:		HitWallOrDoor() - Hit Wall or Door function
// Description:		Return true if one step forward will hit a wall/obstacle or exit the door, if the latter then the mDoorExited flag is set.
//					Otherwise the collision bools for each type of collision (e.g. collision with the piano) are set to true and the appropriate sound 
//					is played.
// In:				N/a
// Out:				Bool - true or false based on if the player has hit a wall or an object (true)
bool Room::HitWallOrDoor() 
{
	// Find the next position of the player from the Player object, based on the direction they are facing
	X3DAUDIO_VECTOR NextPos = mPlayer->GetNextPosition();

	// The array starts at 0, positions of the player must take away 1 to be in the correct cell 
	// Use these values in the array to find what part of the room it is
	int NextX = int(NextPos.x - 1);
	int NextZ = int(NextPos.z - 1);

	// If the Next Position is at a cell with a NORMAL_FLOOR_VALUE, returns false so that player can move in the MovePlayer function
	if (mRoomBase[NextX][NextZ] == NORMAL_FLOOR_VALUE)
	{
		return false;
	}
	// If the next position is at a cell with the DOOR_VALUE, set mDoorTry to true to tell the MovePlayer function the player is colliding
	// with a door and return true so that player can't move in that direction
	else if (mRoomBase[NextX][NextZ] == DOOR_VALUE)
	{
		mDoorTry = true;
		return true;
	}
	// If the next position is at a cell with the TABLE_VALUE, set mTableCollision to true so player knows it collided with a table
	// and return true so that player can't move in that direction
	else if (mRoomBase[NextX][NextZ] == TABLE_VALUE)
	{
		mTableCollision = true;
		return true;
	}
	// If the next position is at a cell with the PIANO_VALUE, set mPianoCollision to true so player knows it collided with the piano
	// and return true so that player can't move in that direction
	else if (mRoomBase[NextX][NextZ] == PIANO_VALUE)
	{
		mPianoCollision = true;
		return true;
	}
	// If the next position is at a cell with the RADIO_VALUE, set mRadioCollision to true so player knows it collided with the radio
	// and return true so that player can't move in that direction
	else if (mRoomBase[NextX][NextZ] == RADIO_VALUE)
	{
		mRadioCollision = true;
		return true;
	}
	// If the next position is at a cell with the TYPEWRITER_VALUE, set mTypeWriterCollision to true so player knows it collided with the typewriter
	// and return true so that player can't move in that direction
	else if (mRoomBase[NextX][NextZ] == TYPEWRITER_VALUE)
	{
		mTypeWriterCollision = true;
		return true;
	}
	// If the next position is at a cell with the WALL_VALUE, and return true so that player can't move in that direction and knows it collided
	// with a wall.
	else if (mRoomBase[NextX][NextZ] == WALL_VALUE)
	{
		return true;
	}

	return false;		// Will not hit a wall or obstacle or exit door.
} 
// end HitWallOrDoor function



// Function:		MovePlayer() - Move Player function
// Description:		Move by 1 step in the direction the player is currently facing based on the listeners
//					current orientation. Calls HitWallOrDoor to check if the player can move in this direction
//					or should be stopped, and plays the appropriate sound either way. 
//					Requires recalculation and application of DSP settings to the emitters within the application
// In:				N/a
// Out:				N/a
void Room::MovePlayer()
{
	// Call the HitWallOrDoor function to find if will collide with something and what that object is.
	if (HitWallOrDoor())
	{ 
		// If the player hasn't exited the door
		if(!DoorExited())
		{
			// Set the mInPain variable for the player to true so that a pain sound will play at the collision
			mPlayer->mInPain = true;

			// If the player collides with the door (mDoorTry is set to true when this happens within HitWallOrDoor)
			if (mDoorTry)
			{	
				// mDoorOpen is true only when the player has defeated the ghost, therefore when they have, they can then exit the room
				if (mDoorOpen)
				{
					// Escaped the room!
					mDoorExited = true;
				}
				// If the door isn't open, the player has collided with the locked door, therefore play the HitDoorSound
				else if (mHitDoorSound->IsValid()) 
				{
					mHitDoorSound->Play();
					// Set the mDoorTry bool to false so that it can be checked again in the next move
					mDoorTry = false;
				}
				return;
			}
			// Else if the player has collided with the Table (this is set to true when this occurs in the HitDoorOrWall function)
			else if (mTableCollision)
			{
				// Play the sound and set the bool to false so it can be checked again next move
				if (mTableCollisionSound->IsValid())
				{
					mTableCollisionSound->Play();
					mTableCollision = false;
				}
				return;
			}
			// Else if the player has collided with the Piano (this is set to true when this occurs in the HitDoorOrWall function)
			else if (mPianoCollision)
			{
				// Play the sound and set the bool to false so it can be checked again next move
				if (mPianoCollisionSound->IsValid())
				{
					mPianoCollisionSound->Play();
					mPianoCollision = false;
				}
				return;
			}
			// Else if the player has collided with the Radio (this is set to true when this occurs in the HitDoorOrWall function)
			else if (mRadioCollision)
			{
				// Play the sound and set the bool to false so it can be checked again next move
				if (mRadioCollisionSound->IsValid())
				{
					mRadioCollisionSound->Play();
					mRadioCollision = false;
				}
				return;
			}
			// Else if the player has collided with the Typewriter (this is set to true when this occurs in the HitDoorOrWall function)
			else if (mTypeWriterCollision)
			{
				// Play the sound and set the bool to false so it can be checked again next move
				if (mTypeWriterCollisionSound->IsValid())
				{
					mTypeWriterCollisionSound->Play();
					mTypeWriterCollision = false;
				}
				return;
			}
			else
			{
				// The player must have hit a normal wall, so play the mHitWallSound
				if (mHitWallSound->IsValid()) 
				{
					mHitWallSound->Play();
				}
				// Return after this so that the rest of the function is not carried out
				return;
			}
		}
	}
	
	// If the player did not hit an obstacle, then move the player in this direction
	// If the player can move forward (can only move forward one step at a time)
	if (mPlayer->mPlayerCanMove)
	{
		// Call the MoveForward function for the Player's listener object
		mPlayer->mPlayerListener->MoveForward();
	}

	// Update the emitters within the room to reflect the player's new position
	mOutdoorEmitter->UpdateEmitter();
	mGhost->UpdateEmitter();
	mCritters->UpdateEmitter();
} 
// end MovePlayer function.



// Function:		TurnPlayerLeft() - Turn Player Left function
// Description:		Calls TurnCounterClockwise twice on the player's Listener object so that it will turn 90 degrees
//					Also updates the emitters in the application so that their new relativity to the player's position
//					and orientation is updated
// In:				N/a
// Out:				N/a
void Room::TurnPlayerLeft()
{
	// If the player can turn (can only turn once at a time)
	if (mPlayer->mPlayerCanTurn)
	{
		// Turn the player's listener CounterClockwise twice in order to turn by 90 degrees (one call turns by 45 degrees)
		mPlayer->mPlayerListener->TurnCounterClockwise();
		mPlayer->mPlayerListener->TurnCounterClockwise();
		// Update the emitters in the room to reflect the player listener's new orientation
		mOutdoorEmitter->UpdateEmitter();
		mGhost->UpdateEmitter();
		mCritters->UpdateEmitter();
	}
} 
// end TurnPlayerLeft function.



// Function:		TurnPlayerRight() - Turn Player Right function
// Description:		Calls TurnClockwise twice on the player's Listener object so that it will turn 90 degrees
//					Also updates the emitters in the application so that their new relativity to the player's position
//					and orientation is updated
// In:				N/a
// Out:				N/a
void Room::TurnPlayerRight()
{
	// If the player can turn (can only turn once at a time)
	if (mPlayer->mPlayerCanTurn)
	{
		// Turn the player's listener Clockwise twice in order to turn by 90 degrees (one call turns by 45 degrees)
		mPlayer->mPlayerListener->TurnClockwise();
		mPlayer->mPlayerListener->TurnClockwise();
		// Update the emitters in the room to reflect the player listener's new orientation
		mOutdoorEmitter->UpdateEmitter();
		mGhost->UpdateEmitter();
		mCritters->UpdateEmitter();
	}
} // end turnPlayerRight function.



// Function:		TryToCaptureGhost() - Try To Capture Ghost function
// Description:		This function is called when the player presses the space bar. 
//					Checks the distance between the player and the ghost, and carries out appropriate reaction if player is in/out of range
//					If in range, the ghost is damaged and will move to the next stage, if out of range, the ghost will laugh
// In:				N/a
// Out:				N/a
void Room::TryToCaptureGhost()
{
	// Else player tries again
	mPlayer->CaptureGhost();

	// If the ghost is active
 	if (mGhost->mGhostActive)
	{
		// If the player is within the detection range of the ghost, and the ghost is not currently moving
		if ((mGhostDistance < DETECTION_RANGE) && (mGhost->mAtStage))
		{
			// Set the new stage of the ghost so that he will move to the next stage
			mGhost->SetNewStage();
		}
		else
		{
			// Else play the ghost laugh sound to tell the player they were unsuccessful
			mGhost->GhostLaugh();
		}
	}
}
// End TryToCaptureGhost function