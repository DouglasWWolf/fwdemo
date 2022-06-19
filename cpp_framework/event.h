//============================================================================
// event.h - Defines an eventfd() based event-manager
//============================================================================
#pragma once
#include <cstdint>


//============================================================================
// CEvent - A waitable event object based on the "eventfd" system call
//============================================================================
class CEvent
{
public:

	// Constructs an event object in the "untriggered" state
	CEvent();

	// Closes the event file-descriptor
	~CEvent();

	// Resets the event to the "untriggered" state
	void	Reset();

	// Triggers the event with the specified value.  If the event is already
	// triggered, this will be added to the existing event value
	void	Set(U64 value = 1);

	// Wait "milliseconds" for the event to become triggered.  Returns either
	// the triggered value, or zero if the event hasn't happened yet.  If the
	// return value is non-zero, the event is automatically reset to the
	// "untriggered" state
 	U64 	Get(int milliseconds = 0);

	// Returns 'true' if the event is in the "triggered" state
	bool	IsTriggered();

	// Returns the event file descriptor for use with "select"
	int		FD() {return m_fd;}


protected:

	int 	m_fd;

};
//============================================================================
