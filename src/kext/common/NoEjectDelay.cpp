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
                                                   serviceMatching("IOHIKeyboard"),
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
namespace {
  const char*
  getProductPropertyCStringNoCopy(IOHIDevice* device)
  {
    const OSString* productname = OSDynamicCast(OSString, device->getProperty(kIOHIDProductKey));
    if (productname) {
      const char* pname = productname->getCStringNoCopy();
      if (pname) {
        return pname;
      }
    }
    return NULL;
  }
}

bool
org_pqrs_driver_NoEjectDelay::notifierfunc_hookKeyboard(void* target, void* refCon, IOService* newService, IONotifier* notifier)
{
  // set Eject delay.
  {
    IOHIDConsumer* consumer = OSDynamicCast(IOHIDConsumer, newService);
    if (consumer) {
      {
        const char* name = consumer->getName();
        const char* pname = getProductPropertyCStringNoCopy(consumer);
        if (name && pname) {
          IOLOG_INFO("notifierfunc_hookKeyboard consumer: %s (%s)\n", pname, name);
        }
      }

      IOHIDEventService* service = consumer->_provider;
      if (service && service->_reserved) {
        const int DELAY = 5;
        service->_reserved->ejectDelayMS = DELAY;
      }
    }
  }

  // disable F12Eject
  {
    IOHIKeyboard* keyboard = OSDynamicCast(IOHIKeyboard, newService);
    if (keyboard) {
      {
        const char* name = keyboard->getName();
        const char* pname = getProductPropertyCStringNoCopy(keyboard);
        if (name && pname) {
          IOLOG_INFO("notifierfunc_hookKeyboard keyboard: %s (%s)\n", pname, name);
        }
      }

      IOHIKeyboardMapper* mapper = keyboard->_keyMap;
      if (mapper && mapper->_reserved) {
        mapper->_reserved->supportsF12Eject = 0;
      }
    }
  }

  // ------------------------------------------------------------
  return true;
}
