extern "C" int __fastcall collision_mesh_no_nonwalk_polys_005972b0(void*,void*,GroundVector*,GroundVector*,float,float,float,void*);
struct NativeRoomValidHelpers final:RoomValidHelperPorts{
 Word collision(Word a,Word b,Word c,Word low,Word high,Word radius,Word hit)override{float l,h,r;std::memcpy(&l,&low,4);std::memcpy(&h,&high,4);std::memcpy(&r,&radius,4);return static_cast<Word>(collision_mesh_no_nonwalk_polys_005972b0(ptr(a),nullptr,static_cast<GroundVector*>(ptr(b)),static_cast<GroundVector*>(ptr(c)),l,h,r,ptr(hit)));}
};
