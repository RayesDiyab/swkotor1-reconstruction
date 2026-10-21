#pragma once
namespace recovered_creature_placement {using U=unsigned;
struct Ports{virtual~Ports()=default;virtual U read(U)=0;virtual void write(U,U)=0;virtual U server()=0;virtual U player(U,U)=0;virtual U area(U)=0;virtual void remove(U,U)=0;virtual U ai(U)=0;virtual U objects(U)=0;virtual U lookup(U,U,U)=0;virtual U success()=0;virtual U cast(U)=0;virtual void associate(U,U,U,U,U,U)=0;virtual U message(U)=0;virtual void transfer(U,U,U,U,U,U,U,U)=0;virtual void set_area(U,U)=0;virtual void position(U,U,U,U,U)=0;virtual void insert(U,U,U)=0;virtual void priority(U,U,U)=0;virtual U mode(U)=0;virtual void notify(U,U,U,U)=0;};
void place(U self,U area,U x,U y,U z,U saved,U finalFlag,U scratch,Ports&);
}
