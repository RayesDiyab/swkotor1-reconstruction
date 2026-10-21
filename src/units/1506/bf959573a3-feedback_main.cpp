// 005FCD10 CClientExoAppInternal::AddFeedbackMessage. Original low-WORD selector, RET0Ch.
#include "027fd41e5b-feedback_context.h"
#include "5b9aa881dd-feedback_lowers.h"
#include "3f0abd0978-tlk_core.h"
#include "8c34f546dd-native_strings.h"
#include "81f59c47ba-string_priority.h"
#include "03e67ad358-window_shared_cells.h"
#include "4583578ecb-feedback_sound_words.h"
#include "51d5fe5525-async_load.h"
#include <new>

extern "C" {
extern void* kotor_cell_tlk_client_007a3a0c;
extern void* kotor_cell_rules_007a3a28;
extern std::uint32_t kotor_rules_cell32_007a224c,kotor_rules_cell32_007a2250;
extern std::uint32_t kotor_rules_cell32_007a2254,kotor_rules_cell32_007a2264;
kotor_reconstruction::StartupString* __fastcall kotor_fastcall_005fb540(void*,void*,kotor_reconstruction::StartupString*,std::uint32_t) noexcept(false);
void __fastcall kotor_fastcall_0041db50(void*,void*,std::uint32_t,kotor_reconstruction::StartupString*) noexcept(false);
unsigned __fastcall recovered_message_get_integer_004d6480(void*,void*,unsigned);
unsigned __fastcall recovered_message_get_object_004d64c0(void*,void*,unsigned);
void* __fastcall recovered_event_string_004d7360(void*,void*,void*,unsigned);
void __fastcall recovered_message_data_destroy_004d6a40(void*,void*);
unsigned __fastcall frame_R4_005ed5b0(void*,void*,unsigned);
void* __fastcall recovered_client_get_door_by_id_005ed600(void*,void*,std::uint32_t) noexcept(false);
void* __fastcall recovered_client_get_placeable_by_id_005ed5f0(void*,void*,std::uint32_t) noexcept(false);
void* __fastcall kotor_m15b_spell_array_get_spell_0059b6d0(void*,void*,std::int32_t);
void __cdecl genuine_x87_ftol_006fae8c();
unsigned __fastcall frame_LogIsEmpty_005e58a0(void*,void*);
void __cdecl live_free(void*);
}
namespace feedback {
String* lookup(Context& c,U reference,ReturnedString& temporary) {
    return temporary.accept(kotor_fastcall_005fb540(c.self,nullptr,temporary.storage(),reference));
}
String* fetch(U reference,void* context,ReturnedString& temporary,U unused,U flags) {
    return temporary.accept(feedback_fetch_005ee390(kotor_cell_tlk_client_007a3a0c,nullptr,
                                                   temporary.storage(),reference,context,unused,flags));
}
void token(U index,String* value) { kotor_fastcall_0041db50(kotor_cell_tlk_007a3a08,nullptr,index,value); }
U integer(Context& c,I index) { return recovered_message_get_integer_004d6480(c.data,nullptr,static_cast<U>(index)); }
U object(Context& c,I index) { return recovered_message_get_object_004d64c0(c.data,nullptr,static_cast<U>(index)); }
U resolve_name(Context& c,U object_id,String* output) { return feedback_object_name_005f6640(c.self,nullptr,object_id,output); }
String* message_string(Context& c,I index,ReturnedString& temporary) {
    return temporary.accept(static_cast<String*>(recovered_event_string_004d7360(c.data,nullptr,temporary.storage(),static_cast<U>(index))));
}
String* from_integer(I value,ReturnedString& temporary) {
    return temporary.accept(resman_string_from_int_005e5bc0(temporary.storage(),nullptr,value));
}
const char* cstr(String* value) { return WindowCStr005e5670(value,nullptr); }
namespace {
void* app() { return read<void*>(kotor_menu_window_cells::manager_007a39fc,4); }
void assign_lookup(Context& c,U reference) {
    ReturnedString temporary; c.output.Assign005e5c50(lookup(c,reference,temporary));
}
void assign_fetch(Context& c,U reference) {
    ReturnedString temporary; c.output.Assign005e5c50(fetch(reference,nullptr,temporary));
}
void integer_token(U index,U value) {
    ReturnedString temporary; token(index,from_integer(static_cast<I>(value),temporary));
}
void string_token(Context& c,U index,I string_index) {
    ReturnedString temporary; token(index,message_string(c,string_index,temporary));
}
void name_token(Context& c,U object_id,U index) {
    if(resolve_name(c,object_id,&c.object_name)==0) {
        ReturnedString temporary; c.object_name.Assign005e5c50(lookup(c,0,temporary));
    }
    token(index,&c.object_name);
}
void object_feedback(Context& c,U reference) {
    name_token(c,object(c,0),0); assign_fetch(c,reference);
}
I truncate_amount(float amount) {
    I result;
    __asm {
        fld dword ptr amount
        call genuine_x87_ftol_006fae8c
        mov result,eax
    }
    return result;
}
void* table(U offset) { return read<void*>(read<void*>(kotor_cell_rules_007a3a28,0xb8),offset); }

// 005FD9B4..005FDC42: door/placeable lock feedback, including original sound and object notification.
void case13(Context& c) {
    const U id=object(c,0);
    {
        String sound_text;
        void* const door=recovered_client_get_door_by_id_005ed600(app(),nullptr,id);
        void* const placeable=recovered_client_get_placeable_by_id_005ed5f0(app(),nullptr,id);
        if(door!=nullptr) {
            I sound_index; // original does not initialize the output before the C2DA lookup.
            const U appearance=read<unsigned char>(door,0x124);
            if(appearance==0) {
                c2da_current_get_int_entry_00413510(static_cast<c2da_current::Table*>(table(0x28)),nullptr,
                    read<unsigned char>(door,0x125),static_cast<I>(kotor_rules_cell32_007a2250),&sound_index);
            } else {
                c2da_current_get_int_entry_00413510(static_cast<c2da_current::Table*>(table(0x24)),nullptr,
                    static_cast<I>(appearance),static_cast<I>(kotor_rules_cell32_007a224c),&sound_index);
            }
            c2da_current_get_string_entry_00413190(static_cast<c2da_current::Table*>(table(0x70)),nullptr,
                sound_index,static_cast<I>(kotor_rules_cell32_007a2264),&sound_text);
            if(sound_text.BorrowedLength005e5790()>0) {
                alignas(4) unsigned char reference[16];
                async_ref_construct_string_00406d60(reinterpret_cast<menu_resources::Ref*>(reference),nullptr,&sound_text);
                play_feedback_sound_words(reference,static_cast<unsigned char*>(door)+0x24,0x3fc00000u);
            }
            ReturnedString temporary; feedback_set_info_0063d2d0(door,nullptr,lookup(c,0x59f,temporary));
        } else if(placeable!=nullptr) {
            I sound_index;
            c2da_current_get_int_entry_00413510(static_cast<c2da_current::Table*>(table(0x2c)),nullptr,
                read<unsigned char>(placeable,0x10c),static_cast<I>(kotor_rules_cell32_007a2254),&sound_index);
            c2da_current_get_string_entry_00413190(static_cast<c2da_current::Table*>(table(0x70)),nullptr,
                sound_index,static_cast<I>(kotor_rules_cell32_007a2264),&sound_text);
            if(sound_text.BorrowedLength005e5790()>0) {
                alignas(4) unsigned char reference[16];
                async_ref_construct_string_00406d60(reinterpret_cast<menu_resources::Ref*>(reference),nullptr,&sound_text);
                play_feedback_sound_words(reference,static_cast<unsigned char*>(placeable)+0x24,0u);
            }
            ReturnedString temporary; feedback_set_info_0063d2d0(placeable,nullptr,lookup(c,0x59f,temporary));
        }
    }
    assign_lookup(c,0x59d);
}

bool dispatch(Context& c) {
    switch(c.selector) {
#include "72d3ebd9e4-feedback_simple_cases.inc"
#include "b12950da09-feedback_medium_cases.inc"
    case 13: case13(c); return true;
    case 62: return case62(c);
    case 160:case 161:case 162:case 163:case 164:case 165: return case160_165(c);
    default: return true;
    }
}
} // anonymous
} // feedback

extern "C" void __fastcall feedback_add_005fcd10(void* self,void*,std::uint32_t selector,void* data,void* prefix) {
    using namespace feedback;
    String object_name; // state0, 005FCD36; destroyed last.
    String output(""); // state1, 005FCD3F.
    void* const gui=read<void*>(self,0x40);
    const U cached_id=read<U>(self,0x3bc);
    const U player_id=read<U>(self,0x20);
    void* player;
    if(cached_id==player_id) {
        player=read<void*>(self,0x3c0);
    } else {
        player=reinterpret_cast<void*>(static_cast<std::uintptr_t>(frame_R4_005ed5b0(
            read<void*>(kotor_menu_window_cells::manager_007a39fc,4),nullptr,player_id)));
        write(self,0x3c0,player);
        write(self,0x3bc,read<U>(self,0x20));
    }
    Context context{self,data,static_cast<std::uint16_t>(selector),player,object_name,output};
    if(dispatch(context) && frame_LogIsEmpty_005e58a0(&output,nullptr)==0) {
        {
            ReturnedString temporary;
            output.Assign005e5c50(temporary.accept(GuiConcat005e5d10(static_cast<String*>(prefix),nullptr,
                                                                   temporary.storage(),&output)));
        }
        feedback_append_0062b5c0(gui,nullptr,&output,0x80,0);
    }
    // Data ownership transfers only on normal completion; original EH cleans strings alone.
    if(data!=nullptr) {
        recovered_message_data_destroy_004d6a40(data,nullptr);
        live_free(data);
    }
}
