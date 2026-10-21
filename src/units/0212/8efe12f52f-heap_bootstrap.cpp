#include "e47fbf4cc4-heap_bootstrap.h"
HeapBootstrapGlobals heap_bootstrap_globals{};
HeapBootstrapApis heap_bootstrap_apis{};
extern "C" __declspec(noinline) std::uint32_t __cdecl source_heap_select(){



    return *heap_bootstrap_globals.platform==2u&&*heap_bootstrap_globals.major>=5u?1u:3u;

}
extern "C" __declspec(noinline) int __cdecl source_sbh_heap_init(std::uint32_t threshold){
    void* table=heap_bootstrap_apis.allocate(*heap_bootstrap_globals.heap,0,0x140u);
    *heap_bootstrap_globals.headers=reinterpret_cast<std::uint32_t>(table);
    if(!table)return 0;
    *heap_bootstrap_globals.spare_header=0;
    *heap_bootstrap_globals.count=0;
    *heap_bootstrap_globals.active_header=reinterpret_cast<std::uint32_t>(table);
    *heap_bootstrap_globals.threshold=threshold;



    *heap_bootstrap_globals.capacity=16u;




    return 1;
}
extern "C" __declspec(noinline) int __cdecl source_heap_init(int multithread){



    const DWORD flags=multithread==0?1u:0u;

    *heap_bootstrap_globals.heap=heap_bootstrap_apis.create(flags,0x1000u,0);
    if(!*heap_bootstrap_globals.heap)return 0;
    *heap_bootstrap_globals.mode=source_heap_select();
    if(*heap_bootstrap_globals.mode==3u&&!source_sbh_heap_init(0x3f8u)){

        heap_bootstrap_apis.destroy(*heap_bootstrap_globals.heap);




        return 0;
    }
    return 1;
}
