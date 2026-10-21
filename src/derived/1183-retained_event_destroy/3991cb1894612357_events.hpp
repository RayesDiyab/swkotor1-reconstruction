#pragma once
#include "b77f404b81972511_module.hpp"
namespace kotor::module {
struct EventStorage {
    virtual ~EventStorage()=default;
    virtual void allocate_list(Word list,Word count)=0; // existing 005E03C0
    virtual void release(Word address)=0; // existing 006FA390
};
Word construct_script_event(Memory&,Word self);
void set_event_object_id(Memory&,EventStorage&,Word self,Word index,Word object_id);
}
