/*
	file:	Listener.hpp
	Version:	2.0 (Previous ver - 1.5)
	Date:	27th April 2015
	Author:	Cassie Bennett

	Exposes:	Listener
	Requires:	X3DAudio

	Description:
	This Listener class is based and expanded upon the Listener initialisation code given by Allan Milne and created
	for the RoomEscape solution.
	The functionality has been placed into a class so that the class can encapsulate the functionality of a Listener without
	other classes needing this functionality if they need a listener.
	The class creates a X3DAUDIO_LISTENER object and initialises it based on the start Position and Orientation passed into the 
	Initialisation function.

	The listener can be moved and rotated:
	*	The orientation of the listener can be turned by 45 degrees clockwise and anticlockwise
	*	The position of the listener can be moved forward in the direction of the orientation by 1 unit
	
	A listener object is used for the player in order to hear the sounds around the room. 
*/

// Define the class
#ifndef LISTENER_H
#define LISTENER_H

// System includes
#include <Windows.h>
#include <X3DAudio.h>
#include <memory>

// Directions namespace for the orientations used for the listener
namespace Directions
{
	// Vectors for the orientations need to be normalised, so directions that aren't North, South, East and West need their vectors normalised
	// This value will help with normalising the vectors
	const float NormalisedDirection = 1 / sqrt(2.0f);

	// Set up the directions for the orientation
	const X3DAUDIO_VECTOR Orientations[] = { // North 
		{0.0f, 0.0f, 1.0f},
		// North East
		{NormalisedDirection, 0.0f, NormalisedDirection},
		// East 
		{1.0f, 0.0f, 0.0f},
		// South East
		{NormalisedDirection, 0.0f, -NormalisedDirection},
		// South
		{0.0f, 0.0f, -1.0f},
		// South West
		{-NormalisedDirection, 0.0f, -NormalisedDirection},
		// West
		{-1.0f, 0.0f, 0.0f},
		// North West
		{-NormalisedDirection, 0.0f, NormalisedDirection} };

} // end anonymous namespace.

using namespace std;

// The Listener Class
class Listener
{

// Public Member functions and variables
public:
	// Enumerated values for orientation as compass directions
	enum Direction {
		North, NorthEast, East, SouthEast,
		South, SouthWest, West, NorthWest
	};
	
	// Function:		Listener(X3DAUDIO_VECTOR, Direction) - Listener Constructor
	// Description:		The constructor creates the Listener object. It zeros the variables within the listener to set up to 
	//					the new position and orientation that is passed into the function.
	// In:				X3DAUDIO_VECTOR StartPosition - This is the starting position of the Listener so the listener can be initialised here
	//					Direction StartOrientation - This is the enum value for the starting orientation of the Listener 
	// Out:				N/a
	Listener::Listener(X3DAUDIO_VECTOR StartPosition, Direction StartOrientation);

	// Function:		Reset() - Reset function
	// Description:		Resets the listener's position and orientation to the initial vectors 
	// In:				N/a
	// Out:				N/a
	void Reset();

	// Move forward supplied number of units in current orientation direction.
	// Function:		MoveForward() - Move Forward function
	// Description:		Uses the current orientation of the Listener, and moves the Listener forward in this direction by the distance
	//					Default distance is set to 1.0f, therefore moves 1 unit in the orientation direction when this function is called
	//					When the Up Arrow is pressed in the Room function when the player can move forward.
	// In:				float ThisDistance - Value for the distance the Listener will move in that direction, by default is set to 1.0f
	// Out:				N/a
	void MoveForward (float ThisDistance = 1.0f);

	// Function:		TurnCounterClockwise - Turn Counter Clockwise Function
	// Description:		This function finds the current direction of the Listener and changes it to the direction that is 'Counterclockwise' 
	//					to the current direction based on the order of them  in the Direction enum. For counterclockwise, the direction  
	//					moves negatively along the enum to turn the player Left 
	// In:				N/a
	// Out:				N/a
	void TurnCounterClockwise ();

	// Function:		TurnClockwise - Turn Counter Clockwise Function
	// Description:		This function finds the current direction of the Listener and changes it to the direction that is 'Clockwise' 
	//					to the current direction based on the order of them  in the Direction enum. For clockwise, the direction  
	//					moves positively along the enum to turn the player right
	// In:				N/a
	// Out:				N/a
	void TurnClockwise ();

	// The XAudio2 listener struct.
	X3DAUDIO_LISTENER mListener;

	// Bools to signify to the player if the Listener is moving or turning in order to play the appropriate sounds
	bool mListenerMoving;
	bool mListenerTurning;

	// Current listener orientation of forward direction as an enum Direction between 0 and 7.
	Direction mBodyOrientation;

// Private member functions and variables
private:

	// Function:		MoveTo(X3DAUDIO_VECTOR) - Move To function
	// Description:		Sets the position of the listener to the new position passed in. 
	// In:				X3DAUDIO_VECTOR NewPosition - the position used to set the new position of the Listener
	// Out:				N/a
	void MoveTo (X3DAUDIO_VECTOR NewPosition);

	// Turn (rotate about Y-axis) the listener to a specific direction.
	// Function:		TurnTo(const Listener::Direction) - Turn To function
	// Description:		Turns the Listener orientation to the direction that is passed in. This function is called within the
	//					TurnClockwise and TurnCounterClockwise functions in order to use the new found direction and set the Listener
	//					to this new direciton
	// In:				const Listener::Direction - The name of the new direction that the Listener is to be set to
	// Out:				N/a
	void TurnTo (const Listener::Direction NewDirection);

	// Used for storing the start position and direction of the Listener so they can be used in the Reset() function
	X3DAUDIO_VECTOR mStartPosition;
	Direction mStartOrientation;

	// Store the current position and body orientation in X3DAUDIO_VECTORS 
	X3DAUDIO_VECTOR mCurrentPosition;
	X3DAUDIO_VECTOR mBodyOrient;

};
// end Listener class.

#endif