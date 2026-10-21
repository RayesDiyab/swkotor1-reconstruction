// EXPLICIT TRAP STUBS for native entries that W2 calls but other owners must
// provide. Link this object only until the named owner lands; then drop it
// (a duplicate-symbol link error is the intended signal). None of these is a
// W2 body and none claims reconstructed-address credit.
extern "C" {
__declspec(noreturn) void __cdecl kotor_unreachable_trap(unsigned long, const char*);
// t4: the five CSWSPlayerLastUpdateObject clears are W2 bodies now (w2_g07_native.cpp).
}
