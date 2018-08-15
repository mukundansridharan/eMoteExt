/*
 * Handlers.h
 *
 *  Created on: Sep 5, 2012
 *      Author: Mukundan
 */

#ifndef HANDLERS_H_
#define HANDLERS_H_

#include <tinyhal.h>
#include <Samraksh/Hal_Util.h>
#include <Samraksh/Message.h>

/*
 *
 */
class EventHandler {
  public:
	void Initialize();
	// the scheduler calls this command at the beginning of a slot;
	// @param slotNum the current slot number
	//  @return the handler's offset (in jitters) in the slot that the radio should be started.
	// a return value of 0 is to start the radio immediately
	UINT16 NextEvent(UINT32 currentSlotNum);
	//UINT64 NextEvent(UINT32 currentSlotNum);
	// the scheduler calls this command at the slot winner
	// @param slotNum the current slot number
	void ExecuteEvent(UINT32 currentSlotNum);
	// the handler signals this event once its access to the radio is finished
	UINT8 ExecuteEventDone();
	// the scheduler calls this command after the radio is stopped
	void PostExecuteEvent();

	//Base class destructor should also be virtual or should be protected and non-virtual
	//virtual ~EventHandler() {};

  /*protected:
	~EventHandler();*/
};


/*extern "C" void __cxa_pure_virtual()
{
  while (1);
}*/

#endif /* HANDLERS_H_ */
