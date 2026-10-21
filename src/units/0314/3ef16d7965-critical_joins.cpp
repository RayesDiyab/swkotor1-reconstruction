#include "cfd53d6e4f-joins.h"
#include "69e15ca630-critical_section.h"
#include <type_traits>
static_assert(std::is_same<decltype(&rim_enter_005e9300),kotor_missing_odd::ThisGate>::value,"exact existing no-stack ECX transport");
static_assert(std::is_same<decltype(&rim_leave_005e9310),kotor_missing_odd::ThisGate>::value,"exact existing no-stack ECX transport");
extern "C" void __fastcall rim_enter_005e9300(void* receiver,void*) {
    static_cast<kotor_startup::CriticalSection*>(receiver)->Enter();
}
extern "C" void __fastcall rim_leave_005e9310(void* receiver,void*) {



    static_cast<kotor_startup::CriticalSection*>(receiver)->Leave();

}
