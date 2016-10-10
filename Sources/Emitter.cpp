/*
	--Original--
	file:	Emitter.hpp
	Version:	3.0
	Date:		1st May 2015
	Author:		Cassie Bennett

	Exposes:	Implementation of the Emitter class.
	Requires:	X3DAudio, XASound, XACore, StuVector3

	Description:
	Has the functionality for the Emitter object to initialise and set the new positions and sounds of emitter objects throughout
	the application. Uses a reference of the Listener in the application in order to update the settings of the emitter's sounds
	based on the position and if available, its cone. 
	*	See Emitter.hpp for details.

	The functions for the Emitters are called through their emitter objects. 
*/

// Application includes
#include "Emitter.hpp"



// Function:		Emitter(XASound, X3DAUDIO_VECTOR, X3DAUDIO_LISTENER, bool) - Emitter Constructor for emitter without a cone
// Description:		This Emitter constructor is used for emitters that do not have a cone. It is passed the sound for the emitter, 
//					the initial position of the emitter, a pointer to the Player's Listener object and a bool to tell the emitter
//					if its sound should be looped. 
//					The function sets the sound for the emitter and the position, setting it to looped or not based on the bool passed in. 
//					It zeros the data within the Emitter struct so that it can be initialised to its settings.
// In:				XASound *EmitterSound - Pointer to the initial XASound for the emitter, X3DAUDIO_VECTOR EmitterPosition - Initial
//					emitter position of the emitter. X3DAUDIO_LISTENER *Listener - pointer to the Player's listener object so that this 
//					can be used when updating the emitter to base its settings on the position/orientation of the player.
//					bool IsLooped - simple bool to tell the constructor whether to loop the sound or not
// Out:				N/a
Emitter::Emitter(XASound *EmitterSound, X3DAUDIO_VECTOR EmitterPosition, X3DAUDIO_LISTENER *Listener, bool IsLooped)
{
	// Set the sound to the XASound passed in for the emitter
	mSound = EmitterSound;

	// Store the start position of the emitter
	mStartPosition = EmitterPosition;

	// Cannot proceed if emitter sound is invalid
	if (!mSound->IsValid()) 
	{
		return;
	}

	// Set the sound to loop based on the bool IsLooped
	mSound->SetLooped(IsLooped);

	// Store the pointer to the Player's Listener struct to use this in the UpdateEmitter function
	mListener = Listener;

	// Initialise emitter by setting all variables of the struct to 0
	SecureZeroMemory (&mEmitter, sizeof(X3DAUDIO_EMITTER));

	// Store the details of the source voice
	XAUDIO2_VOICE_DETAILS details;
	mSound->GetSourceVoice()->GetVoiceDetails(&details);
	mEmitter.ChannelCount = details.InputChannels;
	mEmitter.CurveDistanceScaler = 1.0f;
	mEmitter.Position = EmitterPosition;	// Store the position of the emitter in the struct

	// Cone On = false because this constructor is used for emitters without cones
	// Therefore cone functionality won't be applied to this emitter
	mConeOn = false;
}
// End of Emitter constructor



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

Emitter::Emitter(XASound *EmitterSound, X3DAUDIO_VECTOR EmitterPosition, X3DAUDIO_LISTENER *Listener, v3f *SoundPosition, v3f *OrientationPosition, bool IsLooped)
{
	// Set the sound to the XASound passed in for the emitter
	mSound = EmitterSound;

	// Store the start position of the emitter
	mStartPosition = EmitterPosition;

	// Cannot proceed if emitter sound is invalid
	if (!mSound->IsValid()) 
	{
		return;
	}

	// Set the sound to loop based on the bool IsLooped
	mSound->SetLooped(IsLooped);

	// Store the pointer to the Player's Listener struct to use this in the UpdateEmitter function
	mListener = Listener;

	// Set ConeOn to true so that a cone for this emitter will be created and the functionality for the cone will be applied
	mConeOn = true;

	// Store the Sound and Orientation Position v3f objects for the cone initialisation
	mSoundPosition = *SoundPosition;
	mOrientationPosition = *OrientationPosition;

	// Initialise emitter by setting all variables of the struct to 0
	SecureZeroMemory (&mEmitter, sizeof(X3DAUDIO_EMITTER));

	// Store the details of the source voice
	XAUDIO2_VOICE_DETAILS details;
	mSound->GetSourceVoice()->GetVoiceDetails(&details);
	mEmitter.ChannelCount = details.InputChannels;
	mEmitter.CurveDistanceScaler = 1.0f;
	mEmitter.Position = EmitterPosition;		// Store the position of the emitter in the struct

	// If the cone is on, create the emitter cone
	if (mConeOn)
	{
		CreateEmitterCone();
	}
}
// End of emitter constructor


// Function:		CreateEmitterCone() - Create Emitter Cone function
// Description:		Creates an emitter cone for emitters who have a cone object. It sets the cone orientation based on the sound position
//					and the orientation position, normalises and sets this to the emitter sound's orientation. The Cone struct is initialised 
//					giving it an inner angle/volume and outer angle/volume, and then the cone is created.
// In:				N/a
// Out:				N/a
void Emitter::CreateEmitterCone()
{
	// Set up a sound cone based on the sound position and the orientation position
	// Get 
	stu::v3f coneOrientation = mOrientationPosition - mSoundPosition;
	const float OrientationMagnitude = coneOrientation.Magnitude();
	coneOrientation.Normalise();
	// Set the orientation of the emitter to the normalised cone orientation
	mEmitter.OrientFront.x = coneOrientation.x;
	mEmitter.OrientFront.y = coneOrientation.y;
	mEmitter.OrientFront.z = coneOrientation.z;
	// Initialise the cone struct to 0s
	SecureZeroMemory (&mCone, sizeof(X3DAUDIO_CONE));

	// Set the inner angle/volume and outer angle/volume for the cone
	mCone.InnerAngle = 2.0f * atan((3.0f/2.0f) / OrientationMagnitude);
	mCone.InnerVolume = 1.5f;
	mCone.OuterAngle = mCone.InnerAngle + 0.2f;
	mCone.OuterVolume = 0.5f;
	mEmitter.pCone = &mCone;
}
//End of CreateEmitterCone function



// Function:		UpdateEmitter() - Update Emitter function
// Description:		Calculates and applys 3D audio DSP settings to the emitters sound based on the Listener's position and orientation
//					Therefore updates how the emitters sound settings should be 
// In:				N/a
// Out:				N/a
void Emitter::UpdateEmitter()
{
	// If the Emitter sound is currently playing, calculate and apply the 3D settings to the sound based on the position/orientation
	// of the player using the Apply3D function in the XACore class and passing in the emitters sound, the address of the emitter, the 
	// listener struct and the calculation matrix.
	if (mSound->IsPlaying())
	{
		XACore::GetInstance()->Apply3D(mSound->GetSourceVoice(), &mEmitter, mListener, X3DAUDIO_CALCULATE_MATRIX);
	}
}
// End of UpdateEmitter function



// Function:		ResetEmitter(bool) - Reset Emitter Function
// Description:		Reset function resets the emitter by reseting the cone if there is one, Updating the emitter, and setting the 
//					emitter to Play when this is called.
// In:				bool IsPlaying - bool to tell the function if the sound is currently playing
// Out:				N/a
void Emitter::ResetEmitter(bool IsPlaying)
{
	// If the emitter has a cone, reset the cone
	if (mConeOn)
	{
		mEmitter.pCone = &mCone;
	}
	// If the sound is playing when it is reset, play the sound.
	if (IsPlaying)
	{
		if (mSound->IsValid()) 
		{
			mSound->Play();
		}
	}
	// Update the emitter to its settings
	UpdateEmitter();
}
// End ResetEmitter function



// Function:		ChangeEmitterSound(XASound, X3DAUDIO_VECTOR, bool) - Change Emitter Sound function
// Description:		Function re-initialises the emitter object which already has a sound to the new sound, by changing mSound to the sound
//					passed in and zeroing the Emitter struct so it can be set to the new Emitter details and then play the new sound.
// In:				XASound *EmitterSound - Pointer to the new Emitter sound, X3DAUDIO_VECTOR EmitterPosition - The new emitter position, 
//					bool IsLooped - Bool to tell function if the new sound should be looped or not. 
// Out:				N/a
void Emitter::ChangeEmitterSound(XASound *EmitterSound, X3DAUDIO_VECTOR EmitterPosition, bool IsLooped)
{
	// Stop the current sound from playing
	if (mSound->IsValid()) 
	{
		mSound->Stop();
	}
	
	// Assign the mSound XASound object to the new sound passed into this function
	mSound = EmitterSound;

	// Loop the sound file if it is to be looped
	mSound->SetLooped(IsLooped);

	// Re-initialise emitter for the new sound
	SecureZeroMemory (&mEmitter, sizeof(X3DAUDIO_EMITTER));

	// Get the new source voice details and the new position and assign this info to the emitter struct
	XAUDIO2_VOICE_DETAILS details;
	mSound->GetSourceVoice()->GetVoiceDetails(&details);
	mEmitter.ChannelCount = details.InputChannels;
	mEmitter.CurveDistanceScaler = 1.0f;
	mEmitter.Position = EmitterPosition;

	mConeOn = false;

	// If the sound is valid, play the sound.
	if (mSound->IsValid()) 
	{
		mSound->Play();
	}
	
	// Update the emitter with the new sound and settings
	UpdateEmitter();
}
// End ChangeEmitterSound function



// Function:		StopEmitter() - Stop Emitter function
// Description:		If the emitter sound is valid, this function stops the sound from playing (is usually called in destructors)
// In:				N/a
// Out:				N/a
void Emitter::StopEmitter()
{
	// If the sound is valid, stop playing the sound
	if (mSound->IsValid()) 
	{
		mSound->Stop();
	}
}
// End StopEmitter function



// Function:		PlayEmitter() - Play Emitter function
// Description:		If the emitter sound is valid, this function plays the emitter sound
// In:				N/a
// Out:				N/a
void Emitter::PlayEmitter()
{
	// If the sound is valid, play the sound
	if (mSound->IsValid())
	{
		mSound->Play();
	}
}
// End PlayEmitter function



// Function:		PauseEmitter() - Pause Emitter function
// Description:		If the emitter sound is valid, this function pauses the emitter sound
// In:				N/a
// Out:				N/a
void Emitter::PauseEmitter()
{
	// If the sound is valid, pause the sound
	if (mSound->IsValid())
	{
		mSound->Pause();
	}
}
// End PauseEmitter function


// Function:		SetEmitterFilter(XAUDIO2_FILTER_TYPE, float, float) - Set Emitter Filter function
// Description:		This function sets the type of filter for the sound of the emitter so that the functionality doesn't need
//					to be accessed through the emitters XASound object. Uses the filter type, frequency and attenuation values in
//					the XASound function 'SetFilter'. 
// In:				XAUDIO2_FILTER_TYPE FilterType - this is the type of filter used for the sound. float Frequency - this is the value
//					used for the filter, e.g. if using a LowPassFilter, this is the frequency value above which sounds will be attenuated
//					float AttenuationFactor - this is the strength of the attenuation used for the filter.
// Out:				N/a
void Emitter::SetEmitterFilter(XAUDIO2_FILTER_TYPE FilterType, float Frequency, float AttenuationFactor)
{
	// Set the Filter type for the sound based on the FilterType, frequency, and attenuation factor passed into the function
	mSound->SetFilter(FilterType, Frequency, AttenuationFactor);
}
// End SetEmitterFilter function



// Function:		SetPosition(X3DAUDIO_VECTOR) - Set Position function
// Description:		Sets the position of the emitter to the new position passed in 
// In:				X3DAUDIO_VECTOR NewPosition - The new X3DAUDIO_VECTOR of the position to move the emitter to
// Out:				N/a

void Emitter::SetPosition(X3DAUDIO_VECTOR NewPosition)
{
	// Set the position of the emitter to the new position passed into this function
	mEmitter.Position = NewPosition;
}
// End SetPosition function



// Function:		EmitterActive() - Emitter Active function
// Description:		This function checks to see if the sound of the emitter is currently playing, in order to tell if the 
//					emitter's sound is active or not.
// In:				N/a
// Out:				bool - Returns true if the sound is playing, false if not
bool Emitter::EmitterActive()
{
	// If the sound is currently playing, return true
	if (mSound->IsPlaying())
	{
		return true;
	}
	// Else if the sound is not playing, return false as the sound is no longer active
	else
	{
		return false;
	}
}
// End EmitterActive function

// end of code.