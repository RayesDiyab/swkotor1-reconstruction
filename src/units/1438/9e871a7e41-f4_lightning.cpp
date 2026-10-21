// f4_lightning.cpp - lane-m46-3dmodel (fork F4): LightningEmitter class bodies of swkotor.exe (GOG K1, image
// base 00400000) referenced by the lane-3d-model table 00743610 (aurora_vtable_lightning_emitter_00743610):
//   0049D790 ~LightningEmitter (deleting)   0049D480 Initialize   00498B80 Update
//   00491A50 UpdateBranches                 00497C60 UpdateControlPoints
// plus the callees reached only from them and owned by nobody yet:
//   0049D300 ~LightningEmitter (body)       0049CE30 InitializeBranch   0048E700 PartEmitter::RecursiveFractal
// Evidence: dumpbin /disasm of the pinned swkotor.exe (lane-sound/full.asm); every statement cites its range.
// Not reached on the first main-menu frame (all six menu emitters are Fountain); authored so the D39 table
// 00743610 links to genuine bodies.
// Conventions: emitter.h (lane-3d-emitter): D1 __fastcall transport (callees never read EDX); D21/D24 typed
// tables, no function-pointer casts; D30-FINAL: x87 wide values are double, float only at original fstp dword
// stores (S()), fild of an int is exact; _ftol2 006FAE8C -> ftol2(); CRT rand 006FC447 -> crt_rand_006fc447 (D52).
// Unwind maps 00717C06 / 00717C1B are cited, not emitted: live_operator_new returns NULL like the original
// operator new and no callee throws (D30-FINAL extern "C" rule).
#include "a8ccc70d67-emitter.h"

namespace {
using namespace kotor_emitter;
constexpr U kOneBits = 0x3F800000u;           // 1.0f
constexpr U kMinusOneBits = 0xBF800000u;      // -1.0f
constexpr U kHundredth_0073dfec = 0x3C23D70Au; // 0.01f
constexpr U kTenth_0073e798 = 0x3DCCCCCDu;    // 0.1f
constexpr U kHalf_0073e9ac = 0x3F000000u;     // 0.5f
constexpr U kTwo_00741be0 = 0x40000000u;      // 2.0f
constexpr U kQuarter_0073edf0 = 0x3E800000u;  // 0.25f
constexpr U kZero_0073d700 = 0x00000000u;     // 0.0f
constexpr U kHundred_00741838 = 0x42C80000u;  // 100.0f
constexpr U kOne_0073d764 = 0x3F800000u;      // 1.0f
constexpr U kThree_00740c84 = 0x40400000u;    // 3.0f
constexpr U kMinusThree_00743334 = 0xC0400000u; // -3.0f
constexpr U kFifty_00740cc0 = 0x42480000u;    // 50.0f
constexpr U kDegToRad_007415f8 = 0x3C8EFA35u; // 0.0174533f
constexpr U kRange_461c3c00 = 0x461C3C00u;    // 9999.0f (RegisterCallBack range immediate, 0049D4A6)

inline PointerList* list_at(void* object, U offset) { return static_cast<PointerList*>(at(object, offset)); }
inline U data_word(void* object, U list_offset, I index) {    // [[object+off] + index*4]
    return *reinterpret_cast<const volatile U*>(field<U>(object, list_offset) + static_cast<U>(index) * 4u);
}
inline void* branch_at(void* self, I index) { return as_object(data_word(self, 0x258, index)); }   // list +258 (LightningEmitter*)
inline I target_at(void* self, I index) { return static_cast<I>(data_word(self, 0x264, index)); }   // list +264 (int)
inline void* particle_at(void* self, I index) { return as_object(data_word(self, 0x1C8, index)); }  // list +1C8 (Particle*)
inline Vector* vec_data(void* object, U list_offset) {          // Vector list data pointer
    return static_cast<Vector*>(as_object(field<U>(object, list_offset)));
}
inline Vector* particle_position(void* particle) { return static_cast<Vector*>(at(particle, 0x8)); }   // Particle+8
inline double rand_int_as_double(I value) { return static_cast<double>(value); }   // fild dword (exact)
inline I rand_mod(I divisor) { return crt_rand_006fc447() % divisor; }               // call 006FC447; cdq; idiv
// Gob typed slots (vtables_gen.h, D42): +5C SetPosition(out, Vector), +64 GetPosition(out) -> Vector*.
inline Vector* gob_get_position(void* gob, Vector* out) {
    return static_cast<Vector*>(as_object(gob_table(gob)->s064_gob_get_position(as_gob(gob), nullptr, word_of(out))));
}
inline void gob_set_position(void* gob, Vector* out, Vector position) {
    gob_table(gob)->s05c_gob_set_position(as_gob(gob), nullptr, out, position);
}
} // namespace

extern "C" {
// Borrowed providers (exact M45 symbols; declarations local to this unit).
U __fastcall part_emitter_initialize_0049caa0(void* self, void* unused_edx);                       // lane-3d-model model_lifetime (RET 0)
void* __fastcall aurora_lightning_emitter_construct_0049d1d0(void* self, void* unused_edx, void* node); // lane-3d-model model_instance (RET 4)
U __fastcall aurora_node_control_dispatch_0043f890(void* node, void* unused_edx, void* part, U raw_time, U raw_a, U raw_weight); // lane-3d-anim (RET 10h)
// core (3d-construct/3d-bsp consumers' spelling; owner lane-3d-render core): YawPitchRoll 004ACAC0, cdecl
Quaternion* __cdecl yaw_pitch_roll_004acac0(Quaternion* out, float yaw, float pitch, float roll);

// lane-gui-load gui_objects.h: CExoArrayList<Vector>::deleteAt 0048DDE0 (12-byte elements), thiscall RET 4
void __fastcall gui_load_list12_delete_at_0048dde0(kotor_reconstruction::ArrayMutations* self, void* unused_edx, std::int32_t index);

// authored in this unit (non-table entry points)
void __fastcall lightning_emitter_destruct_0049d300(void* self, void* unused_edx);
void __fastcall lightning_emitter_initialize_branch_0049ce30(void* self, void* unused_edx);
U __fastcall lightning_emitter_deleting_destructor_0049d790(void* self, void* unused_edx, U flags);
U __fastcall lightning_emitter_initialize_0049d480(void* self, void* unused_edx);
U __fastcall lightning_emitter_update_branches_00491a50(void* self, void* unused_edx);
U __fastcall lightning_emitter_update_control_points_00497c60(void* self, void* unused_edx);
U __fastcall lightning_emitter_update_00498b80(void* self, void* unused_edx, U delta_bits);
Vector* __cdecl float_times_vector_00437a60(Vector* out, float scale, const Vector* v);

// ---------------------------------------------------------------- 0049D300 LightningEmitter::~LightningEmitter (body), RET 0
void __fastcall lightning_emitter_destruct_0049d300(void* self, void*) {
    field<const void*>(self, 0) = &aurora_vtable_lightning_emitter_00743610;               // 0049D31E
    void* const node = aurora_node_as_emitter_0043e3c0(field<void*>(self, 0x4), nullptr);  // 0049D324..0049D334
    if (field<I>(self, 0x25C) != 0) {                                                     // 0049D336..0049D33E (list +258 size)
        for (I i = 0; i < field<I>(node, 0x5C); ++i) {                                    // 0049D340..0049D382 (count re-read)
            void* const gob = field<void*>(branch_at(self, i), 0x1E4);                    // 0049D350..0049D359
            if (gob != nullptr)                                                           // 0049D35F..0049D361
                gob_table(gob)->s000_gob_destruct_gob(as_gob(gob), nullptr, 1);           // 0049D363..0049D367 (vtbl+0, 1)
            void* const branch = branch_at(self, i);                                      // 0049D369..0049D36F
            if (branch != nullptr)                                                        // 0049D372..0049D374
                part_table(branch)->s000(branch, nullptr, 1);                             // 0049D376..0049D37A (vtbl+0, 1)
        }
    }
    // CExoArrayList destructors in reverse member order (EH states 8..0): free data only.
    static const U kLists[] = {0x270, 0x264, 0x258, 0x24C, 0x240, 0x234, 0x228, 0x21C, 0x210};  // 0049D385..0049D45A
    for (U offset : kLists) {
        void* const data = field<void*>(self, offset);
        if (data != nullptr) live_free(data);
    }
    part_emitter_destruct_0049cfa0(self, nullptr);                                        // 0049D45D..0049D467
}

// ---------------------------------------------------------------- 0049D790 LightningEmitter deleting destructor, RET 4
U __fastcall lightning_emitter_deleting_destructor_0049d790(void* self, void*, U flags) {
    lightning_emitter_destruct_0049d300(self, nullptr);                                   // 0049D791..0049D793
    if ((flags & 1u) != 0) live_free(self);                                               // 0049D798..0049D7A5 (006FA390)
    return word_of(self);                                                                 // 0049D7A8 mov eax,esi
}

// ---------------------------------------------------------------- 0049CE30 LightningEmitter::InitializeBranch, RET 0
void __fastcall lightning_emitter_initialize_branch_0049ce30(void* self, void*) {
    part_emitter_initialize_0049caa0(self, nullptr);                                      // 0049CE4A..0049CE4C
    aurora_node_control_dispatch_0043f890(field<void*>(self, 0x4), nullptr, self,
                                          kMinusOneBits, 0, kOneBits);                    // 0049CE51..0049CE62
    static const U kVectorLists[] = {0x210, 0x21C, 0x228, 0x234, 0x240, 0x24C};          // 0049CE67..0049CECF
    for (U offset : kVectorLists) {
        vector_list_allocate_00421310(list_at(self, offset), nullptr, 2);                 // push 2; call 00421310
        field<I>(self, offset + 4) = 2;                                                   // mov [edi+4],ebx (size = 2)
    }
    if (field<void*>(self, 0x258) != nullptr) {                                           // 0049CED2..0049CEDA
        live_free(field<void*>(self, 0x258));                                             // 0049CEDC..0049CEDD
        field<U>(self, 0x258) = 0;                                                        // 0049CEE5
    }
    field<U>(self, 0x260) = 0;                                                            // 0049CEEB (capacity)
    field<U>(self, 0x25C) = 0;                                                            // 0049CEF1 (size)
    if (field<void*>(self, 0x264) != nullptr) {                                           // 0049CEF7..0049CEFF
        live_free(field<void*>(self, 0x264));                                             // 0049CF01..0049CF02
        field<U>(self, 0x264) = 0;                                                        // 0049CF0A
    }
    field<U>(self, 0x26C) = 0;                                                            // 0049CF10
    field<U>(self, 0x268) = 0;                                                            // 0049CF16
    field<U8>(self, 0x28C) = 1;                                                           // 0049CF21
    void* const block = live_operator_new(0x1CC);                                         // 0049CF1C..0049CF28 (006FA7E6)
    aurora_model::Gob* const gob = block != nullptr
        ? gob_construct_00449440(static_cast<aurora_model::Gob*>(block), nullptr, "_AUR_lightningbranch") // 0049CF3C..0049CF43 (00743460)
        : nullptr;                                                                        // 0049CF4A
    safe_pointer_assign_00447fa0(at(self, 0x1E4), nullptr, gob);                          // 0049CF4C..0049CF5B
    const U b8 = field<U>(self, 0xB8);                                                    // 0049CF66
    const U f108 = field<U>(self, 0x108);                                                 // 0049CF60
    field<U>(self, 0xB0) = 0;                                                             // 0049CF6D
    field<U>(self, 0xAC) = 0;                                                             // 0049CF73
    field<U>(self, 0xFC) = 0;                                                             // 0049CF79
    field<U>(self, 0xF8) = f108;                                                          // 0049CF7F
    field<U>(self, 0xF4) = b8;                                                            // 0049CF89
}

// ---------------------------------------------------------------- 0049D480 LightningEmitter::Initialize, RET 0
U __fastcall lightning_emitter_initialize_0049d480(void* self, void*) {
    part_emitter_initialize_0049caa0(self, nullptr);                                      // 0049D49A..0049D49C
    void* const gob = field<void*>(self, 0x40);                                           // 0049D4A1
    field<U>(self, 0x78) = kOneBits;                                                      // 0049D4B1
    const U handle = gob_table(gob)->register_callback(as_gob(gob), nullptr, &set_emitter_target_0049b240,
        "_EmitterTarget", static_cast<aurora_model::Part*>(self), kRange_461c3c00, 0);   // 0049D4A4..0049D4BF (007413FC)
    field<U16>(self, 0x1F4) = static_cast<U16>(emitter_render_mode_00798b80);            // 0049D4C2..0049D4C9 (word [00798B80])
    field<U>(self, 0x1F0) = handle;                                                       // 0049D4D3
    void* const node = aurora_node_as_emitter_0043e3c0(field<void*>(self, 0x4), nullptr); // 0049D4D0..0049D4DE
    static const U kIndexLists[] = {0x258u, 0x264u};
    for (U offset : kIndexLists) {                                                   // 0049D4E0..0049D545 (two identical SetSize)
        const I count = field<I>(node, 0x5C);                                             // 0049D4E0 / 0049D516
        PointerList* const list = list_at(self, offset);
        if (count == 0) {                                                                 // 0049D4E3..0049D4EF / 0049D519..0049D521
            if (list->data != 0) {                                                        // 0049D4F1..0049D4F5
                live_free(as_object(static_cast<U>(list->data)));                         // 0049D4F7..0049D4F8
                list->data = 0;                                                           // 0049D500 mov [ebx],edi (edi = 0)
            }
            list->capacity = 0;                                                           // 0049D502 / 0049D534
        } else {
            menu_list_growth::allocate_005e03c0(list, nullptr, static_cast<U>(count));    // 0049D50B..0049D50E / 0049D53D..0049D540
        }
        list->size = count;                                                               // 0049D513 / 0049D545
    }
    U last = field<U>(node, 0x5C);                                                        // 0049D54C
    for (I i = 0; i < static_cast<I>(last); ) {                                           // 0049D54F..0049D553
        void* const block = live_operator_new(0x290);                                     // 0049D555..0049D55A
        void* const branch = block != nullptr
            ? aurora_lightning_emitter_construct_0049d1d0(block, nullptr, field<void*>(self, 0x4)) // 0049D572..0049D578
            : nullptr;                                                                    // 0049D57F
        *reinterpret_cast<volatile U*>(field<U>(self, 0x258) + static_cast<U>(i) * 4u) = word_of(branch); // 0049D581..0049D583
        lightning_emitter_initialize_branch_0049ce30(branch_at(self, i), nullptr);        // 0049D586..0049D593
        last = field<U>(node, 0x5C);                                                      // 0049D598
        ++i;                                                                              // 0049D59B..0049D59E
    }
    return last;   // EAX = last read of node[+5C] (0049D54C / 0049D598)
}

// ---------------------------------------------------------------- 00491A50 LightningEmitter::UpdateBranches, RET 0
// Locals named by their original frame offsets (esp after the four register pushes).
U __fastcall lightning_emitter_update_branches_00491a50(void* self, void*) {
    void* const node = aurora_node_as_emitter_0043e3c0(field<void*>(self, 0x4), nullptr); // 00491A5B..00491A64
    if (field<I>(node, 0x5C) == 0) return 0;                                              // 00491A66..00491A6B (EAX = 0)
    const I r_count = crt_rand_006fc447();                                                // 00491A71
    field<I>(self, 0x288) = r_count % (field<I>(node, 0x5C) + 1);                         // 00491A76..00491A85
    if (field<I>(self, 0x288) <= 0)                                                       // 00491A83..00491A8B
        return static_cast<U>(r_count / (field<I>(node, 0x5C) + 1));   // EAX = idiv quotient (void member; callers ignore it)
    I i = 0;                                                                              // 00491A7F [esp+24]
    do {
        const I r80 = rand_mod(0x50);                                                     // 00491A91..00491A9C
        const float scale = S(rand_int_as_double(r80) * W(F(kHundredth_0073dfec)) + W(F(kTenth_0073e798))); // 00491AA5..00491ABE [esp+10]
        field<float>(branch_at(self, i), 0x90) = S(W(scale) * W(field<float>(self, 0x90))); // 00491AA9..00491ACC
        field<float>(branch_at(self, i), 0xE8) = S(W(scale) * W(field<float>(self, 0xE8))); // 00491AD2..00491AE5
        field<float>(branch_at(self, i), 0x94) = S(W(scale) * W(field<float>(self, 0x94))); // 00491AEB..00491AFE
        field<float>(branch_at(self, i), 0x98) = S(W(scale) * W(field<float>(self, 0x98))); // 00491B04..00491B17
        field<float>(branch_at(self, i), 0xEC) = S(W(scale) * W(field<float>(self, 0xEC))); // 00491B1D..00491B30
        field<float>(branch_at(self, i), 0x9C) = S(W(scale) * W(field<float>(self, 0x9C))); // 00491B36..00491B49
        *reinterpret_cast<volatile I*>(field<U>(self, 0x264) + static_cast<U>(i) * 4u) =
            ftol2(rand_int_as_double(field<I>(self, 0x1CC)) * W(scale));                 // 00491B4F..00491B64
        {
            const Vector source = *particle_position(particle_at(self, target_at(self, i))); // 00491B67..00491B88
            vec_data(branch_at(self, i), 0x210)[0] = source;                             // 00491B8D..00491B9A
            field<Vector>(branch_at(self, i), 0x164) = source;                           // 00491B9D..00491BB9
        }
        // [esp+7C] = this[+110] * this[+27C..+284] (00491BBC..00491BE7)
        Vector v7c;
        const double length = W(field<float>(self, 0x110));
        v7c.x = S(length * W(field<float>(self, 0x27C)));
        v7c.y = S(length * W(field<float>(self, 0x280)));
        v7c.z = S(length * W(field<float>(self, 0x284)));
        const I r360 = rand_mod(0x168);                                                   // 00491BEF..00491BFA
        const float roll = S(rand_int_as_double(r360));                                   // 00491C01..00491C05
        const I r45 = rand_mod(0x2D);                                                     // 00491C0A..00491C15
        const float yaw = S(rand_int_as_double(-r45));                                    // 00491C18..00491C29 (neg; fild)
        Quaternion q140;
        Quaternion* const q = yaw_pitch_roll_004acac0(&q140, yaw, 0.0f, roll);           // 00491C08 push 0 / 00491C2C..00491C32
        Vector v12c;
        const Vector rotated = *quaternion_rotate_004a9bb0(&v12c, q, &v7c);               // 00491C35..00491C3E
        vec_data(branch_at(self, i), 0x234)[0] = rotated;                                 // 00491C47..00491C69
        if (W(scale) > W(F(kHalf_0073e9ac))) {                                            // 00491C43..00491C74 (fcomp; test ah,41h; jne)
            // ---- long branch: anchored at this emitter's gob (00491C7A..00491DA8)
            void* const branch_gob = field<void*>(branch_at(self, i), 0x1E4);             // 00491C7A..00491C83
            Vector vd0;
            const Vector own = *gob_get_position(field<void*>(self, 0x1E4), &vd0);        // 00491C89..00491C99
            Vector vc4;
            gob_set_position(branch_gob, &vc4, own);                                      // 00491C9C..00491CBD
            Vector ve8;
            const Vector own2 = *gob_get_position(field<void*>(self, 0x1E4), &ve8);       // 00491CC0..00491CD0
            vec_data(branch_at(self, i), 0x210)[1] = own2;                                // 00491CD3..00491CF2
            field<U>(branch_at(self, i), 0xFC) = field<U>(self, 0xFC);                    // 00491CF5..00491D04
            const Vector* const target = particle_position(particle_at(self, target_at(self, i))); // 00491D0A..00491D2C
            Vector v118;
            const Vector* const own3 = gob_get_position(field<void*>(self, 0x1E4), &v118); // 00491D1C..00491D2F
            Vector v70;
            const double dz = W(own3->z) - W(target->z);                                 // 00491D32..00491D35
            const double dy = W(own3->y) - W(target->y);                                 // 00491D38..00491D3B
            v70.x = S(W(own3->x) - W(target->x));                                         // 00491D3E..00491D47
            v70.y = S(dy);                                                                // 00491D4B
            v70.z = S(dz);                                                                // 00491D4F
            const double magnitude = vector_magnitude_004ab100(&v70);                     // 00491D53
            field<float>(branch_at(self, i), 0x58) =
                S(magnitude * W(field<float>(self, 0xC4)) + W(F(kTwo_00741be0)));          // 00491D58..00491D70
            const Vector last = vec_data(self, 0x234)[field<I>(self, 0x238) - 1];         // 00491D73..00491D82
            vec_data(branch_at(self, i), 0x234)[1] = last;                                // 00491D86..00491DA5
        } else {
            // ---- short branch: between two particles of this emitter (00491DAD..00492121)
            void* const first = particle_at(self, 0);                                     // 00491DAD..00491DB3
            void* const last_particle = particle_at(self, field<I>(self, 0x1CC) - 1);             // 00491DB5..00491DBB
            Vector v28;
            const double ez = W(field<float>(last_particle, 0x10)) - W(field<float>(first, 0x10)); // 00491DBF..00491DC5
            const double ey = W(field<float>(last_particle, 0x0C)) - W(field<float>(first, 0x0C)); // 00491DCB..00491DCE
            v28.x = S(W(field<float>(last_particle, 0x08)) - W(field<float>(first, 0x08)));       // 00491DD1..00491DDA
            v28.y = S(ey);                                                                // 00491DDE
            v28.z = S(ez);                                                                // 00491DE2
            const float half = S(vector_magnitude_004ab100(&v28) * W(F(kHalf_0073e9ac))); // 00491DE6..00491DF4 [esp+10]
            const I r75 = rand_mod(0x4B);                                                 // 00491DF8..00491E03
            const double t = (rand_int_as_double(r75) * W(F(kHundredth_0073dfec)) + W(F(kQuarter_0073edf0))) * W(half); // 00491E09..00491E19 (ST0)
            if (t == W(F(kZero_0073d700))) {                                              // 00491E1D..00491E28 (fcom; test ah,44h; jnp: equal only)
                // 00491FDC..00492031
                field<U>(branch_at(self, i), 0x58) = 0;                                   // 00491FE7
                void* const branch_gob = field<void*>(branch_at(self, i), 0x1E4);         // 00491FEE..00492009
                const Vector target = *particle_position(particle_at(self, target_at(self, i))); // 00491FF7..00492026
                Vector vc4;
                gob_set_position(branch_gob, &vc4, target);                               // 00492029..00492031
            } else {
                // 00491E2E..00491FDA
                Vector va0;
                va0.x = S(t * W(field<float>(self, 0x27C)));                              // 00491E2E..00491E40
                va0.y = S(t * W(field<float>(self, 0x280)));                              // 00491E47..00491E4F
                va0.z = S(t * W(field<float>(self, 0x284)));                              // 00491E56..00491E5C
                const I r360b = rand_mod(0x168);                                          // 00491E63..00491E6E
                const float pitch = S(rand_int_as_double(r360b));                         // 00491E71..00491E80
                Quaternion q13c;
                Quaternion* const qb = yaw_pitch_roll_004acac0(&q13c, 0.0f, pitch, 0.0f); // 00491E3E/00491E83..00491E8B
                Vector v64;
                quaternion_rotate_004a9bb0(&v64, qb, &va0);                               // 00491E8E..00491E94 (result read from v64)
                const I r25 = rand_mod(0x19);                                             // 00491E99..00491EA4
                const double f = rand_int_as_double(r25) * W(F(kHundredth_0073dfec)) + W(F(kQuarter_0073edf0)); // 00491EAA..00491ECE
                const Vector* const base = vec_data(branch_at(self, i), 0x210);           // 00491EAE..00491EB7
                void* const branch_gob = field<void*>(branch_at(self, i), 0x1E4);         // 00491EC3..00491EC8
                const double xsum = W(v28.x) * f + W(base[0].x);                          // 00491ED6..00491EF8 (wide)
                const float yf = S(W(v28.y) * f);                                         // 00491EDC..00491EE2 [esp+8C]
                const float zf = S(W(v28.z) * f);                                         // 00491EE9..00491EEF [esp+90]
                const double ysum = W(yf) + W(base[0].y);                                 // 00491EFA..00491F01 (wide)
                const float zsum = S(W(zf) + W(base[0].z));                               // 00491F04..00491F10 [esp+B4]
                Vector position;
                position.x = S(W(v64.x) + xsum);                                          // 00491F17..00491F29 [esp+18]
                position.y = S(W(v64.y) + ysum);                                          // 00491F25..00491F2D [esp+1C]
                position.z = S(W(v64.z) + W(zsum));                                       // 00491F35..00491F40 [esp+20]
                Vector v100;
                gob_set_position(branch_gob, &v100, position);                            // 00491F44..00491F5A
                const Vector* const target = particle_position(particle_at(self, target_at(self, i))); // 00491F61..00491F80
                Vector v58;
                const double gz = W(position.z) - W(target->z);                           // 00491F5D..00491F73
                const double gy = W(position.y) - W(target->y);                           // 00491F7C..00491F83
                v58.x = S(W(position.x) - W(target->x));                                  // 00491F8A..00491F93
                v58.y = S(gy);                                                            // 00491F97
                v58.z = S(gz);                                                            // 00491F9B
                field<float>(branch_at(self, i), 0x58) =
                    S(vector_magnitude_004ab100(&v58) * W(field<float>(self, 0xC4)) + W(F(kTwo_00741be0))); // 00491F9F..00491FB5
                Vector* const rays = vec_data(branch_at(self, i), 0x234);                 // 00491FB8..00491FC1
                rays[1] = rays[0];                                                        // 00491FC7..00491FD7
            }
            // 00492034..00492121 (both short-branch arms)
            field<U>(branch_at(self, i), 0xFC) = 0;                                       // 0049203D
            Vector vdc;
            const Vector bpos = *gob_get_position(field<void*>(branch_at(self, i), 0x1E4), &vdc); // 00492047..00492060
            vec_data(branch_at(self, i), 0x210)[1] = bpos;                                // 00492063..00492082
            const Vector* const ends = vec_data(branch_at(self, i), 0x210);               // 00492085..0049208E
            Vector v94;
            const double nz = W(ends[1].z) - W(ends[1].z);                                // 00492094..0049209E (the original subtracts +14 from +14)
            const double ny = W(ends[1].y) - W(ends[1].y);                                // 004920A1..004920A4
            v94.x = S(W(ends[1].x) - W(ends[1].x));                                       // 004920A7..004920B6
            v94.y = S(ny);                                                                // 004920BD
            v94.z = S(nz);                                                                // 004920C4
            Vector vf4;
            const Vector n = *normalize_004ab180(&vf4, &v94);                             // 004920CB..004920DC
            Vector* const axis = static_cast<Vector*>(at(branch_at(self, i), 0x27C));     // 004920E4..0049210E
            axis->x = S(-W(n.y));                                                         // 004920E0..004920EE / 00492114
            axis->y = n.z;                                                                // 0049211E (raw dword)
            axis->z = S(-W(n.x));                                                         // 004920F2..004920FD / 00492116..00492121
        }
        // 00492124..004921A8 (all arms)
        void* const branch = branch_at(self, i);                                          // 00492124..0049212A
        Vector v100b;
        const Vector* const now = gob_get_position(field<void*>(branch, 0x1E4), &v100b);  // 0049212D..0049213D
        Vector* const delta = static_cast<Vector*>(at(branch_at(self, i), 0x1F8));        // 00492148..00492151
        const float dx = S(W(now->x) - W(field<float>(branch, 0x164)));                   // 00492140..00492157
        const float dy = S(W(now->y) - W(field<float>(branch, 0x168)));                   // 0049215B..00492164
        const float dz = S(W(now->z) - W(field<float>(branch, 0x16C)));                   // 00492168..0049217E
        delta->x = dx; delta->y = dy; delta->z = dz;                                      // 00492179..00492186
        void* const again = branch_at(self, i);                                           // 00492189..0049218F
        field<Vector>(again, 0x204) = field<Vector>(again, 0x164);                        // 00492192..004921AB
        ++i;                                                                              // 004921AE..004921BB
    } while (i < field<I>(self, 0x288));
    return static_cast<U>(field<I>(self, 0x288));   // EAX = [esi+288] (004921B2)
}


// ---------------------------------------------------------------- 00497C60 LightningEmitter::UpdateControlPoints, RET 0
// Lists (CExoArrayList<Vector>, 12-byte elements): +210 points P, +21C points A (copy target), +228 jittered
// points, +234 tangents T0, +240 tangents (copy target), +24C jittered tangents. Locals carry their original
// frame offsets (esp after the four register pushes). Quaternions are {w,x,y,z} at +0/+4/+8/+C.
} // extern "C"
namespace {
// zdir-shaped expressions computed inline by the original (2(wy+xz), 2(zy-wx), 1-2(y^2+x^2)); the operand
// order of each x87 product is kept (multiplication is commutative, sums keep their original pairing).
struct WideVector { double x, y, z; };
inline Vector* data_vec(void* self, U list_offset, I index) { return &vec_data(self, list_offset)[index]; }
inline Quaternion* gob_get_orientation(void* gob, Quaternion* out) {
    return static_cast<Quaternion*>(as_object(gob_table(gob)->s068_gob_get_orientation(as_gob(gob), nullptr, word_of(out))));
}
} // namespace
extern "C" {

U __fastcall lightning_emitter_update_control_points_00497c60(void* self, void*) {
    const I jitter_e0 = ftol2(W(field<float>(self, 0x104)) * W(F(kHundred_00741838)));   // 00497C6B..00497C89 [esp+E0]
    const I jitter_34 = ftol2(W(field<float>(self, 0x10C)) * W(F(kHundred_00741838)));   // 00497C7D..00497CA2 [esp+34]
    Vector vbc;
    gob_get_position(field<void*>(self, 0x1E4), &vbc);                                   // 00497C95..00497CA9 (result read from [esp+BC])
    Vector v38;
    v38.x = S(W(vbc.x) - W(field<float>(self, 0x164)));                                  // 00497CAC..00497CBE
    v38.y = S(W(vbc.y) - W(field<float>(self, 0x168)));                                  // 00497CC2..00497CCF
    v38.z = S(W(vbc.z) - W(field<float>(self, 0x16C)));                                  // 00497CD3..00497CE0
    const I n = ftol2(vector_magnitude_004ab100(&v38) * W(field<float>(self, 0x100)) + W(F(kHalf_0073e9ac))) + 2; // 00497CE4..00497D05
    I ebp_bytes = 0;   // n*12, set on both resize paths (00497D53 / 00497DAD) and read at 00497F46 only on them
    if (n != field<I>(self, 0x214)) {                                                    // 00497CFF..00497D0A
        PointerList* const points = list_at(self, 0x210);                                // 00497D10
        const I count = field<I>(self, 0x214);
        if (n > count) {                                                                 // 00497D16 (jle not taken)
            const Vector last = vec_data(self, 0x210)[count - 1];                        // 00497D1C..00497D38 [esp+5C]
            vector_list_set_size_004213a0(points, nullptr, n);                           // 00497D31..00497D3C
            vector_list_set_size_004213a0(list_at(self, 0x234), nullptr, n);             // 00497D41..00497D48
            ebp_bytes = n * 12;                                                          // 00497D53..00497D56
            vec_data(self, 0x210)[n - 1] = last;                                         // 00497D4D..00497D71
            vector_list_set_size_004213a0(list_at(self, 0x21C), nullptr, n);             // 00497D6A..00497D74
            vector_list_set_size_004213a0(list_at(self, 0x228), nullptr, n);             // 00497D79..00497D80
            vector_list_set_size_004213a0(list_at(self, 0x240), nullptr, n);             // 00497D85..00497D8C
            vector_list_set_size_004213a0(list_at(self, 0x24C), nullptr, n);             // 00497D91..00497D98
        } else {
            ebp_bytes = n * 12;                                                          // 00497DAD..00497DB0
            Vector* const data = vec_data(self, 0x210);                                  // 00497DA2
            data[n - 1] = data[count - 1];                                               // 00497DA4..00497DC2
            if (n < field<I>(self, 0x214)) {                                             // 00497DC5..00497DCD
                do {                                                                     // 00497DD0..00497E40
                    gui_load_list12_delete_at_0048dde0(points, nullptr, field<I>(self, 0x214) - 1);
                    gui_load_list12_delete_at_0048dde0(list_at(self, 0x21C), nullptr, field<I>(self, 0x220) - 1);
                    gui_load_list12_delete_at_0048dde0(list_at(self, 0x228), nullptr, field<I>(self, 0x22C) - 1);
                    gui_load_list12_delete_at_0048dde0(list_at(self, 0x234), nullptr, field<I>(self, 0x238) - 1);
                    gui_load_list12_delete_at_0048dde0(list_at(self, 0x240), nullptr, field<I>(self, 0x244) - 1);
                    gui_load_list12_delete_at_0048dde0(list_at(self, 0x24C), nullptr, field<I>(self, 0x250) - 1);
                } while (n < field<I>(self, 0x214));
            }
        }
        // ---- start tangent from this emitter's quaternion (+14 w, +18 x, +1C y, +20 z) (00497E42..00497EC7)
        Vector* const t0 = vec_data(self, 0x234);                                        // 00497E45
        const double qw = W(field<float>(self, 0x14)), qx = W(field<float>(self, 0x18));
        const double qy = W(field<float>(self, 0x1C)), qz = W(field<float>(self, 0x20));
        const float a5c = S((qw * qy + qx * qz) * 2.0);                                  // 00497E42..00497E58 (fadd st0,st0)
        const float a60 = S((qz * qy - qw * qx) * 2.0);                                  // 00497E5C..00497E6C
        const float a64 = S(W(F(kOne_0073d764)) - (qy * qy + qx * qx) * 2.0);             // 00497E70..00497E88 (fsubr 1.0)
        const double length = W(field<float>(self, 0x110));                              // 00497E90
        t0->x = S(W(a5c) * length);                                                      // 00497E96..00497EA8
        t0->y = S(W(a60) * length);                                                      // 00497EA4..00497EBA
        t0->z = S(W(a64) * length);                                                      // 00497EB0..00497EC7
        // ---- end tangent along (gob position - +164) (00497ECA..00497F57)
        const U length_bits = field<U>(self, 0x110);                                     // 00497ED1..00497EF5 [esp+14]
        Vector v44;
        v44.x = S(W(vbc.x) - W(field<float>(self, 0x164)));                              // 00497ECA..00497EE9
        v44.y = S(W(vbc.y) - W(field<float>(self, 0x168)));                              // 00497EEE..00497EFF
        v44.z = S(W(vbc.z) - W(field<float>(self, 0x16C)));                              // 00497F03..00497F10
        Vector vb0;
        const Vector* const dir = normalize_004ab180(&vb0, &v44);                        // 00497F14
        Vector v50;
        v50.x = S(W(F(length_bits)) * W(dir->x));                                        // 00497F19..00497F22
        v50.y = S(W(F(length_bits)) * W(dir->y));                                        // 00497F26..00497F2D
        v50.z = S(W(F(length_bits)) * W(dir->z));                                        // 00497F31..00497F38
        *reinterpret_cast<Vector*>(field<U>(self, 0x234) + static_cast<U>(ebp_bytes) - 12u) = v50; // 00497F3C..00497F57
        // ---- inner points: cubic Bezier P0, P0+L*a, P3+L*zdir(gob), P3 (00497F5A..004984E9)
        I i = 1;                                                                         // 00497F60 ebp
        I m = field<I>(self, 0x214) - 1;                                                 // 00497F5A..00497F8F [esp+18]
        // [esp+8C..94] = 0 (00497F6A..00497F80): overwritten in every iteration before its read.
        U ebx_bytes = 0x0C;                                                              // 00497F99
        while (i < m) {                                                                  // 00497F93 / 004984E3..004984E9
            float t28 = S(rand_int_as_double(i) / rand_int_as_double(m));                 // 00497FA0..00497FA8 (fild/fidiv, fst)
            if (rand_int_as_double(i) / rand_int_as_double(m) > W(F(kOne_0073d764)))     // 00497FAC..00497FB7 (wide ST compare)
                t28 = F(kOne_0073d764);                                                  // 00497FB9
            const double u = W(F(kOne_0073d764)) - W(t28);                               // 00497FC1..00497FC9
            const float u14 = S(u);                                                      // 00497FDA fst [esp+14]
            const double uu = u * W(u14);                                                // 00497FE0
            const float u20 = S(uu);                                                     // 00497FE4 fst [esp+20]
            const float u3 = S(uu * W(u14));                                             // 00497FE8..00497FEC [esp+10]
            const float b1 = S(W(u20) * W(t28) * W(F(kThree_00740c84)));                 // 00497FF0..00497FFE [esp+2C]
            const float b2 = S(W(u14) * W(t28) * W(t28) * W(F(kThree_00740c84)));        // 00498002..00498014 [esp+30]
            const double tt = W(t28) * W(t28);                                           // 00498018..0049801C
            const float t18 = S(tt);                                                     // 00498020 fst [esp+18]
            const double ttt = tt * W(t28);                                              // 00498024
            const Vector p3 = vec_data(self, 0x210)[field<I>(self, 0x214) - 1];          // 00497FC7..00497FD6
            Vector vd4;
            vd4.x = S(ttt * W(p3.x));                                                    // 00498028..0049802C
            vd4.y = S(ttt * W(p3.y));                                                    // 00498033..00498038
            vd4.z = S(ttt * W(p3.z));                                                    // 0049803F..0049804A
            Quaternion o18c;
            const Quaternion* o = gob_get_orientation(field<void*>(self, 0x1E4), &o18c);  // 00497FCD / 00498042..00498051
            const U length1 = field<U>(self, 0x110);                                     // 0049805A..00498063 [esp+1C]
            {
                const double c1 = (W(o->x) * W(o->z) + W(o->y) * W(o->w)) * 2.0;          // 00498054..00498074
                const double c2 = (W(o->z) * W(o->y) - W(o->x) * W(o->w)) * 2.0;          // 00498076..00498083
                const float c3 = S(W(F(kOne_0073d764)) - (W(o->y) * W(o->y) + W(o->x) * W(o->x)) * 2.0); // 00498085..004980A3 [esp+140]
                const Vector p3b = vec_data(self, 0x210)[field<I>(self, 0x214) - 1];     // 00498069..00498091
                const Vector* const p0 = vec_data(self, 0x210);                          // 0049808B
                const float v15c = S(c1 * W(F(length1)));                                // 004980AE..004980B4
                const double c2l = c2 * W(F(length1));                                   // 004980BB
                const double c3l = W(c3) * W(F(length1));                                // 004980BF..004980C6
                const float v108 = S(W(v15c) + W(p3b.x));                                // 004980CA..004980D3
                const float v10c = S(c2l + W(p3b.y));                                    // 004980DA..004980DF
                const double s3 = c3l + W(p3b.z);                                        // 004980E6
                const double xb2 = W(v108) * W(b2);                                      // 004980E9..004980F0 (wide)
                const float v9c = S(W(v10c) * W(b2));                                    // 004980F4..004980FF
                const float va0 = S(s3 * W(b2));                                         // 00498106..0049810C
                const double l = W(field<float>(self, 0x110));                           // 00498113
                const double s0x = W(a5c) * l + W(p0->x);                                // 00498119..0049813B (wide)
                const float v118 = S(W(a60) * l);                                        // 0049811F..00498125
                const float v11c = S(W(a64) * l);                                        // 0049812C..00498132
                const double s0y = W(v118) + W(p0->y);                                   // 0049813D..00498144 (wide)
                const float v188 = S(W(v11c) + W(p0->z));                                // 00498147..00498151
                const float v144 = S(s0x * W(b1));                                       // 00498158..0049815E
                const double s0yb1 = s0y * W(b1);                                        // 00498165
                const double s0zb1 = W(v188) * W(b1);                                    // 00498169..00498170
                const double u3x = W(u3) * W(p0->x);                                     // 00498174..00498178
                const double u3y = W(u3) * W(p0->y);                                     // 0049817A..0049817E
                const float v158 = S(W(u3) * W(p0->z));                                  // 00498181..00498188
                const float b50 = S(u3x + W(v144));                                      // 0049818F..00498198
                const float v54 = S(u3y + s0yb1);                                        // 0049819C..0049819E
                const float v58 = S(W(v158) + s0zb1);                                    // 004981A2..004981AB
                const double xs = xb2 + W(b50);                                          // 004981B3
                const double ys = W(v54) + W(v9c);                                       // 004981B7..004981BB
                const float v170 = S(W(v58) + W(va0));                                   // 004981C2..004981CD
                Vector* const out = reinterpret_cast<Vector*>(field<U>(self, 0x210) + ebx_bytes); // 00498247 / 0049820E (eax = [edi] + ebx)
                const float ve4 = S(xs + W(vd4.x));                                      // 004981D4..004981DD
                const float ve8 = S(ys + W(vd4.y));                                      // 004981E4..004981EB
                const float vec_ = S(W(v170) + W(vd4.z));                                // 004981F9..00498215
                out->x = ve4; out->y = ve8; out->z = vec_;                               // 00498210..0049822D
            }
            // ---- tangent at t (0049821C..004984D5)
            const float m10 = S(W(u20) * W(F(kMinusThree_00743334)));                    // 0049821C..0049823C [esp+10]
            const double three_t = W(t28) * W(F(kThree_00740c84));                       // 00498243..00498249
            const float c2c = S((W(F(kOne_0073d764)) - three_t) * W(u14) * W(F(kThree_00740c84))); // 00498253..00498267 [esp+2C]
            const float c30 = S((W(F(kTwo_00741be0)) - three_t) * W(t28) * W(F(kThree_00740c84))); // 0049826B..0049827D [esp+30]
            const Vector p3t = vec_data(self, 0x210)[field<I>(self, 0x214) - 1];         // 00498230..0049824F
            const double tt3 = W(t18) * W(F(kThree_00740c84));                           // 00498283..00498287
            Vector v74;
            v74.x = S(tt3 * W(p3t.x));                                                   // 0049828D..00498291
            v74.y = S(tt3 * W(p3t.y));                                                   // 00498295..0049829A
            v74.z = S(tt3 * W(p3t.z));                                                   // 0049829E..004982A9
            Quaternion o19c;
            o = gob_get_orientation(field<void*>(self, 0x1E4), &o19c);                   // 00498236 / 004982A1..004982B0
            const U length2 = field<U>(self, 0x110);                                     // 004982B8 / 004982FE [esp+24]
            const double d1 = (W(o->w) * W(o->y) + W(o->x) * W(o->z)) * 2.0;              // 004982B3..004982C6
            const double d2 = (W(o->z) * W(o->y) - W(o->w) * W(o->x)) * 2.0;              // 004982C8..004982D5
            const float d3 = S(W(F(kOne_0073d764)) - (W(o->y) * W(o->y) + W(o->x) * W(o->x)) * 2.0); // 004982D7..004982EF [esp+17C]
            const Vector* const p0t = vec_data(self, 0x210);                             // 004982FA
            const Vector p3u = vec_data(self, 0x210)[field<I>(self, 0x214) - 1];         // 00498306..0049830F
            const float v120 = S(d1 * W(F(length2)));                                    // 004982FC..00498313
            const double d2l = d2 * W(F(length2));                                       // 0049831A
            const double d3l = W(d3) * W(F(length2));                                    // 0049831E..00498325
            const float va4 = S(W(v120) + W(p3u.x));                                     // 00498329..00498332
            const float va8 = S(d2l + W(p3u.y));                                         // 00498339..0049833E
            const double s3t = d3l + W(p3u.z);                                           // 00498345
            const double xa = W(va4) * W(c30);                                           // 00498348..0049834F (wide)
            const float v84 = S(W(va8) * W(c30));                                        // 00498353..0049835E
            const float v88 = S(s3t * W(c30));                                           // 00498365..0049836B
            const double lt = W(field<float>(self, 0x110));                              // 00498372
            const double s0xt = W(a5c) * lt + W(p0t->x);                                 // 00498378..00498394 (wide)
            const float v6c = S(W(a60) * lt);                                            // 0049837E..00498384
            const float v70 = S(W(a64) * lt);                                            // 00498388..0049838E
            const double s0yt = W(v6c) + W(p0t->y);                                      // 00498396..0049839A (wide)
            const float v134 = S(W(v70) + W(p0t->z));                                    // 0049839D..004983A4
            const float vf0 = S(s0xt * W(c2c));                                          // 004983AB..004983B1
            const double s0ytc = s0yt * W(c2c);                                          // 004983B8
            const double s0ztc = W(v134) * W(c2c);                                       // 004983BC..004983C3
            const double mx = W(m10) * W(p0t->x);                                        // 004983C7..004983CB
            const double my = W(m10) * W(p0t->y);                                        // 004983CD..004983D1
            const float v104 = S(W(m10) * W(p0t->z));                                    // 004983D4..004983DB
            const float vc8 = S(mx + W(vf0));                                            // 004983E2..004983EB
            const float vcc = S(my + s0ytc);                                             // 004983F2..004983F4
            const float vd0 = S(W(v104) + s0ztc);                                        // 004983FB..00498404
            const double xs2 = xa + W(vc8);                                              // 0049840F
            const double ys2 = W(vcc) + W(v84);                                          // 00498416..0049841D
            const float vb8 = S(W(vd0) + W(v88));                                        // 00498424..00498432
            Vector v8c;
            v8c.x = S(xs2 + W(v74.x));                                                   // 00498439..00498447
            v8c.y = S(ys2 + W(v74.y));                                                   // 0049844E..00498465
            v8c.z = S(W(vb8) + W(v74.z));                                                // 00498456..0049847B
            vector_normalize_004ab130(&v8c, nullptr);                                    // 0049846C..00498482
            const double ln = W(field<float>(self, 0x110));                              // 00498487
            Vector* const tangent = reinterpret_cast<Vector*>(field<U>(self, 0x234) + ebx_bytes); // 00498494..0049849C
            const float tx = S(W(v8c.x) * ln);                                           // 0049848D..004984A2
            const float ty = S(W(v8c.y) * ln);                                           // 004984AA..004984B7
            const float tz = S(W(v8c.z) * ln);                                           // 004984BB..004984CD
            tangent->x = tx; tangent->y = ty; tangent->z = tz;                           // 004984C8..004984D5
            ++i;                                                                         // 0049849E
            ebx_bytes += 0x0C;                                                           // 0049849F
            m = field<I>(self, 0x214) - 1;                                               // 004984DA..004984E5
        }
    }
    // ---- per-point jitter and copies (004984EF..0049890C)
    for (I k = 0; k < field<I>(self, 0x214); ++k) {                                      // 004984EF..004984F9 / 00498903..0049890C
        *data_vec(self, 0x21C, k) = *data_vec(self, 0x228, k);                           // 00498500..00498524
        *data_vec(self, 0x240, k) = *data_vec(self, 0x24C, k);                           // 00498527..00498546
        if (k != 0 && k != field<I>(self, 0x214) - 1) {                                  // 00498539..00498558
            if (jitter_e0 != 0) {                                                        // 0049855E..00498567
                const I r = rand_mod(jitter_e0);                                         // 0049856D..00498573
                const float magnitude = S(rand_int_as_double(r) * W(F(kHundredth_0073dfec))); // 00498579..00498583 [esp+10]
                double v = W(magnitude);                                                 // 0049858E
                if ((crt_rand_006fc447() & 1) != 0) v = -v;                              // 00498587..00498594 (fchs)
                const Vector* const point = data_vec(self, 0x210, k);                    // 00498596..004985B5
                Vector v80;
                v80.x = S(v * W(field<float>(self, 0x27C)));                             // 0049859C..004985AE
                v80.y = S(v * W(field<float>(self, 0x280)));                             // 004985B7..004985BF
                v80.z = S(v * W(field<float>(self, 0x284)));                             // 004985C6..004985CC
                const I r360 = rand_mod(0x168);                                          // 004985D3..004985DE
                const float pitch = S(rand_int_as_double(r360));                         // 004985E1..004985F0
                Quaternion q19c;
                Quaternion* const q = yaw_pitch_roll_004acac0(&q19c, 0.0f, pitch, 0.0f); // 004985AC / 004985F3..004985FB
                Vector vfc;
                const Vector* const offset = quaternion_rotate_004a9bb0(&vfc, q, &v80);  // 004985FE..00498607
                Vector* const jittered = data_vec(self, 0x228, k);                       // 00498610..00498619
                const float jx = S(W(point->x) + W(offset->x));                          // 0049860C..0049861B
                const float jy = S(W(point->y) + W(offset->y));                          // 00498623..00498629
                const float jz = S(W(point->z) + W(offset->z));                          // 0049862D..0049863C
                jittered->x = jx; jittered->y = jy; jittered->z = jz;                    // 00498637..00498644
            } else {
                *data_vec(self, 0x228, k) = *data_vec(self, 0x210, k);                   // 00498649..00498666
            }
            if (jitter_34 != 0) {                                                        // 00498669..0049866F
                float yaw = S(rand_int_as_double(rand_mod(jitter_34)) * W(F(kHundredth_0073dfec))); // 00498675..0049868B [esp+10]
                if ((crt_rand_006fc447() & 1) != 0) yaw = S(-W(yaw));                    // 0049868F..0049869E
                float pitch = S(rand_int_as_double(rand_mod(jitter_34)) * W(F(kHundredth_0073dfec))); // 004986A2..004986B8 [esp+24]
                if ((crt_rand_006fc447() & 1) != 0) pitch = S(-W(pitch));                // 004986BC..004986CB
                const Vector* const tangent = data_vec(self, 0x234, k);                  // 004986CF..004986E5
                Vector* const target = data_vec(self, 0x24C, k);                         // 004986DD..004986F2
                Quaternion q18c;
                Quaternion* const q = yaw_pitch_roll_004acac0(&q18c, yaw, pitch, 0.0f);  // 004986E6..004986F9
                Vector vf0;
                *target = *quaternion_rotate_004a9bb0(&vf0, q, tangent);                 // 004986FC..0049871A
            } else {
                *data_vec(self, 0x24C, k) = *data_vec(self, 0x234, k);                   // 0049871F..0049873C
            }
            if (field<U8>(self, 0x28C) != 0)                                             // 0049873F..00498747
                *data_vec(self, 0x21C, k) = *data_vec(self, 0x228, k);                   // 0049874D..0049876A
            continue;
        }
        *data_vec(self, 0x228, k) = *data_vec(self, 0x210, k);                           // 00498772..00498791
        if (k == 0) {                                                                    // 00498784..00498794
            // start tangent from the +1A4 quaternion (w +1A4, x +1A8, y +1AC, z +1B0) (0049879A..0049883D)
            const U length = field<U>(self, 0x110);                                      // 004987A0..004987B8 [esp+20]
            const double w = W(field<float>(self, 0x1A4)), x = W(field<float>(self, 0x1A8));
            const double y = W(field<float>(self, 0x1AC)), z = W(field<float>(self, 0x1B0));
            const double c1 = (w * y + x * z) * 2.0;                                     // 0049879A..004987C4
            const double c2 = (z * y - x * w) * 2.0;                                     // 004987C6..004987E0
            const float c3 = S(W(F(kOne_0073d764)) - (y * y + x * x) * 2.0);             // 004987E2..00498800 [esp+B8]
            Vector* const tangent = data_vec(self, 0x24C, 0);                            // 004987AC
            tangent->x = S(c1 * W(F(length)));                                           // 0049880B..00498819
            tangent->y = S(c2 * W(F(length)));                                           // 0049881B..00498832
            tangent->z = S(W(c3) * W(F(length)));                                        // 00498823..0049883D
        } else {
            // end tangent along (gob position - +164) (00498845..004988D6)
            Vector v12c;
            const Vector* const position = gob_get_position(field<void*>(self, 0x1E4), &v12c); // 00498845..00498855
            Vector v68;
            const double dz = W(position->z) - W(field<float>(self, 0x16C));             // 00498858..0049885B
            const double dy = W(position->y) - W(field<float>(self, 0x168));             // 00498867..0049886E
            v68.x = S(W(position->x) - W(field<float>(self, 0x164)));                    // 00498875..00498889
            v68.y = S(dy);                                                               // 0049888D
            v68.z = S(dz);                                                               // 00498891
            const U length = field<U>(self, 0x110);                                      // 00498861 / 00498885 [esp+1C]
            Vector v120;
            const Vector* const dir = normalize_004ab180(&v120, &v68);                   // 00498895
            Vector* const tangent = data_vec(self, 0x24C, k);                            // 0049889E..004988A9
            const float tx = S(W(F(length)) * W(dir->x));                                // 0049889A..004988AB
            const float ty = S(W(F(length)) * W(dir->y));                                // 004988B3..004988BA
            const float tz = S(W(F(length)) * W(dir->z));                                // 004988BE..004988CE
            tangent->x = tx; tangent->y = ty; tangent->z = tz;                           // 004988C9..004988D6
        }
        if (field<U8>(self, 0x28C) != 0)                                                 // 004988D9..004988E1
            *data_vec(self, 0x21C, k) = *data_vec(self, 0x228, k);                       // 004988E3..00498900
    }
    // ---- place the particles along the Hermite-like segments of +21C / +240 (00498912..00498B6C)
    const double step = W(F(kOne_0073d764)) / rand_int_as_double(field<I>(self, 0x220) - 1); // 00498912..00498930 (ST1 "s")
    double s = step;
    I j = 1;                                                                             // 00498927 edx
    float start = 0.0f;                                                                  // 00498936 [esp+14]
    I p = 1;                                                                             // 0049892E edi
    U last_count = field<U>(self, 0x1CC);                                                // 00498918
    while (p < static_cast<I>(last_count)) {                                             // 0049892C..00498942 / 00498B64..00498B6C
        const double tp = rand_int_as_double(p) / rand_int_as_double(static_cast<I>(last_count) - 1); // 00498950..00498959
        if (tp > s) {                                                                    // 0049895D..00498964 (fcom st(1); test ah,41h; jne)
            const I segments = field<I>(self, 0x220);                                    // 00498966
            const double d = W(F(kOne_0073d764)) / rand_int_as_double(segments - 1);     // 0049896C..00498977
            for (;;) {                                                                   // 00498980..0049899A
                ++j;                                                                     // 00498982
                start = S(s);                                                            // 00498980 / 00498985
                s = s + d;                                                               // 00498989..0049898D
                if (j >= segments) {                                                     // 00498983 / 0049898F
                    --j;                                                                 // 004989A4
                    start = S(W(start) - d);                                             // 004989A0..004989A7
                    s = s - d;                                                           // 004989AB (fsubp st(2),st)
                    break;
                }
                if (!(tp > s)) break;                                                    // 00498991..0049899E (fcomp st(3); test ah,41h; je)
            }
        }
        double f = (tp - W(start)) / (s - W(start));                                     // 004989AD..004989B7
        if (f > W(F(kOne_0073d764))) f = W(F(kOne_0073d764));                            // 004989B9..004989CB
        const Vector* const a = data_vec(self, 0x21C, j);                                // 004989D4..004989EA
        const Vector* const t = data_vec(self, 0x240, j);                                // 004989DC..004989EE
        const double u = W(F(kOne_0073d764)) - f;                                        // 004989CE..004989DA
        const double uu = u * u;                                                         // 004989E8..004989EC
        const float h1c = S(u * uu);                                                     // 004989F0..004989F4 [esp+1C]
        const float h20 = S(f * uu * W(F(kThree_00740c84)));                             // 004989F8..00498A02 [esp+20]
        const float h24 = S(u * f * f * W(F(kThree_00740c84)));                          // 00498A08..00498A12 [esp+24]
        const double fff = f * f * f;                                                    // 00498A16..00498A1A
        const double f3ax = fff * W(a->x);                                               // 00498A1C..00498A1E (wide)
        const float v9c = S(fff * W(a->y));                                              // 00498A20..00498A25
        const float va0 = S(fff * W(a->z));                                              // 00498A2C..00498A31
        const double dx = W(a->x) - W(t->x);                                             // 00498A38..00498A3A
        const double dy = W(a->y) - W(t->y);                                             // 00498A3C..00498A3F
        const float vb8 = S(W(a->z) - W(t->z));                                          // 00498A42..00498A48
        const float vf0 = S(dx * W(h24));                                                // 00498A4F..00498A55
        const double dyb = dy * W(h24);                                                  // 00498A5C
        const double dzb = W(vb8) * W(h24);                                              // 00498A60..00498A67
        const double sx = W(t[-1].x) + W(a[-1].x);                                       // 00498A6B..00498A6E
        const double sy = W(t[-1].y) + W(a[-1].y);                                       // 00498A71..00498A74
        const float v104 = S(W(t[-1].z) + W(a[-1].z));                                   // 00498A77..00498A7D
        const float v80 = S(sx * W(h20));                                                // 00498A84..00498A8A
        const float v84 = S(sy * W(h20));                                                // 00498A91..00498A95
        const double sz20 = W(v104) * W(h20);                                            // 00498A9C..00498AA3
        const double ux = W(h1c) * W(a[-1].x);                                           // 00498AA7..00498AAB
        const float v6c = S(W(h1c) * W(a[-1].y));                                        // 00498AAE..00498AB5
        const float v70 = S(W(h1c) * W(a[-1].z));                                        // 00498AB9..00498AC0
        const float va4 = S(ux + W(v80));                                                // 00498AC4..00498ACB
        const double y1 = W(v6c) + W(v84);                                               // 00498AD8..00498ADF
        const float vac = S(W(v70) + sz20);                                              // 00498AE9..00498AF4
        Vector* const position = particle_position(particle_at(self, p));                // 00498AD2..00498AE6
        ++p;                                                                             // 00498AED..00498AF0
        const float h38 = S(W(va4) + W(vf0));                                            // 00498AFB..00498B09
        const float v3c = S(y1 + dyb);                                                   // 00498B0D..00498B0F
        const float v40 = S(W(vac) + dzb);                                               // 00498B15..00498B1E
        const float px = S(W(h38) + f3ax);                                               // 00498B26..00498B2C
        const float py = S(W(v3c) + W(v9c));                                             // 00498B38..00498B43
        const float pz = S(W(v40) + W(va0));                                             // 00498B47..00498B59
        position->x = px; position->y = py; position->z = pz;                            // 00498B34..00498B61
        last_count = field<U>(self, 0x1CC);                                              // 00498B64
    }
    return last_count;   // EAX = last read of [esi+1CC] (00498918 / 00498B64)
}


// ---------------------------------------------------------------- 00437A60 operator*(float, Vector const&), cdecl
// No provider anywhere (symbol + address scan of M45 and every lane object); leaf reached only from 0048E700.
// fld s; fmul z / y / x; fstp x, y, z (each product rounded once at its fstp dword).
Vector* __cdecl float_times_vector_00437a60(Vector* out, float scale, const Vector* v) {
    auto* const resultPointer = out;
    // fld scalar / fmul m32 is intentional: loading v first changes NaN payloads.
    // The original keeps z,y,x on the x87 stack until all inputs have been read.
    __asm {
        mov eax, v
        fld scale
        fmul dword ptr [eax+8]
        fld scale
        fmul dword ptr [eax+4]
        fld scale
        fmul dword ptr [eax]
        mov eax, resultPointer
        fstp dword ptr [eax]
        fstp dword ptr [eax+4]
        fstp dword ptr [eax+8]
    }
    return out;
}

// ---------------------------------------------------------------- 0048E700 PartEmitter::RecursiveFractal, RET 28h
// thiscall(index, count, unused_flag, Vector direction, Vector accumulator, float spread). The original
// unrolls two recursion levels and turns the second self-call of the outermost level into a loop
// (0048EEFA jmp 0048E720); the order of every rand call (pre-order: node, left subtree, right subtree),
// every fstp dword and every placement is identical to this recursive form:
//   accumulator.x = S(rand()%ftol(spread*100) * 0.01 * this[+BC] + accumulator.x)
//   place(index, direction * accumulator.x)        [this->vtbl[84]() ? that[+270][index] = v : particle[index].pos += v]
//   if (count == 0) return; h = ftol(count * 0.5); child spread = S(spread * 0.5)
//   recurse(index - h, h, ...); recurse(index + h, h, ...)  (children start from this node's accumulator)
// The left children place through operator* 00437A60 (0048E8E4 / 0048EA14 / 0048EA53 / 0048E923); the
// other nodes multiply inline (0048E77C..0048E7C3 / 0048EBAF.. / 0048ED99..) - same products, same stores.
// unused_flag (arg 2) is only forwarded (0048EAD9 / 0048EB22 / 0048ECBA / 0048ED03 / 0048EE92).
// Division: rand() % ftol(spread*100) raises #DE in the original when the divisor is 0 (idiv); C++ keeps idiv.
} // extern "C"
namespace {
inline void* emitter_slot84(void* self) {                       // vtbl+84 (lightning: self_return 00641DB0; PartEmitter: return_zero)
    return as_object(part_table(self)->s084(self, nullptr));
}
inline void fractal_place(void* self, I index, const Vector& direction, float scale, bool via_operator) {
    void* const holder = emitter_slot84(self);                                            // call [edx+84h]
    Vector v;
    if (via_operator) {
        float_times_vector_00437a60(&v, scale, &direction);                               // 00437A60
    } else {
        v.x = S(W(direction.x) * W(scale));                                               // fld P.x; fmul s; fstp
        v.y = S(W(direction.y) * W(scale));
        v.z = S(W(direction.z) * W(scale));
    }
    if (holder != nullptr) {
        reinterpret_cast<Vector*>(field<U>(holder, 0x270))[index] = v;                    // [obj+270] + index*12
    } else {
        vector_add_assign_004a9740(particle_position(particle_at(self, index)), nullptr, &v); // particle[index]+8 += v (004A9740)
    }
}
} // namespace
extern "C" {

void __fastcall part_emitter_recursive_fractal_0048e700(void* self, void* unused_edx, I index, I count, U unused_flag,
                                                         Vector direction, Vector accumulator, float spread);

} // extern "C"
namespace {
// one node: jitter its accumulator from the parent's, then place it (returns the node accumulator).
inline Vector fractal_visit(void* self, I index, const Vector& direction, Vector accumulator, float spread, bool via_operator) {
    const I r = crt_rand_006fc447();                                                      // 0048E720 / 0048E883 / 0048E9B6 / 0048EB62 / 0048ED49
    const I divisor = ftol2(W(spread) * W(F(kHundred_00741838)));                         // fld spread; fmul 100.0f; _ftol2
    accumulator.x = S(rand_int_as_double(r % divisor) * W(F(kHundredth_0073dfec)) * W(field<float>(self, 0xBC))
                      + W(accumulator.x));                                                // cdq; idiv; fild; fmul 0.01f; fmul [+BC]; fadd; fstp
    fractal_place(self, index, direction, accumulator.x, via_operator);
    return accumulator;
}
inline I fractal_half(I count) { return ftol2(rand_int_as_double(count) * 0.5); }        // fild; fmul qword 0073F3D0 (0.5); _ftol2
inline float fractal_half_spread(float spread) { return S(W(spread) * W(F(kHalf_0073e9ac))); } // fmul 0073E9AC; fstp
} // namespace
extern "C" {

void __fastcall part_emitter_recursive_fractal_0048e700(void* self, void*, I index, I count, U unused_flag,
                                                         Vector direction, Vector accumulator, float spread) {
    for (;;) {
        accumulator = fractal_visit(self, index, direction, accumulator, spread, false);  // 0048E720..0048E815 (inline products)
        if (count == 0) return;                                                           // 0048E81A..0048E823
        const I h = fractal_half(count);                                                  // 0048E829..0048E836 [esp+1C]
        const float cs = fractal_half_spread(spread);                                     // 0048E83B..0048E862 [esp+4C]/[esp+18]
        // left child (index-h, h), placed through 00437A60 (0048E883..0048E93C)
        const I left = index - h;                                                         // 0048E85A..0048E87F [esp+14]
        const Vector lacc = fractal_visit(self, left, direction, accumulator, cs, true);
        if (h != 0) {                                                                     // 0048E941..0048E947
            const I h2 = fractal_half(h);                                                 // 0048E94D..0048E957 [esp+20]
            const float cs2 = fractal_half_spread(cs);                                    // 0048E95C..0048E984 [esp+48]/[esp+10]
            {   // left-left (left-h2, h2), via 00437A60 (0048E9B6..0048EA6C)
                const I ll = left - h2;                                                   // 0048E993..0048E9B2 [esp+18]
                const Vector llacc = fractal_visit(self, ll, direction, lacc, cs2, true);
                if (h2 != 0) {                                                            // 0048EA71..0048EA77
                    const I h3 = fractal_half(h2);                                        // 0048EA7D..0048EA87
                    const float cs3 = fractal_half_spread(cs2);                           // 0048EA8C..0048EA9C
                    part_emitter_recursive_fractal_0048e700(self, nullptr, ll - h3, h3, unused_flag, direction, llacc, cs3); // 0048EAA4..0048EAE0
                    part_emitter_recursive_fractal_0048e700(self, nullptr, ll + h3, h3, unused_flag, direction, llacc, cs3); // 0048EAE5..0048EB29
                }
            }
            {   // left-right (left+h2, h2), inline products (0048EB2E..0048EC4D)
                const I lr = left + h2;                                                   // 0048EB49..0048EB5E [esp+14]
                const Vector lracc = fractal_visit(self, lr, direction, lacc, cs2, false);
                if (h2 != 0) {                                                            // 0048EC52..0048EC58
                    const I h3 = fractal_half(h2);                                        // 0048EC5E..0048EC68
                    const float cs3 = fractal_half_spread(cs2);                           // 0048EC6D..0048EC7D
                    part_emitter_recursive_fractal_0048e700(self, nullptr, lr - h3, h3, unused_flag, direction, lracc, cs3); // 0048EC85..0048ECC1
                    part_emitter_recursive_fractal_0048e700(self, nullptr, lr + h3, h3, unused_flag, direction, lracc, cs3); // 0048ECC6..0048ED0A
                }
            }
        }
        // right child (index+h, h), inline products (0048ED0F..0048EE25)
        const I right = index + h;                                                        // 0048ED25..0048ED45 [esp+14]
        const Vector racc = fractal_visit(self, right, direction, accumulator, cs, false);
        if (h == 0) return;                                                               // 0048EE2A..0048EE30
        const I h2 = fractal_half(h);                                                     // 0048EE36..0048EE40
        const float cs2 = fractal_half_spread(cs);                                        // 0048EE45..0048EE55
        part_emitter_recursive_fractal_0048e700(self, nullptr, right - h2, h2, unused_flag, direction, racc, cs2); // 0048EE59..0048EE99
        // second self-call as a loop (0048EE9E..0048EEFA): index = right + h2, count = h2, accumulator = racc
        index = right + h2; count = h2; accumulator = racc; spread = cs2;
    }
}


// ---------------------------------------------------------------- 00498B80 LightningEmitter::Update(delta), RET 4
// Locals carry their original frame offsets (esp after the four register pushes; delta = [esp+1D4]).
// Lists: +1C8 particles (Particle*), +1D4 free particles, +210 points P, +21C A, +228 J (jittered points),
// +234 tangents T0, +240 T, +24C JT (jittered tangents), +258 branches, +264 branch targets, +270 offsets.
// Unwind map 00717A0E (state at [esp+1CC] around the new(74h) Particle) cited, not emitted.
} // extern "C"
namespace {
inline const aurora_vt::VTable_00743610* lightning_table(void* self) {
    return static_cast<const aurora_vt::VTable_00743610*>(field<void*>(self, 0));
}
inline Vector* particle_pos_at(void* self, I index) { return particle_position(particle_at(self, index)); }
// CExoArrayList<T*>::DelIndex inlined by the original: size--, shift [index+1..] down by one.
inline void remove_word_at(void* self, U list_offset, I index) {
    PointerList* const list = list_at(self, list_offset);
    list->size = list->size - 1;
    for (I k = index; k < list->size; ++k) {
        U* const data = static_cast<U*>(as_object(static_cast<U>(list->data)));
        data[k] = data[k + 1];
    }
}
// CExoArrayList<T*>::Add with 005E03C0 growth (cap ? 2*cap : 8).
inline void add_word(void* self, U list_offset, U value) {
    PointerList* const list = list_at(self, list_offset);
    if (list->size == list->capacity) {
        const I capacity = list->capacity;
        menu_list_growth::allocate_005e03c0(list, nullptr, capacity != 0 ? static_cast<U>(capacity + capacity) : 8u);
    }
    static_cast<U*>(as_object(static_cast<U>(list->data)))[list->size] = value;
    list->size = list->size + 1;
}
// Segment search shared by the three blend loops (00499103..0049913E / 00499BAC..00499BEA): advance the
// segment while tp > s (no upper bound in these copies); start = S(s) before each step.
inline void advance_segment(double tp, double& s, float& start, I& j, I segments) {
    if (tp > s) {                                                                         // fcom st(1); test ah,41h; jne
        const double d = W(F(kOne_0073d764)) / rand_int_as_double(segments - 1);         // fild; fdivr 1.0
        do {
            ++j;                                                                          // inc edx
            start = S(s);                                                                 // fld st(2); fstp [start]
            s = s + d;                                                                    // fld st0; faddp st(3),st
        } while (tp > s);                                                                 // fld st1; fcomp st3; test ah,41h; je
    }
}
inline double clamp_one(double f) { return (f > W(F(kOne_0073d764))) ? W(F(kOne_0073d764)) : f; }   // fcom 1.0; test ah,41h; jne
} // namespace
extern "C" {

U __fastcall lightning_emitter_update_00498b80(void* self, void*, U delta_bits) {
    const float delta = F(delta_bits);
    const I spawn = ftol2(W(field<float>(self, 0x58)));                                  // 00498BA0..00498BB0 [esp+30]
    Vector v114 = {0.0f, 0.0f, 0.0f};                                                     // 00498BB4..00498BCA
    // [esp+18C..194] = 0 (00498BD5..00498BEB): overwritten before every read (00499A66/00499A83/00499A92).
    Vector vac = {0.0f, 0.0f, 0.0f};                                                      // 00498BF6..00498C0C
    float scale80 = F(kOne_0073d764);                                                     // 00498C17
    bool branches_reset_6b = false;                                                       // 00498C22
    bool moved_first_2f = false;                                                          // 00498C27
    bool jittered_last_37 = false;                                                        // 00498C2C
    if (field<void*>(self, 0x40) != nullptr)                                              // 00498BAB..00498C31
        scale80 = field<float>(field<void*>(self, 0x40), 0x1B4);                          // 00498C33..00498C39 (raw dword)
    field<float>(self, 0xF4) = S(W(delta) + W(field<float>(self, 0xF4)));                 // 00498C42..00498C55
    I count = field<I>(self, 0x1CC);                                                      // 00498C4F ebx
    void* node = nullptr;
    if (spawn == 0 || field<void*>(self, 0x1E4) == nullptr) {                             // 00498C40..00498C69
        // ---- 0049B138: return every particle to the free list (own inline growth through operator new)
        while (count > 0) {                                                               // 0049B138..0049B13A / 0049B1FA..0049B202
            const U particle = data_word(self, 0x1C8, count - 1);                         // 0049B140..0049B14C
            --count;                                                                      // 0049B150
            remove_word_at(self, 0x1C8, count);                                           // 0049B151..0049B178
            if (field<I>(self, 0x1D8) == field<I>(self, 0x1DC)) {                         // 0049B17A..0049B186
                const I capacity = field<I>(self, 0x1DC) != 0 ? field<I>(self, 0x1DC) * 2 : 8; // 0049B188..0049B190
                U* const old = static_cast<U*>(field<void*>(self, 0x1D4));                // 0049B195
                field<I>(self, 0x1DC) = capacity;                                         // 0049B19B
                U* const fresh = static_cast<U*>(live_operator_new(static_cast<U>(capacity) << 2)); // 0049B1A1..0049B1A5 (006FA7E6)
                field<U*>(self, 0x1D4) = fresh;                                           // 0049B1B0
                for (I k = 0; k < field<I>(self, 0x1D8); ++k) fresh[k] = old[k];          // 0049B1B9..0049B1D5
                if (old != nullptr) live_free(old);                                       // 0049B1D7..0049B1DC
            }
            field<U*>(self, 0x1D4)[field<I>(self, 0x1D8)] = particle;                     // 0049B1E4..0049B1F0
            field<I>(self, 0x1D8) = field<I>(self, 0x1D8) + 1;                            // 0049B1F3..0049B1FC
        }
        return 0;   // EAX is not a defined value of this void member (callers through vtbl+80 ignore it)
    }
    // ---- grow / shrink the particle list to `spawn` (00498C6F..00498E30)
    if (count < spawn) {                                                                  // 00498C6F..00498C71
        I remaining = spawn - count;                                                      // 00498C77..00498C7D [esp+B8]
        count = spawn;                                                                    // 00498C84
        do {
            U particle;
            if (field<I>(self, 0x1D8) > 0) {                                              // 00498C86..00498C8E
                const I last = field<I>(self, 0x1D8) - 1;
                particle = data_word(self, 0x1D4, last);                                  // 00498C90..00498C96
                remove_word_at(self, 0x1D4, last);                                        // 00498C9A..00498CC9
            } else {
                void* const block = live_operator_new(0x74);                              // 00498CCB..00498CCD
                particle = word_of(block != nullptr
                    ? particle_construct_00494c40(block, nullptr, self, delta_bits)       // 00498CE8..00498CF3
                    : nullptr);                                                           // 00498CFA
            }
            void* const p = as_object(particle);
            if (((field<U>(field<void*>(self, 0x4), 0x12C) >> 5) & 1u) != 0) {             // 00498D09..00498D17
                I range = ftol2(W(field<float>(self, 0x6C)) - W(field<float>(self, 0x70))) + 1; // 00498D19..00498D26
                if (range < 0) range = -range;                                            // 00498D27..00498D29 (jns; neg)
                if (range != 0) {                                                         // 00498D2B..00498D2D
                    const I r = crt_rand_006fc447() % range;                              // 00498D2F..00498D35
                    field<I>(p, 0x3C) = ftol2(rand_int_as_double(r) + W(field<float>(self, 0x70))); // 00498D37..00498D47
                }
            }
            add_word(self, 0x1C8, particle);                                              // 00498D4A..00498D8F
            --remaining;                                                                  // 00498D8E..00498D95
        } while (remaining != 0);                                                         // 00498D9C
    }
    if (count > spawn) {                                                                  // 00498DA2..00498DA6
        do {
            const U particle = data_word(self, 0x1C8, count - 1);                         // 00498DC0..00498DD1 [esp+18]
            remove_word_at(self, 0x1C8, count - 1);                                       // 00498DCA..00498DFA
            add_word(self, 0x1D4, particle);                                              // 00498DFC..00498E25
            --count;                                                                      // 00498E2C
        } while (count > spawn);                                                          // 00498E28..00498E30
    }
    if (field<I>(self, 0x274) < field<I>(self, 0x1CC))                                    // 00498E32..00498E3E
        vector_list_set_size_004213a0(list_at(self, 0x270), nullptr, field<I>(self, 0x1CC)); // 00498E40..00498E47
    // ---- per-particle age / motion-blur trail (00498E4C..00498F38)
    {
        const U motion_blur = emitter_render_motion_blur_00798b8c;                        // 00498E5E
        for (I k = 0; k < field<I>(self, 0x1CC); ++k) {                                   // 00498E52..00498E58 / 00498F2F..00498F38
            void* const p = particle_at(self, k);                                         // 00498E64..00498E6A
            if (W(field<float>(p, 0x70)) == W(field<float>(p, 0x40))) {                   // 00498E6D..00498E78 (test ah,44h; jp)
                field<U>(p, 0x70) = 0; field<U>(p, 0x40) = 0; field<U>(p, 0x44) = 0;      // 00498E7A..00498E80
            }
            field<float>(p, 0x70) = S(W(delta) + W(field<float>(p, 0x70)));               // 00498E83..00498E8D
            if (field<U>(self, 0x114) == motion_blur) {                                   // 00498E90..00498E98
                const double limit = W(field<float>(self, 0xB4));
                const double t = ((W(delta) < limit) ? W(delta) : limit) / limit;         // 00498E9E..00498EC1 (test ah,5; jp)
                float* const prev = &field<float>(p, 0x60);                               // 00498EC7
                const double u = W(F(kOne_0073d764)) - t;                                 // 00498ECA..00498ED0
                const float ux20 = S(u * W(prev[0]));                                     // 00498ED2..00498ED6
                const float uy24 = S(u * W(prev[1]));                                     // 00498EDA..00498EDF
                const double uz = u * W(prev[2]);                                         // 00498EE3
                const double tx = t * W(field<float>(p, 0x08));                           // 00498EE6..00498EE8
                const double ty = t * W(field<float>(p, 0x0C));                           // 00498EEB..00498EED
                const float tz78 = S(t * W(field<float>(p, 0x10)));                       // 00498EF0..00498EF5
                prev[0] = S(W(ux20) + tx);                                                // 00498EF9..00498F09
                prev[1] = S(ty + W(uy24));                                                // 00498F07..00498F19
                prev[2] = S(W(tz78) + uz);                                                // 00498F1C..00498F2C
            }
        }
    }
    field<float>(self, 0xF8) = S(W(delta) + W(field<float>(self, 0xF8)));                 // 00498F3E..00498F4E
    node = aurora_node_as_emitter_0043e3c0(field<void*>(self, 0x4), nullptr);             // 00498F45..00498F54 [esp+B8]
    if (W(field<float>(self, 0xF8)) > W(field<float>(self, 0x108))) {                     // 00498F59..00498F73
        // ---- new strike: re-anchor the ends, rebuild the control points (00498F79..00499042)
        field<U>(self, 0xF8) = 0;                                                         // 00498F7F
        vec_data(self, 0x210)[0] = field<Vector>(self, 0x164);                            // 00498F79..00498F9C
        Vector vbc;
        const Vector gob_pos = *gob_get_position(field<void*>(self, 0x1E4), &vbc);        // 00498F9F..00498FAF
        vec_data(self, 0x210)[field<I>(self, 0x214) - 1] = gob_pos;                       // 00498FB2..00498FD2
        lightning_table(self)->s088(self, nullptr);                                       // 00498FD5..00498FD9 (UpdateControlPoints)
        if (field<I>(node, 0x5C) > 0 && field<U8>(self, 0x28C) == 0) {                    // 00498FDF..00498FEE
            lightning_table(self)->s08c(self, nullptr);                                   // 00498FF0..00498FF4 (UpdateBranches)
            branches_reset_6b = true;                                                     // 00499004
            for (I i = 0; i < field<I>(self, 0x288); ++i)                                 // 00498FFA..0049902E
                field<U>(branch_at(self, i), 0xF8) = field<U>(self, 0x108);               // 00499010..0049901F
        }
        field<float>(self, 0xF4) = S(W(field<float>(self, 0xB8)) + W(F(kHundredth_0073dfec))); // 00499030..0049903C
    } else {
        void* const node2 = aurora_node_as_emitter_0043e3c0(field<void*>(self, 0x4), nullptr); // 00499047..0049904A
        if (field<U>(node2, 0x60) != 0 && field<U8>(self, 0x28C) == 0) {                  // 0049904F..00499062
            // ---- blend loop 1 (00499068..00499664): particles along the segments, J/JT weighted by w = F8/108
            I j = 1;                                                                      // 0049906E edx / [esp+18]
            float w10 = 0.0f;                                                             // 0049907D
            if (W(field<float>(self, 0x108)) != W(F(kZero_0073d700)))                    // 00499068..0049908A (test ah,44h; jnp)
                w10 = S(W(field<float>(self, 0xF8)) / W(field<float>(self, 0x108)));      // 0049908C..00499098
            double s = W(F(kOne_0073d764)) / rand_int_as_double(field<I>(self, 0x22C) - 1); // 0049909C..004990B9
            float start = 0.0f;                                                           // 004990BF [esp+E0]
            I p = 1;                                                                      // 004990B1 ebp / [esp+5C]
            if (field<I>(self, 0x1CC) - 1 > p) {                                          // 004990A2..004990CE
                const float wa30 = S(W(F(kOne_0073d764)) - W(w10));                       // 004990D4..004990DE ([esp+30], copy [esp+14])
                do {
                    const double tp = rand_int_as_double(p) / rand_int_as_double(field<I>(self, 0x1CC) - 1); // 004990F0..004990FF
                    advance_segment(tp, s, start, j, field<I>(self, 0x22C));             // 00499103..0049913E
                    const double f = clamp_one((tp - W(start)) / (s - W(start)));         // 00499140..00499161
                    const Vector* const J = data_vec(self, 0x228, j);                     // 0049916D..0049917B
                    const Vector* const A = data_vec(self, 0x21C, j);                     // 00499180..00499188
                    const Vector* const JT = data_vec(self, 0x24C, j);                    // 0049918A..00499198
                    const Vector* const T = data_vec(self, 0x240, j);                     // 00499190..0049919C
                    const double u = W(F(kOne_0073d764)) - f;                             // 00499167..00499173
                    const double uu = u * u;                                              // 0049917E..00499186
                    const float h1c = S(uu * u);                                          // 00499196..0049919E
                    const float h6c = S(uu * f * W(F(kThree_00740c84)));                  // 004991A2..004991AA
                    const float h5c = S(u * f * f * W(F(kThree_00740c84)));               // 004991AE..004991B8
                    const double fff = f * f * f;                                         // 004991BC..004991C0
                    const double wJx = W(w10) * W(J->x);                                  // 004991C2..004991C6
                    const double wJy = W(w10) * W(J->y);                                  // 004991C8..004991CC
                    const float wJz = S(W(w10) * W(J->z));                                // 004991CF..004991D6 [1BC]
                    const double waAx = W(wa30) * W(A->x);                                // 004991DD..004991E3
                    const float waAy = S(W(wa30) * W(A->y));                              // 004991E5..004991EA [24]
                    const float waAz = S(W(wa30) * W(A->z));                              // 004991EE..004991F3 [28]
                    const float px15c = S(waAx + wJx);                                    // 004991F7..004991F9
                    const double py = W(waAy) + wJy;                                      // 00499200..00499208
                    const float pz164 = S(W(waAz) + W(wJz));                              // 0049920A..00499215
                    const float h70 = S(fff * W(px15c));                                  // 0049921C..00499225
                    const double fpy = py * fff;                                          // 00499229
                    const float h78 = S(W(pz164) * fff);                                  // 0049922B..00499234
                    const double wJTx = W(w10) * W(JT->x);                                // 00499238..0049923C
                    const double wJTy = W(w10) * W(JT->y);                                // 0049923E..00499242
                    const float wJTz = S(W(w10) * W(JT->z));                              // 00499245..0049924C [1A0]
                    const double waTx = W(wa30) * W(T->x);                                // 00499253..00499259
                    const float waTy = S(W(wa30) * W(T->y));                              // 0049925B..00499260 [13C]
                    const float waTz = S(W(wa30) * W(T->z));                              // 00499267..00499277 [140]
                    const float tx180 = S(waTx + wJTx);                                   // 0049927E..00499280
                    const double ty = W(waTy) + wJTy;                                     // 00499287..00499290
                    const double tz = W(waTz) + W(wJTz);                                  // 00499294..0049929B
                    const double wJx2 = W(w10) * W(J->x);                                 // 004992A2..004992A6
                    const double wJy2 = W(w10) * W(J->y);                                 // 004992A8..004992AC
                    const float wJz2 = S(W(w10) * W(J->z));                               // 004992AF..004992B6 [1B0]
                    const float a50 = S(W(wa30) * W(A->x));                               // 004992BD..004992C6
                    const float a54 = S(W(wa30) * W(A->y));                               // 004992CA..004992D4
                    const float a58 = S(W(wa30) * W(A->z));                               // 004992D8..004992E2
                    const float p174 = S(W(a50) + wJx2);                                  // 004992E6..004992EC
                    const float p178 = S(W(a54) + wJy2);                                  // 004992F3..004992F9
                    const double pz = W(a58) + W(wJz2);                                   // 00499304..00499308
                    const float d144 = S(W(p174) - W(tx180));                             // 0049930F..0049931D
                    const float d148 = S(W(p178) - ty);                                   // 00499324..0049932D
                    const double dz = pz - tz;                                            // 00499334
                    const double hx = W(h5c) * W(d144);                                   // 0049933A..0049933E
                    const float h16c = S(W(d148) * W(h5c));                               // 00499345..00499350
                    const float h170 = S(dz * W(h5c));                                    // 00499357..0049935D
                    const Vector* const Jm = J - 1;
                    const Vector* const Am = A - 1;
                    const Vector* const JTm = JT - 1;
                    const Vector* const Tm = T - 1;
                    const double wJTxm = W(w10) * W(JTm->x);                              // 00499364..00499368
                    const double wJTym = W(w10) * W(JTm->y);                              // 0049936B..0049936F
                    const float wJTzm = S(W(w10) * W(JTm->z));                            // 00499372..0049937B [C4]
                    const float tfc = S(W(wa30) * W(Tm->x));                              // 00499382..0049938D ([esp+60] = wa)
                    const float t100 = S(W(wa30) * W(Tm->y));                             // 00499394..0049939B
                    const float t104 = S(W(wa30) * W(Tm->z));                             // 004993A2..004993AF
                    const float j38 = S(W(w10) * W(Jm->x));                               // 004993B6..004993BD
                    const float j3c = S(W(w10) * W(Jm->y));                               // 004993C1..004993C8
                    const float j40 = S(W(w10) * W(Jm->z));                               // 004993CC..004993D3
                    const float aa0 = S(W(wa30) * W(Am->x));                              // 004993D7..004993DE ([esp+64] = wa)
                    const float aa4 = S(W(wa30) * W(Am->y));                              // 004993E5..004993EC
                    const float aa8 = S(W(wa30) * W(Am->z));                              // 004993F3..004993FA
                    const float pf0 = S(W(aa0) + W(j38));                                 // 00499401..0049940C
                    const float pf4 = S(W(aa4) + W(j3c));                                 // 00499413..0049941E
                    const float pf8 = S(W(aa8) + W(j40));                                 // 00499425..00499430
                    const float q108 = S(W(pf0) + W(tfc));                                // 00499437..00499445
                    const float q10c = S(W(pf4) + W(t100));                               // 0049944C..0049945A
                    const float q110 = S(W(pf8) + W(t104));                               // 00499461..0049946F
                    const float q150 = S(W(q108) + wJTxm);                                // 00499476..0049947F
                    const float q154 = S(W(q10c) + wJTym);                                // 00499486..0049948F
                    const double q1z = W(q110) + W(wJTzm);                                // 0049949A..004994A1
                    const double h6x = W(h6c) * W(q150);                                  // 004994A8..004994AC
                    const float h124 = S(W(q154) * W(h6c));                               // 004994B3..004994BE
                    const float h128 = S(q1z * W(h6c));                                   // 004994C5..004994CB
                    const float jd4 = S(W(w10) * W(Jm->x));                               // 004994D2..004994D9
                    const float jd8 = S(W(w10) * W(Jm->y));                               // 004994E0..004994E7
                    const float jdc = S(W(w10) * W(Jm->z));                               // 004994EE..004994FB
                    const float ac8 = S(W(wa30) * W(Am->x));                              // 00499502..00499509 ([esp+14] = wa)
                    const float acc_ = S(W(wa30) * W(Am->y));                             // 00499510..00499517
                    const float ad0 = S(W(wa30) * W(Am->z));                              // 0049951E..0049952B
                    const float pe4 = S(W(ac8) + W(jd4));                                 // 00499532..00499540
                    const float pe8 = S(W(acc_) + W(jd8));                                // 00499547..00499555
                    const float pec = S(W(ad0) + W(jdc));                                 // 0049955C..0049956A
                    const float h90 = S(W(h1c) * W(pe4));                                 // 00499571..0049957C
                    const float h94 = S(W(pe8) * W(h1c));                                 // 00499583..0049958E
                    const float h98 = S(W(pec) * W(h1c));                                 // 00499595..004995A0
                    const float s84 = S(W(h90) + h6x);                                    // 004995A7..004995B0
                    const float s88 = S(W(h94) + W(h124));                                // 004995B9..004995C7
                    const float s8c = S(W(h98) + W(h128));                                // 004995CE..004995DC
                    const double X = hx + W(s84);                                         // 004995E3
                    const float s130 = S(W(s88) + W(h16c));                               // 004995EA..004995F8
                    const float s134 = S(W(s8c) + W(h170));                               // 004995FF..0049960D
                    Vector* const out = particle_pos_at(self, p);                         // 004994F5 / 00499525..00499528
                    const float ox = S(X + W(h70));                                       // 00499614..00499618
                    const float oy = S(W(s130) + fpy);                                    // 00499620..0049962B
                    const float oz = S(W(s134) + W(h78));                                 // 00499633..0049963E
                    out->x = ox; out->y = oy; out->z = oz;                                // 00499627..0049964D
                    ++p;                                                                  // 00499656 / 0049965A
                } while (p < field<I>(self, 0x1CC) - 1);                                  // 00499650..0049965E
            }
        }
    }
    // ---- 00499666
    if (W(field<float>(self, 0xF4)) > W(field<float>(self, 0xB8))) {                     // 00499666..00499677
        void* const first = particle_at(self, 0);                                         // 00499683..0049968F
        if (W(field<float>(self, 0xAC)) == W(F(kZero_0073d700)) &&
            W(field<float>(self, 0xB0)) == W(F(kZero_0073d700))) {                        // 0049967D..004996A9 (test ah,44h; jp)
            *particle_position(first) = field<Vector>(self, 0x164);                       // 004996AB..004996C1
        } else {
            const I range_y = ftol2(W(scale80) * W(field<float>(self, 0xB0)) * W(F(kHundred_00741838))); // 004996C6..004996D9
            const I range_x = ftol2(W(scale80) * W(field<float>(self, 0xAC)) * W(F(kHundred_00741838))); // 004996DE..004996F2
            particle_random_position_0048d870(first, nullptr, range_x, range_y);          // 004996EB..004996FA
            moved_first_2f = true;                                                        // 004996FF
        }
        Vector* const last_pos = particle_pos_at(self, field<I>(self, 0x1CC) - 1);        // 00499704..00499710 / 00499729 ([esp+7C])
        {
            Vector vbc;
            const Vector gob_pos = *gob_get_position(field<void*>(self, 0x1E4), &vbc);    // 00499714..00499724
            *last_pos = gob_pos;                                                          // 00499727..00499739
        }
        if (W(field<float>(self, 0xFC)) != W(F(kZero_0073d700))) {                        // 0049973C..00499751 (test ah,44h; jnp)
            float jitter[3] = {0.0f, 0.0f, 0.0f};                                         // 00499757..00499767 [esp+38]
            for (int c = 0; c < 3; ++c) {                                                 // 0049976F..0049982B (three copies)
                const I r = crt_rand_006fc447();
                const I divisor = ftol2(W(field<float>(self, 0xFC)) * W(F(kFifty_00740cc0)));
                const I m = r % divisor;
                const I v = ((r & 1) != 0) ? m : -m;                                      // test bl,1; je -> neg edx
                jitter[c] = S(rand_int_as_double(v) * W(F(kHundredth_0073dfec)));
            }
            Vector add;
            std::memcpy(&add, jitter, sizeof(add));
            vector_add_assign_004a9740(last_pos, nullptr, &add);                          // 00499831..0049983C
            jittered_last_37 = true;                                                      // 00499841
        }
        if (moved_first_2f || jittered_last_37) {                                         // 00499846..00499854
            const I points = field<I>(self, 0x214);                                       // 0049985A
            if (points == 2) {                                                            // 00499860..00499863
                if (moved_first_2f) vec_data(self, 0x210)[0] = *particle_pos_at(self, 0); // 00499865..0049987E / 00499896..004998A3
                else vec_data(self, 0x210)[1] = *particle_pos_at(self, field<I>(self, 0x1CC) - 1); // 00499880..004998A3
                lightning_table(self)->s088(self, nullptr);                               // 004998A6..004998AA
            } else if (moved_first_2f) {
                // ---- blend loop 2 (004998C1..0049A10E): first segment only
                const Vector head = *particle_pos_at(self, 0);                            // 004998C1..004998D2
                vec_data(self, 0x228)[0] = head;                                          // 004998C9..004998E1
                vec_data(self, 0x210)[0] = head;                                          // 004998E4..004998F7
                I j = 1;                                                                  // 00499906 edx / [esp+18]
                float w10 = 0.0f;                                                         // 0049990F
                if (W(field<float>(self, 0x108)) != W(F(kZero_0073d700)))                // 004998FA..0049991C
                    w10 = S(W(field<float>(self, 0xF8)) / W(field<float>(self, 0x108))); // 0049991E..0049992A
                double s = W(F(kOne_0073d764)) / rand_int_as_double(field<I>(self, 0x22C) - 1); // 0049992E..0049994B
                float start = 0.0f;                                                       // 00499951 [esp+1C]
                I p = 1;                                                                  // 00499943 ebx / [esp+5C]
                if (field<I>(self, 0x1CC) - 1 > p) {                                      // 00499934..0049995D
                    while (j <= 1) {                                                      // 00499B90..00499B93
                        const double tp = rand_int_as_double(p) / rand_int_as_double(field<I>(self, 0x1CC) - 1); // 00499B99..00499BA8
                        advance_segment(tp, s, start, j, field<I>(self, 0x22C));         // 00499BAC..00499BEA
                        const double f = clamp_one((tp - W(start)) / (s - W(start)));     // 00499BEC..00499C07
                        const Vector* const J = data_vec(self, 0x228, j);                 // 00499C13..00499C21
                        const Vector* const A = data_vec(self, 0x21C, j);                 // 00499C26..00499C2E
                        const Vector* const JT = data_vec(self, 0x24C, j);                // 00499C32..00499C40
                        const Vector* const T = data_vec(self, 0x240, j);                 // 00499C38 / 00499D0D
                        const double u = W(F(kOne_0073d764)) - f;                         // 00499C0D..00499C19
                        const double uu = u * u;                                          // 00499C24..00499C2C
                        const float h60 = S(uu * u);                                      // 00499C30..00499C42
                        const float h64 = S(f * uu * W(F(kThree_00740c84)));              // 00499C46..00499C50
                        const float h6c = S(u * f * f * W(F(kThree_00740c84)));           // 00499C54..00499C60
                        const float fff14 = S(f * f * f);                                 // 00499C64..00499C6A
                        const float j44 = S(W(w10) * W(J->x));                            // 00499C70..00499C76
                        const float j48 = S(W(w10) * W(J->y));                            // 00499C7A..00499C81
                        const float j4c = S(W(w10) * W(J->z));                            // 00499C85..00499C8C
                        const double wa = W(F(kOne_0073d764)) - W(w10);                   // 00499C90..00499C96 (wide)
                        const double waAx = wa * W(A->x);                                 // 00499C9A..00499C9E
                        const float waAy74 = S(wa * W(A->y));                             // 00499CA0..00499CA5
                        const float waAz78 = S(wa * W(A->z));                             // 00499CA9..00499CAE
                        const double px = waAx + W(j44);                                  // 00499CB2
                        const double py = W(waAy74) + W(j48);                             // 00499CB6..00499CBA
                        const float pzc4 = S(W(waAz78) + W(j4c));                         // 00499CBE..00499CC6
                        const float f15c = S(W(fff14) * px);                              // 00499CCD..00499CD3
                        const double fpy = py * W(fff14);                                 // 00499CDA..00499CDE
                        const float f164 = S(W(pzc4) * W(fff14));                         // 00499CE0..00499CEB
                        const double wJTx = W(w10) * W(JT->x);                            // 00499CF2..00499CF6
                        const double wJTy = W(w10) * W(JT->y);                            // 00499CF8..00499CFC
                        const float wJTz188 = S(W(w10) * W(JT->z));                       // 00499CFF..00499D06
                        const double waTx = wa * W(T->x);                                 // 00499D0F..00499D13
                        const float waTy130 = S(wa * W(T->y));                            // 00499D15..00499D1A
                        const float waTz134 = S(wa * W(T->z));                            // 00499D21..00499D26
                        const float t198 = S(waTx + wJTx);                                // 00499D2D..00499D2F
                        const double ty = W(waTy130) + wJTy;                              // 00499D36..00499D3F
                        const double tz = W(waTz134) + W(wJTz188);                        // 00499D43..00499D4A
                        const double wJx = W(w10) * W(J->x);                              // 00499D51..00499D55
                        const double wJy = W(w10) * W(J->y);                              // 00499D57..00499D5B
                        const float wJz1b0 = S(W(w10) * W(J->z));                         // 00499D5E..00499D65
                        float wa14 = S(wa);                                               // 00499D6C..00499D6E (fst [esp+14])
                        const float a84 = S(W(wa14) * W(A->x));                           // 00499D72..00499D74
                        const float a88 = S(W(wa14) * W(A->y));                           // 00499D7B..00499D82
                        const float a8c = S(W(wa14) * W(A->z));                           // 00499D89..00499D90
                        const float p168 = S(W(a84) + wJx);                               // 00499D97..00499DA0
                        const float p16c = S(W(a88) + wJy);                               // 00499DA7..00499DB0
                        const double pz2 = W(a8c) + W(wJz1b0);                            // 00499DBB..00499DC2
                        const float d120 = S(W(p168) - W(t198));                          // 00499DC9..00499DD7
                        const float d124 = S(W(p16c) - ty);                               // 00499DDE..00499DE7
                        const double dz = pz2 - tz;                                       // 00499DEE
                        const double hx = W(h6c) * W(d120);                               // 00499DF4..00499DF8
                        const float h178 = S(W(d124) * W(h6c));                           // 00499DFF..00499E0A
                        const float h17c = S(dz * W(h6c));                                // 00499E11..00499E17
                        const Vector* const Jm = J - 1;
                        const Vector* const Am = A - 1;
                        const Vector* const JTm = JT - 1;
                        const Vector* const Tm = T - 1;
                        const double wJTxm = W(w10) * W(JTm->x);                          // 00499E1E..00499E22
                        const double wJTym = W(w10) * W(JTm->y);                          // 00499E25..00499E29
                        const float wJTzm1bc = S(W(w10) * W(JTm->z));                     // 00499E2C..00499E33
                        wa14 = S(wa);                                                     // 00499E3A..00499E3C
                        const float tc8 = S(W(wa14) * W(Tm->x));                          // 00499E40..00499E43
                        const float tcc = S(W(wa14) * W(Tm->y));                          // 00499E4A..00499E51
                        const float td0 = S(W(wa14) * W(Tm->z));                          // 00499E58..00499E5F
                        const float je4 = S(W(w10) * W(Jm->x));                           // 00499E66..00499E6D
                        const float je8 = S(W(w10) * W(Jm->y));                           // 00499E74..00499E7B
                        const float jec = S(W(w10) * W(Jm->z));                           // 00499E82..00499E89
                        wa14 = S(wa);                                                     // 00499E90..00499E92
                        const float a90 = S(W(wa14) * W(Am->x));                          // 00499E96..00499E99
                        const float a94 = S(W(wa14) * W(Am->y));                          // 00499EA0..00499EA7
                        const float a98 = S(W(wa14) * W(Am->z));                          // 00499EAE..00499EB5
                        const float pd4 = S(W(a90) + W(je4));                             // 00499EBC..00499ECA
                        const float pd8 = S(W(a94) + W(je8));                             // 00499ED1..00499EDF
                        const float pdc = S(W(a98) + W(jec));                             // 00499EE6..00499EF4
                        const float q108 = S(W(pd4) + W(tc8));                            // 00499EFB..00499F09
                        const float q10c = S(W(pd8) + W(tcc));                            // 00499F10..00499F1E
                        const float q110 = S(W(pdc) + W(td0));                            // 00499F25..00499F33
                        const float q150 = S(W(q108) + wJTxm);                            // 00499F3A..00499F43
                        const float q154 = S(W(q10c) + wJTym);                            // 00499F4A..00499F53
                        const double qz = W(q110) + W(wJTzm1bc);                          // 00499F5E..00499F65
                        const double h64x = W(h64) * W(q150);                             // 00499F6C..00499F70
                        const float h148 = S(W(q154) * W(h64));                           // 00499F77..00499F82
                        const float h14c = S(qz * W(h64));                                // 00499F89..00499F8F
                        const float jf0 = S(W(w10) * W(Jm->x));                           // 00499F96..00499F9D
                        const float jf4 = S(W(w10) * W(Jm->y));                           // 00499FA4..00499FAB
                        const float jf8 = S(W(w10) * W(Jm->z));                           // 00499FB2..00499FB9
                        wa14 = S(wa);                                                     // 00499FC0..00499FC2 (fxch st3; fst)
                        const float afc = S(W(wa14) * W(Am->x));                          // 00499FC6..00499FC9
                        const float a100 = S(W(wa14) * W(Am->y));                         // 00499FD0..00499FD7
                        const float a104 = S(W(wa14) * W(Am->z));                         // 00499FDE..00499FE5
                        const float p50 = S(W(afc) + W(jf0));                             // 00499FEC..00499FFA
                        const float p54 = S(W(a100) + W(jf4));                            // 00499FFE..0049A00C
                        const float p58 = S(W(a104) + W(jf8));                            // 0049A010..0049A01E
                        const float ha0 = S(W(h60) * W(p50));                             // 0049A022..0049A02A
                        const float ha4 = S(W(p54) * W(h60));                             // 0049A031..0049A039
                        const float ha8 = S(W(p58) * W(h60));                             // 0049A040..0049A048
                        const float s38 = S(W(ha0) + h64x);                               // 0049A04F..0049A058
                        const float s3c = S(W(ha4) + W(h148));                            // 0049A05E..0049A06C
                        const float s40 = S(W(ha8) + W(h14c));                            // 0049A070..0049A07E
                        const double X = hx + W(s38);                                     // 0049A082..0049A086
                        const float s13c = S(W(s3c) + W(h178));                           // 0049A088..0049A093
                        const float s140 = S(W(s40) + W(h17c));                           // 0049A09A..0049A0A5
                        const float ox = S(X + W(f15c));                                  // 0049A0AC..0049A0B5
                        const float oy = S(W(s13c) + fpy);                                // 0049A0B9..0049A0C2
                        const float oz = S(W(s140) + W(f164));                            // 0049A0C8..0049A0D6
                        Vector* const out = particle_pos_at(self, p);                     // 0049A0DA..0049A0EB
                        out->x = ox; out->y = oy; out->z = oz;                            // 0049A0EE..0049A0F7
                        ++p;                                                              // 0049A100 / 0049A104
                        if (!(p < field<I>(self, 0x1CC) - 1)) break;                      // 0049A0FA..0049A108
                    }
                }
            } else {   // jittered_last_37 only (0049A113..0049A6E0): last segment
                const Vector tail = *particle_pos_at(self, field<I>(self, 0x1CC) - 1);    // 0049A132..0049A13F
                vec_data(self, 0x228)[points - 1] = tail;                                 // 0049A11F..0049A14E (index = [esi+214]-1)
                vec_data(self, 0x210)[field<I>(self, 0x214) - 1] = tail;                  // 0049A151..0049A171
                const I n = field<I>(self, 0x22C);                                        // 0049A180
                float w10 = 0.0f;                                                         // 0049A18D
                if (W(field<float>(self, 0x108)) != W(F(kZero_0073d700)))                // 0049A174..0049A19A
                    w10 = S(W(field<float>(self, 0xF8)) / W(field<float>(self, 0x108))); // 0049A19C..0049A1A8
                const float a1c = S(rand_int_as_double(n - 2) / rand_int_as_double(n - 1)); // 0049A1AC..0049A1C5
                I p = ftol2(rand_int_as_double(field<I>(self, 0x1CC)) * W(a1c)) + 1;     // 0049A1B7..0049A1D8
                if (p < field<I>(self, 0x1CC) - 1) {                                      // 0049A1D9..0049A1E0 (edi read once at 0049A1B7)
                    const U last_bytes = static_cast<U>(n - 1) * 12u;                     // 0049A1EC..0049A1F3 ebx
                    const float e0 = S(W(F(kOne_0073d764)) - W(a1c));                     // 0049A1E6..0049A1F6
                    const float wa30 = S(W(F(kOne_0073d764)) - W(w10));                   // 0049A1FD..0049A207 ([esp+30], copy [esp+60])
                    do {
                        const double tp = rand_int_as_double(p) / rand_int_as_double(field<I>(self, 0x1CC) - 1); // 0049A213..0049A222
                        const double f = clamp_one((tp - W(a1c)) / W(e0));                // 0049A226..0049A240
                        const Vector* const J = reinterpret_cast<const Vector*>(field<U>(self, 0x228) + last_bytes); // 0049A24C..0049A25A
                        const Vector* const A = reinterpret_cast<const Vector*>(field<U>(self, 0x21C) + last_bytes); // 0049A254..0049A25C
                        const Vector* const JT = reinterpret_cast<const Vector*>(field<U>(self, 0x24C) + last_bytes); // 0049A260..0049A270
                        const Vector* const T = reinterpret_cast<const Vector*>(field<U>(self, 0x240) + last_bytes); // 0049A268..0049A272
                        const double u = W(F(kOne_0073d764)) - f;                         // 0049A246..0049A252
                        const double uu = u * u;                                          // 0049A25E..0049A266
                        const float h9c = S(uu * u);                                      // 0049A26E..0049A276
                        const float h64 = S(uu * f * W(F(kThree_00740c84)));              // 0049A27D..0049A285
                        const float h14 = S(u * f * f * W(F(kThree_00740c84)));           // 0049A289..0049A293
                        const double fff = f * f * f;                                     // 0049A297..0049A29B
                        const double wJx = W(w10) * W(J->x);                              // 0049A29D..0049A2A1
                        const double wJy = W(w10) * W(J->y);                              // 0049A2A3..0049A2A7
                        const float wJzc4 = S(W(w10) * W(J->z));                          // 0049A2AA..0049A2B1
                        const double waAx = W(wa30) * W(A->x);                            // 0049A2B8..0049A2BE
                        const float waAy74 = S(W(wa30) * W(A->y));                        // 0049A2C0..0049A2C5
                        const float waAz78 = S(W(wa30) * W(A->z));                        // 0049A2C9..0049A2CE
                        const float p12c = S(waAx + wJx);                                 // 0049A2D2..0049A2D4
                        const double py = W(waAy74) + wJy;                                // 0049A2DB..0049A2E1
                        const float p134 = S(W(waAz78) + W(wJzc4));                       // 0049A2E5..0049A2F0
                        const float f50 = S(fff * W(p12c));                               // 0049A2F7..0049A300
                        const double fpy = py * fff;                                      // 0049A304
                        const float f58 = S(W(p134) * fff);                               // 0049A306..0049A30F
                        const double wJTx = W(w10) * W(JT->x);                            // 0049A313..0049A317
                        const double wJTy = W(w10) * W(JT->y);                            // 0049A319..0049A31D
                        const float wJTz188 = S(W(w10) * W(JT->z));                       // 0049A320..0049A327
                        const double waTx = W(wa30) * W(T->x);                            // 0049A32E..0049A334
                        const float waTy16c = S(W(wa30) * W(T->y));                       // 0049A336..0049A33B
                        const float waTz170 = S(W(wa30) * W(T->z));                       // 0049A342..0049A347
                        const float t198 = S(waTx + wJTx);                                // 0049A34E..0049A350
                        const double ty = W(waTy16c) + wJTy;                              // 0049A357..0049A360
                        const double tz = W(waTz170) + W(wJTz188);                        // 0049A364..0049A36B
                        const double wJx2 = W(w10) * W(J->x);                             // 0049A372..0049A376
                        const double wJy2 = W(w10) * W(J->y);                             // 0049A378..0049A37C
                        const float wJz1b0 = S(W(w10) * W(J->z));                         // 0049A37F..0049A386
                        const float a20 = S(W(wa30) * W(A->x));                           // 0049A38D..0049A395
                        const float a24 = S(W(wa30) * W(A->y));                           // 0049A399..0049A39E
                        const float a28 = S(W(wa30) * W(A->z));                           // 0049A3A2..0049A3A5
                        const float p120 = S(W(a20) + wJx2);                              // 0049A3A9..0049A3AF
                        const double py2 = W(a24) + wJy2;                                 // 0049A3B6..0049A3BC
                        const float p128 = S(W(a28) + W(wJz1b0));                         // 0049A3C0..0049A3CB
                        const float d150 = S(W(p120) - W(t198));                          // 0049A3D2..0049A3E0
                        const float d154 = S(py2 - ty);                                   // 0049A3E7..0049A3E9
                        const double dz = W(p128) - tz;                                   // 0049A3F0..0049A3F9
                        const double hx = W(h14) * W(d150);                               // 0049A3FD..0049A401
                        const float h100 = S(W(d154) * W(h14));                           // 0049A408..0049A413
                        const float h104 = S(dz * W(h14));                                // 0049A41A..0049A420
                        const Vector* const Jm = J - 1;
                        const Vector* const Am = A - 1;
                        const Vector* const JTm = JT - 1;
                        const Vector* const Tm = T - 1;
                        const double wJTxm = W(w10) * W(JTm->x);                          // 0049A427..0049A42B
                        const double wJTym = W(w10) * W(JTm->y);                          // 0049A42E..0049A432
                        const float wJTzm1bc = S(W(w10) * W(JTm->z));                     // 0049A435..0049A43C
                        const float t84 = S(W(wa30) * W(Tm->x));                          // 0049A443..0049A44C
                        const float t88 = S(W(wa30) * W(Tm->y));                          // 0049A453..0049A460 ([esp+6C] = wa)
                        const float t8c = S(W(wa30) * W(Tm->z));                          // 0049A467..0049A46A
                        const double wJxm = W(w10) * W(Jm->x);                            // 0049A471..0049A475
                        const float j148 = S(W(w10) * W(Jm->y));                          // 0049A478..0049A47F
                        const float j14c = S(W(w10) * W(Jm->z));                          // 0049A486..0049A48D
                        const float a44 = S(W(wa30) * W(Am->x));                          // 0049A494..0049A49B
                        const float a48 = S(W(wa30) * W(Am->y));                          // 0049A49F..0049A4A6
                        const float a4c = S(W(wa30) * W(Am->z));                          // 0049A4AA..0049A4B1
                        const float p174 = S(W(a44) + wJxm);                              // 0049A4B5..0049A4BB
                        const double y1 = W(a48) + W(j148);                               // 0049A4C4..0049A4C8
                        const float p17c = S(W(a4c) + W(j14c));                           // 0049A4CF..0049A4DA
                        const float q90 = S(W(p174) + W(t84));                            // 0049A4E1..0049A4EF
                        const float q94 = S(y1 + W(t88));                                 // 0049A4F6..0049A4FD
                        const float q98 = S(W(p17c) + W(t8c));                            // 0049A504..0049A512
                        const float qa0 = S(W(q90) + wJTxm);                              // 0049A519..0049A522
                        const double qy = W(q94) + wJTym;                                 // 0049A529..0049A532
                        const double qz = W(q98) + W(wJTzm1bc);                           // 0049A536..0049A53D
                        const float h15c = S(W(h64) * W(qa0));                            // 0049A544..0049A54F
                        const float h160 = S(qy * W(h64));                                // 0049A556..0049A55C
                        const double h64z = qz * W(h64);                                  // 0049A563
                        const double wJxm2 = W(w10) * W(Jm->x);                           // 0049A567..0049A56B
                        const double wJym2 = W(w10) * W(Jm->y);                           // 0049A56E..0049A572
                        const float j40 = S(W(w10) * W(Jm->z));                           // 0049A575..0049A57C
                        const float ae4 = S(W(wa30) * W(Am->x));                          // 0049A580..0049A587 ([esp+60] = wa)
                        const float ae8 = S(W(wa30) * W(Am->y));                          // 0049A594..0049A59B
                        const float aec = S(W(wa30) * W(Am->z));                          // 0049A5A2..0049A5B0
                        Vector* const out = particle_pos_at(self, p);                     // 0049A58E / 0049A5A9..0049A5AC
                        ++p;                                                              // 0049A5AF / 0049A5BE
                        const float p138 = S(W(ae4) + wJxm2);                             // 0049A5B7..0049A5C4
                        const float p13c = S(W(ae8) + wJym2);                             // 0049A5CB..0049A5D4
                        const double p1z = W(aec) + W(j40);                               // 0049A5DF..0049A5E6
                        const double hx9 = W(h9c) * W(p138);                              // 0049A5EA..0049A5F1
                        const float h10c = S(W(p13c) * W(h9c));                           // 0049A5F8..0049A606
                        const float h110 = S(p1z * W(h9c));                               // 0049A60D..0049A616
                        const float sd4 = S(hx9 + W(h15c));                               // 0049A61D..0049A624
                        const float sd8 = S(W(h10c) + W(h160));                           // 0049A62B..0049A639
                        const float sdc = S(W(h110) + h64z);                              // 0049A640..0049A649
                        const double X = hx + W(sd4);                                     // 0049A652
                        const float sf4 = S(W(sd8) + W(h100));                            // 0049A659..0049A667
                        const float sf8 = S(W(sdc) + W(h104));                            // 0049A66E..0049A67C
                        const float ox = S(X + W(f50));                                   // 0049A683..0049A687
                        const float oy = S(W(sf4) + fpy);                                 // 0049A695..0049A6A0
                        const float oz = S(W(sf8) + W(f58));                              // 0049A6B5..0049A6C0
                        out->x = ox; out->y = oy; out->z = oz;                            // 0049A69C..0049A6CE
                    } while (p < field<I>(self, 0x1CC) - 1);                              // 0049A6D1..0049A6DA
                }
            }
        }
        // ---- 00499965: bolt between the two ends
        {
            const Vector* const head = particle_pos_at(self, 0);                          // 00499965..00499976
            Vector d;
            d.x = S(W(head->x) - W(last_pos->x));                                         // 0049996D..00499989
            d.y = S(W(head->y) - W(last_pos->y));                                         // 00499991..0049999F
            d.z = S(W(head->z) - W(last_pos->z));                                         // 004999A3..004999BF
            vac = d;                                                                      // 004999AD..004999D0 [esp+AC..B4]
        }
        Vector v70 = {0.0f, 0.0f, 0.0f};                                                  // 00499979..00499994
        if (aurora_vector_equal_004aa980(&vac, &v70) == 0) {                              // 004999C7..004999E1
            field<U>(self, 0xF4) = 0;                                                     // 004999E7..004999E9
            if (emitter_bss_0083049c != 0) {                                              // 004999EF..004999FD
                const I depth = ftol2(vector_magnitude_004ab100(&vac) * W(field<float>(self, 0xC4)) * W(F(kHalf_0073e9ac))); // 00499A03..00499A20
                Vector vbc;
                const Vector n = *normalize_004ab180(&vbc, &vac);                         // 00499A19..00499A3C
                v114 = n;                                                                 // 00499A32..00499A83 [esp+114..11C]
                Vector perp;                                                              // [esp+18C..194]
                perp.x = S(-W(n.y));                                                      // 00499A43..00499A66
                perp.y = n.z;                                                             // 00499A83 (raw)
                perp.z = S(-W(n.x));                                                      // 00499A57..00499A92
                Vector vc4;
                const Vector cross = *vector_cross_004ab210(&vc4, &v114, &perp);          // 00499A8A..00499AA2
                part_emitter_recursive_fractal_0048e700(self, nullptr, depth, depth, 1, perp, cross, scale80); // 00499AA7..00499AEF
                const double length = std::sqrt(W(vac.z) * W(vac.z) + W(vac.y) * W(vac.y) + W(vac.x) * W(vac.x)); // 00499AF4..00499B34 (fsqrt: IEEE sqrt, exact)
                const float step18 = S(length / rand_int_as_double(field<I>(self, 0x1CC)) * 0.5); // 00499B36..00499B40 (fidiv count; fmul qword 0.5)
                for (I k = 1; k < field<I>(self, 0x1CC) - 1; ) {                          // 00499B02..00499B44 / 0049A7F1..0049A7FA
                    if (!(W(step18) >= 0.01)) break;                                          // 00499B50..00499B5F (fcomp qword 00743310; test ah,1)
                    float v38[3];
                    if ((crt_rand_006fc447() & 1) != 0) {                                 // 00499B65..00499B6C
                        v38[0] = v114.x; v38[1] = v114.y; v38[2] = v114.z;                // 00499B72..00499B87
                    } else {
                        v38[0] = S(-W(v114.x)); v38[1] = S(-W(v114.y)); v38[2] = S(-W(v114.z)); // 0049A6E5..0049A714
                    }
                    const I r = crt_rand_006fc447();                                      // 0049A724
                    const I divisor = ftol2(W(step18) * W(F(kHundred_00741838)));         // 0049A729..0049A735
                    const I m = r % divisor;                                              // 0049A73E..0049A73F
                    const U scale14 = field<U>(self, 0x24);                               // 0049A741..0049A74A
                    Vector* const pos = particle_pos_at(self, k);                         // 0049A744..0049A751
                    ++k;                                                                  // 0049A754
                    const U scale1c = field<U>(self, 0xC0);                               // 0049A75D..0049A763
                    const double sm = rand_int_as_double(m) * 0.01;                       // 0049A759..0049A767 (fmul qword 00743310)
                    const double x1 = W(v38[0]) * sm * W(F(scale1c));                     // 0049A76D..0049A78B
                    const float y74 = S(W(v38[1]) * sm);                                  // 0049A773..0049A779
                    const float z78 = S(W(v38[2]) * sm);                                  // 0049A77D..0049A783
                    const double y1 = W(y74) * W(F(scale1c));                             // 0049A78D..0049A791
                    const float zc4 = S(W(z78) * W(F(scale1c)));                          // 0049A795..0049A79D
                    const float x180 = S(x1 * W(F(scale14)));                             // 0049A7A4..0049A7AA
                    const double y2 = y1 * W(F(scale14));                                 // 0049A7B1
                    const double z2 = W(zc4) * W(F(scale14));                             // 0049A7B5..0049A7BC
                    const float nx = S(W(x180) + W(pos->x));                              // 0049A7C0..0049A7C9
                    const float ny = S(y2 + W(pos->y));                                   // 0049A7D1..0049A7D6
                    const float nz = S(z2 + W(pos->z));                                   // 0049A7DE..0049A7E6
                    pos->x = nx; pos->y = ny; pos->z = nz;                                // 0049A7E1..0049A7EE
                }
            } else {
                // ---- 0049A805: spiral offsets into +270
                const double length = std::sqrt(W(vac.z) * W(vac.z) + W(vac.y) * W(vac.y) + W(vac.x) * W(vac.x)); // 0049A805..0049A83B
                const float scale1c = S(length / rand_int_as_double(field<I>(self, 0x1CC)) * 0.5
                                        * W(field<float>(self, 0xC0)) * W(field<float>(self, 0x24))); // 0049A83D..0049A852
                Vector vbc;
                const Vector* const dir = normalize_004ab180(&vbc, &vac);                 // 0049A80C..0049A856
                Vector v44;
                v44.x = S(W(scale1c) * W(dir->x));                                        // 0049A85B..0049A864
                v44.y = S(W(scale1c) * W(dir->y));                                        // 0049A868..0049A86F
                v44.z = S(W(scale1c) * W(dir->z));                                        // 0049A873..0049A888
                Vector v38 = field<Vector>(self, 0x27C);                                  // 0049A87A..0049A89A
                const I twist = ftol2(W(field<float>(self, 0xC8)));                       // 0049A88C..0049A8AB [esp+18]
                Vector axis;                                                              // [esp+20..28]
                {
                    Vector vbc2;
                    const Vector* const gob_pos = gob_get_position(field<void*>(self, 0x1E4), &vbc2); // 0049A8A3..0049A8B7
                    axis.x = S(W(gob_pos->x) - W(field<float>(self, 0x164)));             // 0049A8BA..0049A8D0
                    axis.y = S(W(gob_pos->y) - W(field<float>(self, 0x168)));             // 0049A8D4..0049A8E1
                    axis.z = S(W(gob_pos->z) - W(field<float>(self, 0x16C)));             // 0049A8E5..0049A8EE
                }
                I angle = 0;                                                              // 004999E7 ebp = 0
                U out_bytes = 0x0C;                                                       // 0049A8F8 [esp+14]
                for (I k = 1; k < field<I>(self, 0x1CC) - 1; ++k) {                       // 0049A8C2..0049A8F2 / 0049AB2C..0049AB35
                    if (twist != 0) {                                                     // 0049A900..0049A906
                        I step = crt_rand_006fc447() % twist;                             // 0049A90C..0049A916
                        if ((crt_rand_006fc447() & 1) != 0) step = 0x168 - step;          // 0049A918..0049A928
                        angle += step;                                                    // 0049A92E
                        const float radians = S(rand_int_as_double(angle) * W(F(kDegToRad_007415f8))); // 0049A934..0049A94C
                        Quaternion q1b4;
                        quaternion_construct_004ac960(&q1b4, nullptr, axis, radians);     // 0049A943..0049A963
                        Vector v180;
                        v38 = *quaternion_rotate_004a9bb0(&v180, &q1b4, &v38);            // 0049A968..0049A989
                    }
                    float frac80 = S(rand_int_as_double(k) / rand_int_as_double(field<I>(self, 0x1CC))); // 0049A98D..0049A997
                    if (rand_int_as_double(k) / rand_int_as_double(field<I>(self, 0x1CC)) > W(F(kHalf_0073e9ac))) // 0049A99E..0049A9A9 (wide ST)
                        frac80 = S(W(F(kOne_0073d764)) - W(frac80));                      // 0049A9AB..0049A9B8
                    const I r1 = crt_rand_006fc447() % 0x64;                              // 0049A9BF..0049A9CA
                    const float radius1c = S(rand_int_as_double(r1) * ((W(frac80) + W(frac80)) * W(field<float>(self, 0xBC)))
                                             * W(F(kHundredth_0073dfec)));                // 0049A9D0..0049A9EB
                    const I r2 = crt_rand_006fc447() % 0x64;                              // 0049A9EF..0049A9FA
                    const double s2 = rand_int_as_double(r2) * W(F(kHundredth_0073dfec)); // 0049AA00..0049AA04
                    float ja0 = S(W(v44.x) * s2), ja4 = S(W(v44.y) * s2), ja8 = S(W(v44.z) * s2); // 0049AA0A..0049AA40
                    float j50 = ja0, j54 = ja4, j58 = ja8;                                // 0049AA22 / 0049AA3C / 0049AA50
                    if ((crt_rand_006fc447() & 1) != 0) {                                 // 0049AA54..0049AA5B
                        j50 = S(-W(ja0)); j54 = S(-W(ja4)); j58 = S(-W(ja8));             // 0049AA5D..0049AAAA
                    }
                    Vector* const dst = reinterpret_cast<Vector*>(field<U>(self, 0x270) + out_bytes); // 0049AAB2..0049AAC4
                    const double ox = W(radius1c) * W(v38.x);                             // 0049AAAE..0049AAB8
                    const double oy = W(v38.y) * W(radius1c);                             // 0049AAC0..0049AAC6
                    const float zc4 = S(W(v38.z) * W(radius1c));                          // 0049AACB..0049AADE
                    out_bytes += 0x0C;                                                    // 0049AACF..0049AADA
                    dst->x = S(ox + W(j50));                                              // 0049AAE5..0049AAF9
                    dst->y = S(oy + W(j54));                                              // 0049AAFB..0049AB18
                    dst->z = S(W(zc4) + W(j58));                                          // 0049AB06..0049AB29
                }
                // branch anchors follow their target particle + offset (0049AB3B..0049AC92)
                if (field<I>(node, 0x5C) > 0 && field<I>(self, 0x25C) != 0) {             // 0049AB3B..0049AB55
                    for (I i = 0; i < field<I>(self, 0x288); ++i) {                       // 0049AB5B..0049AB65 / 0049AC89..0049AC92
                        const I target = target_at(self, i);                              // 0049AB7F..0049AB85
                        const Vector* const offset = &reinterpret_cast<const Vector*>(field<U>(self, 0x270))[target]; // 0049AB88..0049AB94
                        const Vector* const ppos = particle_pos_at(self, target);         // 0049AB97..0049ABA3
                        Vector anchor;
                        anchor.x = S(W(offset->x) + W(ppos->x));                          // 0049AB91..0049ABA6
                        anchor.y = S(W(ppos->y) + W(offset->y));                          // 0049ABAA..0049ABB0
                        anchor.z = S(W(ppos->z) + W(offset->z));                          // 0049ABB4..0049ABC7
                        vec_data(branch_at(self, i), 0x228)[0] = anchor;                  // 0049AB70..0049ABCF
                        vec_data(branch_at(self, i), 0x21C)[0] = anchor;                  // 0049ABD2..0049ABE6
                        field<Vector>(branch_at(self, i), 0x164) = anchor;                // 0049ABE9..0049ABFD
                        void* const branch = branch_at(self, i);                          // 0049AC00..0049AC06
                        Vector vbc3;
                        const Vector* const bpos = gob_get_position(field<void*>(branch, 0x1E4), &vbc3); // 0049AC09..0049AC19
                        const float dx = S(W(bpos->x) - W(field<float>(branch, 0x164)));  // 0049AC1C..0049AC24
                        const float dy = S(W(bpos->y) - W(field<float>(branch, 0x168)));  // 0049AC2C..0049AC35
                        const float dz = S(W(bpos->z) - W(field<float>(branch, 0x16C)));  // 0049AC39..0049AC5A
                        Vector* const d1f8 = static_cast<Vector*>(at(branch_at(self, i), 0x1F8)); // 0049AC3C..0049AC4F
                        d1f8->x = dx; d1f8->y = dy; d1f8->z = dz;                         // 0049AC55..0049AC62
                        void* const again = branch_at(self, i);                           // 0049AC65..0049AC6B
                        field<Vector>(again, 0x204) = field<Vector>(again, 0x164);        // 0049AC6E..0049AC86
                    }
                }
            }
        }
    } else {
        // ---- 0049AC9D: follow the gob: rotate everything by the arc from the old to the new end direction
        Vector vbc;
        const Vector* const gob_pos = gob_get_position(field<void*>(self, 0x1E4), &vbc);  // 0049AC9D..0049ACAD
        Vector to50;
        to50.x = S(W(gob_pos->x) - W(field<float>(self, 0x164)));                         // 0049ACB0..0049ACC4
        to50.y = S(W(gob_pos->y) - W(field<float>(self, 0x168)));                         // 0049ACC8..0049ACD3
        to50.z = S(W(gob_pos->z) - W(field<float>(self, 0x16C)));                         // 0049ACD7..0049ACED
        Quaternion q198;
        rotation_arc_004ab630(&q198, field<Vector>(self, 0x1F8), to50);                   // 0049ACE7..0049AD12
        for (I k = 0; k < field<I>(self, 0x1CC); ++k) {                                   // 0049AD17..0049AD24 / 0049ADCB..0049ADD4
            const Vector* const p = particle_pos_at(self, k);                             // 0049AD30..0049AD3C
            Vector d70;
            const double dz = W(p->z) - W(field<float>(self, 0x20C));                     // 0049AD39..0049AD3F
            const double dy = W(p->y) - W(field<float>(self, 0x208));                     // 0049AD4C..0049AD56
            d70.x = S(W(p->x) - W(field<float>(self, 0x204)));                            // 0049AD5C..0049AD6B
            d70.y = S(dy);                                                                // 0049AD6F
            d70.z = S(dz);                                                                // 0049AD76
            Vector vbc2;
            const Vector* const r = quaternion_rotate_004a9bb0(&vbc2, &q198, &d70);       // 0049AD68..0049AD7D
            const float nx = S(W(r->x) + W(field<float>(self, 0x164)));                   // 0049AD82..0049AD8D
            const float ny = S(W(r->y) + W(field<float>(self, 0x168)));                   // 0049AD95..0049AD9E
            const float nz = S(W(r->z) + W(field<float>(self, 0x16C)));                   // 0049ADA2..0049ADBD
            Vector* const out = particle_pos_at(self, k);                                 // 0049ADA5..0049ADB8
            out->x = nx; out->y = ny; out->z = nz;                                        // 0049ADBB..0049ADC8
        }
        for (I k = 0; k < field<I>(self, 0x214); ++k) {                                   // 0049ADDA..0049ADE4 / 0049AEB3..0049AEBF
            const Vector* const point = data_vec(self, 0x210, k);                         // 0049ADF0..0049ADFA
            Vector d70;
            const double dz = W(point->z) - W(field<float>(self, 0x20C));                 // 0049ADF6..0049ADFC
            const double dy = W(point->y) - W(field<float>(self, 0x208));                 // 0049AE09..0049AE13
            d70.x = S(W(point->x) - W(field<float>(self, 0x204)));                        // 0049AE19..0049AE28
            d70.y = S(dy);                                                                // 0049AE2C
            d70.z = S(dz);                                                                // 0049AE33
            Vector vbc2;
            const Vector* const r = quaternion_rotate_004a9bb0(&vbc2, &q198, &d70);       // 0049AE25..0049AE3A
            const float nx = S(W(r->x) + W(field<float>(self, 0x164)));                   // 0049AE3F..0049AE47
            const float ny = S(W(r->y) + W(field<float>(self, 0x168)));                   // 0049AE4F..0049AE58
            const float nz = S(W(r->z) + W(field<float>(self, 0x16C)));                   // 0049AE5C..0049AE76
            Vector* const out = data_vec(self, 0x210, k);                                 // 0049AE5F..0049AE6F
            out->x = nx; out->y = ny; out->z = nz;                                        // 0049AE71..0049AE7E
            Vector* const tangent = data_vec(self, 0x234, k);                             // 0049AE81..0049AE87
            Vector v180;
            *tangent = *quaternion_rotate_004a9bb0(&v180, &q198, tangent);                // 0049AE8A..0049AEB0
        }
        if (field<I>(node, 0x5C) > 0 && field<I>(self, 0x25C) != 0) {                     // 0049AEC5..0049AEDF
            for (I i = 0; i < field<I>(self, 0x288); ++i) {                               // 0049AEE5..0049AEEF / 0049B074..0049B07D
                const I target = target_at(self, i);                                      // 0049AF0F..0049AF15
                const Vector* const offset = &reinterpret_cast<const Vector*>(field<U>(self, 0x270))[target]; // 0049AF18..0049AF24
                const Vector* const ppos = particle_pos_at(self, target);                 // 0049AF27..0049AF33
                Vector anchor;
                anchor.x = S(W(offset->x) + W(ppos->x));                                  // 0049AF21..0049AF36
                anchor.y = S(W(ppos->y) + W(offset->y));                                  // 0049AF3A..0049AF40
                anchor.z = S(W(ppos->z) + W(offset->z));                                  // 0049AF44..0049AF57
                vec_data(branch_at(self, i), 0x228)[0] = anchor;                          // 0049AF00..0049AF5F
                vec_data(branch_at(self, i), 0x21C)[0] = anchor;                          // 0049AF62..0049AF76
                field<Vector>(branch_at(self, i), 0x164) = anchor;                        // 0049AF79..0049AF8D
                void* const bgob = field<void*>(branch_at(self, i), 0x1E4);               // 0049AF90..0049AF99
                Vector vbc2;
                const Vector bpos = *gob_get_position(bgob, &vbc2);                       // 0049AF9F..0049AFC6 [esp+44..4C]
                Vector d70;
                d70.x = S(W(bpos.x) - W(field<float>(self, 0x204)));                      // 0049AFB8..0049AFCE
                d70.y = S(W(bpos.y) - W(field<float>(self, 0x208)));                      // 0049AFD3..0049AFED
                d70.z = S(W(bpos.z) - W(field<float>(self, 0x20C)));                      // 0049AFF4..0049AFFE
                Vector v180;
                const Vector* const r = quaternion_rotate_004a9bb0(&v180, &q198, &d70);   // 0049AFCA..0049B005
                Vector moved;
                moved.x = S(W(field<float>(self, 0x164)) + W(r->x));                      // 0049B00A..0049B012
                moved.y = S(W(r->y) + W(field<float>(self, 0x168)));                      // 0049B019..0049B022
                moved.z = S(W(r->z) + W(field<float>(self, 0x16C)));                      // 0049B029..0049B038
                void* const bgob2 = field<void*>(branch_at(self, i), 0x1E4);              // 0049B032..0049B042
                Vector v1a8;
                gob_set_position(bgob2, &v1a8, moved);                                    // 0049B048..0049B071
            }
        }
        field<Vector>(self, 0x1F8) = to50;                                                // 0049B083..0049B0AF
        field<Vector>(self, 0x204) = field<Vector>(self, 0x164);                          // 0049B09B..0049B0BE
    }
    // ---- 0049B0C1: update the branches
    if (field<I>(node, 0x5C) > 0 && field<U8>(self, 0x28C) == 0) {                        // 0049B0C1..0049B0DB
        for (I i = 0; i < field<I>(self, 0x288); ++i) {                                   // 0049B0E1..0049B0EB / 0049B128..0049B131
            void* const branch = branch_at(self, i);                                      // 0049B102..0049B108
            const U arg = branches_reset_6b
                ? float_to_bits(S(W(field<float>(self, 0xB8)) + W(F(kHundredth_0073dfec)))) // 0049B10F..0049B11C
                : delta_bits;                                                             // 0049B121 push ebp ([esp+1D4])
            part_table(branch)->s080(branch, nullptr, arg);                               // 0049B10B / 0049B122 (vtbl+80 Update)
        }
    }
    return 0;   // EAX is not a defined value of this void member (callers through vtbl+80 ignore it)
}

} // extern "C"
