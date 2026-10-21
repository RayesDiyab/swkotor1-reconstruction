#pragma once
#include <cstdint>
namespace recovered_template_inventory {using U=std::uint32_t;
struct Ports {virtual ~Ports()=default;virtual U read(U)=0;virtual U word(U)=0;virtual void write(U,U)=0;virtual void default_inventory(U,U)=0;virtual U party()=0;virtual U list(U,U,U,const char*)=0;virtual U count(U,U)=0;virtual void element(U,U,U,U)=0;virtual U object_id(U,U,U)=0;virtual U find(U)=0;virtual U item(U)=0;virtual U allocate(U)=0;virtual U construct(U,U)=0;virtual void empty_ref(U)=0;virtual void ref(U,U,U,const char*,U,U)=0;virtual U load_template(U,U)=0;virtual U dropable(U,U,U)=0;virtual U load_saved(U,U,U)=0;virtual void destroy(U)=0;virtual U type(U,U)=0;virtual void owner(U,U,U,U,U)=0;virtual U can_equip(U,U,U)=0;virtual void equip(U,U,U)=0;virtual U inventory(U,U,U,U,U,U)=0;virtual U finish(U)=0;};
void load(U self,U gff,U root,U restore_ids,U owner_mode,U templates,U scratch,Ports&);
}
