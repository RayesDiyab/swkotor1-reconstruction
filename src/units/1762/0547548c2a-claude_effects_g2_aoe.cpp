// Lane effects-g2 (AoE subset): CSWSEffectListHandler::OnRemoveAreaOfEffect 004DB010, reconstructed from the
// original swkotor.exe (read-only).  Original: thiscall(CSWSObject* object, CGameEffect* effect), RET 8 (verified:
// 004DB087 `ret 8`; the remove handlers take no load_game argument) -> @kotor_fastcall_004db010@16.
//
// OnApplyAreaOfEffect 004DADE0 is NOT implemented here: its closure needs the whole CSWSAreaOfEffectObject class
// (ctor 00594480 + base CSWSObject::CSWSObject 004CFCB0, vtable 00749EA8 with dtor/AIUpdate/EventHandler and ~16
// inherited CSWSObject slots that exist only as file-local bodies) - see the lane report for the exact list.
//
// Conventions: thiscall -> extern "C" __fastcall(self, unused_edx, ...); only existing C-linkage providers of the
// c27 candidate are bound; no original address is dereferenced.
#include "842d6b1618-w5_t3.hpp"

extern "C" {
using w5::U;
void* __fastcall recovered_object_array_004aed70(void* server, void*);                         // GetObjectArray [w2_party_native]
void* __fastcall recovered_server_ai_master_004aed80(void* server, void*);                     // GetServerAIMaster [creature_destructor_r2]
}

namespace {
constexpr unsigned char k_object_found_0074666c = 0;   // .rdata 0074666C byte 00 (CGameObjectArray success code)
constexpr U k_event_type_007441e0 = 0x0b;              // .rdata 007441E0 dword 0Bh (AI event: remove/destroy AoE)
// CGameEffect::GetObjectID 005036E0 (mov eax,[ecx+eax*4+78h]; ret 4): inline view, no address credit
inline U effect_object_id(void* effect, U index) { return w5::at<U>(effect, 0x78 + index * 4); }
}

extern "C" unsigned __fastcall kotor_fastcall_004db010(void* /*self*/, void*, void* object, void* effect)
{
    // 004DB011..004DB01E  if (!object->vtbl[0x30]()) return 1
    if (w5::vcall<void*>(object, 0x30) == nullptr) return 1;
    // 004DB020..004DB024  second call; result discarded (EAX overwritten below) - original quirk kept
    (void)w5::vcall<void*>(object, 0x30);
    // 004DB027..004DB032  id = effect->GetObjectID(0)
    const U id = effect_object_id(effect, 0);
    // 004DB034..004DB049  GetObjectArray()->GetGameObject(id, &found) (out slot reuses the object argument slot)
    void* found = object;
    void* array = recovered_object_array_004aed70(w5::server_app(), nullptr);
    const unsigned char status = static_cast<unsigned char>(kotor_fastcall_004d8230(array, nullptr, id, &found));
    // 004DB04E..004DB05C  status == [74666C] && found != 0
    if (status == k_object_found_0074666c && found != nullptr) {
        // 004DB05E..004DB07C  AIMaster->AddEventDeltaTime(0, 0, id, id, [7441E0]=0Bh, NULL)
        void* ai = recovered_server_ai_master_004aed80(w5::server_app(), nullptr);
        (void)recovered_event_enqueue_004b08d0(ai, nullptr, 0, 0, id, id, k_event_type_007441e0, nullptr);
    }
    // 004DB081..004DB087
    return 1;
}
