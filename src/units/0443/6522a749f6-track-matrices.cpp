#include "6caffdfb9c-peer-track.h"
extern "C" __declspec(noinline) void __cdecl renderer_track_004a2400(){
    auto& v=renderer_track::bindings();




#define READ_ARB (*v.arb_0078e5ec)

    if(!READ_ARB){
        const auto transform=*v.transform_0073f224,matrix=*v.matrix_0073f218;
        (*v.track_007bb744)(0x8620,0,matrix,transform);



        if(!READ_ARB){

            const auto next_transform=*v.transform_0073f224,next_matrix=*v.matrix_0073f21c;



            (*v.track_007bb744)(0x8620,8,next_matrix,next_transform);

        }
    }
    if(READ_ARB)(*v.arb_bind_007bb788)(0x8620,0);else (*v.nv_bind_007bb834)(0x8620,0);
#undef READ_ARB
}
