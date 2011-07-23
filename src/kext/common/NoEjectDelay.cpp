// -*- Mode: c++; indent-tabs-mode: nil; -*-

#include "base.hpp"
#include "NoEjectDelay.hpp"
#include "version.hpp"

// ----------------------------------------------------------------------
// http://developer.apple.com/documentation/DeviceDrivers/Conceptual/WritingDeviceDriver/CPluPlusRuntime/chapter_2_section_3.html

// This convention makes it easy to invoke base class member functions.
#define super    IOService
// You cannot use the "super" macro here, however, with the
//  OSDefineMetaClassAndStructors macro.
OSDefineMetaClassAndStructors(org_pqrs_driver_NoEjectDelay, IOService)

// ----------------------------------------------------------------------
bool
org_pqrs_driver_NoEjectDelay::init(OSDictionary* dict)
{
  IOLOG_INFO("init %s\n", NoEjectDelay_version);
  return super::init(dict);
}

void
org_pqrs_driver_NoEjectDelay::free(void)
{
  IOLOG_INFO("free\n");
  super::free();
}

IOService*
org_pqrs_driver_NoEjectDelay::probe(IOService* provider, SInt32* score)
{
  IOService* res = super::probe(provider, score);
  return res;
}

bool
org_pqrs_driver_NoEjectDelay::start(IOService* provider)
{
  IOLOG_INFO("start\n");

  bool res = super::start(provider);
  if (! res) { return res; }

  notifier_hookKeyboard_ = addMatchingNotification(gIOMatchedNotification,
                                                   serviceMatching("IOHIDConsumer"),
                                                   org_pqrs_driver_NoEjectDelay::notifierfunc_hookKeyboard,
                                                   this, NULL, 0);
  if (notifier_hookKeyboard_ == NULL) {
    IOLOG_ERROR("addNotification(gIOMatchedNotification) Keyboard\n");
    return false;
  }

  return res;
}

void
org_pqrs_driver_NoEjectDelay::stop(IOService* provider)
{
  IOLOG_INFO("stop\n");

  if (notifier_hookKeyboard_) notifier_hookKeyboard_->remove();

  super::stop(provider);
}

// ----------------------------------------
bool
org_pqrs_driver_NoEjectDelay::notifierfunc_hookKeyboard(void* target, void* refCon, IOService* newService, IONotifier* notifier)
{
  // set Eject delay.
  IOHIDConsumer* consumer = OSDynamicCast(IOHIDConsumer, newService);
  if (! consumer) return true;

  //IOLOG_INFO("notifierfunc_hookKeyboard name = %s\n", consumer->getName());

  IOHIDEventService* service = consumer->_provider;
  if (! service) return true;
  if (! service->_reserved) return true;

  const int DELAY = 5;
  service->_reserved->ejectDelayMS = DELAY;

  // ------------------------------------------------------------
  return true;
}
