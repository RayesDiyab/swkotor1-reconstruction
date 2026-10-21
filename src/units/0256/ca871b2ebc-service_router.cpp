// lane-m50-diag NON-ORIGINAL HOST DIAGNOSTIC REPLACEMENT COPY of M49 unit services-router
// (isolated-inputs/r12-mode1-router-ini-r3/services-router/service_router.cpp sha ff9369c8...). Only change:
// invalid_request() takes a stable check id (+ call site/target words) and reports it via
// kotor_host_diag_words (D94 proposal) before the unchanged ExitProcess(86); `!known || !heap` split in order.
// Task6-owned routing boundary; actual source bodies/ABI adapters are external.
#include "3d96915e09-start_body.h"
#include "397879e70b-startup_string.h"
#include "05a3c5e4fb-allocator_live.h"
#include "4e2136d0b8-functional_runtime.h"
#include <new>
using Word=client_services_start::Word;
using Call=client_services_start::Call;
using Abi=client_services_start::Abi;
using String=kotor_reconstruction::StartupString;
extern "C" Word __cdecl kotor_menu_service_005E6750(const Call*);
extern "C" Word __cdecl kotor_menu_service_004AE4C0(const Call*);
extern "C" Word __cdecl kotor_menu_service_004D7D80(const Call*);
extern "C" Word __cdecl kotor_menu_service_00409BF0(const Call*);
extern "C" Word __cdecl kotor_menu_service_005E67C0(const Call*);
extern "C" Word __cdecl kotor_menu_service_005E5630(const Call*);
extern "C" Word __cdecl kotor_menu_service_0044ED70(const Call*);
extern "C" Word __cdecl kotor_menu_service_005EE990(const Call*);
extern "C" Word __cdecl kotor_menu_service_005D54A0(const Call*);
extern "C" Word __cdecl kotor_menu_service_0054EB10(const Call*);
extern "C" Word __cdecl kotor_menu_service_0064B510(const Call*);
extern "C" Word __cdecl kotor_menu_service_00604B20(const Call*);
extern "C" Word __cdecl kotor_menu_service_00604790(const Call*);
extern "C" Word __cdecl kotor_menu_service_005E93F0(const Call*);
extern "C" Word __cdecl kotor_menu_service_00408800(const Call*);
extern "C" Word __cdecl kotor_menu_service_0041D8D0(const Call*);
extern "C" Word __cdecl kotor_menu_service_004085F0(const Call*);
extern "C" Word __cdecl kotor_menu_service_004087E0(const Call*);
extern "C" Word __cdecl kotor_menu_service_0070D9C0(const Call*);
extern "C" Word __cdecl kotor_menu_service_004087A0(const Call*);
extern "C" Word __cdecl kotor_menu_service_00406D80(const Call*);
extern "C" Word __cdecl kotor_menu_service_00408BC0(const Call*);
extern "C" Word __cdecl kotor_menu_service_004087C0(const Call*);
extern "C" Word __cdecl kotor_menu_service_005F1F70(const Call*);
extern "C" Word __cdecl kotor_menu_service_005E5C50(const Call*);
extern "C" Word __cdecl kotor_menu_service_005F6480(const Call*);
extern "C" Word __cdecl kotor_menu_service_0041E5A0(const Call*);
extern "C" Word __cdecl kotor_menu_service_005F4180(const Call*);
extern "C" Word __cdecl kotor_menu_service_005D5550(const Call*);
extern "C" Word __cdecl kotor_menu_service_006FC43A(const Call*);
extern "C" Word __cdecl kotor_menu_service_00643F40(const Call*);
extern "C" Word __cdecl kotor_menu_service_005F14A0(const Call*);
extern "C" Word __cdecl kotor_menu_service_005E5680(const Call*);
extern "C" Word __cdecl kotor_menu_service_005E67D0(const Call*);
extern "C" Word __cdecl kotor_menu_service_0061DA60(const Call*);
extern "C" Word __cdecl kotor_menu_service_005F4710(const Call*);
extern "C" Word __cdecl kotor_menu_service_005F5AB0(const Call*);
extern "C" Word __cdecl kotor_menu_service_005ED700(const Call*);
extern "C" Word __cdecl kotor_menu_service_0061D8E0(const Call*);
extern "C" Word __cdecl kotor_menu_service_0061D8C0(const Call*);
extern "C" Word __cdecl kotor_menu_service_0061D710(const Call*);
extern "C" Word __cdecl kotor_menu_service_00403800(const Call*);
extern "C" Word __cdecl kotor_menu_service_0041E8F0(const Call*);
extern "C" Word __cdecl kotor_menu_service_0041E0F0(const Call*);
extern "C" Word __cdecl kotor_menu_service_006FE34D(const Call*);
extern "C" Word __cdecl kotor_menu_service_005EE400(const Call*);
extern "C" Word __cdecl kotor_menu_service_005EE920(const Call*);
extern "C" Word __cdecl kotor_menu_service_00678C10(const Call*);
extern "C" Word __cdecl kotor_menu_service_005F0AD0(const Call*);
extern "C" Word __cdecl kotor_menu_service_005F1C20(const Call*);
extern "C" Word __cdecl kotor_menu_service_0040BAD0(const Call*);
extern "C" Word __cdecl kotor_menu_service_0040BE70(const Call*);
extern "C" Word __cdecl kotor_menu_service_005DF5D0(const Call*);
extern "C" Word __cdecl kotor_menu_service_005DF6E0(const Call*);
extern "C" Word __cdecl kotor_menu_service_0040B060(const Call*);
extern "C" Word __cdecl kotor_menu_service_0061F980(const Call*);
extern "C" Word __cdecl kotor_menu_service_00409F00(const Call*);
extern "C" Word __cdecl kotor_menu_service_0062FED0(const Call*);
extern "C" Word __cdecl kotor_menu_service_005F2BF0(const Call*);
extern "C" Word __cdecl kotor_menu_service_005E67E0(const Call*);
struct Site {Word site,target;};
static const Site allowed_sites[]={
 {0x005f8577,0x005e6750},
 {0x005f8588,0x005b3190},
 {0x005f8596,0x005b3190},
 {0x005f85a6,0x006fa7e6},
 {0x005f85b3,0x006fa7e6},
 {0x005f85ca,0x004ae4c0},
 {0x005f85dc,0x006fa7e6},
 {0x005f85f3,0x004ae4c0},
 {0x005f8605,0x006fa7e6},
 {0x005f861c,0x004ae4c0},
 {0x005f862e,0x006fa7e6},
 {0x005f8645,0x004ae4c0},
 {0x005f8657,0x006fa7e6},
 {0x005f8670,0x004d7d80},
 {0x005f8682,0x006fa7e6},
 {0x005f8699,0x00409bf0},
 {0x005f86b4,0x005e5a90},
 {0x005f86c7,0x005e5a90},
 {0x005f86da,0x005e5a90},
 {0x005f86fc,0x005e67c0},
 {0x005f870c,0x005e5c20},
 {0x005f871a,0x005e5c20},
 {0x005f8727,0x005e5c20},
 {0x005f8734,0x005e5630},
 {0x005f8744,0x0044ed70},
 {0x005f8750,0x005ee990},
 {0x005f875a,0x006fa7e6},
 {0x005f8771,0x005d54a0},
 {0x005f878d,0x006fa7e6},
 {0x005f87a4,0x0054eb10},
 {0x005f87b6,0x006fa7e6},
 {0x005f87cd,0x0064b510},
 {0x005f87e2,0x006fa7e6},
 {0x005f87f9,0x00604b20},
 {0x005f880c,0x00604790},
 {0x005f881a,0x005e5a90},
 {0x005f882d,0x005e5a90},
 {0x005f8840,0x005e5a90},
 {0x005f8862,0x005e67c0},
 {0x005f8872,0x005e5c20},
 {0x005f8880,0x005e5c20},
 {0x005f888d,0x005e5c20},
 {0x005f889a,0x005e5630},
 {0x005f88c1,0x005e5a90},
 {0x005f88d9,0x005e93f0},
 {0x005f88e6,0x005e5c20},
 {0x005f88f4,0x005e5a90},
 {0x005f8909,0x00408800},
 {0x005f8916,0x005e5c20},
 {0x005f8924,0x005e5a90},
 {0x005f8939,0x00408800},
 {0x005f8946,0x005e5c20},
 {0x005f8954,0x005e5a90},
 {0x005f8969,0x00408800},
 {0x005f8976,0x005e5c20},
 {0x005f897d,0x006fa7e6},
 {0x005f8996,0x0041d8d0},
 {0x005f89c5,0x005e5a90},
 {0x005f89e4,0x004085f0},
 {0x005f89ed,0x005e5c20},
 {0x005f89fa,0x005e5c20},
 {0x005f8a08,0x005e5a90},
 {0x005f8a27,0x004085f0},
 {0x005f8a30,0x005e5c20},
 {0x005f8a3d,0x005e5c20},
 {0x005f8a4b,0x005e5a90},
 {0x005f8a6a,0x004085f0},
 {0x005f8a73,0x005e5c20},
 {0x005f8a80,0x005e5c20},
 {0x005f8a8e,0x005e5a90},
 {0x005f8aa3,0x004087e0},
 {0x005f8ab0,0x005e5c20},
 {0x005f8abe,0x005e5a90},
 {0x005f8ad3,0x00408800},
 {0x005f8ae0,0x005e5c20},
 {0x005f8ae7,0x006fa7e6},
 {0x005f8afe,0x0070d9c0},
 {0x005f8b19,0x005e5a90},
 {0x005f8b2e,0x00408800},
 {0x005f8b3b,0x005e5c20},
 {0x005f8b49,0x005e5a90},
 {0x005f8b5e,0x00408800},
 {0x005f8b6b,0x005e5c20},
 {0x005f8b79,0x005e5a90},
 {0x005f8b90,0x004087a0},
 {0x005f8b9d,0x005e5c20},
 {0x005f8bab,0x005e5a90},
 {0x005f8bc2,0x004087a0},
 {0x005f8bcf,0x005e5c20},
 {0x005f8bdd,0x005e5a90},
 {0x005f8bf2,0x00408800},
 {0x005f8bff,0x005e5c20},
 {0x005f8c0d,0x005e5a90},
 {0x005f8c22,0x00408800},
 {0x005f8c2f,0x005e5c20},
 {0x005f8c3d,0x005e5a90},
 {0x005f8c52,0x00408800},
 {0x005f8c5f,0x005e5c20},
 {0x005f8c6d,0x00406d80},
 {0x005f8c84,0x00408bc0},
 {0x005f8c96,0x005e5a90},
 {0x005f8cab,0x004087c0},
 {0x005f8cb8,0x005e5c20},
 {0x005f8cbf,0x005f1f70},
 {0x005f8cd5,0x005e5c50},
 {0x005f8cdc,0x005f6480},
 {0x005f8cea,0x005e5a90},
 {0x005f8d09,0x004085f0},
 {0x005f8d18,0x005e5c50},
 {0x005f8d26,0x005e5c20},
 {0x005f8d33,0x005e5c20},
 {0x005f8d43,0x0041e5a0},
 {0x005f8d4a,0x005f4180},
 {0x005f8d5c,0x005d5550},
 {0x005f8d61,0x0073d0f8},
 {0x005f8d68,0x006fc43a},
 {0x005f8d72,0x006fa7e6},
 {0x005f8d89,0x00643f40},
 {0x005f8daa,0x005e5a90},
 {0x005f8dbd,0x005e5a90},
 {0x005f8dd0,0x005e5a90},
 {0x005f8df2,0x005e67c0},
 {0x005f8e02,0x005e5c20},
 {0x005f8e10,0x005e5c20},
 {0x005f8e1d,0x005e5c20},
 {0x005f8e26,0x005e5630},
 {0x005f8e3f,0x005f14a0},
 {0x005f8e4d,0x005e5a90},
 {0x005f8e60,0x005e5a90},
 {0x005f8e73,0x005e5a90},
 {0x005f8e95,0x005e67c0},
 {0x005f8ea5,0x005e5c20},
 {0x005f8eb3,0x005e5c20},
 {0x005f8ec0,0x005e5c20},
 {0x005f8ecd,0x005e5630},
 {0x005f8eef,0x005e5680},
 {0x005f8f00,0x005e5a90},
 {0x005f8f13,0x005e5a90},
 {0x005f8f26,0x005e5a90},
 {0x005f8f48,0x005e67d0},
 {0x005f8f56,0x005e5c20},
 {0x005f8f64,0x005e5c20},
 {0x005f8f71,0x005e5c20},
 {0x005f8f7f,0x005e5a90},
 {0x005f8f92,0x005e5a90},
 {0x005f8fa5,0x005e5a90},
 {0x005f8fc7,0x005e67c0},
 {0x005f8fd7,0x005e5c20},
 {0x005f8fe5,0x005e5c20},
 {0x005f8ff2,0x005e5c20},
 {0x005f8fff,0x005e5630},
 {0x005f9036,0x0061da60},
 {0x005f903d,0x005f4710},
 {0x005f905f,0x005f5ab0},
 {0x005f906d,0x005e5a90},
 {0x005f9080,0x005e5a90},
 {0x005f9093,0x005e5a90},
 {0x005f90b5,0x005e67c0},
 {0x005f90c5,0x005e5c20},
 {0x005f90d3,0x005e5c20},
 {0x005f90e0,0x005e5c20},
 {0x005f90f1,0x005ed700},
 {0x005f9107,0x005e5630},
 {0x005f910f,0x005ed700},
 {0x005f9116,0x0061d8e0},
 {0x005f9124,0x005e5a90},
 {0x005f9137,0x005e5a90},
 {0x005f914a,0x005e5a90},
 {0x005f916c,0x005e67c0},
 {0x005f917c,0x005e5c20},
 {0x005f918a,0x005e5c20},
 {0x005f9197,0x005e5c20},
 {0x005f91a8,0x005ed700},
 {0x005f91be,0x005e5630},
 {0x005f91c6,0x005ed700},
 {0x005f91cd,0x0061d8c0},
 {0x005f91db,0x005e5a90},
 {0x005f91ee,0x005e5a90},
 {0x005f9201,0x005e5a90},
 {0x005f9223,0x005e67c0},
 {0x005f9233,0x005e5c20},
 {0x005f9241,0x005e5c20},
 {0x005f924e,0x005e5c20},
 {0x005f9263,0x005ed700},
 {0x005f9279,0x005e5630},
 {0x005f9281,0x005ed700},
 {0x005f9288,0x0061d710},
 {0x005f9291,0x005e5630},
 {0x005f92b3,0x005ed700},
 {0x005f92d9,0x00403800},
 {0x005f92ee,0x0041e8f0},
 {0x005f92ff,0x005e5c50},
 {0x005f930c,0x005e5c20},
 {0x005f9317,0x0041e0f0},
 {0x005f932d,0x006fe34d},
 {0x005f9350,0x006fa7e6},
 {0x005f9367,0x005ee400},
 {0x005f937c,0x005ee920},
 {0x005f9383,0x006fa7e6},
 {0x005f939a,0x00678c10},
 {0x005f93af,0x005f0ad0},
 {0x005f93b6,0x005f1c20},
 {0x005f93c0,0x006fa7e6},
 {0x005f93d7,0x0040bad0},
 {0x005f93fa,0x0040be70},
 {0x005f941f,0x005df5d0},
 {0x005f942a,0x005df6e0},
 {0x005f9439,0x0040b060},
 {0x005f9446,0x0061f980},
 {0x005f9451,0x00409f00},
 {0x005f945b,0x006fa7e6},
 {0x005f9472,0x0062fed0},
 {0x005f9488,0x005f2bf0},
 {0x005f9496,0x005e5c20},
 {0x005f94a4,0x005e5c20},
 {0x005f94b5,0x005e67e0},
};
static __declspec(noreturn) void invalid_request(const char*,Word=0,Word=0){;ExitProcess(86);}
// Contract-failure endpoint for typed adapters; uses the canonical rejection policy.
extern "C" __declspec(noreturn) void __cdecl kotor_menu_service_contract_violation(const Call* c) {
 invalid_request("router.contract_violation",c?c->site:0u,c?c->target:0u);
}
extern "C" Word __cdecl unresolved_client_services_provider(const Call* c) {
 if(!c) invalid_request("router.call_null");
 bool known=false;for(const auto& s:allowed_sites)if(s.site==c->site&&s.target==c->target){known=true;break;}
 if(!known) invalid_request("router.site_not_allowed",c->site,c->target);
 if(!kotor_functional::source_heap_identity()) invalid_request("router.heap_identity",c->site,c->target);
 switch(c->target){
 case 0x005b3190: if(c->abi!=Abi::Thiscall||c->count!=0) invalid_request("router.abi.005b3190",c->site,c->target); return reinterpret_cast<Word>(new(reinterpret_cast<String*>(c->self)) String());
 case 0x005e5a90: if(c->abi!=Abi::Thiscall||c->count!=1) invalid_request("router.abi.005e5a90",c->site,c->target); return reinterpret_cast<Word>(new(reinterpret_cast<String*>(c->self)) String(reinterpret_cast<const char*>(c->args[0])));
 case 0x005e5c20: if(c->abi!=Abi::Thiscall||c->count!=0) invalid_request("router.abi.005e5c20",c->site,c->target); reinterpret_cast<String*>(c->self)->~StartupString(); return 0; // Actual source destructor; EAX is dead at these caller sites.
 case 0x006fa7e6: if(c->abi!=Abi::Cdecl||c->count!=1||c->self) invalid_request("router.abi.006fa7e6",c->site,c->target); return reinterpret_cast<Word>(live_operator_new(c->args[0]));
 case 0x0073d0f8: if(c->abi!=Abi::Stdcall||c->count!=0||c->self) invalid_request("router.abi.0073d0f8",c->site,c->target); return GetTickCount();
 case 0x005e6750: return kotor_menu_service_005E6750(c);
 case 0x004ae4c0: return kotor_menu_service_004AE4C0(c);
 case 0x004d7d80: return kotor_menu_service_004D7D80(c);
 case 0x00409bf0: return kotor_menu_service_00409BF0(c);
 case 0x005e67c0: return kotor_menu_service_005E67C0(c);
 case 0x005e5630: return kotor_menu_service_005E5630(c);
 case 0x0044ed70: return kotor_menu_service_0044ED70(c);
 case 0x005ee990: return kotor_menu_service_005EE990(c);
 case 0x005d54a0: return kotor_menu_service_005D54A0(c);
 case 0x0054eb10: return kotor_menu_service_0054EB10(c);
 case 0x0064b510: return kotor_menu_service_0064B510(c);
 case 0x00604b20: return kotor_menu_service_00604B20(c);
 case 0x00604790: return kotor_menu_service_00604790(c);
 case 0x005e93f0: return kotor_menu_service_005E93F0(c);
 case 0x00408800: return kotor_menu_service_00408800(c);
 case 0x0041d8d0: return kotor_menu_service_0041D8D0(c);
 case 0x004085f0: return kotor_menu_service_004085F0(c);
 case 0x004087e0: return kotor_menu_service_004087E0(c);
 case 0x0070d9c0: return kotor_menu_service_0070D9C0(c);
 case 0x004087a0: return kotor_menu_service_004087A0(c);
 case 0x00406d80: return kotor_menu_service_00406D80(c);
 case 0x00408bc0: return kotor_menu_service_00408BC0(c);
 case 0x004087c0: return kotor_menu_service_004087C0(c);
 case 0x005f1f70: return kotor_menu_service_005F1F70(c);
 case 0x005e5c50: return kotor_menu_service_005E5C50(c);
 case 0x005f6480: return kotor_menu_service_005F6480(c);
 case 0x0041e5a0: return kotor_menu_service_0041E5A0(c);
 case 0x005f4180: return kotor_menu_service_005F4180(c);
 case 0x005d5550: return kotor_menu_service_005D5550(c);
 case 0x006fc43a: return kotor_menu_service_006FC43A(c);
 case 0x00643f40: return kotor_menu_service_00643F40(c);
 case 0x005f14a0: return kotor_menu_service_005F14A0(c);
 case 0x005e5680: return kotor_menu_service_005E5680(c);
 case 0x005e67d0: return kotor_menu_service_005E67D0(c);
 case 0x0061da60: return kotor_menu_service_0061DA60(c);
 case 0x005f4710: return kotor_menu_service_005F4710(c);
 case 0x005f5ab0: return kotor_menu_service_005F5AB0(c);
 case 0x005ed700: return kotor_menu_service_005ED700(c);
 case 0x0061d8e0: return kotor_menu_service_0061D8E0(c);
 case 0x0061d8c0: return kotor_menu_service_0061D8C0(c);
 case 0x0061d710: return kotor_menu_service_0061D710(c);
 case 0x00403800: return kotor_menu_service_00403800(c);
 case 0x0041e8f0: return kotor_menu_service_0041E8F0(c);
 case 0x0041e0f0: return kotor_menu_service_0041E0F0(c);
 case 0x006fe34d: return kotor_menu_service_006FE34D(c);
 case 0x005ee400: return kotor_menu_service_005EE400(c);
 case 0x005ee920: return kotor_menu_service_005EE920(c);
 case 0x00678c10: return kotor_menu_service_00678C10(c);
 case 0x005f0ad0: return kotor_menu_service_005F0AD0(c);
 case 0x005f1c20: return kotor_menu_service_005F1C20(c);
 case 0x0040bad0: return kotor_menu_service_0040BAD0(c);
 case 0x0040be70: return kotor_menu_service_0040BE70(c);
 case 0x005df5d0: return kotor_menu_service_005DF5D0(c);
 case 0x005df6e0: return kotor_menu_service_005DF6E0(c);
 case 0x0040b060: return kotor_menu_service_0040B060(c);
 case 0x0061f980: return kotor_menu_service_0061F980(c);
 case 0x00409f00: return kotor_menu_service_00409F00(c);
 case 0x0062fed0: return kotor_menu_service_0062FED0(c);
 case 0x005f2bf0: return kotor_menu_service_005F2BF0(c);
 case 0x005e67e0: return kotor_menu_service_005E67E0(c);
 default: invalid_request("router.target_unknown",c->site,c->target);
 }
}
