// Lane "dialogue-start", fork B2, part 3: CSWBehaviorCameraDialog::SetShot 006BD260 (the line-of-action shot
// set-up), transcribed from the original swkotor.exe (read-only). x87 conventions as in claude_dialogue_camera.cpp.
#include "1289b1b830-claude_dialogue_camera.hpp"

using namespace dlg;

extern "C" {
void* __fastcall kotor_fastcall_005ed580(void* client, void*, U id);                     // CClientExoApp::GetGameObject
void* __fastcall safe_pointer_assign_00447fa0(void* safe_pointer, void*, void* object);   // SafePointer<CAurObject>::=
void __fastcall behavior_add_dependency_0049f130(void* behavior, void*, void* object);    // CAurBehavior::AddDependency
void __fastcall int_list_add_00444fe0(void* list, void*, U value);                        // CExoArrayList<int>::Add
void* __fastcall gob_get_property_0x68_0063c070(void* gob, void*);                        // GetProperty0x68
void* __fastcall aurora_hit_info_construct_004578f0(void* hit, void*, U a, U b, U c);    // CHitInfo::CHitInfo
void __fastcall aurora_hit_info_clear_output_00456a60(void* hit, void*);                  // CHitInfo::ClearOutput
void __fastcall chit_info_destruct_00456b30(void* hit, void*);                            // CHitInfo::~CHitInfo
}

namespace {
constexpr U kHookHeight = 0x3dcccccdu;   // [0073E798] 0.1f
constexpr U kHalf = 0x3f000000u;         // [0073E9AC] 0.5f

struct HitInfo {                          // CHitInfo (constructed at [esp+70h]; 64h bytes of frame)
    alignas(4) unsigned char bytes[0x64];
};

// The model resolution shared by both participants (006BD306..006BD39D / 006BD3A2..006BD439): the creature
// (slot 2Ch), else slot 4Ch, else slot 14Ch of the client object supplies the SafePointer target and the height.
bool bind_participant(void* self, U id, U safe_offset, U height_offset) {
    void* object = kotor_fastcall_005ed580(client(), nullptr, id);
    if (!object) return false;
    void* creature = vcall<void*>(object, 0x2c);
    void* model = vcall<void*>(object, 0x4c);
    void* other = vcall<void*>(object, 0x14);
    void* source = creature ? creature : (model ? model : other);
    if (!source) return false;
    (void)safe_pointer_assign_00447fa0(at(self, safe_offset), nullptr, vcall<void*>(source, 0xd0));
    wr(self, height_offset, fs(vcall<double>(source, 0xd8)));
    behavior_add_dependency_0049f130(self, nullptr, rp(self, safe_offset));
    return true;
}

// 006BD509..006BD5BD: add the participant's slot-8(FEh) object to the hit-info ignore list
void ignore_participant(HitInfo& hit, U id) {
    void* object = kotor_fastcall_005ed580(client(), nullptr, id);
    if (!object || !vcall<void*>(object, 0xc)) return;
    if (!gob_get_property_0x68_0063c070(vcall<void*>(object, 0xc), nullptr)) return;
    void* p = gob_get_property_0x68_0063c070(vcall<void*>(object, 0xc), nullptr);
    const U item = vcall<U>(p, 0x8, 0xfeu);
    if (item) int_list_add_00444fe0(hit.bytes + 8, nullptr, item);
}
}  // namespace

extern "C" {

// 006BD260 CSWBehaviorCameraDialog::SetShot(id_a, id_b, int* shot, U* a4, U* a5, float range) (RET 18h)
void __fastcall kotor_fastcall_006bd260(void* self, void*, U id_a, U id_b, void* shot, void* a4, void* a5, U range) {
    V3 position{0, 0, 0};                                                                  // [esp+3Ch]..44h (first)
    Q4 orientation{0x3f800000u, 0, 0, 0};                                                  // [esp+60h]..6Ch
    if (rd(self, 0x28) == 3 && rd(shot, 0) == 3) return;                                   // 006BD280..006BD2C9
    if (rd(shot, 0) == 6) return;                                                          // 006BD2CF..006BD2D9
    wr(self, 0x34, 0);                                                                     // 006BD2E2
    if (vcall<void*>(rp(self, 0x10), 0x4))                                                 // 006BD2DF..006BD2F0
        vcall<void>(vcall<void*>(rp(self, 0x10), 0x4), 0x44, 0x425c0000u);                // 006BD2F2..006BD303 (55.0f)
    if (!bind_participant(self, id_a, 0x64, 0x14)) return;                                 // 006BD306..006BD39D
    if (!bind_participant(self, id_b, 0x68, 0x18)) return;                                 // 006BD3A2..006BD439
    wr(self, 0x5c, id_a);                                                                  // 006BD460
    wr(self, 0x60, id_b);                                                                  // 006BD463
    wr(self, 0x1c, rd(a4, 0));                                                             // 006BD459..006BD468
    wr(self, 0x20, rd(a5, 0));                                                             // 006BD46B..006BD474
    U r = range;                                                                           // 006BD43E fld
    if (!jp(fcom(fl(r), fl(0x3a83126fu)), 0x05)) {                                         // fcom [0073D708] 0.001f
        if (!jne(fcom(fl(r), fl(0xba83126fu)), 0x41)) r = 0;                               // fcom [00747D80] -0.001f
    }
    wr(self, 0x24, r);                                                                     // 006BD494
    const U side = kotor_fastcall_006bcec0(self, nullptr, id_a, id_b);                     // 006BD493..006BD49A ([esp+1Ch])
    position = V3{0, 0, 0};                                                                // [esp+30h..38h]
    V3 hit_out{0, 0, 0};                                                                   // [esp+54h..5Ch]
    HitInfo hit;
    (void)aurora_hit_info_construct_004578f0(&hit, nullptr, 0, 0xbfffffffu, 0);            // 006BD4A1..006BD4E0
    wr(hit.bytes, 0, 0);                                                                   // 006BD4F4
    int_list_add_00444fe0(hit.bytes + 8, nullptr, rd(self, 0x64));                         // 006BD4E5..006BD4F8
    int_list_add_00444fe0(hit.bytes + 8, nullptr, rd(self, 0x68));                         // 006BD4FD..006BD504
    ignore_participant(hit, rd(self, 0x5c));                                               // 006BD509..006BD561
    ignore_participant(hit, rd(self, 0x60));                                               // 006BD566..006BD5BD
    V3 a, b;                                                                               // [esp+10h], [esp+24h]
    (void)vcall<void*>(rp(self, 0x64), 0x64, &a);                                          // 006BD5C2..006BD5CC
    (void)vcall<void*>(rp(self, 0x68), 0x64, &b);                                          // 006BD5CF..006BD5D8
    const bool hooks = rd(self, 0x38) != 0;                                                // 006BD5E5..006BD5E8
    void* object_a = rp(self, 0x64);                                                       // 006BD5EA (ebp)
    a.z = fs(fl(a.z) + fl(kHookHeight));                                                   // 006BD5DB..006BD5ED
    // midpoint between the participants (stack values kept unrounded where the original keeps them on the FPU)
    V3 mid;                                                                                // [esp+48h]..50h
    {
        const double bz = fl(b.z) + fl(kHookHeight);                                       // 006BD5F1..006BD5F5
        b.z = fs(bz);                                                                      // 006BD5FB fst
        const double dz = bz - fl(a.z);                                                    // 006BD5FF
        const double dy = fl(b.y) - fl(a.y);                                               // 006BD603..006BD607
        const double dx = fl(b.x) - fl(a.x);                                               // 006BD60B..006BD60F
        U t48 = fs(dx);                                                                    // 006BD613
        U t20 = fs(dz * fl(kHalf));                                                        // 006BD619..006BD61F
        const double hy = dy * fl(kHalf);                                                  // 006BD623
        t48 = fs(fl(t48) * fl(kHalf));                                                     // 006BD629..006BD633
        t20 = fs(fl(t20) + fl(a.z));                                                       // 006BD637..006BD63F
        const double my = hy + fl(a.y);                                                    // 006BD643
        mid.x = fs(fl(t48) + fl(a.x));                                                     // 006BD647..006BD64F
        mid.y = fs(my);                                                                    // 006BD653
        mid.z = fs(((fl(rd(self, 0x14)) - fl(rd(self, 0x18))) * fl(kHalf) + fl(t20)) + fl(rd(self, 0x14))); // ..006BD66A
    }
    Q4 out_orientation;                                                                    // [esp+D4h]
    if (hooks) {                                                                           // 006BD66E
        V3 buf_b, buf_a;                                                                   // [esp+E4h], [esp+F0h]
        const V3* pb = vcall<const V3*>(rp(self, 0x68), 0x64, &buf_b);                     // 006BD670..006BD67E (edi)
        const V3* pa = vcall<const V3*>(object_a, 0x64, &buf_a);                           // 006BD681..006BD690
        const V3 va = *pa, vb = *pb;
        kotor_fastcall_006bb7b0(self, nullptr, rd(shot, 0), side, va, vb, &position, &out_orientation); // ..006BD7B1
    } else {
        V3 hook{0, 0, 0};                                                                  // [esp+3Ch] (the zero vector)
        a.z = fs(fl(a.z) + fl(rd(self, 0x14)));                                            // 006BD6DE..006BD6EF
        b.z = fs(fl(b.z) + fl(rd(self, 0x18)));                                            // 006BD6F8..006BD701
        if (vcall<U>(object_a, 0x98, "CAMERAHOOK", &hook, &orientation)) a = hook;         // 006BD705..006BD726
        if (vcall<U>(rp(self, 0x68), 0x98, "CAMERAHOOK", &hook, &orientation)) b = hook;   // 006BD72A..006BD75D
        kotor_fastcall_006bb7b0(self, nullptr, rd(shot, 0), side, a, b, &position, &out_orientation); // ..006BD7B1
    }
    // line-of-sight tests from the computed camera position (vtable slot 38h of the scene, slot 104h of the owner)
    U blocked;
    if (rd(shot, 0) == 3) {                                                                // 006BD7B6..006BD7C3
        blocked = vcall<U>(vcall<void*>(rp(self, 0x10), 0x104), 0x38, mid, position, &hit_out, &hit); // ..006BD811
        aurora_hit_info_clear_output_00456a60(&hit, nullptr);                              // 006BD814..006BD81A
        if (rd(self, 0x38) == 0) {                                                         // 006BD81F..006BD824
            blocked += vcall<U>(vcall<void*>(rp(self, 0x10), 0x104), 0x38, a, position, &hit_out, &hit); // ..006BD874
            aurora_hit_info_clear_output_00456a60(&hit, nullptr);                          // 006BD870..006BD876
            blocked += vcall<U>(vcall<void*>(rp(self, 0x10), 0x104), 0x38, b, position, &hit_out, &hit); // ..006BD8C5
            aurora_hit_info_clear_output_00456a60(&hit, nullptr);                          // 006BD916..006BD91A
        }
    } else {
        blocked = vcall<U>(vcall<void*>(rp(self, 0x10), 0x104), 0x38, a, position, &hit_out, &hit); // 006BD8C9..006BD914
        aurora_hit_info_clear_output_00456a60(&hit, nullptr);                              // 006BD916..006BD91A
    }
    if (blocked) wr(self, 0x34, 1);                                                        // 006BD91F..006BD928
    wr(self, 0x28, rd(shot, 0));                                                           // 006BD92B..006BD93A
    wr(self, 0x3c, 0);                                                                     // 006BD93D
    (void)vcall<U>(self, 0x8, 0u);                                                         // 006BD934..006BD944 Control(0)
    wr(self, 0x3c, 1);                                                                     // 006BD94B
    chit_info_destruct_00456b30(&hit, nullptr);                                            // 006BD947..006BD959
}

}  // extern "C"
