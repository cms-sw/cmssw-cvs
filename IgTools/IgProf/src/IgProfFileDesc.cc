#include "IgTools/IgProf/src/IgProf.h"
#include "IgTools/IgProf/src/IgProfTrace.h"
#include "IgTools/IgHook/interface/IgHook.h"
#include "IgTools/IgHook/interface/IgHookTrace.h"
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cerrno>
#include <sys/socket.h>
#include <pthread.h>

// -------------------------------------------------------------------
// Traps for this profiling module
IGPROF_DUAL_HOOK(3, int, doopen, _main, _libc,
		 (const char *fn, int flags, int mode), (fn, flags, mode),
		 "open", 0, "libc.so.6")
IGPROF_DUAL_HOOK(3, int, doopen64, _main, _libc,
		 (const char *fn, int flags, int mode), (fn, flags, mode),
		 "__open64", 0, "libc.so.6")
IGPROF_DUAL_HOOK(1, int, doclose, _main, _libc,
		 (int fd), (fd),
		 "close", 0, "libc.so.6")
IGPROF_DUAL_HOOK(1, int, dodup, _main, _libc,
		 (int fd), (fd),
		 "dup", 0, "libc.so.6")
IGPROF_DUAL_HOOK(2, int, dodup2, _main, _libc,
		 (int fd, int newfd), (fd, newfd),
		 "dup2", 0, "libc.so.6")
IGPROF_DUAL_HOOK(3, int, dosocket, _main, _libc,
		 (int domain, int type, int proto), (domain, type, proto),
		 "socket", 0, "libc.so.6")
IGPROF_DUAL_HOOK(3, int, doaccept, _main, _libc,
		 (int fd, sockaddr *addr, socklen_t *len), (fd, addr, len),
		 "accept", 0, "libc.so.6")

// Data for this profiling module
static IgProfTrace::CounterDef	s_ct_used	= { "FD_USED", IgProfTrace::TICK, -1 };
static IgProfTrace::CounterDef	s_ct_live	= { "FD_LIVE", IgProfTrace::TICK_PEAK, -1 };
static bool			s_count_used	= 0;
static bool			s_count_live	= 0;
static bool			s_initialized	= false;
static int			s_moduleid	= -1;

/** Record file descriptor.  Increments counters in the tree. */
static void __attribute__((noinline))
add (int fd)
{
  IgProfTrace *buf = IgProf::buffer(s_moduleid);
  if (! buf)
    return;

  void			*addresses[IgProfTrace::MAX_DEPTH];
  int			depth = IgHookTrace::stacktrace(addresses, IgProfTrace::MAX_DEPTH);
  IgProfTrace::Record	entries [2];
  int			nentries = 0;

  if (s_count_used)
  {
    entries[nentries].type = IgProfTrace::COUNT;
    entries[nentries].def = &s_ct_used;
    entries[nentries].amount = 1;
    entries[nentries].ticks = 1;
    nentries++;
  }

  if (s_count_live)
  {
    entries[nentries].type = IgProfTrace::COUNT | IgProfTrace::ACQUIRE;
    entries[nentries].def = &s_ct_live;
    entries[nentries].amount = 1;
    entries[nentries].ticks = 1;
    entries[nentries].resource = fd;
    nentries++;
  }

  // Drop two bottom frames, four top ones (stacktrace, me, two for hook).
  buf->push(addresses+4, depth-5, entries, nentries);
}

/** Remove knowledge about the file descriptor.  If we are tracking
    leaks, removes the descriptor from the live map and subtracts
    from the live descriptor counters.  */
static void
remove (int fd)
{
  if (s_count_live)
  {
    IgProfTrace *buf = IgProf::buffer(s_moduleid);
    if (! buf)
      return;

    IgProfTrace::Record entry
      = { IgProfTrace::RELEASE, &s_ct_live, 0, 0, fd };
    buf->push(0, 0, &entry, 1);
  }
}

// -------------------------------------------------------------------
/** Initialise file descriptor profiling.  Traps various system
    calls to keep track of usage, and if requested, leaks.  */
static void
initialize(void)
{
  if (s_initialized) return;
  s_initialized = true;

  const char	*options = IgProf::options();
  bool		enable = false;
  bool		opts = false;

  while (options && *options)
  {
    while (*options == ' ' || *options == ',')
      ++options;

    if (! strncmp(options, "fd", 2))
    {
      enable = true;
      options += 2;
      while (*options)
      {
	if (! strncmp(options, ":used", 5))
	{
	  s_count_used = 1;
	  options += 5;
	  opts = true;
	}
	else if (! strncmp(options, ":live", 5))
	{
	  s_count_live = 1;
	  options += 5;
	  opts = true;
	}
	else if (! strncmp(options, ":all", 4))
	{
	  s_count_used = 1;
	  s_count_live = 1;
	  options += 4;
	  opts = true;
	}
	else
	  break;
      }
    }
    else
      options++;

    while (*options && *options != ',' && *options != ' ')
      options++;
  }

  if (! enable)
    return;

  if (! IgProf::initialize(&s_moduleid, 0, false))
    return;

  IgProf::disable(true);
  if (!opts)
  {
    IgProf::debug("FD: defaulting to total descriptor counting\n");
    s_count_used = 1;
  }
  else
  {
    if (s_count_used)
      IgProf::debug("FD: enabling usage counting\n");
    if (s_count_live)
      IgProf::debug("FD: enabling live counting\n");
  }


  IgHook::hook(doopen_hook_main.raw);
  IgHook::hook(doopen64_hook_main.raw);
  IgHook::hook(doclose_hook_main.raw);
  IgHook::hook(dodup_hook_main.raw);
  IgHook::hook(dodup2_hook_main.raw);
  IgHook::hook(dosocket_hook_main.raw);
  IgHook::hook(doaccept_hook_main.raw);
#if __linux
  if (doopen_hook_main.raw.chain)   IgHook::hook(doopen_hook_libc.raw);
  if (doopen64_hook_main.raw.chain) IgHook::hook(doopen64_hook_libc.raw);
  if (doclose_hook_main.raw.chain)  IgHook::hook(doclose_hook_libc.raw);
  if (dodup_hook_main.raw.chain)    IgHook::hook(dodup_hook_libc.raw);
  if (dodup2_hook_main.raw.chain)   IgHook::hook(dodup2_hook_libc.raw);
  if (dosocket_hook_main.raw.chain) IgHook::hook(dosocket_hook_libc.raw);
  if (doaccept_hook_main.raw.chain) IgHook::hook(doaccept_hook_libc.raw);
#endif
  IgProf::debug("File descriptor profiler enabled\n");
  IgProf::enable(true);
}

// -------------------------------------------------------------------
// Trapped system calls.  Track live file descriptor usage.
static int
doopen(IgHook::SafeData<igprof_doopen_t> &hook, const char *fn, int flags, int mode)
{
  bool enabled = IgProf::disable(false);
  int result = (*hook.chain)(fn, flags, mode);
  int err = errno;

  if (enabled && result != -1)
    add(result);

  errno = err;
  IgProf::enable(false);
  return result;
}

static int
doopen64(IgHook::SafeData<igprof_doopen64_t> &hook, const char *fn, int flags, int mode)
{
  bool enabled = IgProf::disable(false);
  int result = (*hook.chain)(fn, flags, mode);
  int err = errno;

  if (enabled && result != -1)
    add(result);

  errno = err;
  IgProf::enable(false);
  return result;
}

static int
doclose(IgHook::SafeData<igprof_doclose_t> &hook, int fd)
{
  IgProf::disable(false);
  int result = (*hook.chain)(fd);
  int err = errno;

  if (result != -1)
    remove(fd);

  errno = err;
  IgProf::enable(false);
  return result;
}


static int
dodup(IgHook::SafeData<igprof_dodup_t> &hook, int fd)
{
  bool enabled = IgProf::disable(false);
  int result = (*hook.chain)(fd);
  int err = errno;

  if (enabled && result != -1)
    add(result);

  errno = err;
  IgProf::enable(false);
  return result;
}

static int
dodup2(IgHook::SafeData<igprof_dodup2_t> &hook, int fd, int newfd)
{
  bool enabled = IgProf::disable(false);
  int result = (*hook.chain)(fd, newfd);
  int err = errno;

  if (result != -1)
  {
    remove(fd);
    if (enabled)
      add(newfd);
  }

  errno = err;
  IgProf::enable(false);
  return result;
}

static int
dosocket(IgHook::SafeData<igprof_dosocket_t> &hook, int domain, int type, int proto)
{
  bool enabled = IgProf::disable(false);
  int result = (*hook.chain)(domain, type, proto);
  int err = errno;

  if (enabled && result != -1)
    add(result);

  errno = err;
  IgProf::enable(false);
  return result;
}

static int
doaccept(IgHook::SafeData<igprof_doaccept_t> &hook,
	 int fd, struct sockaddr *addr, socklen_t *len)
{
  bool enabled = IgProf::disable(false);
  int result = (*hook.chain)(fd, addr, len);
  int err = errno;

  if (enabled && result != -1)
    add(result);

  errno = err;
  IgProf::enable(false);
  return result;
}

// -------------------------------------------------------------------
static bool autoboot = (initialize(), true);
