#include "d31ceadfd3-safe_position_search.inc"
extern "C" void* __fastcall surface_hit_info_construct_00581320(void*,void*,unsigned);
struct NativeAreaFindRoom final:AreaFindRoomPorts {
 Word offset_z(Word bits,bool up)override{float input,result,delta=1000.0f;std::memcpy(&input,&bits,4);if(up){__asm{fld input
 fadd delta
 fstp result}}else{__asm{fld input
 fsub delta
 fstp result}}Word out;std::memcpy(&out,&result,4);return out;}
 void hit_construct(Word a,Word b)override{(void)surface_hit_info_construct_00581320(ptr(a),nullptr,b);}
 Word ray(Word a,Word x,Word y,Word z,Word u,Word v,Word w,Word hit)override{NativeMemory m;NativeRoomGroundRay p;Word from[3]={x,y,z},to[3]={u,v,w};alignas(8) unsigned char scratch[0x18];return room_ground_ray_00579560(m,p,a,word(from),word(to),hit,word(scratch));}
};
#include "02e38abf8b-area_safe_valid_native_reaction.inc"
struct NativeAreaSafePosition final:AreaSafePositionPorts {NativeMemory&m;explicit NativeAreaSafePosition(NativeMemory&v):m(v){}
 Word room(Word a,Word b,Word c)override{NativeAreaFindRoom p;alignas(8) unsigned char scratch[0x48];return area_find_room_004bb600(m,p,a,b,c,word(scratch));}
 Word height_bits(Word a,Word b,Word x,Word y,Word z)override{NativeRoomHeight p;Word scratch[3];return room_height_bits_00579470(m,p,a,b,x,y,z,word(scratch));}
 Word valid(Word a,Word b,Word c)override{NativeAreaSafeValid p(m);alignas(8) unsigned char scratch[0x5c];return area_safe_valid_004be5e0(m,p,a,b,c,word(scratch));}
 Word search_004be93d(Word area,Word input,Word range_bits,Word shape,Word mode,Word output,Word scratch)override{
  using namespace safe_position_search;
  const Word current=scratch+0x54;
  W1Vector walk_point{m.read(current),m.read(current+4),m.read(current+8)};
  if(recovered_area_position_walkable_00506400(ptr(area),nullptr,walk_point)){
   Word top;
   initial_box(static_cast<Word*>(ptr(current)),m.read(shape+0x14),static_cast<Word*>(ptr(scratch+0x6c)),static_cast<Word*>(ptr(scratch+0x60)),&top);
   if(!doors(area,scratch+0x6c,scratch+0x60,m.read(current+8),top,m.read(shape+4),scratch+0x30))mode=0;
  }else mode=0;
  Word radius=0x3f800000u;
  if(!below(radius,range_bits))return 0;
  const Word cached_step=m.read(scratch+0x18);
  const auto succeed=[&](){
   const Word x=m.read(current),y=m.read(current+4);
   m.write(output,x);const Word z=m.read(current+8);
   m.write(output+4,y);m.write(output+8,z);return Word(1);
  };
  // Each call performs the original room query and valid-position test. Scratch
  // z is retained until room-height receives its by-value point.
  const auto valid_point=[&](Word x,Word y){
   m.write(current,x);m.write(current+4,y);
   const Word found=room(area,current,scratch+0x50);
   Word z=0;
   if(found)z=height_store(w5_room_compute_height_00579430(ptr(found),nullptr,vector_at(current),0));
   m.write(current+8,z);
   return valid(area,current,shape)==1;
  };
  do{
   const Word xmin=subtract(m.read(input),radius);
   const Word xmax=add(radius,m.read(input));
   Word ymin=subtract(m.read(input+4),radius);
   Word ymax=add(radius,m.read(input+4));
   Word x=xmin;
   if(at_most(xmin,xmax)){
    do{
     Word y=ymin;
     if(at_most(ymin,ymax)){
      do{
       if(valid_point(x,y)){
        Word band_lower,band_upper;
        horizontal_band(m.read(current+8),m.read(input+8),m.read(shape+0x14),&band_lower,&band_upper);
        if(!mode)return succeed();
        const int line=direct_line(area,current,input,m.read(shape+4),m.read(shape+0x14));
        if((line==1||line==-3)&&doors(area,current,input,band_lower,band_upper,cached_step,scratch+0x30))return succeed();
       }
      }while(opposite(&y,ymin,ymax));
     }
    }while(advance(&x,cached_step,xmax,false));
   }
   x=xmin;
   if(at_most(xmin,xmax)){
    ymin=add(ymin,cached_step);ymax=subtract(ymax,cached_step);
    do{
     Word y=ymin;
     if(at_most(y,ymax)){
      do{
       if(valid_point(x,y)){
        Word band_lower,band_upper;vertical_band(m.read(current+8),m.read(input+8),&band_lower,&band_upper);
        const Word height=m.read(shape+0x14);
        if(!mode)return succeed();
        if(direct_line(area,current,input,m.read(shape+4),height)){
         band_upper=add(height,band_upper);band_lower=subtract(band_lower,0x3dcccccdu);
         if(doors(area,current,input,band_lower,band_upper,cached_step,scratch+0x30))return succeed();
        }
       }
      }while(advance(&y,cached_step,ymax,false));
     }
    }while(opposite(&x,xmin,xmax));
   }
  }while(advance(&radius,cached_step,range_bits,true));
  return 0;
 }
};
Word native_area_safe_position(Word a,Word b,Word c,Word d,Word e,Word f){NativeMemory m;NativeAreaSafePosition p(m);alignas(8) unsigned char scratch[0x90];return area_safe_position_004be860(m,p,a,b,c,d,e,f,word(scratch));}
