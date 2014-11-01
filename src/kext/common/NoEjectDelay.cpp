// -*- Mode: c++; indent-tabs-mode: nil; -*-

#define protected public // A hack for access private member of IOHIKeyboard
#define private public
#include <IOKit/hidsystem/IOHIKeyboard.h>
#include <IOKit/hidevent/IOHIDEventService.h>
#undef protected
#undef private
#include <IOKit/IOLib.h>

#include "GlobalLock.hpp"
#include "IOLogWrapper.hpp"
#include "NoEjectDelay.hpp"
#include "version.hpp"

// ----------------------------------------------------------------------
// http://developer.apple.com/documentation/DeviceDrivers/Conceptual/WritingDeviceDriver/CPluPlusRuntime/chapter_2_section_3.html

// This convention makes it easy to invoke base class member functions.
#define super IOService
// You cannot use the "super" macro here, however, with the
//  OSDefineMetaClassAndStructors macro.
OSDefineMetaClassAndStructors(org_pqrs_driver_NoEjectDelay, IOService);

// ----------------------------------------------------------------------
bool org_pqrs_driver_NoEjectDelay::init(OSDictionary* dict) {
  IOLOG_INFO("init %s\n", NoEjectDelay_version);

  notifier_hookKeyboard_ = NULL;
  notifier_unhookKeyboard_ = NULL;
  workLoop_ = NULL;
  timerEventSource_ = NULL;
  for (int i = 0; i < MAXNUM_DEVICES; ++i) {
    devices_[i] = NULL;
  }

  return super::init(dict);
}

void
org_pqrs_driver_NoEjectDelay::free(void) {
  IOLOG_INFO("free\n");
  super::free();
}

IOService*
org_pqrs_driver_NoEjectDelay::probe(IOService* provider, SInt32* score) {
  IOService* res = super::probe(provider, score);
  return res;
}

bool
org_pqrs_driver_NoEjectDelay::start(IOService* provider) {
  IOLOG_INFO("start\n");

  bool res = super::start(provider);
  if (!res) { return res; }

  // ----------------------------------------
  org_pqrs_NoEjectDelay::GlobalLock::initialize();

  // ----------------------------------------
  notifier_hookKeyboard_ = addMatchingNotification(gIOMatchedNotification,
                                                   serviceMatching("IOHIKeyboard"),
                                                   org_pqrs_driver_NoEjectDelay::IOHIKeyboard_gIOMatchedNotification_callback,
                                                   this, NULL, 0);
  if (notifier_hookKeyboard_ == NULL) {
    IOLOG_ERROR("initialize_notification notifier_hookKeyboard_ == NULL\n");
    return false;
  }

  notifier_unhookKeyboard_ = addMatchingNotification(gIOTerminatedNotification,
                                                     serviceMatching("IOHIKeyboard"),
                                                     org_pqrs_driver_NoEjectDelay::IOHIKeyboard_gIOTerminatedNotification_callback,
                                                     this, NULL, 0);
  if (notifier_unhookKeyboard_ == NULL) {
    IOLOG_ERROR("initialize_notification notifier_unhookKeyboard_ == NULL\n");
    return false;
  }

  notifier_hookEventService_ = addMatchingNotification(gIOMatchedNotification,
                                                       serviceMatching("IOHIDEventService"),
                                                       org_pqrs_driver_NoEjectDelay::IOHIKeyboard_gIOMatchedNotification_callback,
                                                       this, NULL, 0);
  if (notifier_hookEventService_ == NULL) {
    IOLOG_ERROR("initialize_notification notifier_hookEventService_ == NULL\n");
    return false;
  }

  notifier_unhookEventService_ = addMatchingNotification(gIOTerminatedNotification,
                                                         serviceMatching("IOHIDEventService"),
                                                         org_pqrs_driver_NoEjectDelay::IOHIKeyboard_gIOTerminatedNotification_callback,
                                                         this, NULL, 0);
  if (notifier_unhookEventService_ == NULL) {
    IOLOG_ERROR("initialize_notification notifier_unhookEventService_ == NULL\n");
    return false;
  }

  // ----------------------------------------
  workLoop_ = IOWorkLoop::workLoop();
  if (!workLoop_) return false;

  timerEventSource_ = IOTimerEventSource::timerEventSource(this, timer_callback);
  if (!timerEventSource_) return false;

  if (workLoop_->addEventSource(timerEventSource_) != kIOReturnSuccess) return false;

  timerEventSource_->setTimeoutMS(TIMER_INTERVAL_MS);

  // ----------------------------------------
  return res;
}

void
org_pqrs_driver_NoEjectDelay::stop(IOService* provider) {
  IOLOG_INFO("stop\n");

  // ----------------------------------------
  org_pqrs_NoEjectDelay::GlobalLock::terminate();

  // ----------------------------------------
  if (timerEventSource_) {
    if (workLoop_) {
      workLoop_->removeEventSource(timerEventSource_);
    }

    timerEventSource_->release();
    timerEventSource_ = NULL;
  }

  if (workLoop_) {
    workLoop_->release();
    workLoop_ = NULL;
  }

  // ----------------------------------------
  if (notifier_hookKeyboard_) { notifier_hookKeyboard_->remove(); }
  if (notifier_unhookKeyboard_) { notifier_unhookKeyboard_->remove(); }
  if (notifier_hookEventService_) { notifier_hookEventService_->remove(); }
  if (notifier_unhookEventService_) { notifier_unhookEventService_->remove(); }

  super::stop(provider);
}

// ----------------------------------------
namespace {
const char*
getProductPropertyCStringNoCopy(IOService* service) {
  const OSString* productname = OSDynamicCast(OSString, service->getProperty(kIOHIDProductKey));
  if (!productname) return NULL;

  const char* pname = productname->getCStringNoCopy();
  if (!pname) return NULL;

  return pname;
}
}

// ----------------------------------------
bool
org_pqrs_driver_NoEjectDelay::IOHIKeyboard_gIOMatchedNotification_callback(void* target, void* refCon, IOService* newService, IONotifier* notifier) {
  org_pqrs_NoEjectDelay::GlobalLock::ScopedLock lk;
  if (!lk) return false;

  // ----------------------------------------
  org_pqrs_driver_NoEjectDelay* self = reinterpret_cast<org_pqrs_driver_NoEjectDelay*>(target);
  if (!self) return false;

  if (!newService) return false;

  // ----------------------------------------
  const char* name = newService->getName();
  const char* pname = getProductPropertyCStringNoCopy(newService);
  if (name && pname) {
    IOLOG_INFO("device: %s (%s)\n", pname, name);
  }

  // ----------------------------------------
  for (int i = 0; i < MAXNUM_DEVICES; ++i) {
    if (!self->devices_[i]) {
      self->devices_[i] = newService;
      return true;
    }
  }

  IOLOG_WARN("There is no space for the new device. Please increase MAXNUM_DEVICES.\n");
  return true;
}

bool
org_pqrs_driver_NoEjectDelay::IOHIKeyboard_gIOTerminatedNotification_callback(void* target,
                                                                              void* refCon,
                                                                              IOService* newService,
                                                                              IONotifier* notifier) {
  org_pqrs_NoEjectDelay::GlobalLock::ScopedLock lk;
  if (!lk) return false;

  // ----------------------------------------
  org_pqrs_driver_NoEjectDelay* self = reinterpret_cast<org_pqrs_driver_NoEjectDelay*>(target);
  if (!self) return false;

  // ----------------------------------------
  for (int i = 0; i < MAXNUM_DEVICES; ++i) {
    if (self->devices_[i] == newService) {
      self->devices_[i] = NULL;
    }
  }
  return true;
}

void
org_pqrs_driver_NoEjectDelay::timer_callback(OSObject* target, IOTimerEventSource* sender) {
  org_pqrs_NoEjectDelay::GlobalLock::ScopedLock lk;
  if (!lk) return;

  // ----------------------------------------
  org_pqrs_driver_NoEjectDelay* self = OSDynamicCast(org_pqrs_driver_NoEjectDelay, target);
  if (!self) return;

  // ----------------------------------------
  // supportsF12Eject becomes true when user logged in even if supportsF12Eject is false.
  // Therefore, we need to overwrite this value from timer.

  for (int i = 0; i < MAXNUM_DEVICES; ++i) {
    if (!self->devices_[i]) continue;

    // set Eject delay.
    {
      {
        IOHIDEventService* service = OSDynamicCast(IOHIDEventService, self->devices_[i]);
        if (service) {
          setEjectDelayMS(service);
        }
      }
    }

    // disable F12Eject
    {
      IOHIKeyboard* keyboard = OSDynamicCast(IOHIKeyboard, self->devices_[i]);
      if (keyboard) {
        IOHIKeyboardMapper* mapper = OSDynamicCast(IOHIKeyboardMapper, keyboard->_keyMap);
        if (mapper &&
            mapper->_reserved &&
            mapper->_reserved->supportsF12Eject != 0) {
          IOLOG_INFO("Unset supportsF12Eject\n");
          mapper->_reserved->supportsF12Eject = 0;
        }
      }
    }
  }

  if (sender) {
    sender->setTimeoutMS(TIMER_INTERVAL_MS);
  }
}

void
org_pqrs_driver_NoEjectDelay::setEjectDelayMS(IOHIDEventService* service) {
  if (!service) return;
  if (!service->_reserved) return;

  const int DELAY = 5;
  if ((service->_reserved->keyboard).eject.delayMS != DELAY) {
    IOLOG_INFO("Set eject.delayMS\n");
    (service->_reserved->keyboard).eject.delayMS = DELAY;
  }
}
