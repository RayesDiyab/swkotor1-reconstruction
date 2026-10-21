// Fork F1 (lane-renderer-bodies): RequestPoolManager / Sys / VBO and the two
// pool initializers. Every behavior cites the original instruction address.
#include "16ea3f255d-render_pool.h"
#include "61e3cacb4e-renderer_gl_procs.h"
#include "39c670fc66-allocator_live.h"
#include <cstring>
namespace kotor_renderer_gl {
namespace {
using namespace kotor_renderer_gl_procs;
// Element view of a CExoArrayList data word (x86 pointer representation).
inline U32* Words(const List& list) { return reinterpret_cast<U32*>(static_cast<std::uintptr_t>(list.data)); }
inline void* AsPointer(U32 word) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(word)); }
inline U32 AsWord(void* pointer) { return static_cast<U32>(reinterpret_cast<std::uintptr_t>(pointer)); }
// Inline CExoArrayList::Add growth rule used at every call site:
// capacity==size ? allocate(capacity ? capacity*2 : 8) (e.g. 00435780..004357A7,
// 00435960..00435978), then data[size]=value; ++size (re-reading size/data).
enum class Grow { Allocate00437d60, Allocate005e03c0 };
void Add(List& list, U32 value, Grow grow) {
    const I32 capacity = list.capacity;
    if (list.size == capacity) {
        const U32 next = capacity != 0 ? static_cast<U32>(capacity) * 2u : 8u;
        if (grow == Grow::Allocate00437d60) allocate_00437d60(&list, nullptr, next);
        else menu_list_growth::allocate_005e03c0(&list, nullptr, next);
    }
    Words(list)[list.size] = value;
    list.size = list.size + 1;
}
// GL_ARRAY_BUFFER_ARB / GL_ELEMENT_ARRAY_BUFFER_ARB selection by RequestPool kind
// (00435383..0043538C, 004355E6..004355F1).
inline U32 TargetFor(I32 kind) { return kind != 0 ? 0x8893u : 0x8892u; }
inline U32 PageRound(U32 bytes) { return (bytes + 0xfffu) & 0xfffff000u; }
}

// ---- CExoArrayList lowers -------------------------------------------------
// 00437D60: capacity store (00437D67), new(capacity*4) (00437D71), data store
// (00437D79), copy while index < live size (00437D84..00437D92), free old if
// non-null (00437D94..00437D99).
void __fastcall allocate_00437d60(List* self, void*, U32 capacity) {
    self->capacity = static_cast<I32>(capacity);
    const U32 old = self->data;
    self->data = AsWord(live_operator_new(capacity << 2));
    for (I32 index = 0; index < self->size; ++index)
        Words(*self)[index] = reinterpret_cast<const U32*>(static_cast<std::uintptr_t>(old))[index];
    if (old != 0) live_free(AsPointer(old));
}
// 006857A0: size decremented first (006857A0..006857A6), then elements above
// index shift down by one while index < live size (006857B2..006857C3).
void __fastcall deleteAt_006857a0(List* self, void*, I32 index) {
    self->size = self->size - 1;
    for (; index < self->size; ++index) Words(*self)[index] = Words(*self)[index + 1];
}

// ---- OwnedList / base -----------------------------------------------------
OwnedList::OwnedList() noexcept { list.size = 0; list.capacity = 0; list.data = 0; }
OwnedList::~OwnedList() { if (list.data != 0) live_free(AsPointer(list.data)); }
void* RequestPoolManager::operator new(std::size_t size) noexcept { return live_operator_new(static_cast<std::uint32_t>(size)); }
void RequestPoolManager::operator delete(void* memory) noexcept { live_free(memory); }
// Inlined base construction (004356FE..00435711 / 00434F52..00434F61).
RequestPoolManager::RequestPoolManager() noexcept {
    free_indices_04.size = 0; free_indices_04.capacity = 0; free_indices_04.data = 0;
    locks_10.size = 0; locks_10.capacity = 0; locks_10.data = 0;
}
// 0042E250: vptr 0073F488, free locks data then free-index data when non-null.
RequestPoolManager::~RequestPoolManager() {
    if (locks_10.data != 0) live_free(AsPointer(locks_10.data));
    if (free_indices_04.data != 0) live_free(AsPointer(free_indices_04.data));
}

// ---- RequestPoolManagerSys ------------------------------------------------
// 004356E0: count=100 (00435727); pools=new(190h) (00435714/0043572E);
// sizes=new(count*4) (0043573D); zero sizes then pools over count*4 bytes
// (00435752..00435777); for i in [0,100): free.Add(i), locks.Add(0) with the
// inline growth (00435780..00435846, constant bound 64h at 00435840).
RequestPoolManagerSys::RequestPoolManagerSys() {
    count_1c = 100;
    pools_20 = static_cast<void**>(live_operator_new(0x190));
    sizes_24 = static_cast<U32*>(live_operator_new(count_1c << 2));
    std::memset(sizes_24, 0, count_1c << 2);
    std::memset(pools_20, 0, count_1c << 2);
    for (U32 index = 0; index < 0x64u; ++index) {
        Add(free_indices_04, index, Grow::Allocate00437d60);
        Add(locks_10, 0, Grow::Allocate00437d60);
    }
}
// 0042E540: free every pools[i] for i < count (unsigned jbe/jb, 0042E549..0042E568),
// then pools (0042E56E) and sizes (0042E577) unconditionally; base dtor follows.
RequestPoolManagerSys::~RequestPoolManagerSys() {
    for (U32 index = 0; index < count_1c; ++index) live_free(pools_20[index]);
    live_free(pools_20);
    live_free(sizes_24);
}
// 00435890 (RET 0Ch). Only the second stack argument (size) is read.
I32 RequestPoolManagerSys::RequestPool(I32, U32 size, U32) {
    using namespace kotor_renderer_gl_cells;
    // 00435898..004358BD: both counters updated before the zero-size test.
    const U32 total = pool_total_bytes_007a68a8;
    const U32 pages = pool_page_bytes_007a68ac;
    pool_total_bytes_007a68a8 = total + size;
    pool_page_bytes_007a68ac = pages + PageRound(size);
    if (size == 0) return -1;                                  // 004358C3..004358CC
    pool_sys_bytes_007a6988 = pool_sys_bytes_007a6988 + size;  // 004358CF..004358DD
    if (free_indices_04.size != 0) {                           // 004358D5..004358E3
        void* memory = live_operator_new(size);                // 004358E6
        const I32 last = free_indices_04.size - 1;
        const U32 index = Words(free_indices_04)[last];        // 004358F3
        deleteAt_006857a0(&free_indices_04, nullptr, last);    // 004358FF
        pools_20[index] = memory;                              // 00435907
        sizes_24[index] = size;                                // 0043590E
        return static_cast<I32>(index);
    }
    Add(locks_10, 0, Grow::Allocate005e03c0);                  // 0043591A..00435948
    for (U32 index = count_1c + 1; index < count_1c * 2u; ++index) {  // 0043594B..004359BC
        Add(free_indices_04, index, Grow::Allocate00437d60);   // 00435960..00435985
        Add(locks_10, 0, Grow::Allocate005e03c0);              // 00435988..004359B1
    }
    void** pools = static_cast<void**>(live_operator_new(count_1c << 3));   // 004359C5
    U32* sizes = static_cast<U32*>(live_operator_new(count_1c << 3));      // 004359D3
    std::memset(pools, 0, count_1c << 3);                      // 004359E7..004359F2
    std::memcpy(pools, pools_20, count_1c << 2);               // 004359F4..00435A0B
    live_free(pools_20);                                       // 00435A11
    std::memset(sizes, 0, count_1c << 3);                      // 00435A16..00435A30
    std::memcpy(sizes, sizes_24, count_1c << 2);               // 00435A32..00435A49
    live_free(sizes_24);                                       // 00435A4F
    pools_20 = pools;                                          // 00435A5D
    sizes_24 = sizes;                                          // 00435A60
    void* memory = live_operator_new(size);                    // 00435A63
    pools_20[count_1c] = memory;                               // 00435A68..00435A6E
    sizes_24[count_1c] = size;                                 // 00435A71..00435A7A
    const U32 result = count_1c;                               // 00435A7D
    count_1c = result * 2u;                                    // 00435A82..00435A86
    return static_cast<I32>(result);
}
// 0042E5B0 (RET 8): -1 or already locked -> 0; else lock and return the pool.
void* RequestPoolManagerSys::LockPool(I32 index, U32) {
    if (index == -1) return nullptr;
    U32& lock = Words(locks_10)[index];
    if (lock != 0) return nullptr;
    lock = 1;                                                  // 0042E5CD
    return pools_20[index];                                    // 0042E5D3..0042E5D6
}
// 0042E5E0 (RET 4). Original EAX residue (index or &locks[index]) is not modeled.
void RequestPoolManagerSys::UnlockPool(I32 index) {
    if (index == -1) return;
    U32& lock = Words(locks_10)[index];
    if (lock != 0) lock = 0;                                   // 0042E5EF..0042E5F4
}
// 00423B50 (RET 4), shared by slots 4 and 5.
void* RequestPoolManagerSys::GetPool(I32 index) { return index == -1 ? nullptr : pools_20[index]; }
void* RequestPoolManagerSys::GetIndexPool(I32 index) { return index == -1 ? nullptr : pools_20[index]; }
// 00435A90 (RET 4).
void RequestPoolManagerSys::DeletePool(I32 index) {
    using namespace kotor_renderer_gl_cells;
    if (index == -1) return;                                   // 00435A96
    Add(free_indices_04, static_cast<U32>(index), Grow::Allocate00437d60);  // 00435AA1..00435ACC
    pool_total_bytes_007a68a8 = pool_total_bytes_007a68a8 - sizes_24[index];       // 00435ACF..00435AE3
    pool_page_bytes_007a68ac = pool_page_bytes_007a68ac - PageRound(sizes_24[index]);  // 00435AE9..00435AFD
    Words(locks_10)[index] = 0;                                // 00435B06
    if (pools_20[index] != nullptr) live_free(pools_20[index]); // 00435B10..00435B19
    pools_20[index] = nullptr;                                 // 00435B24
    sizes_24[index] = 0;                                       // 00435B2E
}
// 00423A90 (RET 8): counters by delta, new block, copy the OLD size (even when
// larger than the new size), free old, store new size and pointer.
void RequestPoolManagerSys::ResizePool(I32 index, U32 size) {
    using namespace kotor_renderer_gl_cells;
    pool_total_bytes_007a68a8 = pool_total_bytes_007a68a8 + (size - sizes_24[index]);       // 00423AA1..00423AB1
    pool_page_bytes_007a68ac = pool_page_bytes_007a68ac + (PageRound(size) - PageRound(sizes_24[index]));  // 00423AB7..00423ADF
    void* old = pools_20[index];                               // 00423AE5..00423AEC
    void* memory = live_operator_new(size);                    // 00423AF0
    std::memcpy(memory, old, sizes_24[index]);                 // 00423AF5..00423B12
    live_free(old);                                            // 00423B14
    sizes_24[index] = size;                                    // 00423B23
    pools_20[index] = memory;                                  // 00423B2F
}
// 00423B40 (RET 4): no -1 test.
U32 RequestPoolManagerSys::GetPoolSize(I32 index) { return sizes_24[index]; }
void RequestPoolManagerSys::RefreshPool(U32, U32) {}           // 005B5E90 ret 8
void RequestPoolManagerSys::BackUpPoolManager() {}             // 004015A0 ret
void RequestPoolManagerSys::RestorePoolManager() {}            // 004015A0 ret

// ---- RequestPoolManagerVBO ------------------------------------------------
// 00434F30: count=100 (00434FA4); for i < live count (unsigned, 00435195..0043519B):
// free.Add(i), tags.Add(0), locks.Add(0), buffers.Add(0), targets.Add(8892h)
// (inline growth 00434FB0, 0043500D, 0043506E, 004350CF, 00435130).
RequestPoolManagerVBO::RequestPoolManagerVBO() {
    count_1c = 100;
    for (U32 index = 0; index < count_1c; ++index) {
        Add(free_indices_04, index, Grow::Allocate00437d60);
        Add(tags_20.list, 0, Grow::Allocate00437d60);
        Add(locks_10, 0, Grow::Allocate00437d60);
        Add(buffers_2c.list, 0, Grow::Allocate00437d60);
        Add(targets_50.list, 0x8892u, Grow::Allocate00437d60);
    }
}
// 004351C0: delete non-zero buffers for i < count (004351F8..0043521A), free
// every backup (00435225..0043523A), then clear backups/backup_sizes/targets
// (data free+null, capacity 0, size 0; 0043523C..00435284). Member dtors then
// free targets/backup_sizes/backups/buffers/tags data when non-null
// (00435287..004352D4) and the base dtor runs (004352D7..004352FA).
RequestPoolManagerVBO::~RequestPoolManagerVBO() {
    for (U32 index = 0; index < count_1c; ++index) {
        U32* name = Words(buffers_2c.list) + index;
        if (*name != 0) {
            glDeleteBuffersARB_007bb7bc(1, name);
            Words(buffers_2c.list)[index] = 0;
        }
    }
    for (I32 index = 0; index < backups_38.list.size; ++index) live_free(AsPointer(Words(backups_38.list)[index]));
    if (backups_38.list.data != 0) { live_free(AsPointer(backups_38.list.data)); backups_38.list.data = 0; }
    backups_38.list.capacity = 0; backups_38.list.size = 0;
    if (backup_sizes_44.list.data != 0) { live_free(AsPointer(backup_sizes_44.list.data)); backup_sizes_44.list.data = 0; }
    backup_sizes_44.list.capacity = 0; backup_sizes_44.list.size = 0;
    if (targets_50.list.data != 0) { live_free(AsPointer(targets_50.list.data)); targets_50.list.data = 0; }
    targets_50.list.capacity = 0; targets_50.list.size = 0;
}
// 00435310 (RET 0Ch): kind selects the target, size the store, tag goes to tags.
I32 RequestPoolManagerVBO::RequestPool(I32 kind, U32 size, U32 tag) {
    using namespace kotor_renderer_gl_cells;
    if (size == 0) return -1;                                  // 00435315..00435321
    pool_vbo_bytes_007a6984 = pool_vbo_bytes_007a6984 + size;  // 00435324..00435333
    if (free_indices_04.size != 0) {                           // 0043532A..00435339
        const I32 last = free_indices_04.size - 1;
        const U32 index = Words(free_indices_04)[last];        // 00435342
        deleteAt_006857a0(&free_indices_04, nullptr, last);    // 0043534B
        Words(locks_10)[index] = 0;                            // 0043535E
        Words(tags_20.list)[index] = tag;                      // 00435368
        Words(buffers_2c.list)[index] = 0;                     // 0043536E
        glGenBuffersARB_007bb5b4(1, Words(buffers_2c.list) + index);  // 00435375..0043537D
        const U32 target = TargetFor(kind);
        Words(targets_50.list)[index] = target;                // 0043538E / 004353C3
        glBindBufferARB_007bb7b4(target, Words(buffers_2c.list)[index]);  // 004353A1 / 004353D6
        glBufferDataARB_007bb544(target, static_cast<I32>(size), nullptr, 0x88e8u);  // 004353B4 / 004353E9
        return static_cast<I32>(index);
    }
    Add(locks_10, 0, Grow::Allocate005e03c0);                  // 004353F8..00435429
    Add(tags_20.list, 0, Grow::Allocate00437d60);              // 0043542A..0043545D
    Add(buffers_2c.list, 0, Grow::Allocate00437d60);           // 0043545E..00435490
    Add(targets_50.list, 0x8892u, Grow::Allocate00437d60);     // 00435491..004354C0
    for (U32 index = count_1c + 1; index < count_1c * 2u; ++index) {  // 004354C3..004355CF
        Add(free_indices_04, index, Grow::Allocate00437d60);
        Add(locks_10, 0, Grow::Allocate005e03c0);
        Add(tags_20.list, 0, Grow::Allocate00437d60);
        Add(buffers_2c.list, 0, Grow::Allocate00437d60);
        Add(targets_50.list, 0x8892u, Grow::Allocate00437d60);
    }
    glGenBuffersARB_007bb5b4(1, Words(buffers_2c.list) + count_1c);  // 004355D5..004355E0
    const U32 target = TargetFor(kind);
    Words(targets_50.list)[count_1c] = target;                 // 004355F3 / 00435621
    glBindBufferARB_007bb7b4(target, Words(buffers_2c.list)[count_1c]);  // 00435608 / 00435636
    glBufferDataARB_007bb544(target, static_cast<I32>(size), nullptr, 0x88e8u);  // 0043564D
    const U32 result = count_1c;                               // 00435653
    count_1c = result * 2u;                                    // 00435657..0043565B
    return static_cast<I32>(result);
}
// 0042E2A0 (RET 8).
void* RequestPoolManagerVBO::LockPool(I32 index, U32 flags) {
    using namespace kotor_renderer_gl_cells;
    if (index == -1 || Words(locks_10)[index] != 0) return nullptr;   // 0042E2A6..0042E2BA
    glBindBufferARB_007bb7b4(Words(targets_50.list)[index], Words(buffers_2c.list)[index]);  // 0042E2BD..0042E2CB
    Words(tags_20.list)[index] = flags;                        // 0042E2DA
    Words(locks_10)[index] = 1;                                // 0042E2E0
    U32 access;
    if ((flags & 8u) != 0) access = 0x88b8u;                   // 0042E2D8/0042E2EC READ_ONLY
    else if ((flags & 0x10u) != 0) access = 0x88bau;           // 0042E301/0042E308 READ_WRITE
    else access = renderer_flag_0078e398 != 0 ? 0x88b9u : 0x88bau;  // 0042E31D..0042E329
    return glMapBufferARB_007bb634(Words(targets_50.list)[index], access);  // 0042E2F6/0042E312/0042E333
}
// 0042E340 (RET 4): tail-jumps to glUnmapBufferARB (0042E380); its AL result is
// not carried by this interface (open issue in PART.json).
void RequestPoolManagerVBO::UnlockPool(I32 index) {
    if (index == -1) return;
    U32& lock = Words(locks_10)[index];
    if (lock == 0) return;                                     // 0042E353..0042E358
    lock = 0;                                                  // 0042E35A
    glBindBufferARB_007bb7b4(Words(targets_50.list)[index], Words(buffers_2c.list)[index]);  // 0042E360..0042E36E
    glUnmapBufferARB_007bb6e0(Words(targets_50.list)[index]);  // 0042E374..0042E380
}
// 0042E390 (RET 4): buffer name as the EAX word.
void* RequestPoolManagerVBO::GetPool(I32 index) { return index == -1 ? nullptr : AsPointer(Words(buffers_2c.list)[index]); }
// 0042E3B0 (RET 4): binds ELEMENT_ARRAY to the buffer, always returns 0.
void* RequestPoolManagerVBO::GetIndexPool(I32 index) {
    if (index != -1) glBindBufferARB_007bb7b4(0x8893u, Words(buffers_2c.list)[index]);  // 0042E3B9..0042E3C5
    return nullptr;                                            // 0042E3CB
}
// 00435670 (RET 4).
void RequestPoolManagerVBO::DeletePool(I32 index) {
    if (index == -1) return;                                   // 00435675
    Add(free_indices_04, static_cast<U32>(index), Grow::Allocate00437d60);  // 0043567D..004356A8
    Words(locks_10)[index] = 0;                                // 004356AE
    U32 name = Words(buffers_2c.list)[index];                  // 004356B5..004356C2
    glDeleteBuffersARB_007bb7bc(1, &name);                     // 004356C6
    Words(buffers_2c.list)[index] = 0;                         // 004356CF
}
// 0042E420 (RET 8).
void RequestPoolManagerVBO::ResizePool(I32 index, U32 size) {
    if (index == -1) return;                                   // 0042E42D
    glBindBufferARB_007bb7b4(Words(targets_50.list)[index], Words(buffers_2c.list)[index]);  // 0042E43E..0042E44B
    I32 old_size;
    glGetBufferParameterivARB_007bb5a4(Words(targets_50.list)[index], 0x8764u, &old_size);   // 0042E451..0042E462
    void* copy = live_operator_new(static_cast<U32>(old_size));                              // 0042E46D
    const void* mapped = glMapBufferARB_007bb634(Words(targets_50.list)[index], 0x88b8u);   // 0042E47A..0042E487
    std::memcpy(copy, mapped, static_cast<U32>(old_size));                                    // 0042E48D..0042E49F
    glUnmapBufferARB_007bb6e0(Words(targets_50.list)[index]);                                // 0042E4A1..0042E4A8
    U32 name = Words(buffers_2c.list)[index];                  // 0042E4AE..0042E4BB
    glDeleteBuffersARB_007bb7bc(1, &name);                     // 0042E4BF
    Words(buffers_2c.list)[index] = 0;                         // 0042E4C8
    glGenBuffersARB_007bb5b4(1, Words(buffers_2c.list) + index);  // 0042E4CF..0042E4D7
    glBindBufferARB_007bb7b4(Words(targets_50.list)[index], Words(buffers_2c.list)[index]);  // 0042E4DD..0042E4EB
    glBufferDataARB_007bb544(Words(targets_50.list)[index], static_cast<I32>(size), nullptr, 0x88e8u);  // 0042E4F1..0042E504
    glBufferSubDataARB_007bb75c(Words(targets_50.list)[index], 0, old_size, copy);           // 0042E50A..0042E51D
    live_free(copy);                                           // 0042E524
}
// 0042E3D0 (RET 4).
U32 RequestPoolManagerVBO::GetPoolSize(I32 index) {
    if (index == -1) return 0;                                 // 0042E3D6..0042E3E1
    glBindBufferARB_007bb7b4(Words(targets_50.list)[index], Words(buffers_2c.list)[index]);  // 0042E3E4..0042E3F2
    I32 value;
    glGetBufferParameterivARB_007bb5a4(Words(targets_50.list)[index], 0x8764u, &value);       // 0042E3F8..0042E409
    return static_cast<U32>(value);                            // 0042E40F
}
void RequestPoolManagerVBO::RefreshPool(U32, U32) {}           // 005B5E90 ret 8
// 00434CC0 (RET): appends one backup/size pair per buffer slot; index i of the
// appended lists is assumed to equal the loop index (00434E09..00434E15).
void RequestPoolManagerVBO::BackUpPoolManager() {
    for (I32 index = 0; index < buffers_2c.list.size; ++index) {         // 00434CC6..00434CD5, 00434E4D..00434E5B
        const U32 name = Words(buffers_2c.list)[index];                  // 00434CE8..00434CEB
        if (name == 0) {
            Add(backups_38.list, 0, Grow::Allocate005e03c0);             // 00434CF2..00434D1C
            Add(backup_sizes_44.list, 0, Grow::Allocate00437d60);        // 00434D20..00434D4F
            continue;
        }
        glBindBufferARB_007bb7b4(Words(targets_50.list)[index], name);   // 00434D57..00434D5F
        I32 size;
        glGetBufferParameterivARB_007bb5a4(Words(targets_50.list)[index], 0x8764u, &size);  // 00434D65..00434D76
        const I32 saved = size;                                          // 00434D86
        Add(backup_sizes_44.list, static_cast<U32>(saved), Grow::Allocate00437d60);  // 00434D80..00434DB9
        void* memory = live_operator_new(static_cast<U32>(size));        // 00434DBC
        Add(backups_38.list, AsWord(memory), Grow::Allocate005e03c0);    // 00434DC1..00434DF4
        const void* mapped = glMapBufferARB_007bb634(Words(targets_50.list)[index], 0x88b8u);  // 00434DF7..00434E03
        std::memcpy(AsPointer(Words(backups_38.list)[index]), mapped, Words(backup_sizes_44.list)[index]);  // 00434E09..00434E22
        glUnmapBufferARB_007bb6e0(Words(targets_50.list)[index]);       // 00434E28..00434E2F
        glDeleteBuffersARB_007bb7bc(1, Words(buffers_2c.list) + index);  // 00434E35..00434E3D
        Words(buffers_2c.list)[index] = 0;                               // 00434E43..00434E46
    }
}
// 00434E70 (RET): LoadExtensions calls this through vtable +2C (VBO route).
void RequestPoolManagerVBO::RestorePoolManager() {
    for (I32 index = 0; index < backups_38.list.size; ++index) {         // 00434E75..00434E7F, 00434EE1..00434EE7
        if (Words(backups_38.list)[index] == 0) continue;                // 00434E81..00434E87
        U32 name;
        glGenBuffersARB_007bb5b4(1, &name);                              // 00434E89..00434E90
        Words(buffers_2c.list)[index] = name;                            // 00434E96..00434E9D
        glBindBufferARB_007bb7b4(Words(targets_50.list)[index], name);   // 00434EA0..00434EAC
        glBufferDataARB_007bb544(Words(targets_50.list)[index], static_cast<I32>(Words(backup_sizes_44.list)[index]),
            AsPointer(Words(backups_38.list)[index]), 0x88e8u);          // 00434EB2..00434ECC
        live_free(AsPointer(Words(backups_38.list)[index]));             // 00434ED2..00434ED9
    }
    backups_38.list.size = 0;                                            // 00434EE9
    backup_sizes_44.list.size = 0;                                       // 00434EEC
    if (backups_38.list.data != 0) { live_free(AsPointer(backups_38.list.data)); backups_38.list.data = 0; }  // 00434EEF..00434EFF
    backups_38.list.capacity = 0; backups_38.list.size = 0;              // 00434F02..00434F05
    if (backup_sizes_44.list.data != 0) { live_free(AsPointer(backup_sizes_44.list.data)); backup_sizes_44.list.data = 0; }  // 00434F08..00434F18
    backup_sizes_44.list.capacity = 0; backup_sizes_44.list.size = 0;    // 00434F1C..00434F1F
}
}

namespace kotor_renderer_init {
// 00436390: only when 007A68A4 is null (004363A6..004363AD): operator new 28h
// (004363AF), null test (004363BD), ctor (004363CB), store EAX or 0
// (004363D0 / 004363E6). Exception during construction frees the block
// (EH state 0 at 004363BF) via the class operator delete.
void __stdcall InitSystemPool_00436390() {
    using kotor_renderer_gl_cells::pool_007a68a4;
    if (pool_007a68a4 == nullptr) pool_007a68a4 = new kotor_renderer_gl::RequestPoolManagerSys;
}
// 00436400: same shape, operator new 5Ch (0043641F), ctor 00434F30 (0043643B).
void __stdcall InitVertexBufferPool_00436400() {
    using kotor_renderer_gl_cells::pool_007a68a4;
    if (pool_007a68a4 == nullptr) pool_007a68a4 = new kotor_renderer_gl::RequestPoolManagerVBO;
}
}
