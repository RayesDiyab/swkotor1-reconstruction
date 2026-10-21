#include "01382a435f-bindings.h"
namespace kotor_reporter_action_data {
#include "be8d2499a9-messages.inc"
Tables tables={
 {
#include "fbe6f3a370-initial_records.inc"
 },
 {
#include "ef8b6aaea9-initial_actions.inc"
 },3,7,120,10
};
ReporterView reporter_view(){





 return {tables.reporter,tables.actions};

}
}
namespace kotor_signal_storage {
Word signal2,signal21,signal22,signal15;
Word& fpe_first=kotor_reporter_action_data::tables.fpe_first;
Word& fpe_count=kotor_reporter_action_data::tables.fpe_count;
Word& action_bytes=kotor_reporter_action_data::tables.action_bytes;




Word& lookup_count=kotor_reporter_action_data::tables.lookup_count;

Action (&action_table)[10]=kotor_reporter_action_data::tables.actions;
}
namespace kotor_signal {
const Globals globals={&kotor_signal_storage::signal2,&kotor_signal_storage::signal21,&kotor_signal_storage::signal22,&kotor_signal_storage::signal15,&kotor_reporter_action_data::tables.fpe_first,&kotor_reporter_action_data::tables.fpe_count};
}
namespace kotor_runtime_reporter_dependencies {
// Existing task25 declaration, now a borrowed physical view of canonical data.
// Typed reporter_view is the proposed defined-access source interface for index18.
const volatile ErrorRecord* error_records_007a2ee8=kotor_reporter_action_data::tables.reporter;
}
