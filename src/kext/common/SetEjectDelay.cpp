// -*- Mode: c++; indent-tabs-mode: nil; -*-

#include "base.hpp"
#include "SetEjectDelay.hpp"
#include "config.hpp"
#include "keymap.hpp"

org_pqrs_driver_SetEjectDelay::HookedKeyboard org_pqrs_driver_SetEjectDelay::hookedKeyboard[MAXNUM_KEYBOARD];

// ----------------------------------------------------------------------
// http://developer.apple.com/documentation/DeviceDrivers/Conceptual/WritingDeviceDriver/CPluPlusRuntime/chapter_2_section_3.html

// This convention makes it easy to invoke base class member functions.
#define super    IOService
// You cannot use the "super" macro here, however, with the
//  OSDefineMetaClassAndStructors macro.
OSDefineMetaClassAndStructors(org_pqrs_driver_SetEjectDelay, IOService)

// ----------------------------------------------------------------------
void
org_pqrs_driver_SetEjectDelay::customizeAllKeymap(void) {
  for ( int i = 0; i < MAXNUM_KEYBOARD; i++ ) {
    hookedKeyboard[i].refresh();
  }
}


// ----------------------------------------------------------------------
void
org_pqrs_driver_SetEjectDelay::HookedKeyboard::initialize(IOHIKeyboard *p)
{
  IOHIDKeyboard *hid = OSDynamicCast(IOHIDKeyboard, p);
  if (hid) {
    kbd = p;
#include "generate/output/include.code_initialize.cpp"
  }
  refresh();
}

void
org_pqrs_driver_SetEjectDelay::HookedKeyboard::terminate(void)
{
  if (! kbd) return;

  IOHIDKeyboard *hid = OSDynamicCast(IOHIDKeyboard, kbd);
  if (hid) {
#include "generate/output/include.code_terminate.cpp"
    kbd = NULL;
  }
}

namespace {
  void setkeycode(IOHIDKeyboard *hid, org_pqrs_SetEjectDelay::KeyMapIndex::KeyMapIndex index,
                  bool enable, int replaceKeyCode, int originalKeyCode)
  {
    if (hid) {
      if (enable) {
        hid->_usb_2_adb_keymap[index] = replaceKeyCode;
      } else {
        hid->_usb_2_adb_keymap[index] = originalKeyCode;
      }
    }
  }
}

void
org_pqrs_driver_SetEjectDelay::HookedKeyboard::refresh(void)
{
  if (! kbd) return;

  IOHIDKeyboard *hid = OSDynamicCast(IOHIDKeyboard, kbd);
  if (hid) {
#include "generate/output/include.code_setkeycode.cpp"
  }
}

// ----------------------------------------------------------------------
bool
org_pqrs_driver_SetEjectDelay::init(OSDictionary *dict)
{
  IOLog("SetEjectDelay::init\n");

  bool res = super::init(dict);
  org_pqrs_SetEjectDelay::sysctl_register();

  for (int i = 0; i < MAXNUM_KEYBOARD; ++i) {
    hookedKeyboard[i].kbd = NULL;
  }

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

  keyboardNotifier = addNotification(gIOMatchedNotification,
                                     serviceMatching("IOHIKeyboard"),
                                     ((IOServiceNotificationHandler)&(org_pqrs_driver_SetEjectDelay::notifier_hookKeyboard)),
                                     this, NULL, 0);
  if (keyboardNotifier == NULL) {
    IOLog("[SetEjectDelay ERROR] addNotification(gIOMatchedNotification)\n");
    return false;
  }

  terminatedNotifier = addNotification(gIOTerminatedNotification,
                                       serviceMatching("IOHIKeyboard"),
                                       ((IOServiceNotificationHandler)&(org_pqrs_driver_SetEjectDelay::notifier_unhookKeyboard)),
                                       this, NULL, 0);
  if (terminatedNotifier == NULL) {
    IOLog("[SetEjectDelay ERROR] addNotification(gIOTerminatedNotification)\n");
    return false;
  }

  return res;
}

void
org_pqrs_driver_SetEjectDelay::stop(IOService *provider)
{
  for (int i = 0; i < MAXNUM_KEYBOARD; ++i) {
    if (hookedKeyboard[i].kbd != NULL) {
      restoreKeyMap(hookedKeyboard[i].kbd);
    }
  }

  if (keyboardNotifier) {
    keyboardNotifier->remove();
  }
  if (terminatedNotifier) {
    terminatedNotifier->remove();
  }

  IOLog("SetEjectDelay::stop\n");
  super::stop(provider);
}


// ----------------------------------------------------------------------
org_pqrs_driver_SetEjectDelay::HookedKeyboard *
org_pqrs_driver_SetEjectDelay::new_hookedKeyboard(void)
{
  for (int i = 0; i < MAXNUM_KEYBOARD; ++i) {
    if (hookedKeyboard[i].kbd == NULL) {
      return hookedKeyboard + i;
    }
  }
  return NULL;
}

org_pqrs_driver_SetEjectDelay::HookedKeyboard *
org_pqrs_driver_SetEjectDelay::search_hookedKeyboard(const IOHIKeyboard *kbd)
{
  if (kbd == NULL) {
    return NULL;
  }
  for (int i = 0; i < MAXNUM_KEYBOARD; ++i) {
    if (hookedKeyboard[i].kbd == kbd) {
      return hookedKeyboard + i;
    }
  }
  return NULL;
}

// ----------------------------------------------------------------------
bool
org_pqrs_driver_SetEjectDelay::notifier_hookKeyboard(org_pqrs_driver_SetEjectDelay *self, void *ref, IOService *newService)
{
  IOLog("SetEjectDelay::notifier_hookKeyboard\n");

  IOHIKeyboard *kbd = OSDynamicCast(IOHIKeyboard, newService);
  return customizeKeyMap(kbd);
}

bool
org_pqrs_driver_SetEjectDelay::notifier_unhookKeyboard(org_pqrs_driver_SetEjectDelay *self, void *ref, IOService *newService)
{
  IOLog("SetEjectDelay::notifier_unhookKeyboard\n");

  IOHIKeyboard *kbd = OSDynamicCast(IOHIKeyboard, newService);
  return restoreKeyMap(kbd);
}

bool
org_pqrs_driver_SetEjectDelay::customizeKeyMap(IOHIKeyboard *kbd)
{
  if (! kbd) return false;

  const char *name = kbd->getName();
  IOLog("SetEjectDelay::customizeKeymap name = %s\n", name);

  // AppleADBKeyboard == PowerBook, IOHIKeyboard == MacBook, MacBook Pro, Mac mini, ...
  if (strcmp(name, "IOHIDKeyboard") != 0 && strcmp(name, "AppleADBKeyboard") != 0) return false;

  HookedKeyboard *p = new_hookedKeyboard();
  if (! p) return false;

  p->initialize(kbd);
  return true;
}

bool
org_pqrs_driver_SetEjectDelay::restoreKeyMap(IOHIKeyboard *kbd)
{
  if (! kbd) return false;

  HookedKeyboard *p = search_hookedKeyboard(kbd);
  if (! p) return false;

  IOLog("SetEjectDelay::restoreKeyMap 0x%x\n", kbd);
  p->terminate();
  return true;
}
