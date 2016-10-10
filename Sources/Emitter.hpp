/*
	file:	Emitter.hpp
	Version:	3.0
	Date:		1st May 2015
	Author:		Cassie Bennett

	Exposes:	Emitter
	Requires:	X3DAudio, XASound, XACore, StuVector3

	Description:
	This Emitter class is based and expanded upon the Emitter example initialisation given by Allan Milne in the RoomEscape solution
	that was originally the initialisation for the OutsideNoise in the Room Class. The code has been expanded and used in a class 
	so that emitter objects can be created in the application without needing to re-type code that is better suited as an object for
	an object oriented program. The class encapsulates all of the behaviour of an Emitter such as initialising their positions and orientations
	as well as setting up a cone if needed and updating them, and more behaviour such as changing the sounds used for a single emitter.
	
	The object can be interacted with to set up and change the emitter in the following ways:
	*	Set up the emitter with a cone or without a cone, and give it its initial sound/position
	*	The emitters sound can be changed during run time
	*	Set the filter of the emitter's sound
	*	Play, Stop, Pause, and check if the emitter is active
	*	Set the emitter to loop, and also reset the emitter

	Emitter objects are used throughout the application to encapsulate an object in the scene that produces sounds at specific positions

*/

// Define the class
#ifndef EMITTER_H
#define EMITTER_H

// System includes
#include <Windows.h>
#include <X3DAudio.h>
#include <memory>

// Framework includes.
#include "XASound.hpp"
using AllanMilne::Audio::XASound;

#include "XACore.hpp"
using AllanMilne::Audio::XACore;

#include "StuVector3.hpp"
using namespace stu;

// The Emitter Class
class Emitter
{

// Public Member Functions and Variables
public:
	// Function:		Emitter(XASound, X3DAUDIO_VECTOR, X3DAUDIO_LISTENER, bool) - Emitter Constructor for emitter without a cone
	// Description:		This Emitter constructor is used for emitters that do not have a cone. 
	//					The function sets the sound for the emitter and the position, setting it to looped or not based on the bool passed in. 
	//					It zeros the data within the Emitter struct so that it can be initialised to its settings.
	// In:				XASound *EmitterSound - Pointer to the initial XASound for the emitter, X3DAUDIO_VECTOR EmitterPosition - Initial
	//					emitter position of the emitter. X3DAUDIO_LISTENER *Listener - pointer to the Player's listener object so that this 
	//					can be used when updating the emitter to base its settings on the position/orientation of the player.
	//					bool IsLooped - simple bool to tell the constructor whether to loop the sound or not
	// Out:				N/a
	Emitter(XASound *EmitterSound, X3DAUDIO_VECTOR EmitterPosition, X3DAUDIO_LISTENER *Listener, bool IsLooped);
	// For emitters with cones

	// Function:		Emitter(XASound, X3DAUDIO_VECTOR, X3DAUDIO_LISTENER, v3f, v3f, bool) - Emitter constructor for an emitter with a cone
	// Description:		Very similar to the normal constructor, however takes more data in order to set up a cone for the emitter.
	//					The function sets the sound for the emitter and the position, setting it to looped or not based on the bool passed in. 
	//					It zeros the data within the Emitter struct so that it can be initialised to its settings. It also uses the Sound Position and 
	//					Orientation Position and stores them in order to set up the cone using the CreateEmitterCone() function
	// In:				XASound *EmitterSound - Pointer to the initial XASound for the emitter, X3DAUDIO_VECTOR EmitterPosition - Initial
	//					emitter position of the emitter. X3DAUDIO_LISTENER *Listener - pointer to the Player's listener object so that this 
	//					can be used when updating the emitter to base its settings on the position/orientation of the player.
	//					v3f *SoundPosition - a Vector3 of the sounds current position
	//					v3f *OrientationPosition - the vector position the sound is pointing towards
	//					bool IsLooped - simple bool to tell the constructor whether to loop the sound or not
	// Out:				N/a
	Emitter(XASound *EmitterSound, X3DAUDIO_VECTOR EmitterPosition, X3DAUDIO_LISTENER *Listener, v3f *SoundPosition, v3f *OrientationPosition, bool IsLooped);
	
	// Function:		UpdateEmitter() - Update Emitter function
	// Description:		Calculates and applys 3D audio DSP settings to the emitters sound based on the Listener's position and orientation
	//					Therefore updates how the emitters sound settings should be 
	// In:				N/a
	// Out:				N/a
	void UpdateEmitter();

	// Function:		ResetEmitter(bool) - Reset Emitter Function
	// Description:		Reset function resets the emitter by reseting the cone if there is one, Updating the emitter, and setting the 
	//					emitter to Play when this is called.
	// In:				bool IsPlaying - bool to tell the function if the sound is currently playing
	// Out:				N/a
	void ResetEmitter(bool IsPlaying);

	// Function:		ChangeEmitterSound(XASound, X3DAUDIO_VECTOR, bool) - Change Emitter Sound function
	// Description:		Function re-initialises the emitter object which already has a sound to the new sound, by changing mSound to the sound
	//					passed in and zeroing the Emitter struct so it can be set to the new Emitter details and then play the new sound.
	// In:				XASound *EmitterSound - Pointer to the new Emitter sound, X3DAUDIO_VECTOR EmitterPosition - The new emitter position, 
	//					bool IsLooped - Bool to tell function if the new sound should be looped or not. 
	// Out:				N/a
	void ChangeEmitterSound(XASound *EmitterSound, X3DAUDIO_VECTOR EmitterPosition, bool IsLooped);


	// Function:		StopEmitter() - Stop Emitter function
	// Description:		If the emitter sound is valid, this function stops the sound from playing (is usually called in destructors)
	// In:				N/a
	// Out:				N/a
	void StopEmitter();

	// Function:		PlayEmitter() - Play Emitter function
	// Description:		If the emitter sound is valid, this function plays the emitter sound
	// In:				N/a
	// Out:				N/a
	void PlayEmitter();

	// Function:		PauseEmitter() - Pause Emitter function
	// Description:		If the emitter sound is valid, this function pauses the emitter sound
	// In:				N/a
	// Out:				N/a
	void PauseEmitter();

	// Function:		SetEmitterFilter(XAUDIO2_FILTER_TYPE, float, float) - Set Emitter Filter function
	// Description:		This function sets the type of filter for the sound of the emitter so that the functionality doesn't need
	//					to be accessed through the emitters XASound object. Uses the filter type, frequency and attenuation values in
	//					the XASound function 'SetFilter'. 
	// In:				XAUDIO2_FILTER_TYPE FilterType - this is the type of filter used for the sound. float Frequency - this is the value
	//					used for the filter, e.g. if using a LowPassFilter, this is the frequency value above which sounds will be attenuated
	//					float AttenuationFactor - this is the strength of the attenuation used for the filter.
	// Out:				N/a
	void SetEmitterFilter(XAUDIO2_FILTER_TYPE FilterType, float Frequency, float AttenuationFactor);

	// Function:		SetPosition(X3DAUDIO_VECTOR) - Set Position function
	// Description:		Sets the position of the emitter to the new position passed in 
	// In:				X3DAUDIO_VECTOR NewPosition - The new X3DAUDIO_VECTOR of the position to move the emitter to
	// Out:				N/a
	void SetPosition(X3DAUDIO_VECTOR NewPosition);

	// Function:		EmitterActive() - Emitter Active function
	// Description:		This function checks to see if the sound of the emitter is currently playing, in order to tell if the 
	//					emitter's sound is active or not.
	// In:				N/a
	// Out:				bool - Returns true if the sound is playing, false if not
	bool EmitterActive();

	// The XAudio2 emitter struct.
	X3DAUDIO_EMITTER mEmitter;

// Private Member Functions and Variables
private:
	// Function:		CreateEmitterCone() - Create Emitter Cone function
	// Description:		Creates an emitter cone for emitters who have a cone object. It sets the cone orientation based on the sound position
	//					and the orientation position, normalises and sets this to the emitter sound's orientation. The Cone struct is initialised 
	//					giving it an inner angle/volume and outer angle/volume, and then the cone is created.
	// In:				N/a
	// Out:				N/a
	void CreateEmitterCone();

	// Function:		LoopEmitter(bool) - Loop Emitter function
	// Description:		This function sets the sound of the emitter to loop or not depending on the bool passed in
	// In:				bool Loop - if true will set the sound to loop, if false will set sound to not loop
	// Out:				N/a
	inline void LoopEmitter(bool Loop) { mSound->SetLooped(Loop); }

	// This is the XASound object for the emitter
	XASound *mSound;

	// The Cone struct
	X3DAUDIO_CONE mCone;

	// This is the reference object for the Player's Listener struct
	X3DAUDIO_LISTENER *mListener;

	// Stores the Start Position of the emitter
	X3DAUDIO_VECTOR mStartPosition;

	// Stores the sound position and orientation position as v3f objects so they can be used to initialise the cone
	v3f mSoundPosition;
	v3f mOrientationPosition; 

	// Bool to tell the emitter if it has a cone or not
	bool mConeOn;

	// Bool to tell if the emitter is currently playing a sound or not
	bool mActive;
};
// End of Emitter class

#endif