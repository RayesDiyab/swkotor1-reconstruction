// lane-m48-b crt: the task-23 descriptor_io.h include (which drags io_locked.h and the excluded task-35 helpers.h) is
// replaced by the one declaration this unit uses (same signature as descriptor_io.h:5).
namespace kotor_descriptor_support { int* __cdecl errno_006ffe78(); }
// PTD owner in the M47 link: model46-m46-ptd-join compat_getptd (locale-publisher Ptd, 0x68+ bytes; +8 = original
// _tiddata::_terrno slot, written only through this accessor in the link).
extern "C" void* __cdecl compat_getptd();
namespace kotor_descriptor_support {
int* __cdecl errno_006ffe78() {
    return reinterpret_cast<int*>(static_cast<unsigned char*>(compat_getptd())+8);
}
}
