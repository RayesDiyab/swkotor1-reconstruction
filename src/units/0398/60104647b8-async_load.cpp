#include "496dab8016-async_load.h"
#include "26555ce7ea-canonical_assign.h"
#include "04fadd4a60-string_priority.h"
#include <new>
using namespace async_load;
namespace {
// Original dead-temporary storage is reused for the16-byte Ref output and
// subsequent query DWORD. Keep both address identity and legal active lifetimes.
struct Scratch {
    Word other,gap;
    union Shared {Ref result;Word words[4];} shared;
    Ref* Output(){return ::new(static_cast<void*>(&shared.result)) Ref;}
    Word* QueryWord(){Word old;memcpy(&old,&shared,sizeof(old));shared.words[0]=old;return &shared.words[0];}
};
static_assert(offsetof(Scratch,shared)==8,"original output-word separation");
Word Query(Manager* m,Ref* ref,Word type,Scratch& scratch,bool first=false) {
    Word* payload=scratch.QueryWord();
    return unresolved_key_entry_00407230(m,nullptr,ref,type,
        first?payload:&scratch.other,first?&scratch.other:payload);
}
void ConcatenateAssign(String* target,String* left,String* right) {
    alignas(String) unsigned char storage[sizeof(String)];
    // The accepted copy-construction carrier within concat starts output life.
    String* joined=GuiConcat005e5d10(left,nullptr,reinterpret_cast<String*>(storage),right);
    async_assign_string_005e5c50(target,nullptr,joined);
    joined->~String();
}
void Prefix(String* target,const char* text,String* right) {
    String literal(text);ConcatenateAssign(target,&literal,right);
}
void Suffix(String* target,String* left,const char* text) {
    String literal(text);ConcatenateAssign(target,left,&literal);
}
void Flag(Manager* m,unsigned char mask) {
    m->bytes[0x5c]=static_cast<unsigned char>(m->bytes[0x5c]|mask);
}
}
extern "C" __declspec(noinline) void __fastcall rim_load_004094a0(void* receiver,void*) {
    Manager* manager=static_cast<Manager*>(receiver);
    rim_enter_005e9300(manager->bytes+0x40,nullptr);
    String work;
    String unused_second; // Observed construction and destruction must remain.
    Ref reference;
    Scratch scratch;
    Word success=1;
    Word completed=0;
    String* name=reinterpret_cast<String*>(manager->bytes+0x4c);
    if(manager->word(0x54)==0) {
        Prefix(&work,"RIMS:",name);
        success=unresolved_add_key_00406e20(manager,nullptr,&work,4,0);
        goto FinalState;
    }
    Suffix(&work,name,"_a");
    async_ref_assign_string_004061f0(&reference,nullptr,scratch.Output(),&work);
    if(Query(manager,&reference,0xbba,scratch,true)) {
        Prefix(&work,"RIMS:",&work);
        success=unresolved_add_key_00406e20(manager,nullptr,&work,4,0);
        if(!success)goto CurrentStage;
        Flag(manager,0x10);
    }
    Suffix(&work,name,"_adx");
    async_ref_assign_string_004061f0(&reference,nullptr,scratch.Output(),&work);
    if(Query(manager,&reference,0xbba,scratch)) {
        Prefix(&work,"RIMS:",&work);
        success=unresolved_add_key_00406e20(manager,nullptr,&work,4,0);
        if(!success)goto CurrentStage;
        Flag(manager,0x20);
    }
    {
        String literal("MODULES:");
        (void)unresolved_add_key_00406e20(manager,nullptr,&literal,2,0);
    }
    async_ref_assign_string_004061f0(&reference,nullptr,scratch.Output(),name);
    if(!Query(manager,&reference,0x7db,scratch)) {
        Suffix(&work,name,"_s");
        async_ref_assign_string_004061f0(&reference,nullptr,scratch.Output(),&work);
        if(Query(manager,&reference,0xbba,scratch)) {
            Prefix(&work,"MODULES:",&work);
            success=unresolved_add_key_00406e20(manager,nullptr,&work,4,0);
            if(success) {completed=1;Flag(manager,0x08);}
        }
    } else {
        Prefix(&work,"MODULES:",name);
        success=unresolved_add_key_00406e20(manager,nullptr,&work,3,2);
        if(success)Flag(manager,0x02);
    }
    {
        String literal("MODULES:");
        (void)async_remove_key_00407830(manager,nullptr,&literal,2);
    }
CurrentStage:
    if(manager->word(0x58)==0 && success!=0) {
        Prefix(&work,"CURRENTGAME:",name);
        {
            String literal("CURRENTGAME:");
            (void)unresolved_add_key_00406e20(manager,nullptr,&literal,2,0);
        }
        async_ref_assign_string_004061f0(&reference,nullptr,scratch.Output(),name);
        Word found=Query(manager,&reference,0xbc1,scratch);
        if(!found) {
            async_ref_construct_string_00406d60(&reference,nullptr,name);
            found=Query(manager,&reference,0xbba,scratch);
        }
        if(found) {
            success=unresolved_add_key_00406e20(manager,nullptr,&work,4,0);
            if(success)Flag(manager,0x01);
        }
        {
            String literal("CURRENTGAME:");
            (void)async_remove_key_00407830(manager,nullptr,&literal,2);
        }
    } else {
        Prefix(&work,"CURRENTGAME:",name);
        success=unresolved_add_key_00406e20(manager,nullptr,&work,3,2);
        if(success)Flag(manager,0x04);
    }
FinalState:



    manager->bytes[0x44]=success?static_cast<unsigned char>(completed?2:3):4;

    rim_leave_005e9310(manager->bytes+0x40,nullptr);
    // Destruction order is unused_second then work, after Leave, as observed.
}
