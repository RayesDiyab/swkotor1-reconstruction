#include "8ae0e4efda-bridges.h"
#include "3b3fee4107-provider_headers.h"
#include "9281d0a58c-warning_runtime.h"
#include "5eff1770a2-manager_factory.h"

using namespace service_adapter_closure;

extern "C" Word __cdecl kotor_menu_service_005E6750(const Call* c) {
    checked(c,0x005E6750);
    return reinterpret_cast<Word>(reinterpret_cast<base_ini_reconstruction::PublicIni*>(c->self)->construct_005e6750());
}

extern "C" Word __cdecl kotor_menu_service_004AE4C0(const Call* c) {
    checked(c,0x004AE4C0);
    return reinterpret_cast<Word>(reinterpret_cast<kotor_menu_10_19::WorldTimer*>(c->self)->construct_004ae4c0());
}

extern "C" Word __cdecl kotor_menu_service_004D7D80(const Call* c) {
    checked(c,0x004D7D80);
    return reinterpret_cast<Word>(reinterpret_cast<kotor_menu_10_19::ObjectArray*>(c->self)->construct_004d7d80(static_cast<int>(c->args[0])));
}

extern "C" Word __cdecl kotor_menu_service_00409BF0(const Call* c) {
    checked(c,0x00409BF0);
    return reinterpret_cast<Word>(menu_resources::ConstructOwnedManager00409bf0(reinterpret_cast<void*>(c->self),nullptr));
}

extern "C" Word __cdecl kotor_menu_service_005E67C0(const Call* c) {
    checked(c,0x005E67C0);
    const Word result=static_cast<Word>(reinterpret_cast<base_ini_reconstruction::PublicIni*>(c->self)->read_005e67c0(reinterpret_cast<base_ini_reconstruction::String*>(c->args[0]),reinterpret_cast<base_ini_reconstruction::String*>(c->args[1]),reinterpret_cast<base_ini_reconstruction::String*>(c->args[2]),reinterpret_cast<base_ini_reconstruction::String*>(c->args[3])));
    return result;
}

extern "C" Word __cdecl kotor_menu_service_005E5630(const Call* c) {
    checked(c,0x005E5630);
    const Word result=static_cast<Word>(reinterpret_cast<kotor_menu20::String*>(c->self)->as_int_005e5630());
    return result;
}

extern "C" Word __cdecl kotor_menu_service_0044ED70(const Call* c) {
    checked(c,0x0044ED70);
    return reinterpret_cast<Word>(new_aurora_0044ed70());
}

extern "C" Word __cdecl kotor_menu_service_005EE990(const Call* c) {
    checked(c,0x005EE990);
    reinterpret_cast<kotor_client_normal::ClientPrefix*>(c->self)->CreateWorldJournal();
    return 0; // Caller-specific dead EAX; real source side effects were executed.
}

extern "C" Word __cdecl kotor_menu_service_005D54A0(const Call* c) {
    checked(c,0x005D54A0);
    return reinterpret_cast<Word>(reinterpret_cast<kotor_menu20::NetLayer*>(c->self)->construct_normal_005d54a0());
}

extern "C" Word __cdecl kotor_menu_service_0054EB10(const Call* c) {
    checked(c,0x0054EB10);
    return reinterpret_cast<Word>(reinterpret_cast<kotor_menu_10_19::ServerInfo*>(c->self)->construct_0054eb10());
}

extern "C" Word __cdecl kotor_menu_service_0064B510(const Call* c) {
    checked(c,0x0064B510);
    return reinterpret_cast<Word>(reinterpret_cast<services_tail5::Message*>(c->self)->Construct());
}

extern "C" Word __cdecl kotor_menu_service_00604B20(const Call* c) {
    checked(c,0x00604B20);
    return reinterpret_cast<Word>(reinterpret_cast<menu_providers_40_49::AI*>(c->self)->construct_00604b20());
}

extern "C" Word __cdecl kotor_menu_service_00604790(const Call* c) {
    checked(c,0x00604790);
    reinterpret_cast<menu_providers_40_49::AI*>(c->self)->set_app_00604790(static_cast<Word>(c->args[0]));
    return 0; // Caller-specific dead EAX; real source side effects were executed.
}

extern "C" Word __cdecl kotor_menu_service_005E93F0(const Call* c) {
    checked(c,0x005E93F0);
    reinterpret_cast<kotor_menu20::Debug*>(c->self)->open_logs_005e93f0(static_cast<Word>(c->args[0]));
    return 0; // Caller-specific dead EAX; real source side effects were executed.
}

extern "C" Word __cdecl kotor_menu_service_00408800(const Call* c) {
    checked(c,0x00408800);
    const Word result=static_cast<Word>(reinterpret_cast<menu_resources::Manager*>(c->self)->directory_00408800(reinterpret_cast<menu_resources::String*>(c->args[0])));
    return result;
}

extern "C" Word __cdecl kotor_menu_service_0041D8D0(const Call* c) {
    checked(c,0x0041D8D0);
    return reinterpret_cast<Word>(reinterpret_cast<kotor_menu_10_19::Tlk*>(c->self)->construct_0041d8d0());
}

extern "C" Word __cdecl kotor_menu_service_004085F0(const Call* c) {
    checked(c,0x004085F0);
    return reinterpret_cast<Word>(reinterpret_cast<menu_resources::Manager*>(c->self)->cache_004085f0(reinterpret_cast<menu_resources::String*>(c->args[0]),reinterpret_cast<menu_resources::String*>(c->args[1]),static_cast<Word>(c->args[2])));
}

extern "C" Word __cdecl kotor_menu_service_004087E0(const Call* c) {
    checked(c,0x004087E0);
    const Word result=static_cast<Word>(reinterpret_cast<menu_resources::Manager*>(c->self)->fixed_004087e0(reinterpret_cast<menu_resources::String*>(c->args[0])));
    return result;
}

extern "C" Word __cdecl kotor_menu_service_0070D9C0(const Call* c) {
    checked(c,0x0070D9C0);
    return reinterpret_cast<Word>(reinterpret_cast<services_tail5::Interface*>(c->self)->Construct());
}

extern "C" Word __cdecl kotor_menu_service_004087A0(const Call* c) {
    checked(c,0x004087A0);
    const Word result=static_cast<Word>(reinterpret_cast<menu_resources::Manager*>(c->self)->encapsulated_004087a0(reinterpret_cast<menu_resources::String*>(c->args[0]),static_cast<Word>(c->args[1])));
    return result;
}

extern "C" Word __cdecl kotor_menu_service_00406D80(const Call* c) {
    checked(c,0x00406D80);
    return reinterpret_cast<Word>(reinterpret_cast<menu_resources::Ref*>(c->self)->construct_00406d80(reinterpret_cast<const char*>(c->args[0])));
}

extern "C" Word __cdecl kotor_menu_service_00408BC0(const Call* c) {
    checked(c,0x00408BC0);
    const Word result=static_cast<Word>(reinterpret_cast<menu_resources::Manager*>(c->self)->exists_00408bc0(reinterpret_cast<menu_resources::Ref*>(c->args[0]),static_cast<Word>(c->args[1]),reinterpret_cast<Word*>(c->args[2])));



    return result;

}

extern "C" Word __cdecl kotor_menu_service_004087C0(const Call* c) {
    checked(c,0x004087C0);
    const Word result=static_cast<Word>(reinterpret_cast<menu_resources::Manager*>(c->self)->image_004087c0(reinterpret_cast<menu_resources::String*>(c->args[0])));
    return result;
}

extern "C" Word __cdecl kotor_menu_service_005F1F70(const Call* c) {
    checked(c,0x005F1F70);
    const Word result=static_cast<Word>(audio::audio_005f1f70());
    return result;
}

extern "C" Word __cdecl kotor_menu_service_005E5C50(const Call* c) {
    checked(c,0x005E5C50);
    return reinterpret_cast<Word>(reinterpret_cast<kotor_reconstruction::StartupString*>(c->self)->Assign005e5c50(reinterpret_cast<kotor_reconstruction::StartupString*>(c->args[0])));
}

extern "C" Word __cdecl kotor_menu_service_005F6480(const Call* c) {
    checked(c,0x005F6480);
    reinterpret_cast<menu_normal_retained_40_49::MusicInternal*>(c->self)->music_005f6480();
    return 0; // Caller-specific dead EAX; real source side effects were executed.
}

extern "C" Word __cdecl kotor_menu_service_005D5550(const Call* c) {
    checked(c,0x005D5550);
    const Word result=static_cast<Word>(reinterpret_cast<kotor_menu20::NetLayer*>(c->self)->initialize_005d5550(reinterpret_cast<void*>(c->args[0])));
    return result;
}

extern "C" Word __cdecl kotor_menu_service_006FC43A(const Call* c) {
    checked(c,0x006FC43A);
    const Word result=static_cast<Word>(services_tail5::Seed(static_cast<Word>(c->args[0])));
    return result;
}

extern "C" Word __cdecl kotor_menu_service_00643F40(const Call* c) {
    checked(c,0x00643F40);
    return reinterpret_cast<Word>(reinterpret_cast<menu_normal_retained_40_49::Module*>(c->self)->construct_00643f40());
}

extern "C" Word __cdecl kotor_menu_service_005F14A0(const Call* c) {
    checked(c,0x005F14A0);
    const Word result=static_cast<Word>(reinterpret_cast<options_video::Client*>(c->self)->SetTexturePack_005f14a0(static_cast<unsigned char>(c->args[0]),static_cast<int>(c->args[1])));
    return result;
}

extern "C" Word __cdecl kotor_menu_service_005E5680(const Call* c) {
    checked(c,0x005E5680);
    kotor_reconstruction::StartupFormat005e5680(reinterpret_cast<kotor_reconstruction::StartupString*>(c->args[0]),reinterpret_cast<const char*>(c->args[1]),static_cast<int>(c->args[2]));
    return 0; // Caller-specific dead EAX; real source side effects were executed.
}

extern "C" Word __cdecl kotor_menu_service_005E67D0(const Call* c) {
    checked(c,0x005E67D0);
    const Word result=static_cast<Word>(reinterpret_cast<base_ini_reconstruction::PublicIni*>(c->self)->write_005e67d0(reinterpret_cast<base_ini_reconstruction::String*>(c->args[0]),reinterpret_cast<base_ini_reconstruction::String*>(c->args[1]),reinterpret_cast<base_ini_reconstruction::String*>(c->args[2]),reinterpret_cast<base_ini_reconstruction::String*>(c->args[3])));
    return result;
}

extern "C" Word __cdecl kotor_menu_service_0061DA60(const Call* c) {
    checked(c,0x0061DA60);
    reinterpret_cast<menu_providers_40_49::Options*>(c->self)->defaults_0061da60();
    return 0; // Caller-specific dead EAX; real source side effects were executed.
}

extern "C" Word __cdecl kotor_menu_service_005F4710(const Call* c) {
    checked(c,0x005F4710);
    const Word result=static_cast<Word>(reinterpret_cast<kotor_client_normal::ClientPrefix*>(c->self)->LoadClientOptions());
    return result;
}

extern "C" Word __cdecl kotor_menu_service_005F5AB0(const Call* c) {
    checked(c,0x005F5AB0);
    reinterpret_cast<options_video::Client*>(c->self)->ReadAndSetVideoMode_005f5ab0(static_cast<int>(c->args[0]),static_cast<int>(c->args[1]));
    return 0; // Caller-specific dead EAX; real source side effects were executed.
}

extern "C" Word __cdecl kotor_menu_service_005ED700(const Call* c) {
    checked(c,0x005ED700);
    return reinterpret_cast<Word>(reinterpret_cast<kotor_menu20::ClientApp*>(c->self)->get_options_005ed700());
}

extern "C" Word __cdecl kotor_menu_service_0061D8E0(const Call* c) {
    checked(c,0x0061D8E0);
    reinterpret_cast<menu_providers_40_49::Options*>(c->self)->vsync_0061d8e0(forward_word(c->args[0]));
    return 0; // Caller-specific dead EAX; real source side effects were executed.
}

extern "C" Word __cdecl kotor_menu_service_0061D8C0(const Call* c) {
    checked(c,0x0061D8C0);
    reinterpret_cast<menu_providers_40_49::Options*>(c->self)->anisotropy_0061d8c0(static_cast<Word>(c->args[0]));
    return 0; // Caller-specific dead EAX; real source side effects were executed.
}

extern "C" Word __cdecl kotor_menu_service_0061D710(const Call* c) {
    checked(c,0x0061D710);
    reinterpret_cast<menu_providers_40_49::Options*>(c->self)->antialias_0061d710(static_cast<unsigned char>(c->args[0]));
    return 0; // Caller-specific dead EAX; real source side effects were executed.
}


extern "C" Word __cdecl kotor_menu_service_0041E8F0(const Call* c) {
    checked(c,0x0041E8F0);
    return reinterpret_cast<Word>(reinterpret_cast<kotor_menu_10_19::Tlk*>(c->self)->simple_0041e8f0(reinterpret_cast<kotor_menu_10_19::String*>(c->args[0]),static_cast<Word>(c->args[1])));
}

extern "C" Word __cdecl kotor_menu_service_0041E0F0(const Call* c) {
    checked(c,0x0041E0F0);
    const Word result=static_cast<Word>(reinterpret_cast<kotor_menu_10_19::Tlk*>(c->self)->language_0041e0f0());
    return result;
}

extern "C" Word __cdecl kotor_menu_service_006FE34D(const Call* c) {
    checked(c,0x006FE34D);
    if(!services_tail5::locale_bindings.boundary || services_tail5::locale_bindings.boundary->outside_contract) kotor_menu_service_contract_violation(c);
    const Word result=static_cast<Word>(services_tail5::Locale(static_cast<int>(c->args[0]),static_cast<Word>(c->args[1])));
    if(services_tail5::locale_bindings.boundary->outside_contract) kotor_menu_service_contract_violation(c);
    return result;
}

extern "C" Word __cdecl kotor_menu_service_005EE400(const Call* c) {
    checked(c,0x005EE400);
    return reinterpret_cast<Word>(reinterpret_cast<kotor_menu20::Ime*>(c->self)->construct_normal_005ee400());
}

extern "C" Word __cdecl kotor_menu_service_005EE920(const Call* c) {
    checked(c,0x005EE920);
    reinterpret_cast<kotor_client_normal::ClientPrefix*>(c->self)->CreateQuestJournal();
    return 0; // Caller-specific dead EAX; real source side effects were executed.
}

extern "C" Word __cdecl kotor_menu_service_00678C10(const Call* c) {
    checked(c,0x00678C10);
    return reinterpret_cast<Word>(reinterpret_cast<services_tail5::SoundSetList*>(c->self)->Construct());
}

extern "C" Word __cdecl kotor_menu_service_005F1C20(const Call* c) {
    checked(c,0x005F1C20);
    reinterpret_cast<options_video::Client*>(c->self)->InitializeClientOptions_005f1c20();
    return 0; // Caller-specific dead EAX; real source side effects were executed.
}

extern "C" Word __cdecl kotor_menu_service_0040BAD0(const Call* c) {
    checked(c,0x0040BAD0);
    return reinterpret_cast<Word>(reinterpret_cast<gui_ctor::Gui*>(c->self)->construct_0040bad0());
}

extern "C" Word __cdecl kotor_menu_service_0040BE70(const Call* c) {
    checked(c,0x0040BE70);
    reinterpret_cast<gui_size::Gui*>(c->self)->set_size_0040be70(static_cast<std::int32_t>(c->args[0]),static_cast<std::int32_t>(c->args[1]));
    return 0; // Caller-specific dead EAX; real source side effects were executed.
}

extern "C" Word __cdecl kotor_menu_service_005DF5D0(const Call* c) {
    checked(c,0x005DF5D0);
    reinterpret_cast<kotor_menu20::Input*>(c->self)->set_mouse_005df5d0(static_cast<int>(c->args[0]),static_cast<int>(c->args[1]));
    return 0; // Caller-specific dead EAX; real source side effects were executed.
}

extern "C" Word __cdecl kotor_menu_service_0040B060(const Call* c) {
    checked(c,0x0040B060);
    const Word result=static_cast<Word>(reinterpret_cast<gui_mouse::Gui*>(c->self)->activate_0040b060());
    return result;
}

extern "C" Word __cdecl kotor_menu_service_0061F980(const Call* c) {
    checked(c,0x0061F980);
    reinterpret_cast<menu_providers_40_49::Internal*>(c->self)->mouse_0061f980(static_cast<Word>(c->args[0]),static_cast<Word>(c->args[1]),static_cast<Word>(c->args[2]));
    return 0; // Caller-specific dead EAX; real source side effects were executed.
}

extern "C" Word __cdecl kotor_menu_service_00409F00(const Call* c) {
    checked(c,0x00409F00);
    const Word result=static_cast<Word>(reinterpret_cast<gui_sounds::Gui*>(c->self)->load_00409f00());
    return result;
}

extern "C" Word __cdecl kotor_menu_service_0062FED0(const Call* c) {
    checked(c,0x0062FED0);
    return reinterpret_cast<Word>(reinterpret_cast<menu_normal_40_49::Gui*>(c->self)->construct_0062fed0());
}

extern "C" Word __cdecl kotor_menu_service_005F2BF0(const Call* c) {
    checked(c,0x005F2BF0);
    reinterpret_cast<kotor_client_normal::ClientPrefix*>(c->self)->InitializeMoviePlayer();
    return 0; // Caller-specific dead EAX; real source side effects were executed.
}

extern "C" Word __cdecl kotor_menu_service_005E67E0(const Call* c) {
    checked(c,0x005E67E0);
    reinterpret_cast<kotor_menu20::Ini*>(c->self)->destroy_005e67e0();
    return 0; // Caller-specific dead EAX; real source side effects were executed.
}
