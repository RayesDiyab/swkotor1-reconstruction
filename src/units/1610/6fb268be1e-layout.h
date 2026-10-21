#pragma once
#include "c0a1ef64ff-signal_dispatch.h"
#include "b274ea6022-runtime_reporter.h"
#include <type_traits>
// Windows' legacy function-name macro otherwise rewrites the already-declared
// Action field token. Only this candidate's translation unit is affected.
#ifdef exception_code
#undef exception_code
#endif
namespace kotor_reporter_action_data {
using kotor_signal::Word;
using kotor_signal::Action;
using ErrorRecord=kotor_runtime_reporter_dependencies::ErrorRecord;
struct Tables {
 ErrorRecord reporter[18];
 Action actions[10];
 Word fpe_first,fpe_count,action_bytes,lookup_count;
};
static_assert(std::is_standard_layout<Tables>::value,"explicit x86 data layout");
static_assert(sizeof(void*)==4&&sizeof(int)==4&&alignof(Tables)==4,"historical words/alignment");
static_assert(offsetof(Tables,actions)==144&&offsetof(Tables,fpe_first)==264&&offsetof(Tables,lookup_count)==276&&sizeof(Tables)==280,"007a2ee8..007a2fff exact offsets");
extern Tables tables;
// Borrowed slot proxies retain live field rereads; index18 reads the actual
// first action fields and does not manufacture a reporter sentinel object.
struct CodeView {
 const volatile ErrorRecord* record;
 const volatile Action* action;
 operator int()const{return action?static_cast<std::int32_t>(action->exception_code):record->code;}
};
struct TextView {
 const volatile ErrorRecord* record;
 const volatile Action* action;
 operator const char*()const{return action?reinterpret_cast<const char*>(static_cast<Word>(action->signal)):record->text;}
};
struct ReporterSlot {CodeView code;TextView text;};
struct ReporterView {
 const volatile ErrorRecord* records;
 const volatile Action* actions;
 ReporterSlot operator[](unsigned index)const{
  // The original reporter's fixed search produces indices0..18.
  const volatile ErrorRecord* r=index==18?nullptr:&records[index];
  const volatile Action* a=index==18?actions:nullptr;
  return {{r,a},{r,a}};
 }
};
ReporterView reporter_view();
}
