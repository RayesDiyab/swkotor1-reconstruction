#pragma once
// Fork F1 (lane-renderer-bodies): render pool managers used by
// LoadExtensions_00436490 (00437289..004374E9 tail: pool init + slot +2C call).
// Abstract base vtable 0073F488 = [0042E280 scalar-deleting dtor, 11 x 006FB8DE
// _purecall]; derived vtables 0073F4B8 (Sys) / 0073F4FC (VBO). Slot order and
// the Ghidra slot names come from swkotor.exe.h RequestPoolManagerVirtualFunctions.
// All methods are __thiscall (ECX receiver) with callee stack cleanup (RET n
// cited per slot). Single inheritance at offset 0: the base pointer stored in
// 007A68A4 equals the derived object address (00436390/00436400 store EAX).
#include <cstddef>
#include <cstdint>
// M41 owner of CExoArrayList::Allocate 005E03C0 (viewport-list-growth.obj) and
// its 12-byte ArrayMutations list view (data, size, capacity).
#include "21e45d1112-growth.h"
namespace kotor_renderer_gl {
using U32 = std::uint32_t;
using I32 = std::int32_t;
using List = kotor_reconstruction::ArrayMutations;

// CExoArrayList member whose destructor frees data when non-null
// (VBO dtor member cleanup 00435287..004352D4; base dtor 0042E259..0042E279).
struct OwnedList {
    List list;
    OwnedList() noexcept;   // size, capacity, data = 0 (00434F72..00434F78 order)
    ~OwnedList();
};
static_assert(sizeof(OwnedList) == 12 && offsetof(OwnedList, list) == 0, "CExoArrayList x86 layout");

class RequestPoolManager {
public:
    // Allocation through the original operator new 006FC0D2 (-> 006FA7E6) with
    // the null result test of 004363BD/0043642D; delete through 006FC0CD/006FA390.
    static void* operator new(std::size_t size) noexcept;
    static void operator delete(void* memory) noexcept;
    virtual ~RequestPoolManager();                              // slot 0  (RET 4 flags in deleting dtor)
    virtual I32 RequestPool(I32 kind, U32 size, U32 tag) = 0;   // slot 1  RET 0Ch
    virtual void* LockPool(I32 index, U32 flags) = 0;           // slot 2  RET 8
    virtual void UnlockPool(I32 index) = 0;                     // slot 3  RET 4
    virtual void* GetPool(I32 index) = 0;                       // slot 4  RET 4
    virtual void* GetIndexPool(I32 index) = 0;                  // slot 5  RET 4
    virtual void DeletePool(I32 index) = 0;                     // slot 6  RET 4
    virtual void ResizePool(I32 index, U32 size) = 0;           // slot 7  RET 8
    virtual U32 GetPoolSize(I32 index) = 0;                     // slot 8  RET 4
    virtual void RefreshPool(U32 first, U32 second) = 0;        // slot 9  RET 8 (005B5E90)
    virtual void BackUpPoolManager() = 0;                       // slot 10 RET
    virtual void RestorePoolManager() = 0;                      // slot 11 RET (+2C, called by 00436490)
protected:
    RequestPoolManager() noexcept;
    List free_indices_04;   // field1_0x4: recycled pool indices
    List locks_10;          // pool_locks
};
static_assert(sizeof(RequestPoolManager) == 0x1c, "base extent before derived +1C");

class RequestPoolManagerSys final : public RequestPoolManager {
public:
    RequestPoolManagerSys();                                    // 004356E0
    ~RequestPoolManagerSys() override;                          // 0042E540 (+ 00435870 deleting)
    I32 RequestPool(I32 kind, U32 size, U32 tag) override;      // 00435890
    void* LockPool(I32 index, U32 flags) override;              // 0042E5B0
    void UnlockPool(I32 index) override;                        // 0042E5E0
    void* GetPool(I32 index) override;                          // 00423B50
    void* GetIndexPool(I32 index) override;                     // 00423B50 (same body)
    void DeletePool(I32 index) override;                        // 00435A90
    void ResizePool(I32 index, U32 size) override;              // 00423A90
    U32 GetPoolSize(I32 index) override;                        // 00423B40
    void RefreshPool(U32 first, U32 second) override;           // 005B5E90 ret 8
    void BackUpPoolManager() override;                          // 004015A0 ret
    void RestorePoolManager() override;                         // 004015A0 ret
    U32 count_1c;
    void** pools_20;
    U32* sizes_24;
};

class RequestPoolManagerVBO final : public RequestPoolManager {
public:
    RequestPoolManagerVBO();                                    // 00434F30
    ~RequestPoolManagerVBO() override;                          // 004351C0 (+ 00436470 deleting)
    I32 RequestPool(I32 kind, U32 size, U32 tag) override;      // 00435310
    void* LockPool(I32 index, U32 flags) override;              // 0042E2A0
    void UnlockPool(I32 index) override;                        // 0042E340
    void* GetPool(I32 index) override;                          // 0042E390
    void* GetIndexPool(I32 index) override;                     // 0042E3B0
    void DeletePool(I32 index) override;                        // 00435670
    void ResizePool(I32 index, U32 size) override;              // 0042E420
    U32 GetPoolSize(I32 index) override;                        // 0042E3D0
    void RefreshPool(U32 first, U32 second) override;           // 005B5E90 ret 8
    void BackUpPoolManager() override;                          // 00434CC0
    void RestorePoolManager() override;                         // 00434E70
    U32 count_1c;
    OwnedList tags_20;          // RequestPool tag / LockPool flags per index
    OwnedList buffers_2c;       // GL buffer object names
    OwnedList backups_38;       // BackUp system copies (pointer words)
    OwnedList backup_sizes_44;  // BackUp byte sizes
    OwnedList targets_50;       // 0x8892 ARRAY / 0x8893 ELEMENT_ARRAY
};
static_assert(sizeof(RequestPoolManagerSys) == 0x28, "operator new 28h at 004363AF");
static_assert(offsetof(RequestPoolManagerSys, count_1c) == 0x1c && offsetof(RequestPoolManagerSys, pools_20) == 0x20 &&
    offsetof(RequestPoolManagerSys, sizes_24) == 0x24, "Sys fields");
static_assert(sizeof(RequestPoolManagerVBO) == 0x5c, "operator new 5Ch at 0043641F");
static_assert(offsetof(RequestPoolManagerVBO, count_1c) == 0x1c && offsetof(RequestPoolManagerVBO, tags_20) == 0x20 &&
    offsetof(RequestPoolManagerVBO, buffers_2c) == 0x2c && offsetof(RequestPoolManagerVBO, backups_38) == 0x38 &&
    offsetof(RequestPoolManagerVBO, backup_sizes_44) == 0x44 && offsetof(RequestPoolManagerVBO, targets_50) == 0x50, "VBO fields");

// CExoArrayList::allocate 00437D60 (thiscall, RET 4) as the ABI-identical
// __fastcall(List*, unused EDX, capacity) used by the M41 005E03C0 owner (D1).
void __fastcall allocate_00437d60(List* self, void* unused_edx, U32 capacity);
// CExoArrayList::deleteAt 006857A0 (thiscall, RET 4), same D1 spelling.
void __fastcall deleteAt_006857a0(List* self, void* unused_edx, I32 index);
}
namespace kotor_renderer_gl_cells {
// Requested cells (storage owned by the lane cells TU; BSS zero in the original).
extern kotor_renderer_gl::RequestPoolManager* pool_007a68a4;
extern std::uint32_t pool_total_bytes_007a68a8;   // 00435898/004358B7, 00423AA1, 00435AD5
extern std::uint32_t pool_page_bytes_007a68ac;    // 004358A0/004358BD, 00423ABD, 00435ADB
extern std::uint32_t pool_vbo_bytes_007a6984;     // 00435324/00435333
extern std::uint32_t pool_sys_bytes_007a6988;     // 004358CF/004358DD
extern std::uint8_t renderer_flag_0078e398;       // already declared by renderer_gl_cells.h (same object)
}
namespace kotor_renderer_init {
// Consumer spelling from the reviewed extension_loader.h (zero-arg stdcall == plain ret).
void __stdcall InitSystemPool_00436390();
void __stdcall InitVertexBufferPool_00436400();
}
