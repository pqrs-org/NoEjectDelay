// -*- Mode: c++; indent-tabs-mode: nil; -*-

#include "base.hpp"
#include "NoEjectDelay.hpp"
#include "config.hpp"

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
  IOLog("NoEjectDelay::init\n");

  bool res = super::init(dict);
  org_pqrs_NoEjectDelay::sysctl_register();

  return res;
}

void
org_pqrs_driver_NoEjectDelay::free(void)
{
  IOLog("NoEjectDelay::free\n");

  org_pqrs_NoEjectDelay::sysctl_unregister();
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
  bool res = super::start(provider);
  IOLog("NoEjectDelay::start\n");
  if (! res) { return res; }

  notifier_hookKeyboard_ = addMatchingNotification(gIOMatchedNotification,
                                                   serviceMatching("IOHIKeyboard"),
                                                   org_pqrs_driver_NoEjectDelay::notifierfunc_hookKeyboard,
                                                   this, NULL, 0);
  if (notifier_hookKeyboard_ == NULL) {
    IOLog("[NoEjectDelay ERROR] addNotification(gIOMatchedNotification) Keyboard\n");
    return false;
  }

  return res;
}

void
org_pqrs_driver_NoEjectDelay::stop(IOService* provider)
{
  IOLog("NoEjectDelay::stop\n");

  if (notifier_hookKeyboard_) notifier_hookKeyboard_->remove();

  super::stop(provider);
}

// ----------------------------------------
int
org_pqrs_driver_NoEjectDelay::setEjectDelay(IOHIKeyboard* kbd, int delay)
{
  if (! kbd) return -1;

  const char* name = kbd->getName();
  if (strcmp(name, "IOHIDConsumer") != 0) return -1;

  IOHIDConsumer* consumer = OSDynamicCast(IOHIDConsumer, kbd);
  IOHIDEventService* service = consumer->_provider;
  if (! service->_reserved) return -1;

  service->_reserved->ejectDelayMS = delay;
  return 0;
}

bool
org_pqrs_driver_NoEjectDelay::notifierfunc_hookKeyboard(void* target, void* refCon, IOService* newService, IONotifier* notifier)
{
  IOLog("NoEjectDelay::notifierfunc_hookKeyboard\n");

  IOHIKeyboard* kbd = OSDynamicCast(IOHIKeyboard, newService);

  setEjectDelay(kbd, 5);
  return true;
}
