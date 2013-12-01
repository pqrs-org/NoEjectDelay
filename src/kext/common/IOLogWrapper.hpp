#ifndef IOLOGWRAPPER_HPP
#define IOLOGWRAPPER_HPP

#define IOLOG_DEBUG(...) {                                     \
    if (! org_pqrs_NoEjectDelay::IOLogWrapper::suppressed()) { \
      if (Config::get_debug()) {                               \
        IOLog("NoEjectDelay --Debug-- " __VA_ARGS__);          \
      }                                                        \
    }                                                          \
}
#define IOLOG_DEBUG_POINTING(...) {                            \
    if (! org_pqrs_NoEjectDelay::IOLogWrapper::suppressed()) { \
      if (Config::get_debug_pointing()) {                      \
        IOLog("NoEjectDelay --Debug-- " __VA_ARGS__);          \
      }                                                        \
    }                                                          \
}
#define IOLOG_DEVEL(...) {                                     \
    if (! org_pqrs_NoEjectDelay::IOLogWrapper::suppressed()) { \
      if (Config::get_debug_devel()) {                         \
        IOLog("NoEjectDelay --Devel-- " __VA_ARGS__);          \
      }                                                        \
    }                                                          \
}

#define IOLOG_ERROR(...) {                                     \
    if (! org_pqrs_NoEjectDelay::IOLogWrapper::suppressed()) { \
      IOLog("NoEjectDelay --Error-- " __VA_ARGS__);            \
    }                                                          \
}

#define IOLOG_INFO(...) {                                      \
    if (! org_pqrs_NoEjectDelay::IOLogWrapper::suppressed()) { \
      IOLog("NoEjectDelay --Info-- "  __VA_ARGS__);            \
    }                                                          \
}

#define IOLOG_WARN(...) {                                      \
    if (! org_pqrs_NoEjectDelay::IOLogWrapper::suppressed()) { \
      IOLog("NoEjectDelay --Warn-- "  __VA_ARGS__);            \
    }                                                          \
}


// ------------------------------------------------------------
namespace org_pqrs_NoEjectDelay {
  class IOLogWrapper {
  public:
    static bool suppressed(void) { return suppressed_; }
    static void suppress(bool v) { suppressed_ = v; }

    class ScopedSuppress {
    public:
      ScopedSuppress(void) { original = suppressed(); suppress(true); }
      ~ScopedSuppress(void) { suppress(original); }

    private:
      bool original;
    };

  private:
    static bool suppressed_;
  };
}

#endif
