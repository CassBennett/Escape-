/*
	-- Original --
	File:	RoomEscape.hpp
	Version:	1.0
	Date:	17th March 2014.
	Author:	Allan C. Milne.

	-- Edit --
	File: GhostEscape.hpp
	Edited by: Cassie Bennett
	Version:	2.0
	Date:	5th May 2015

	Requires:	IState, Room.
	
	Description:
	This class was provided by Allan Milne in the RoomEscape lab. It has been edited slightly for this application.
	This class controls the application and is a concrete realisation of the IState interface.
	This is the concrete strategy class called  from the context WinCore class as part of a strategy pattern.
	*	See IState.hpp for details of this interface.

	This class exposes a user interface controlling the user walking and turning within a room.
	The Room class models a room layout and is composed with a Player class that encapsulates the X3DAudio listener that is ultimately being controlled.

	Smart pointers are used for all references.
	
*/

// Define the class
#ifndef __GHOSTESCAPE_HPP__
#define __GHOSTESCAPE_HPP__

// System includes.
#include <memory>
using std::unique_ptr;

// Framework includes.
#include "IState.hpp"
using AllanMilne::IState;

// Forward declarations.
class Room;

// The GhostEscape class (inheriting from IState)
class GhostEscape : public IState 
{
// Public Member Functions and Variables
public:

	//=== Implementation of the IState interface.

	// Function:		Initialize(HWND) - Initilize function
	// Description:		Creates and initializes the Room object. Since this application has no graphics the argument is not used.
	// In:				HWND - Handle to the window
	// Out:				N/a
	bool Initialize (HWND aWindow);

	// Function:		Setup() - Setup function
	// Description:		Called at the start of processing; resets room, displays instructions and plays the sounds.
	// In:				N/a
	// Out:				N/a
	bool Setup ();

	// Function:		ProcessFrame(const float) - Process Frame function
	// Description:		Uses delta time to process a single application frame.  Check for key user pressess, pass actions as 
	//					appropriate to the Room object for action.
	// In:				const float deltaTime
	// Out:				N/a
	bool ProcessFrame (const float deltaTime);

	// Function:		Cleanup() - Cleanup function
	// Description:		Stop the sounds playing.
	// In:				N/a
	// Out:				N/a
	void Cleanup ();

	//=== Application specific behaviour.

	// Function:		GhostEscape() - GhostEscape constructor
	// Description:		Default constructor.
	// In:				N/a
	// Out:				N/a
	GhostEscape (); 
	
	// Function:		~GhostEscape() - GhostEscape destructor
	// Description:		Destructor - no need for any deletion since smart pointers are used; should stop sound playing.
	// In:				N/a
	// Out:				N/a
	virtual ~GhostEscape();

// Private Member Functions and Variables
private:
	
	//--- Encapsulation of the Room object.
	unique_ptr<Room> mRoom;
	
}; 
// end GhostEscape class.

#endif