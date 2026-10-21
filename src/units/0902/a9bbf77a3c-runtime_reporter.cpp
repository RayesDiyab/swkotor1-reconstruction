// lane-m48-b crt staging copy of task-25 reporter-data-consumer/source/runtime_reporter.cpp (00701D50 __NMSG_WRITE /
// __displayRuntimeError). Only change: the two relative includes point at the co-staged copies (same files).
#include "8d5467951c-layout.h"
#include "2513c38919-runtime_reporter.h"
#include <malloc.h>

namespace kotor_selected_termination_dependencies {
void __cdecl report_runtime_error_00701d50(int code) {
    namespace d = kotor_runtime_reporter_dependencies;
    // Borrow the canonical live fields; index18 aliases action0 without
    // an out-of-bounds ErrorRecord access or a copied sentinel.
    const auto records=kotor_reporter_action_data::reporter_view();
    unsigned index=0;
    while (index<18 && code!=records[index].code) ++index;
    // Original00701D82 compares index18 after an unsuccessful search.
    if (code!=records[index].code) return;
    const int mode=*d::error_mode_00833c34;
    if (mode==1 || (mode==0 && *d::application_type_007a2764==1)) {
        const DWORD count=static_cast<DWORD>(d::string_length_00701300(records[index].text));
        // Reread after strlen, before GetStdHandle; preserve that call ordering.
        const char* text=records[index].text;
        const HANDLE output=d::standard_handle_0073d104(static_cast<DWORD>(-12));
        // Original WriteFile uses the incoming code argument's own stack slot
        // for lpNumberOfBytesWritten; its BOOL result is ignored.
        d::write_file_0073d204(output,text,count,reinterpret_cast<DWORD*>(&code),nullptr);
        return;
    }
    if (code==252) return;
    char module_name[261];
    module_name[260]='\0';
    if (d::module_file_name_0073d18c(nullptr,module_name,260)==0)
        d::string_copy_00705910(module_name,"<program name unknown>");
    char* display=module_name;
    if (d::string_length_00701300(module_name)+1u>60u) {
        display=module_name+d::string_length_00701300(module_name)-59u;
        d::string_copy_n_006fab80(display,"...",3);
    }
    const std::uint32_t path_length=static_cast<std::uint32_t>(d::string_length_00701300(display));
    const std::uint32_t message_length=static_cast<std::uint32_t>(d::string_length_00701300(records[index].text));
    const std::uint32_t size=(path_length+message_length+28u+3u)&~3u;
    auto* output=static_cast<char*>(_alloca(size));
    d::string_copy_00705910(output,"Runtime Error!\n\nProgram: ");
    d::string_concat_00705920(output,display);
    d::string_concat_00705920(output,"\n\n");
    // Original reloads the selected table message after the earlier helpers.
    d::string_concat_00705920(output,records[index].text);
    d::message_box_007092de(output,"Microsoft Visual C++ Runtime Library",0x12010u);
}
}
