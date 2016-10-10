/*
	--Original--
	file:	Listener.cpp
	Version:	2.5
	Date:	27th April 2015
	Author:	Cassie Bennett

	Exposes:	Implementation of the Listener class.
	Requires:	X3DAudio

	Description:
	Has the functionality for the Listener object to initialise and set the new positions/orientations of the Listener throughout
	the application to reflect the players position/direction and then use this Listener object with the emitters in order to change
	the panning/volume of the emitter sounds based on the listener position/orientation
	*	See Listener.hpp for details.

	The functions for the Listener are called in the Player class, or through the Player object within the Room class
*/


// Application includes.
#include "Listener.hpp"

// Use the Directions namespace set up in the Listener.hpp file
using namespace Directions;



// Function:		Listener(X3DAUDIO_VECTOR, Direction) - Listener Constructor
// Description:		The constructor creates the Listener object. It zeros the variables within the listener to set up to 
//					the new position and orientation that is passed into the function.
// In:				X3DAUDIO_VECTOR StartPosition - This is the starting position of the Listener so the listener can be initialised here
//					Direction StartOrientation - This is the enum value for the starting orientation of the Listener 
// Out:				N/a
Listener::Listener(X3DAUDIO_VECTOR StartPosition, Direction StartOrientation)
{
	// Store the start position and orientation so they can be used in the Reset() function
	mStartPosition = StartPosition;
	mStartOrientation = StartOrientation;	
	
	// Set up the X3DAudio listener struct; zeroing will set position to (0,0,0).
	SecureZeroMemory (&mListener, sizeof(X3DAUDIO_LISTENER));

	// Set the Position and Orientation based on the information passed into the constructor
	mListener.Position = mStartPosition;
	mListener.OrientFront = Orientations[mStartOrientation];
	mCurrentPosition = mListener.Position;
	mBodyOrient = mListener.OrientFront;
	mBodyOrientation = StartOrientation;

	// Set the OrientTop vector so that positive y is the upright axis
	mListener.OrientTop.x = 0.0f;
	mListener.OrientTop.y = 1.0f;
	mListener.OrientTop.z = 0.0f;

	// Bools to signify to the player class that sounds should be played
	mListenerMoving = false;
	mListenerTurning = false;
}
// End Listener constructor



// Function:		Reset() - Reset function
// Description:		Resets the listener's position and orientation to the initial vectors 
// In:				N/a
// Out:				N/a
void Listener::Reset()
{
	// Resets listener back to start with start position and start orientation
	MoveTo(mStartPosition);
	TurnTo(mStartOrientation);
}
// End Reset function



// Function:		MoveTo(X3DAUDIO_VECTOR) - Move To function
// Description:		Sets the position of the listener to the new position passed in. 
// In:				X3DAUDIO_VECTOR NewPosition - the position used to set the new position of the Listener
// Out:				N/a
void Listener::MoveTo (X3DAUDIO_VECTOR NewPosition)
{
	// Set the position of the Listener to the new position passed into the function
	mListener.Position = NewPosition;
	// Set the mListenerMoving function to true to tell the Player class that the Listener has moved
	mListenerMoving = true;
} 
// end MoveTo function.



// Move forward supplied number of units in current orientation direction.
// Function:		MoveForward() - Move Forward function
// Description:		Uses the current orientation of the Listener, and moves the Listener forward in this direction by the distance
//					Default distance is set to 1.0f, therefore moves 1 unit in the orientation direction when this function is called
//					When the Up Arrow is pressed in the Room function when the player can move forward.
// In:				float ThisDistance - Value for the distance the Listener will move in that direction, by default is set to 1.0f
// Out:				N/a
void Listener::MoveForward(float Distance)
{
	// Use the current orientation of the player and find the values of the x y and z
	// Add these values multiplied by the distance passed in in order to move the Listener in that direction
	// E.g. if mBodyOrientation = South {0.0f, 0.0f, -1.0f} therefore will move by -1 + (-1 * distance) in the z axis
	mListener.Position.x += Orientations[mBodyOrientation].x * Distance;
	mListener.Position.y += Orientations[mBodyOrientation].y * Distance;
	mListener.Position.z += Orientations[mBodyOrientation].z * Distance;

	// Set mListenerMoving to true to tell the Player class that the Listener is moving
	mListenerMoving = true;
}
// End MoveForward function



// Turn (rotate about Y-axis) the listener to a specific direction.
// Function:		TurnTo(const Listener::Direction) - Turn To function
// Description:		Turns the Listener orientation to the direction that is passed in. This function is called within the
//					TurnClockwise and TurnCounterClockwise functions in order to use the new found direction and set the Listener
//					to this new direciton
// In:				const List
void Listener::TurnTo (const Listener::Direction NewDirection)
{
	// Set the orientation of the Listener to the new direction passed into the function
	mBodyOrientation = NewDirection;
	// Use the orientation within the array of Orientations to find the vector to respresent this direction
	mListener.OrientFront = Orientations[mBodyOrientation];
	// Set mListenerTurning to true to tell the Player class that the Listener is turning
	mListenerTurning = true;
} 
// end TurnTo function.



// Function:		TurnCounterClockwise - Turn Counter Clockwise Function
// Description:		This function finds the current direction of the Listener and changes it to the direction that is 'Counterclockwise' 
//					to the current direction based on the order of them  in the Direction enum. For counterclockwise, the direction  
//					moves negatively along the enum to turn the player Left 
// In:				N/a
// Out:				N/a
void Listener::TurnCounterClockwise()
{
	// Get the integer value of the enum current BodyOrientation
	int NewDirection = int(mBodyOrientation);

	// Decrement this integer in order to move negatively down the enumerator variables, to turn the Listener left
	NewDirection -= 1;
	
	// If the NewDirection becomes less than 0, reset to 7 so that the turning wraps around the enum variables
	if (NewDirection < 0)
	{
		NewDirection = 7;
	}

	// Set the body orientation to the new direction enum value
	mBodyOrientation = (Direction)NewDirection;
	
	// Use this orientation to turn the Listener to this new orientation in the TurnTo function
	TurnTo(mBodyOrientation);

	// Set the mListenerTurning function to true to tell the player that the Listener is turning
	mListenerTurning = true;
} 
// end TurnCounterClockwise funtion.



// Function:		TurnClockwise - Turn Counter Clockwise Function
// Description:		This function finds the current direction of the Listener and changes it to the direction that is 'Clockwise' 
//					to the current direction based on the order of them  in the Direction enum. For clockwise, the direction  
//					moves positively along the enum to turn the player right
// In:				N/a
// Out:				N/a
void Listener::TurnClockwise ()
{
	// Get the integer value of the enum current BodyOrientation
	int NewDirection = int(mBodyOrientation);

	// Increment this integer in order to move positively up the enumerator variables, to turn the Listener right
	NewDirection += 1;

	// If the NewDirection becomes more than 7, reset to 0 so that the turning wraps around the enum variables
	if (NewDirection > 7)
	{
		NewDirection = 0;
	}
	
	// Set the body orientation to the new direction enum value
	mBodyOrientation = (Direction)NewDirection;

	// Use this orientation to turn the Listener to this new orientation in the TurnTo function
	TurnTo(mBodyOrientation);

	// Set the mListenerTurning function to true to tell the player that the Listener is turning
	mListenerTurning = true;
} 
// end TurnClockwise function.

// end of code.