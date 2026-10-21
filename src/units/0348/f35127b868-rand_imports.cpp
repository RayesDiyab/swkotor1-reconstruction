#include "e8a5e6befc-rand_constructor.h"
namespace startup_rand {
const RandImports rand_imports={&::QueryPerformanceCounter,&::GetSystemInfo,&::GetTickCount,&AlignedGenSeed};
}
