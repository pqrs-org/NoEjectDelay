#include <sys/types.h>
#include <sys/sysctl.h>

#include "config.hpp"
#include "version.hpp"

namespace org_pqrs_NoEjectDelay {
  // ----------------------------------------------------------------------
  // SYSCTL staff
  // http://developer.apple.com/documentation/Darwin/Conceptual/KernelProgramming/boundaries/chapter_14_section_7.html#//apple_ref/doc/uid/TP30000905-CH217-TPXREF116
  SYSCTL_DECL(_noejectdelay);
  SYSCTL_NODE(, OID_AUTO, noejectdelay, CTLFLAG_RW, 0, "");

  SYSCTL_STRING(_noejectdelay, OID_AUTO, version, CTLFLAG_RD, config_version, 0, "");

  // ----------------------------------------------------------------------
  void
  sysctl_register(void)
  {
    sysctl_register_oid(&sysctl__noejectdelay);
    sysctl_register_oid(&sysctl__noejectdelay_version);
  }

  void
  sysctl_unregister(void)
  {
    sysctl_unregister_oid(&sysctl__noejectdelay);
    sysctl_unregister_oid(&sysctl__noejectdelay_version);
  }
}
