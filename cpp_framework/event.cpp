//============================================================================
// Event.cpp - Implements an event manager
//============================================================================
#include "event.h"
#include <unistd.h>
#include <sys/eventfd.h>
#include <sys/select.h>

size_t dummy_size_t;

//============================================================================
// Constructor - Creates the event object in the untriggered state
//============================================================================
CEvent::CEvent()
{
	m_fd = eventfd(0, 0);
}
//============================================================================


//============================================================================
// Destructor - Closes the event file descriptor
//============================================================================
CEvent::~CEvent()
{
	if (m_fd > 0) close(m_fd);
	m_fd = -1;
}
//============================================================================


//============================================================================
// IsTriggered() - Returns 'true' if the event is currently triggered
//============================================================================
bool CEvent::IsTriggered()
{
	fd_set rfds;

	// We're effectively going to poll
	timeval timeout = {0,0};

	// We're only polling just the one specified file-descriptor
	FD_ZERO(&rfds);
	FD_SET(m_fd, &rfds);

	// Is there data on this descriptor?
	int count = select(m_fd+1, &rfds, NULL, NULL, &timeout);

	// Tell the caller whether there is an event value ready for reading
	return (count > 0);
}
//============================================================================



//============================================================================
// Reset() - Resets this object to the "untriggered" state
//============================================================================
void CEvent::Reset()
{
	char value[8];

	// If the event is in the triggered state, clear it
	if (IsTriggered()) dummy_size_t = read(m_fd, &value, sizeof(value));
}
//============================================================================


//============================================================================
// Set() - Triggers the event with the specified value
//============================================================================
void CEvent::Set(unsigned long long value)
{
	dummy_size_t = write(m_fd, &value, sizeof(value));
}
//============================================================================


//============================================================================
// Get() - Waits the specified number of milliseconds for the event to
//         become triggered.
//
// Returns:  The event value, or 0 if the event is untriggered
//============================================================================
uint64_t CEvent::Get(int milliseconds)
{
	uint64_t	event_value;
	fd_set		rfds;

	// Convert milliseconds to microseconds
	int microseconds = milliseconds * 1000;

	// We're going to wait the specified number of milliseconds for the event
	// to become triggered
	timeval timeout = {microseconds / 1000000, microseconds % 1000000};

	// We're only polling just the one specified file-descriptor
	FD_ZERO(&rfds);
	FD_SET(m_fd, &rfds);

	// Is there data on this descriptor?
	int count = select(m_fd+1, &rfds, NULL, NULL, &timeout);

	// If there is no data available, tell the caller that we're untriggered
	if (count < 1) return 0;

	// We're triggered.  Read the event value
	dummy_size_t = read(m_fd, &event_value, sizeof(event_value));

	// Hand the caller the event value
	return event_value;
}
//============================================================================
