// -*- Mode: c++; indent-tabs-mode: nil; -*-

#include "base.hpp"
#include "SetEjectDelay.hpp"
#include "config.hpp"

// ----------------------------------------------------------------------
// http://developer.apple.com/documentation/DeviceDrivers/Conceptual/WritingDeviceDriver/CPluPlusRuntime/chapter_2_section_3.html

// This convention makes it easy to invoke base class member functions.
#define super    IOService
// You cannot use the "super" macro here, however, with the
//  OSDefineMetaClassAndStructors macro.
OSDefineMetaClassAndStructors(org_pqrs_driver_SetEjectDelay, IOService)

// ----------------------------------------------------------------------
bool
org_pqrs_driver_SetEjectDelay::init(OSDictionary *dict)
{
  IOLog("SetEjectDelay::init\n");

  bool res = super::init(dict);
  org_pqrs_SetEjectDelay::sysctl_register();

  return res;
}

void
org_pqrs_driver_SetEjectDelay::free(void)
{
  IOLog("SetEjectDelay::free\n");

  org_pqrs_SetEjectDelay::sysctl_unregister();
  super::free();
}

IOService *
org_pqrs_driver_SetEjectDelay::probe(IOService *provider, SInt32 *score)
{
  IOService *res = super::probe(provider, score);
  return res;
}

bool
org_pqrs_driver_SetEjectDelay::start(IOService *provider)
{
  bool res = super::start(provider);
  IOLog("SetEjectDelay::start\n");
  if (!res) { return res; }

  notifier_hookKeyboard = addNotification(gIOMatchedNotification,
                                          serviceMatching("IOHIKeyboard"),
                                          ((IOServiceNotificationHandler)&(org_pqrs_driver_SetEjectDelay::notifierfunc_hookKeyboard)),
                                          this, NULL, 0);
  if (notifier_hookKeyboard == NULL) {
    IOLog("[SetEjectDelay ERROR] addNotification(gIOMatchedNotification) Keyboard\n");
    return false;
  }

  notifier_unhookKeyboard = addNotification(gIOTerminatedNotification,
                                            serviceMatching("IOHIKeyboard"),
                                            ((IOServiceNotificationHandler)&(org_pqrs_driver_SetEjectDelay::notifierfunc_unhookKeyboard)),
                                            this, NULL, 0);
  if (notifier_unhookKeyboard == NULL) {
    IOLog("[SetEjectDelay ERROR] addNotification(gIOTerminatedNotification) Keyboard\n");
    return false;
  }

  return res;
}

void
org_pqrs_driver_SetEjectDelay::stop(IOService *provider)
{
  IOLog("SetEjectDelay::stop\n");
  super::stop(provider);
}

// ----------------------------------------
int
org_pqrs_driver_SetEjectDelay::setEjectDelay(IOHIKeyboard *kbd, int delay)
{
  if (! kbd) return -1;

  const char *name = kbd->getName();
  if (strcmp(name, "IOHIDConsumer") != 0) return -1;

  IOHIDConsumer *consumer = reinterpret_cast<IOHIDConsumer *>(kbd);
  IOHIDEventService *service = consumer->_provider;
  if (! service->_reserved) return -1;

  int orig = service->_reserved->ejectDelayMS;
  service->_reserved->ejectDelayMS = delay;
  return orig;
}

bool
org_pqrs_driver_SetEjectDelay::notifierfunc_hookKeyboard(org_pqrs_driver_SetEjectDelay *self, void *ref, IOService *newService)
{
  IOLog("SetEjectDelay::notifierfunc_hookKeyboard\n");

  IOHIKeyboard *kbd = OSDynamicCast(IOHIKeyboard, newService);

  int orig = setEjectDelay(kbd, 5);
  if (orig != -1) {
    self->originalDelay = orig;
  }

  return true;
}

bool
org_pqrs_driver_SetEjectDelay::notifierfunc_unhookKeyboard(org_pqrs_driver_SetEjectDelay *self, void *ref, IOService *newService)
{
  IOLog("SetEjectDelay::notifierfunc_unhookKeyboard\n");

  IOHIKeyboard *kbd = OSDynamicCast(IOHIKeyboard, newService);

  setEjectDelay(kbd, self->originalDelay);

  return true;
}
