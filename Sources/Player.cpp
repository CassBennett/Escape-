/*
	--Original--
	file:	Player.cpp
	Version:	1.0
	Date:	19th March 2014.
	author:	Allan c. Milne.

	--Edit--
	Edited by: Cassie Bennett
	Version 3.0

	Exposes:	implementation of the Player class.

	Description:
	This is an implementation of the Player class that models the user (listener).
	* see Player.hpp for details.

	All of the string file names for the XASound objects within the class are set in the anonymous namespace to be used for initialising

	Uses the Directions namespace from Listener.hpp in order to set the initial direction of the player
*/

// System includes.
#include <Windows.h>
#include <memory>
#include <string>
#include <math.h>
#include <X3DAudio.h>
using std::unique_ptr;
using std::string;

//--- Framework includes.
#include "XASound.hpp"
using AllanMilne::Audio::XASound;

//--- application includes.
#include "Player.hpp"

// Use the namespace Directions from the Listener.hpp file in order to set the initial direction of the player's Listener object
using namespace Directions;

// Anonymous namespace defining the sound files
namespace 
{
	const string CaptureNoiseFile = "Sounds/CaptureNoise.wav";
	const string CreakingFootstepsFile = "Sounds/CreakingFootsteps.wav";
	const string DefaultBreathingFile = "Sounds/DefaultBreathing.wav";
	const string DefaultFootstepsFile = "Sounds/DefaultFootsteps.wav";
	const string DefaultTurningFile = "Sounds/DefaultTurning.wav";
	const string FootstepsVariant1File = "Sounds/FootstepsVariant1.wav";
	const string HeartBeatFile = "Sounds/HeartBeat.wav";
	const string IntroSoundFile = "Sounds/RoomIntro.wav";
	const string OutroSoundFile = "Sounds/PlayerOutro.wav";
	const string PainSound1 = "Sounds/PainSound1.wav";
	const string PainSound2 = "Sounds/PainSound2.wav";
	const string PainSound3 = "Sounds/PainSound3.wav";
}



// Function:		Player() - Player Constructor
// Description:		The constructor for the Player class creates the Listener object and sets the object with its start position/orientation
//					Initialises all variables to their defaults at the start of the game and creates the XASound objects with the string
//					file names in the anonymous namespace within Player.cpp.
// In:				N/a
// Out:				N/a
Player::Player ()
{
	// Set the starting position and orientation of the player/listener
	X3DAUDIO_VECTOR StartPos = {8.0f, 0.0f, 2.0f};
	Listener::Direction StartOrient = Listener::North;
	// Create the Listener object with these settings
	mPlayerListener = new Listener(StartPos, StartOrient);

	// Initialise bools for state of the player
	mPlayerCanMove = true;
	mPlayerCanTurn = true;
	mInPain = false;
	mPlayerActive = false;

	// Set up the intro sound for the player and play it
	mIntroSound = new XASound(IntroSoundFile);
	mIntroSound->Play();

	// Set up the Outro sound for the player
	mOutroPlayed = false;
	mOutroSound = new XASound(OutroSoundFile);

	// Create the new XASound objects using the string files from the anonymous namespace above
	mNormalFootsteps = new XASound(DefaultFootstepsFile);		// Normal footsteps sound
	mPlayerSounds.push_back(mNormalFootsteps);

	mCreakingFootsteps = new XASound(CreakingFootstepsFile);	// Creaking Footsteps sound
	mPlayerSounds.push_back(mCreakingFootsteps);

	mVariant1Footsteps = new XASound(FootstepsVariant1File);	// Variant 1 Footsteps sound
	mPlayerSounds.push_back(mVariant1Footsteps);

	mTurning = new XASound(DefaultTurningFile);					// Turning sound
	mPlayerSounds.push_back(mTurning);

	mCaptureSound = new XASound(CaptureNoiseFile);				// Capture Sound
	mPlayerSounds.push_back(mCaptureSound);

	mPainSound1 = new XASound(PainSound1);						// Pain Sound 1
	mPlayerSounds.push_back(mPainSound1);

	mPainSound2 = new XASound(PainSound2);						// Pain Sound 2
	mPlayerSounds.push_back(mPainSound2);

	mPainSound3 = new XASound(PainSound3);						// Pain Sound 3
	mPlayerSounds.push_back(mPainSound3);

	// Set up the breathing and heartbeat sounds to loop
	mBreathing = new XASound(DefaultBreathingFile);
	mBreathing->SetLooped(true);
	mPlayerSounds.push_back(mBreathing);

	mHeartBeat = new XASound(HeartBeatFile);
	mHeartBeat->SetLooped(true);
	mPlayerSounds.push_back(mHeartBeat);

	// Create the footsteps and turning timers used to limit how fast the player can turn and move
	mFootstepTimer = new GameTimer();
	mFootstepTimer->Reset();
	mTurningTimer = new GameTimer();
	mTurningTimer->Reset();

	// Set the initial footsteps and pain sounds
	mFootsteps = mNormalFootsteps;
	mCurrentPainSound = mPainSound1;
} 
// end Player constructor.



// Function:		IsValid() - Is Valid function
// Description:		This function checks to see if the sounds needed for the player have been created successfully
//					and if the Listener has been created successfully.
// In:				N/a
// Out:				bool - True if all sounds/objects created successfully, false if not.
bool Player::IsValid()
{
	// Check through all XASound objects in the player class and check if they are valid
	for (auto PlayerSound = mPlayerSounds.begin(); PlayerSound != mPlayerSounds.end(); ++ PlayerSound)
	{
		if (!(*PlayerSound)->IsValid())
		{
			// Return false if any of the sounds are not valid
			return false;
		}
	}
	return true;
}
// End IsValid function



// Function:		GetNextPosition() - Get Next Position function
// Description:		This function uses the players current position and the listeners orientation and adds this orientation to the 
//					current position in order to find the position the player would be in if they were to move in this direction.
//					This is used in the Room.cpp file to check for collisions within the room and it is called every frame
// In:				N/a
// Out:				N/a
X3DAUDIO_VECTOR Player::GetNextPosition ()
{
	// Get the Listener's position and the vector of the listeners current orientation
	X3DAUDIO_VECTOR ListenerPosition = mPlayerListener->mListener.Position;
	X3DAUDIO_VECTOR DirectionVector = mPlayerListener->mListener.OrientFront;

	// Find the next position the player will move in if they moved forward in this direction
	// Add the x y and z values of the direction vector to the current position to get the next position
	X3DAUDIO_VECTOR NextPosition;
	NextPosition.x = ListenerPosition.x + DirectionVector.x;
	NextPosition.y = ListenerPosition.y + DirectionVector.y;
	NextPosition.z = ListenerPosition.z + DirectionVector.z;

	// Return this position so it can be used to check collision in the HitWallOrDoor function in Room.cpp
	return NextPosition;
}
// End GetNextPosition function



// Function:		UpdatePlayer(float, float) - Update Player function
// Description:		This function plays sounds given the current state of the player. For example, if the Listener is moving, 
//					this function checks the mListenerMoving bool and plays the footsteps sound. It stores the distance from the 
//					player to the ghost for use in the UpdateHeartbeat function and calls Tick for the gametimers in the class every frame.
//					Plays the footsteps, turning and pain sounds based on their booleans.
// In:				N/a
// Out:				N/a
void Player::UpdatePlayer(float GhostDistance, bool PlayerFree)
{
	// If the intro sound is no longer playing, and the player is not yet active, set the player to active
	if (!mIntroSound->IsPlaying())
	{
		if (!mPlayerActive)
		{
			SetPlayerActive();
		}
	}

	// If the player is free to leave the room and the outro sound has not yet played, play the outro sound
	if ((PlayerFree) && (!mOutroPlayed))
	{
		mOutroSound->Play();
		mOutroPlayed = true;
	}

	// Store the distance for the player to the ghost for the UpdateHeartbeat function
	mDistanceFromGhost = GhostDistance;

	// Update the heartbeat/breathing every frame based on the current distance from the ghost
	UpdateHeartbeat();

	// Call Tick on the Gametimers to update them
	mFootstepTimer->Tick();
	mTurningTimer->Tick();

	// If the footsteps sound is no longer playing, and the footstep timer is above 1.0 (1 second) the player can move
	if ((!mFootsteps->IsPlaying()) && (mFootstepTimer->GetElapsedTime() > 1.0))
	{
		mPlayerCanMove = true;
	}
	// If the turning sound is no longer playing, and the turning timer is above 0.5 (0.5 seconds) the player can turn
	if ((!mTurning->IsPlaying()) && (mTurningTimer->GetElapsedTime() > 0.5))
	{
		mPlayerCanTurn = true;
	}

	// If the listener has moved, play the footsteps sound and set the next footsteps sound
	if (mPlayerListener->mListenerMoving)
	{
		if (mFootsteps->IsValid())
		{
			mFootsteps->Stop();
			mFootsteps->Play();
			// Set the new random footsteps sound
			SetFootstepsSound();
		}
		mPlayerListener->mListenerMoving = false;
	}

	// If the listener has turned, play the turning sound
	if (mPlayerListener->mListenerTurning)
	{
		if (mTurning->IsValid()) 
		{
			mTurning->Play();
			mPlayerCanTurn = false;
		}
		mPlayerListener->mListenerTurning = false;
	}

	// If the player has collided with an object/wall, this is set to true in Room.cpp
	if (mInPain)
	{
		// Play the current pain sound
		if (mCurrentPainSound->IsValid())
		{
			mCurrentPainSound->Play();
		}
		// Get the next random pain sound
		SetPainSound();
	}
}
// End UpdatePlayer function


// Function:		Stop() - Stop function
// Description:		Stop the Player sounds from playing
// In:				N/a
// Out:				N/a
void Player::Stop()
{
	// Check through all XASound objects in the player class and stop them
	for (auto PlayerSound = mPlayerSounds.begin(); PlayerSound != mPlayerSounds.end(); ++ PlayerSound)
	{
		if ((*PlayerSound)->IsValid())
		{
			(*PlayerSound)->Stop();
		}
	}
}


// Function:		SetPlayerActive() - Set Player Active function
// Description:		Plays the breathing and heartbeat files once the player is active 
// In:				N/a
// Out:				N/a
void Player::SetPlayerActive()
{
	// Play the breathing and heartbeat sound files
	mBreathing->Play();
	mHeartBeat->Play();
	// Let the application know the player is now active
	mPlayerActive = true;
}
// End SetPlayerActive function



// Function:		CaptureGhost() - Capture Ghost function
// Description:		Is called when the player presses the Spacebar from the TryToCaptureGhost function in Room.cpp
//					Plays the mCaptureSound XASound object
// In:				N/a
// Out:				N/a
void Player::CaptureGhost()
{
	// Play the capture ghost sound file
	if (mCaptureSound->IsValid())
	{
		mCaptureSound->Play();
	}
}
// End CaptureGhost function



// Function:		UpdateHeartbeat() - Update Heartbeat function
// Description:		If the player is active, this function updates the Heartbeat and Breathing sound files based on the distance of
//					the player from the ghost. The volume of both sounds increases the closer the player is to the ghost's position
//					and the pitch of the heartbeat file increase based on the distance, therefore plays faster and acts as though
//					the player's heartbeat races faster the closer they are. 
// In:				N/a
// Out:				N/a
void Player::UpdateHeartbeat()
{
	// If the player is not active, don't update the heartbeat or breathing
	if (!mPlayerActive)
	{
		return;
	}
	// If the outro sound file has played, pause the heartbeat and breathing so they don't play when the player can escape
	if (mOutroPlayed)
	{
		mHeartBeat->Pause();
		mBreathing->Pause();
	}
	// Else update the breathing and heartbeat sound files
	else
	{
		// Set the max and min volume as well as the volume adjustment
		float maxVolume = 0.0f;
		float minVolume = -20.0f;
		float mVolumeAdjustment = 0.0f;

		// Base the volume on the distance from the ghost, 
		mVolumeAdjustment = 2 * mDistanceFromGhost;

		// The volume is the max volume minus the volume adjustment
		// meaning that the volume is smaller the larger the distance from the ghost is
		float volume = maxVolume - mVolumeAdjustment;

		// Limit the volume to the min volume amount
		if (volume <= minVolume) 
		{
			volume = minVolume;
		} 
		// Set the volume of the heartbeat and breathing files to this new volume
		mHeartBeat->SetVolume (volume);
		mBreathing->SetVolume (volume);

		// Set the pitch/frequency of the heartbeat so it beats faster the closer the player is to the ghost
		mDistanceFromGhost *= 5;

		// The pitch is the max pitch 50 - the distance from the ghost multiplied by 5 (above)
		float pitch = 50 - mDistanceFromGhost;

		// Limit the pitch so that it doesn't fall below 0
		if (pitch <= 0)
		{
			pitch = 0;
		}

		// Set the pitch of the heartbeat to the new pitch so that it will beat faster if the distance is smaller
		mHeartBeat->SetPitch(pitch);
	}
}
// End UpdateHeartbeat function



// Function:		SetFootstepsSound() - Set Footsteps Sound function
// Description:		This function is called whenever the footsteps sound is played when the player moves in order to randomly
//					select the next footsteps sound. There are 3 different footstep sound files and one is randomly selected to give the 
//					room diversity by making the floor sound different in different places
// In:				N/a
// Out:				N/a
void Player::SetFootstepsSound()
{
	// Reset the footstep timer because a footstep has just happened
	mFootstepTimer->Reset();

	// Get the random next footstep ID between 1 and 3
	int FootstepsID = rand() % 3 + 1;

	// Use this random ID to select the next footsteps sound file
	if (FootstepsID == DEFAULT_FOOTSTEPS)
	{
		mFootsteps = mNormalFootsteps;
	}
	if (FootstepsID == FOOTSTEPS_2_VARIANT)
	{
		mFootsteps = mVariant1Footsteps;
	}
	if (FootstepsID == FOOTSTEPS_3_VARIANT)
	{
		mFootsteps = mCreakingFootsteps;
	}
	// Set PlayerCanMove to false so that the player cannot move until the timer has reached 1 second
	mPlayerCanMove = false;
}
// End SetFootstepsSound function



// Function:		SetPainsound() - Set Pain Sound function
// Description:		This function is called when the player collides with a wall or an object in order to randomly select the next
//					pain sound file to use. There are 3 different pain sound files and one is randomly selected to give the player 
//					more variety in how they sound when they collide with an object.
// In:				N/a
// Out:				N/a
void Player::SetPainSound()
{
	// Get the random next pain sound ID between 1 and 3
	int PainID = rand() % 3 + 1;

	// Use this ID to select the next pain sound
	if (PainID == 1)
	{
		mCurrentPainSound = mPainSound1;
	}
	if (PainID == 2)
	{
		mCurrentPainSound = mPainSound2;
	}
	if (PainID == 3)
	{
		mCurrentPainSound = mPainSound3;
	}
	// Set InPain to false so that the pain sound doesn't keep playing after it has played once
	mInPain = false;
}
// End SetPainSound function


// end of code.