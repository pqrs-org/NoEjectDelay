// -*- indent-tabs-mode: nil; -*-

#ifndef _NoEjectDelay_h
#define _NoEjectDelay_h

#include "base.hpp"

// http://developer.apple.com/documentation/Darwin/Conceptual/KEXTConcept/KEXTConceptIOKit/hello_iokit.html#//apple_ref/doc/uid/20002366-CIHECHHE
class org_pqrs_driver_NoEjectDelay : public IOService
{
  OSDeclareDefaultStructors(org_pqrs_driver_NoEjectDelay);

public:
  virtual bool init(OSDictionary* dictionary = 0);
  virtual void free(void);
  virtual IOService* probe(IOService* provider, SInt32* score);
  virtual bool start(IOService* provider);
  virtual void stop(IOService* provider);

private:
  static bool notifierfunc_hookKeyboard(void* target, void* refCon, IOService* newService, IONotifier* notifier);

  IONotifier* notifier_hookKeyboard_;
};

#endif
