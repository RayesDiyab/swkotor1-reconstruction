// _clock 006FC546 (Ghidra library match). Original: GetSystemTimeAsFileTime (IAT 0073D194),
// ((hi<<32)+lo - start[00833C38]) / 10000 via __alldiv 006FB030, low 32 bits as clock_t.
// UCRT clock(): QueryPerformanceCounter since CRT start, ms. Same type (long), same
// CLOCKS_PER_SEC (1000), same epoch class (CRT initialization) and same unit; the only
// caller on this path (AurMakeCurrent 0044EDB4..0044EDCB) uses it for a target-delta poll
// that is statically unreachable (007B9270 has no writer). Divergences outside reachable
// inputs are documented in PACKET.json (wall-clock vs monotonic source; wrap vs -1 after
// LONG_MAX ms). Typed adapter only; no function-pointer cast.
#include <ctime>
#include <type_traits>
static_assert(CLOCKS_PER_SEC == 1000, "original CRT _clock unit is milliseconds");
static_assert(std::is_same<std::clock_t, long>::value, "original clock_t is long (EAX)");
extern "C" long __cdecl crt_clock_006fc546() { return std::clock(); }
