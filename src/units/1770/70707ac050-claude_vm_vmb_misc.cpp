// Lane vmB, file "misc": math / string / vector / time / sound-object / lock / encounter NWScript engine-command
// handlers of package vmB (CSWVirtualMachineCommands::ExecuteCommand*), reconstructed from the original swkotor.exe
// (read-only, capstone).  Every handler: original int __thiscall (int routine, int argc), RET 8 (verified) ->
//   extern "C" int __fastcall kotor_fastcall_<addr>(self, edx, routine, argc) @16.
// Addresses in comments are original instruction ranges.  Conventions: claude_vm_vmB_common.hpp.
// .rdata facts used as literals: [00747E00] double 0.017453292519943295; [0073EDE8] double 1.0; [0073EDF8] double 0.0;
// [0073D700] float 0.0; [00742EF8] float 57.29578 (42652EE1h); [007415F8] float 0.017453292 (3C8EFA35h);
// [00741868] float 360.0; [007486D8] "0x%08x"; [0073D71C] ""; [0073ECD0] byte 0; [00746639] byte 0Ah (door type);
// [00746638] byte 09h (placeable type).
// x87: the handlers' FPU sequences are kept as inline __asm (53-bit precision control, dword/qword widths as in the
// original); CRT ST0 routines are called through their linked providers.
#include "54db204e3b-claude_vm_vmb_common.hpp"
#include <cmath>

namespace kotor_movie { void* __fastcall string_element_construct_005b3190(void* string, void*); }   // CExoString() [0603]
namespace table29_required { void __fastcall gui_volume_005d5b50(void* source, void*, unsigned volume); } // CExoStreamingSoundSource::SetVolume [0674]

extern "C" {
// ---- existing providers (c31 map; object in brackets) ----
struct CrtExtendedVmb { unsigned char bytes[10]; };
void __cdecl crt_acos_x87_006fc0e0(const CrtExtendedVmb* st0_in, CrtExtendedVmb* st0_out);   // facos ST0->ST0 transported [t11b]
void __cdecl pointto_fasin_006fcff0();                                                        // fasin ST0->ST0 [asin-shell]
void __cdecl genuine_x87_pow_006fc1f0();                                                      // _CIpow ST1^ST0 -> ST0 [0365]
void __cdecl frame_LogFormat_005e5680(void* string, const char* format, ...);                 // CExoString::Format [0125]
void __fastcall frame_GetWorldTime_004ade40(void* timer, void*, U* day, U* time);             // CWorldTimer::GetWorldTime [0457]
void* __fastcall kotor_fastcall_004aede0(void* server, void*);                                // GetWorldTimer [0457]
U __fastcall tlk_fetch_0041e550(void* tlk, void*, U strref, void* out, U flag);               // CTlkTable::Fetch RET 0Ch [0385]
extern void* kotor_cell_tlk_007a3a08;                                                         // [007A3A08] CTlkTable*
void* __fastcall recovered_expression_construct_0059d990(void* e, void*);                     // CSWSExpression() [vm_actions_t1]
void __fastcall recovered_expression_list_delete_list_0059d920(void* list, void*);            // CSWSExpressionList::DeleteList [vm_actions_t1]
I __fastcall recovered_expression_parse_string_0059dab0(void* e, void*, vmx::ExoString by_value); // ParseString(CExoString) RET 8 (callee destroys) [vm_actions_t1]
I __fastcall recovered_expression_test_string_0059e170(void* e, void*, void* string, I flag); // TestString RET 8 [w2_broadcast_native]
void* __fastcall kotor_fastcall_004ae800(void* server, void*, U id);                          // GetSoundObjectByGameObjectID [claude_vm_actions_game]
void* __fastcall recovered_client_get_sound_object_005ed620(void* client, void*, U id);       // CClientExoApp::GetSoundObjectByGameObjectID [1204-01]
void __fastcall kotor_fastcall_005d5950(void* source, void*, U volume);                       // CExoSoundSource::SetVolume RET 4 [1205-24]
void* __fastcall recovered_placeable_lookup_004ae7a0(void* server, void*, U id);              // GetPlaceableByGameObjectID [0886]
U __fastcall recovered_placeable_light_get_00585130(void* placeable, void*);                  // CSWSPlaceable::GetLightIsOn [0886]
U __fastcall recovered_module_get_waypoint_004c6e00(void* module, void*, const void* tag);    // CSWSModule::GetWaypoint RET 4 [vm_actions_t4]
U __fastcall recovered_module_find_object_by_tag_004c5fe0(void* module, void*, const void* tag, int nth); // FindObjectByTagOrdinal RET 8 [vm_actions_t3]
void* __fastcall w5_door_get_linked_object_00589580(void* door, void*);                       // CSWSDoor::GetLinkedObject [1206-14]
void* __fastcall kotor_fastcall_004ae7f0(void* server, void*, U id);                          // GetEncounterByGameObjectID [claude_w5_creature_actions]
// ---- reconstructed in this file (section at the end) ----
U __fastcall kotor_fastcall_004ae090(void* timer, void*);                                     // CWorldTimer::GetWorldTimeHour
U __fastcall kotor_fastcall_004ae140(void* timer, void*);                                     // CWorldTimer::GetWorldTimeMillisecond
void __fastcall kotor_fastcall_005e57b0(void* string, void*, const void* other, I position);  // CExoString::Insert RET 8
void __fastcall kotor_fastcall_0059d9e0(void* expression, void*);                             // CSWSExpression::~CSWSExpression
void __fastcall kotor_fastcall_005c8df0(void* sound, void*, I volume);                        // CSWSSoundObject::ChangeVolume RET 4
void __fastcall kotor_fastcall_0066ad00(void* sound, void*, U volume);                        // CSWCSoundObject::ChangeVolume RET 4
}

using namespace vmb;

namespace {
const double k_deg_to_rad_00747e00 = 0.017453292519943295;   // .rdata 00747E00 (3F91DF46A2529D39h)
const float k_rad_to_deg_00742ef8 = 57.29578f;               // .rdata 00742EF8 (42652EE1h)
const U k_deg_to_rad_f_007415f8 = 0x3c8efa35u;               // .rdata 007415F8 float 0.017453292
// inline strlen + rep movsd/movsb copy of the terminated source onto the end of dst (the original inlines strcat)
inline void append(char* dst, const char* src) {
    while (*dst) ++dst;
    do { *dst++ = *src; } while (*src++);
}
inline void str_empty(ExoString* s) { kotor_movie::string_element_construct_005b3190(s, nullptr); }

// ---- Math x87 sequences (0053F641..0053F795): result stored as a dword ----
float m_fabs(float x) { float r; __asm { fld dword ptr x
    fabs
    fstp dword ptr r } return r; }
float m_cos(float x) { float r; __asm { fld dword ptr x
    fmul qword ptr k_deg_to_rad_00747e00
    fcos
    fstp dword ptr r } return r; }
float m_sin(float x) { float r; __asm { fld dword ptr x
    fmul qword ptr k_deg_to_rad_00747e00
    fsin
    fstp dword ptr r } return r; }
float m_tan(float x) { float r; __asm { fld dword ptr x
    fmul qword ptr k_deg_to_rad_00747e00
    fptan
    fstp st(0)
    fstp dword ptr r } return r; }
float m_acos_deg(float x) {                                   // fld x; call facos; fmul dword [742EF8]; fstp dword
    CrtExtendedVmb operand, image;
    CrtExtendedVmb* const in = &operand;
    CrtExtendedVmb* const out = &image;
    float r;
    __asm { fld dword ptr x
        mov eax, in
        fstp tbyte ptr [eax] }
    crt_acos_x87_006fc0e0(in, out);
    __asm { mov eax, out
        fld tbyte ptr [eax]
        fmul dword ptr k_rad_to_deg_00742ef8
        fstp dword ptr r }
    return r;
}
float m_acos_raw(float x) {                                   // fld x; call facos; fstp dword (VectorToAngle)
    CrtExtendedVmb operand, image;
    CrtExtendedVmb* const in = &operand;
    CrtExtendedVmb* const out = &image;
    float r;
    __asm { fld dword ptr x
        mov eax, in
        fstp tbyte ptr [eax] }
    crt_acos_x87_006fc0e0(in, out);
    __asm { mov eax, out
        fld tbyte ptr [eax]
        fstp dword ptr r }
    return r;
}
float m_asin_deg(float x) { float r; __asm { fld dword ptr x
    call pointto_fasin_006fcff0
    fmul dword ptr k_rad_to_deg_00742ef8
    fstp dword ptr r } return r; }
float m_atan_deg(float x) { float r; __asm { fld dword ptr x
    fld1
    fpatan
    fmul dword ptr k_rad_to_deg_00742ef8
    fstp dword ptr r } return r; }
float m_log(float x) { float r; __asm { fldln2
    fld dword ptr x
    fyl2x
    fstp dword ptr r } return r; }
float m_pow(float x, float y) { float r; __asm { fld dword ptr x
    fld dword ptr y
    call genuine_x87_pow_006fc1f0
    fstp dword ptr r } return r; }
float m_sqrt(float x) { float r; __asm { fld dword ptr x
    fsqrt
    fstp dword ptr r } return r; }
// GetFacingFromLocation: fld [loc+10]; fld [loc+0C]; fpatan; fmul dword [742EF8]; fstp dword
float m_facing(const float* y, const float* x) { float r; __asm { mov eax, y
    fld dword ptr [eax]
    mov eax, x
    fld dword ptr [eax]
    fpatan
    fmul dword ptr k_rad_to_deg_00742ef8
    fstp dword ptr r } return r; }
// AngleToVector: fld f; fmul dword [7415F8]; fld st(0); fcos; fstp dword; fsin; fstp dword
void m_angle_to_vector(float f, float* c, float* s) { float rc, rs; __asm { fld dword ptr f
    fmul dword ptr k_deg_to_rad_f_007415f8
    fld st(0)
    fcos
    fstp dword ptr rc
    fsin
    fstp dword ptr rs }
    *c = rc; *s = rs; }

// CSWSExpression is 0x20 bytes (0059D990): +0, +4 CExoString** data, +8 count, +0C alloc, +10, +14 list, +18 CExoString
struct Expression { U d[6]; ExoString text; };
static_assert(sizeof(Expression) == 0x20, "CSWSExpression is 0x20 bytes");
// CTlkTable::Fetch result (STR_RES, 0x1C bytes): CExoString text, CResRef sound (0x10), float sound length
struct StrRes { ExoString text; unsigned char sound[0x10]; float length; };
static_assert(sizeof(StrRes) == 0x1c, "STR_RES is 0x1C bytes");
}  // namespace

extern "C" {

// ===================================================================================================== 0053F5C0
// Math: abs (77, integer) and fabs/cos/sin/tan/acos/asin/atan/log/pow/sqrt (67..76, float) [jump table 0053F7CC].
// The float result lives in the routine argument slot, zeroed before the pop; out-of-domain inputs give 0.0.
int __fastcall kotor_fastcall_0053f5c0(void*, void*, int routine, unsigned) {
    if (routine == 0x4d) {                                                               // 0053F5C8..CB
        I value = routine;                                                               // popped into the routine slot
        if (!pop_int(&value)) return k_pop_failed;                                       // 0053F5CD..DF
        const U sign = static_cast<U>(value >> 31);                                      // 0053F5EF cdq
        return push_int(static_cast<I>((static_cast<U>(value) ^ sign) - sign));          // 0053F5F0..0B xor/sub
    }
    float x = 0.0f;
    float result = 0.0f;                                                                 // 0053F619 slot = 0
    if (!pop_float(&x)) return k_pop_failed;                                             // 0053F60E..28
    const U index = static_cast<U>(routine - 0x43);                                      // 0053F62E..34 (ja unsigned)
    if (index <= 9) {
        switch (index) {
        case 0: result = m_fabs(x); break;                                               // 0053F641 fabs
        case 1: result = m_cos(x); break;                                                // 0053F650 cos
        case 2: result = m_sin(x); break;                                                // 0053F665 sin
        case 3: result = m_tan(x); break;                                                // 0053F67A tan
        case 4:                                                                          // 0053F691 acos
            // fabs; fcomp qword 1.0; test ah,41h; jp -> 0 when |x| > 1 or unordered
            result = (static_cast<double>(std::fabs(x)) <= 1.0) ? m_acos_deg(x) : 0.0f; break;
        case 5:                                                                          // 0053F6C0 asin
            result = (static_cast<double>(std::fabs(x)) <= 1.0) ? m_asin_deg(x) : 0.0f; break;
        case 6: result = m_atan_deg(x); break;                                           // 0053F6EF atan
        case 7:                                                                          // 0053F706 log: fcomp 0.0; test ah,41h; jne
            result = (x > 0.0f) ? m_log(x) : 0.0f; break;
        case 8: {                                                                        // 0053F729 pow
            float y = 0.0f;
            if (!pop_float(&y)) return k_pop_failed;                                     // 0053F729..3B
            // fcomp qword 0.0; test ah,44h; jp (x != 0 or unordered) -> pow; else fcomp y,0.0; test ah,1; jne -> 0
            if (static_cast<double>(x) == 0.0 && !(y >= 0.0f)) result = 0.0f;           // 0053F749..69
            else result = m_pow(x, y);                                                   // 0053F76B..78
            break;
        }
        default:                                                                         // 0053F77E sqrt: fcomp 0.0; test ah,41h; jne
            result = (x > 0.0f) ? m_sqrt(x) : 0.0f; break;
        }
    }
    return push_float(result);                                                           // 0053F7A3..C9
}

// ===================================================================================================== 0053F500
// GetPositionFromLocation (223) / GetFacingFromLocation (225).  Any other routine pushes nothing and succeeds.
// The popped location is freed (dtor 004015A0 is a bare RET) on every non-pop-failure path.
int __fastcall kotor_fastcall_0053f500(void*, void*, int routine, unsigned) {
    void* location = nullptr;
    if (!pop_engine(2, &location)) return k_pop_failed;                                  // 0053F509..21
    U ok = 1;                                                                            // 0053F52D edi = 1
    if (routine == 0xdf) {                                                               // 0053F528..32
        Vector p; std::memcpy(&p, location, 12);                                         // 0053F561..7D
        ok = push_vector_raw(p);                                                         // 0053F580..85
    } else if (routine == 0xe1) {                                                        // 0053F534..37
        const float* l = static_cast<const float*>(location);
        ok = push_float_raw(m_facing(l + 4, l + 3));                                     // 0053F539..5F (atan2(o.y, o.x) deg)
    }
    if (location != nullptr) { location_dtor_004015a0(location); live_free(location); } // 0053F587..9C
    return ok ? 0 : k_push_failed;                                                       // 0053F59F..AF setne/dec/and
}

// ===================================================================================================== 0053E020
// GetTimeHour() [routine 16]
int __fastcall kotor_fastcall_0053e020(void*, void*, int, unsigned) {
    void* timer = kotor_fastcall_004aede0(server_app(), nullptr);                        // 0053E020..28
    return push_int(static_cast<I>(kotor_fastcall_004ae090(timer, nullptr)));            // 0053E02D..4E
}

// ===================================================================================================== 0053E060
// GetTimeMillisecond() [routine 19]
int __fastcall kotor_fastcall_0053e060(void*, void*, int, unsigned) {
    void* timer = kotor_fastcall_004aede0(server_app(), nullptr);                        // 0053E060..68
    return push_int(static_cast<I>(kotor_fastcall_004ae140(timer, nullptr)));            // 0053E06D..8E
}

// ===================================================================================================== 0053E960
// InsertString(string sDestination, string sString, int nPosition) [routine 64]
int __fastcall kotor_fastcall_0053e960(void*, void*, int, unsigned) {
    ExoString source, destination;                                                       // [esp+10], [esp+8]
    str_empty(&source);                                                                  // 0053E97F..83
    str_empty(&destination);                                                             // 0053E98A..92
    I position = 0;
    if (!pop_string(&destination) || !pop_string(&source) || !pop_int(&position)) {      // 0053E997..D6
        exo_destroy(&destination); exo_destroy(&source);                                 // 0053E9D8..F1
        return k_pop_failed;
    }
    kotor_fastcall_005e57b0(&destination, nullptr, &source, position);                   // 0053EA0D..1B
    const U pushed = push_string_raw(&destination);                                      // 0053EA20..30
    exo_destroy(&destination); exo_destroy(&source);                                     // 0053EA3C..4D / 0053EA69..7A
    return pushed ? 0 : k_push_failed;
}

// ===================================================================================================== 00544EF0
// TestStringAgainstPattern(string sPattern, string sStringToTest) [routine 177]
int __fastcall kotor_fastcall_00544ef0(void*, void*, int, unsigned) {
    ExoString pattern, text;                                                             // [esp+C], [esp+4]
    str_empty(&pattern);                                                                 // 00544F08..0C
    str_empty(&text);                                                                    // 00544F11..1D
    if (!pop_string(&pattern) || !pop_string(&text)) {                                   // 00544F22..51
        exo_destroy(&text); exo_destroy(&pattern);                                       // 0054501B..35
        return k_pop_failed;
    }
    Expression expression;                                                               // [esp+14]
    recovered_expression_construct_0059d990(&expression, nullptr);                       // 00544F57..5B
    ExoString copy;                                                                      // by-value argument
    WindowCopy005e5b00(&copy, nullptr, &pattern);                                        // 00544F60..73
    (void)recovered_expression_parse_string_0059dab0(&expression, nullptr, copy);        // 00544F78..7C (callee destroys copy)
    const I matched = recovered_expression_test_string_0059e170(&expression, nullptr, &text, 1); // 00544F81..8C
    const U pushed = push_int_raw(matched);                                              // 00544F91..9D
    kotor_fastcall_0059d9e0(&expression, nullptr);                                       // 00544FAA / 00544FE4
    exo_destroy(&text); exo_destroy(&pattern);                                           // 00544FAF..C9 / 00544FE9..03
    return pushed ? 0 : k_push_failed;
}

// ===================================================================================================== 005451C0
// AngleToVector (144) / VectorToAngle (145).  Other routines: 0.
// Quirk kept: VectorToAngle returns acos(normalized x) in RADIANS, and 360.0 - radians when normalized y < 0.
int __fastcall kotor_fastcall_005451c0(void*, void*, int routine, unsigned) {
    if (routine == 0x91) {                                                               // 005451C7..CC
        Vector v{0.0f, 0.0f, 0.0f};                                                      // 005451DD..ED
        if (!pop_vector(&v)) return k_pop_failed;                                        // 005451D2..FC
        Vector n;
        normalize_004ab180(&n, &v);                                                      // 00545202..0C
        v = n;                                                                           // 00545211..28 (copied over the local)
        float angle = m_acos_raw(v.x);                                                   // 00545217, 0054522C..31
        if (v.y < 0.0f)                                                                  // 00545235..44 (test ah,5; jp)
            angle = static_cast<float>(360.0 - static_cast<double>(angle));              // 00545246..50
        return push_float(angle);                                                        // 00545254..74
    }
    if (routine == 0x90) {                                                               // 00545277..7C
        float degrees = 0.0f;
        if (!pop_float(&degrees)) return k_pop_failed;                                   // 00545282..B6
        Vector out; out.z = 0.0f;                                                        // 005452C8
        m_angle_to_vector(degrees, &out.x, &out.y);                                      // 005452B9..EC
        return push_status(push_vector_raw(out));                                        // 005452EF..09
    }
    return 0;                                                                            // 0054530C
}

// ===================================================================================================== 00545320
// VectorMagnitude(vector v) [routine 104]
int __fastcall kotor_fastcall_00545320(void*, void*, int, unsigned) {
    Vector v{0.0f, 0.0f, 0.0f};                                                          // 0054532E..3E
    if (!pop_vector(&v)) return k_pop_failed;                                            // 00545323..57
    const float m = static_cast<float>(kotor_cdecl_004ab100(&v));                        // 0054535A..64 (fstp dword)
    return push_float(m);                                                                // 00545368..8C
}

// ===================================================================================================== 0053EAF0
// IntToHexString(int n) [routine 396]: Format "0x%08x"
int __fastcall kotor_fastcall_0053eaf0(void*, void*, int, unsigned) {
    ExoString s; str_empty(&s);                                                          // 0053EB08..0C
    I value = 0;
    if (!pop_int(&value)) { exo_destroy(&s); return k_pop_failed; }                      // 0053EB11..51
    frame_LogFormat_005e5680(&s, "0x%08x", value);                                       // 0053EB54..6E [007486D8]
    const U pushed = push_string_raw(&s);                                                // 0053EB71..7B
    exo_destroy(&s);                                                                     // 0053EB8B / 0053EBA6
    return pushed ? 0 : k_push_failed;
}

// ===================================================================================================== 0053DCA0
// GetStrRefSoundDuration(int nStrRef) [routine 571]: STR_RES sound length, 0.0 for a negative strref or a failed
// fetch.  The TLK pointer [007A3A08] is used unchecked (original).
int __fastcall kotor_fastcall_0053dca0(void*, void*, int, unsigned) {
    StrRes res;                                                                          // [esp+4]
    str_empty(&res.text);                                                                // 0053DCB8..BC
    c2da_default_ref_00405ed0(res.sound, nullptr);                                       // 0053DCC1..CD
    res.length = 0.0f;   // not initialised by the original; only read after a successful Fetch (which fills it)
    I strref = 0;
    if (!pop_int(&strref)) { exo_destroy(&res.text); return k_pop_failed; }              // 0053DCD2..12
    if (strref < 0 ||                                                                    // 0053DD15..1B (jl)
        tlk_fetch_0041e550(kotor_cell_tlk_007a3a08, nullptr, static_cast<U>(strref), &res, 0) == 0) // 0053DD1D..39 [0073ECD0]=0
        res.length = 0.0f;                                                               // 0053DD3B
    const U pushed = push_float_raw(res.length);                                         // 0053DD43..53
    exo_destroy(&res.text);                                                              // 0053DD63 / 0053DD7E
    return pushed ? 0 : k_push_failed;
}

// ===================================================================================================== 00539BD0
// GetIsDusk() [routine 408]: module time-of-day state (+1BCh byte) == 4
int __fastcall kotor_fastcall_00539bd0(void*, void*, int, unsigned) {
    I result = 0;                                                                        // 00539BD9
    void* module = server_get_module_004ae6b0(server_app(), nullptr);                    // 00539BD0..DB
    if (module != nullptr && at<U8>(module, 0x1bc) == 4) result = 1;                     // 00539BE0..ED
    return push_int(result);                                                             // 00539BF2..0D
}

// ===================================================================================================== 00543DA0
// SoundObjectSetVolume(object oSound, int nVolume) [routine 415]
int __fastcall kotor_fastcall_00543da0(void*, void*, int, unsigned) {
    U id = 0; I volume = 0;
    if (!pop_object(&id) || !pop_int(&volume)) return k_pop_failed;                      // 00543DA0..D3
    void* sound = kotor_fastcall_004ae800(server_app(), nullptr, id);                    // 00543DD6..E4
    if (sound != nullptr) kotor_fastcall_005c8df0(sound, nullptr, volume);               // 00543DE9..F4
    return 0;                                                                            // 00543DF9
}

// ===================================================================================================== 00543B10
// SoundObjectGetPitchVariance(object oSound) [routine 689]: +240h float.
// Divergence (documented): when the object is not a sound object the original pushes its uninitialised stack
// dword [esp+4]; the reconstruction pushes 0.0 there (the original value is unspecified caller stack content).
int __fastcall kotor_fastcall_00543b10(void*, void*, int, unsigned) {
    U id = 0; U value = 0;
    if (!pop_object(&id)) return k_pop_failed;                                           // 00543B10..2F
    void* sound = kotor_fastcall_004ae800(server_app(), nullptr, id);                    // 00543B32..40
    if (sound != nullptr) value = at<U>(sound, 0x240);                                   // 00543B45..4F
    return push_float(flt(value));                                                       // 00543B53..74
}

// ===================================================================================================== 00543AA0
// SoundObjectGetFixedVariance(object oSound) [routine 188]: +248h float (same uninitialised-slot divergence).
int __fastcall kotor_fastcall_00543aa0(void*, void*, int, unsigned) {
    U id = 0; U value = 0;
    if (!pop_object(&id)) return k_pop_failed;                                           // 00543AA0..BF
    void* sound = kotor_fastcall_004ae800(server_app(), nullptr, id);                    // 00543AC2..D0
    if (sound != nullptr) value = at<U>(sound, 0x248);                                   // 00543AD5..DF
    return push_float(flt(value));                                                       // 00543AE3..04
}

// ===================================================================================================== 0053CA00
// GetPlaceableIllumination(object oPlaceable=OBJECT_SELF) [routine 545]
int __fastcall kotor_fastcall_0053ca00(void*, void*, int, unsigned) {
    U id = 0;
    if (!pop_object(&id)) return k_pop_failed;                                           // 0053CA00..1B
    I result = 0;                                                                        // 0053CA2D
    void* placeable = recovered_placeable_lookup_004ae7a0(server_app(), nullptr, id);    // 0053CA1E..2F
    if (placeable != nullptr) result = static_cast<I>(recovered_placeable_light_get_00585130(placeable, nullptr)); // 0053CA34..3F
    return push_int(result);                                                             // 0053CA41..5D
}

// ===================================================================================================== 0053BA10
// GetLockKeyRequired (537), GetLockKeyTag (538), GetLockLockable (539), GetLockUnlockDC (540), GetLockLockDC (541).
// Door (type 0Ah) and placeable (type 09h) fields; other objects give 0 / "".  Other routines push nothing.
int __fastcall kotor_fastcall_0053ba10(void*, void*, int routine, unsigned) {
    U id = 0;
    if (!pop_object(&id)) return k_pop_failed;                                           // 0053BA2E..4F
    void* o = game_object(id);                                                           // 0053BA52..66
    auto door = [&]() { return as_door(o); };
    auto placeable = [&]() { return as_placeable(o); };
    if (routine == 0x219) {                                                              // 0053BA6C..71
        I r = 0;
        if (o != nullptr) {
            if (type_of(o) == 0xa) r = at<I>(door(), 0x2d8);                             // 0053BA79..89
            else if (type_of(o) == 9) r = at<I>(placeable(), 0x26c);                     // 0053BA91..9E
        }
        return push_int(r);                                                              // 0053BAA4..CC
    }
    if (routine == 0x21a) {                                                              // 0053BACF..D4
        void* tag = nullptr;
        if (o != nullptr) {
            if (type_of(o) == 0xa) tag = static_cast<unsigned char*>(door()) + 0x2d0;    // 0053BADE..EE
            else if (type_of(o) == 9) tag = static_cast<unsigned char*>(placeable()) + 0x264; // 0053BAF5..02
        }
        if (tag == nullptr) {                                                            // 0053BB07..09
            ExoString empty;
            frame_LogStringCtor_005e5a90(&empty, nullptr, "");                           // 0053BB0B..14 [0073D71C]
            const U pushed = push_string_raw(&empty);                                    // 0053BB19..3B
            exo_destroy(&empty);                                                         // 0053BB45
            return pushed ? 0 : k_push_failed;                                           // 0053BB4A..66
        }
        return push_status(push_string_raw(static_cast<ExoString*>(tag)));               // 0053BB69..91
    }
    if (routine == 0x21b) {                                                              // 0053BB94..99
        I r = 0;
        if (o != nullptr) {
            if (type_of(o) == 0xa) r = at<I>(door(), 0x2c8);                             // 0053BBA1..B1
            else if (type_of(o) == 9) r = at<I>(placeable(), 0x330);                     // 0053BBB9..C6
        }
        return push_int(r);                                                              // 0053BBCC..F4
    }
    if (routine == 0x21c) {                                                              // 0053BBF7..FC
        I r = 0;
        if (o != nullptr) {
            if (type_of(o) == 0xa) r = at<U8>(door(), 0x2bf);                            // 0053BC04..14
            else if (type_of(o) == 9) r = at<U8>(placeable(), 0x274);                    // 0053BC1D..2A
        }
        return push_int(r);                                                              // 0053BC31..55
    }
    if (routine == 0x21d) {                                                              // 0053BC58..5D
        I r = 0;
        if (o != nullptr) {
            if (type_of(o) == 0xa) r = at<U8>(door(), 0x2c0);                            // 0053BC65..75
            else if (type_of(o) == 9) r = at<U8>(placeable(), 0x275);                    // 0053BC7E..8B
        }
        return push_int(r);                                                              // 0053BC92..A0
    }
    return 0;                                                                            // 0053BCA6
}

// ===================================================================================================== 0053E3C0
// GetTransitionTarget(object oTransition) [routine 198]: trigger linked to a waypoint (+240h == 2) or tagged
// object (+230h tag); door -> linked object id.  The module pointer is used unchecked (original).
int __fastcall kotor_fastcall_0053e3c0(void*, void*, int, unsigned) {
    U result = k_invalid;                                                                // 0053E3CF
    U id = 0;
    if (!pop_object(&id)) return k_pop_failed;                                           // 0053E3C0..E6
    void* object = nullptr;
    if (lookup(id, &object)) {                                                           // 0053E3E9..0E
        if (vcall<void*>(object, 0x40) != nullptr) {                                     // 0053E414..23 AsSWSTrigger
            void* tag = static_cast<unsigned char*>(vcall<void*>(object, 0x40)) + 0x230; // 0053E425..3B
            if (at<U8>(vcall<void*>(object, 0x40), 0x240) == 2)                          // 0053E42A..44
                result = recovered_module_get_waypoint_004c6e00(server_get_module_004ae6b0(server_app(), nullptr), nullptr, tag); // 0053E446..5B
            else
                result = recovered_module_find_object_by_tag_004c5fe0(server_get_module_004ae6b0(server_app(), nullptr), nullptr, tag, 0); // 0053E45F..77
        } else if (as_door(object) != nullptr) {                                         // 0053E47B..82
            void* linked = w5_door_get_linked_object_00589580(as_door(object), nullptr);  // 0053E484..8F
            if (linked != nullptr) result = at<U>(linked, 4);                            // 0053E494..98
        }
    }
    return push_object(result);                                                          // 0053E49B..B9
}

// ===================================================================================================== 00537E30
// GetEncounterActive (276) / GetEncounterSpawnsMax (278) / GetEncounterSpawnsCurrent (280) /
// GetEncounterNumberOfCreatures... (297) on oEncounter=OBJECT_INVALID default when argc < 1.
// Quirk kept: when the object is not an encounter (or the routine is unmatched) the handler pushes argc.
int __fastcall kotor_fastcall_00537e30(void*, void*, int routine, unsigned argc) {
    U id = k_invalid;                                                                    // 00537E36
    if (static_cast<I>(argc) >= 1 && !pop_object(&id)) return k_pop_failed;              // 00537E31..5A (jl signed)
    void* encounter = kotor_fastcall_004ae7f0(server_app(), nullptr, id);                // 00537E5D..6B
    I value = static_cast<I>(argc);                                                      // 00537EB8
    if (encounter != nullptr) {                                                          // 00537E70..72
        if (routine == 0x114) value = at<I>(encounter, 0x238);                           // 00537E78..80
        else if (routine == 0x129) value = at<I>(encounter, 0x248);                      // 00537E88..90
        else if (routine == 0x116) value = at<I>(encounter, 0x2c4);                      // 00537E98..A0
        else if (routine == 0x118) value = at<I>(encounter, 0x2c8);                      // 00537EA8..B0
    }
    return push_int(value);                                                              // 00537EBC..D7
}

// =====================================================================================================
// Reconstructed callees without a callable provider in the c31 map.
// =====================================================================================================

// 004AE090 CWorldTimer::GetWorldTimeHour(): thiscall, RET.  (ms / 1000 / 60) / minutes-per-hour (byte +38h).
U __fastcall kotor_fastcall_004ae090(void* timer, void*) {
    U day = 0, time = 0;                                                                 // [esp+8], [esp+4]
    frame_GetWorldTime_004ade40(timer, nullptr, &day, &time);                            // 004AE096..A2
    return time / 1000u / 60u / static_cast<U>(at<U8>(timer, 0x38));                     // 004AE0A7..C5 (div by 0 faults as original)
}

// 004AE140 CWorldTimer::GetWorldTimeMillisecond(): thiscall, RET.  time-of-day ms % 1000.
U __fastcall kotor_fastcall_004ae140(void* timer, void*) {
    U day = 0, time = 0;                                                                 // [esp+4], [esp]
    frame_GetWorldTime_004ade40(timer, nullptr, &day, &time);                            // 004AE143..4D
    return time % 1000u;                                                                 // 004AE152..5F
}

// 005E57B0 CExoString::Insert(const CExoString& s, int position): thiscall, RET 8.
// Acts only when s is non-empty, position >= 0 and position < strlen(this).  Builds a new buffer
// (strlen(this)+strlen(s)+1, game heap) = this[0..position) + s + this[position..], frees the old one.
// Quirk kept: the cached length (+4) is not updated.
void __fastcall kotor_fastcall_005e57b0(void* string, void*, const void* other, I position) {
    ExoString* self = static_cast<ExoString*>(string);
    const ExoString* s = static_cast<const ExoString*>(other);
    if (s->text == nullptr) return;                                                      // 005E57B6..BC
    const U insert_length = static_cast<U>(std::strlen(s->text));                        // 005E57C2..CF
    const U own_length = self->text ? static_cast<U>(std::strlen(self->text)) : 0u;      // 005E57CC..EB
    if (insert_length == 0 || position < 0 || own_length <= static_cast<U>(position)) return; // 005E57ED..03
    char* buffer = static_cast<char*>(live_operator_new(own_length + insert_length + 1)); // 005E5809..0E
    buffer[0] = 0;                                                                       // 005E5813
    char* old = self->text;                                                              // 005E5816
    self->text = buffer;                                                                 // 005E581D
    if (old != nullptr) std::strncpy(buffer, old, static_cast<U>(position));             // 005E581F..29 _strncpy
    buffer[position] = 0;                                                                // 005E582C..2E
    append(buffer, s->text);                                                             // 005E5832..5F (inline strcat)
    append(buffer, old + position);                                                      // 005E585C..87 (inline strcat)
    if (old != nullptr) live_free(old);                                                  // 005E5885..91
}

// 0059D9E0 CSWSExpression::~CSWSExpression(): thiscall, RET.
void __fastcall kotor_fastcall_0059d9e0(void* expression, void*) {
    unsigned char* e = static_cast<unsigned char*>(expression);
    if (at<void*>(e, 0x14) != nullptr) {                                                 // 0059D9FE..0D
        recovered_expression_list_delete_list_0059d920(at<void*>(e, 0x14), nullptr);     // 0059DA0F
        if (at<void*>(e, 0x14) != nullptr) live_free(at<void*>(e, 0x14));                // 0059DA14..21
    }
    for (I i = 0; i < at<I>(e, 8); ++i) {                                                // 0059DA24..50 (jle/jl signed)
        ExoString* s = at<ExoString**>(e, 4)[i];                                         // 0059DA30..33
        if (s != nullptr) { exo_destroy(s); live_free(s); }                              // 0059DA36..47
    }
    if (at<void*>(e, 4) != nullptr) {                                                    // 0059DA53..59
        live_free(at<void*>(e, 4));                                                      // 0059DA5B..61
        at<U>(e, 4) = 0;                                                                 // 0059DA64
        at<U>(e, 0xc) = 0;                                                               // 0059DA67
    }
    at<U>(e, 8) = 0;                                                                     // 0059DA6D
    exo_destroy(&at<ExoString>(e, 0x18));                                                // 0059DA6A..74 (member CExoString)
    live_free(at<void*>(e, 4));                                                          // 0059DA79..85 (CExoArrayList dtor: frees +4, now null)
    at<U>(e, 4) = 0;                                                                     // 0059DA91
}

// 005C8DF0 CSWSSoundObject::ChangeVolume(int volume): thiscall, RET 4.  Clamp to [0, 7Fh] (signed); when changed,
// store +234h and forward to the client sound object.
void __fastcall kotor_fastcall_005c8df0(void* sound, void*, I volume) {
    if (volume > 0x7f) volume = 0x7f;                                                    // 005C8DF0..F9 (jle)
    const I clamped = volume < 0 ? 0 : volume;                                           // 005C8DFE..07 setl/dec/and
    if (clamped == at<I>(sound, 0x234)) return;                                          // 005C8E09..13
    at<I>(sound, 0x234) = clamped;                                                       // 005C8E18
    const U client_id = kotor_fastcall_005eda50(client_app(), nullptr, at<U>(sound, 4)); // 005C8E15..28
    void* client_sound = recovered_client_get_sound_object_005ed620(client_app(), nullptr, client_id); // 005C8E2D..37
    if (client_sound != nullptr) kotor_fastcall_0066ad00(client_sound, nullptr, static_cast<U>(clamped)); // 005C8E3C..43
}

// 0066AD00 CSWCSoundObject::ChangeVolume(int volume): thiscall, RET 4.  Streaming source (+168h != 0 -> +100h)
// or plain source (+FCh); +108h = volume.
void __fastcall kotor_fastcall_0066ad00(void* sound, void*, U volume) {
    if (at<U>(sound, 0x168) == 0)                                                        // 0066AD03..11
        kotor_fastcall_005d5950(at<void*>(sound, 0xfc), nullptr, volume);                 // 0066AD13..19
    else
        table29_required::gui_volume_005d5b50(at<void*>(sound, 0x100), nullptr, volume);  // 0066AD29..2F
    at<U>(sound, 0x108) = volume;                                                        // 0066AD1E / 0066AD34
}

}  // extern "C"
