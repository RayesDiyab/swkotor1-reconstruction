extern "C" unsigned __fastcall area_no_nonwalk_polys_detailed_00506650(void*,void*,GroundVector*,GroundVector*,float,float,float,unsigned,unsigned*,int,kotor_reconstruction::ArrayMutations*,int,void*,int);
struct NativeAreaRegion final:AreaRegionPorts{
 Word detailed(Word self,Word a,Word b,Word c,Word d,Word e,Word f,Word out,Word p8,Word p9,Word p10,Word p11,Word p12)override{float x,y,z;std::memcpy(&x,&c,4);std::memcpy(&y,&d,4);std::memcpy(&z,&e,4);return area_no_nonwalk_polys_detailed_00506650(ptr(self),nullptr,static_cast<GroundVector*>(ptr(a)),static_cast<GroundVector*>(ptr(b)),x,y,z,f,static_cast<unsigned*>(ptr(out)),static_cast<int>(p8),static_cast<kotor_reconstruction::ArrayMutations*>(ptr(p9)),static_cast<int>(p10),ptr(p11),static_cast<int>(p12));}
};
