/*
	-- Original -- 
	File:	RoomEscape.cpp
	Version:	1.0
	Date:	20th March 2014.
	Author:	Allan C. Milne.

	-- Edit --
	File: GhostEscape.hpp
	Edited by: Cassie Bennett
	Version:	2.0
	Date:	5th May 2015

	Exposes:	implementation of the GhostEscape class.
	Requires:	XACore, Room.
	
	Description:
	This is the implementation of the Room class members;
	*	see GhostEscape.hpp for the class definition and further information on the application functionality.

*/

// System includes.
#include <memory>
#include <string>
#include <sstream>
#include <iostream>
using namespace std;

// Framework includes.
#include "XACore.hpp"
using AllanMilne::Audio::XACore;

// Application includes.
#include "GhostEscape.hpp"
#include "Room.hpp"

//--- Anonymous namespace defining constants and helper functions used in the implementation.
namespace 
{
	//--- Display instructions on using this application.
	void Instructions () 
	{
		stringstream msg;
		msg<<endl <<" The ghost trapped you in the mansion! You must try to ward the ghost off! " <<endl <<endl;
		msg <<" Locate the ghost in the room and use the spirit drainer to try and force him to open the doors." <<endl;
		msg <<" The doors will open when the ghost leaves, and you will be free to escape!" << endl;
		msg <<" Spacebar \t\t = Try to capture ghost with the Spirit Drainer." << endl;
		msg <<" Up arrow \t\t = move forward 1 step in the current direction. " <<endl;
		msg <<" left/right arrows \t = turn 90 degrees counter-clockwise/" <<endl;
		msg <<" \t\t clockwise respectively. " <<endl;
		msg <<" 'R' \t\t = Reset player back to start position in the room. " <<endl;
		msg <<" 'I' \t\t = Display these instructions. " <<endl;
		msg <<" escape \t\t = end the application. " <<endl <<endl;
		const string msgStr = msg.str();
		MessageBox (NULL, msgStr.c_str(), TEXT("Ghost Escape"), MB_OK | MB_ICONINFORMATION);
	} // end Instructions function.

} // end anonymous namespace.

//=== Implementation of the IState interface.

// Function:		Initialize(HWND) - Initilize function
// Description:		Creates and initializes the Room object. Since this application has no graphics the argument is not used.
// In:				N/a
// Out:				N/a
bool GhostEscape::Initialize (HWND aWindow)
{
	// guard against uninitialized or invalid XACore.
	if (XACore::GetStatus() != XACore::OK) return false;

	mRoom.reset(new Room ());
	if (!mRoom->IsValid()) 
	{
		MessageBox (NULL, "Error creating the room.", TEXT ("Initialize() - FAILED"), MB_OK | MB_ICONERROR );
		return false;
	}
	
	return true;		// everything was initialized correctly.
} 
// end Initialize function.


	
// Function:		Setup() - Setup function
// Description:		Called at the start of processing; resets room, displays instructions and plays the sounds.
// In:				N/a
// Out:				N/a
bool GhostEscape::Setup ()
{
	Instructions();
	mRoom->Reset();
	return true;		// All has been setup without error.
} 
// end Setup function.



// Function:		ProcessFrame(const float) - Process Frame function
// Description:		Uses delta time to process a single application frame.  Check for key user pressess, pass actions as 
//					appropriate to the Room object for action.
// In:				const float deltaTime
// Out:				N/a
bool GhostEscape::ProcessFrame (const float deltaTime)
{
	// Check if keys were pressed since last key state call - least significant bit.
	if (GetAsyncKeyState (VK_UP) & 0x0001) {
		mRoom->MovePlayer();
		// check for exiting the room - this indicates success in this application.
		if (mRoom->DoorExited())
		{
			MessageBox (NULL, TEXT("\n\n You escaped the ghost! \n\n "), TEXT(" Ghost Escape "), MB_OK | MB_ICONINFORMATION);
			mRoom->Stop();
			return false;
		}
	}
	// If the left arrow is pressed, turn the player left
	if (GetAsyncKeyState (VK_LEFT) & 0x0001) 
	{
		mRoom->TurnPlayerLeft();
	}
	// If the right arrow is pressed, turn the player right
	if (GetAsyncKeyState (VK_RIGHT) & 0x0001) 
	{
		mRoom->TurnPlayerRight();
	}
	// If the Space key is pressed, call TryToCaptureGhost from the mRoom object
	if (GetAsyncKeyState (VK_SPACE) & 0x0001)
	{
		mRoom->TryToCaptureGhost();
	}
	// If the R key is pressed, call the Reset function from the mRoom object
	if (GetAsyncKeyState ('R') & 0x0001) 
	{
		mRoom->Reset();
	}
	// If the I key is pressed, show the instructions
	if (GetAsyncKeyState ('I') & 0x0001) 
	{
		Instructions();
	}

	// Updates
	mRoom->UpdateRoom();

	return true;
} 
// end ProcessFrame function.



// Function:		Cleanup() - Cleanup function
// Description:		Stop the sounds playing.
// In:				N/a
// Out:				N/a
void GhostEscape::Cleanup ()
{
	mRoom->Stop();
} 
// end cleanup function.


//=== Implementation of application specific functions.



// Function:		GhostEscape() - GhostEscape constructor
// Description:		Default constructor.
// In:				N/a
// Out:				N/a
GhostEscape::GhostEscape()
	: mRoom(nullptr)
{ 
} 
// end GhostEscape constructor.



// Function:		~GhostEscape() - GhostEscape destructor
// Description:		Destructor - no need for any deletion since smart pointers are used; should stop sound playing.
// In:				N/a
// Out:				N/a
GhostEscape::~GhostEscape()
{
	mRoom->Stop();
} // end GhostEscape destructor function.

//=== end of code.