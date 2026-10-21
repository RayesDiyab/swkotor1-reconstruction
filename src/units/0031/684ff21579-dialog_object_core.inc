// Full original0057A470 control flow. Native bindings in dialogue_object.cpp.
// Original virtual/direct calls remain distinct; x87 intermediates use original double width.
U dialog_object(U self,U node){
    auto fail=[&](){v(self,0xc0,{literal(0x746644)});c(0x62ec60,c(0x5ed690,client()),{0});return literal(0x745dc4);};
    if(v(self,0x94)==1 || c(0x63e7f0,self)==1)return fail(); //0057A493..4AC
    U target=rd(node,0x38),p40,p44,p48,p4c;
    String text; //005B3190, owned until every original normal/exception exit
    text.Assign005e5c50(reinterpret_cast<String*>(rd(node,0x3c)));
    p40=rd(node,0x40);p44=rd(node,0x44);p48=rd(node,0x48);p4c=rd(node,0x4c);
    auto requeue=[&](U receiver,U sixth_type){
        c(0x4ccb00,receiver,{0x18,r16(node,0x6c),3,addr(&target),4,addr(&text),1,addr(&p40),1,addr(&p44),1,addr(&p48),sixth_type,addr(&p4c),0,0,0,0,0,0,0,0,0,0,0,0,0,0});
    };
    I party_count=static_cast<I>(rd(c(0x5ed8b0,client()),0));
    if(party_count>0){
        I index=0;
        for(;index<party_count;++index){
            U member=c(0x6346c0,c(0x5ed8b0,client()),{static_cast<U>(index)});
            if(member && c(0x60fb20,member) && !c(0x4ef890,c(0x60fb20,member)))break;
        }
        if(index==party_count)return fail();
    }
    if(p4c==0x7f000000){
        U found;const U status=kotor_fastcall_004d8230(ptr(c(0x4aed70,server())),nullptr,target,&found); //0057A5AF typed output slot
        if((status&0xff)!=literal(0x74666c))return fail();
        U other=v(found,0x10);
        if(!rd(other,0xe8) || v(other,0x94)==1)return fail();
        (void)sc(0x4ae910,{rd(self,4)}); // original unused call retained
        U player_creature=c(0x4b2820,server());
        if(!player_creature)return literal(0x745dbc);
        U render=cc(0x5ed580,{cc(0x5eda50,{rd(player_creature,4)})});
        if(!render || !v(render,0x0c))return literal(0x745dbc);
        if(c(0x5edad0,client()) || c(0x5edc10,client()) || c(0x5edef0,client())){requeue(self,3);return literal(0x745dc0);}
        c(0x62ec60,c(0x5ed690,client()),{1});
        if(v(self,0x30)){
            U creature=v(self,0x30);c(0x4f87d0,creature,{2});c(0x4f00a0,creature,{4,0,0});
            if(!p48){
                bool need_move=false;U sixth_type=1;
                if(!p44){
                    Vector difference; c(0x423bc0,0,{addr(&difference),self+0x90,other+0x90});
                    // fcompp threshold^2 vs distance; unordered/equal skip movement.
                    need_move=move_distance_exceeds(difference);
                }else{
                    p44=0;need_move=c(0x4f6000,v(self,0x30),{target,0x3f800000,0})==0;sixth_type=3;
                }
                if(need_move){
                    U area=rd(v(found,0x10),0x8c);Vector point{0,0,0};float range;
                    (void)w5_creature_get_use_range_004ee440(ptr(v(self,0x30)),nullptr,target,&point,&range,0); //0057A7F3 typed outputs
                    range=static_cast<float>(static_cast<double>(range)+w5::flt(literal(0x73d764)));
                    Vector sum,mid;c(0x423b90,0,{addr(&sum),addr(&point),self+0x90});
                    c(0x423bf0,0,{addr(&mid),addr(&sum),0x3f000000});
                    point=mid;requeue(self,sixth_type);
                    Vector zero{0,0,0};
                    c(0x4f8a50,creature,{r16(node,0x6c),addr(&point),area,target,1,bits(range),0,0,0,0,0,0,0,addr(&zero),0,0,0});
                    return literal(0x745dc0);
                }
            }
        }
        (void)v(other,0x30); // original unused virtual call
        U player=sc(0x4ae910,{c(0x4aea40,server())});
        if(player && c(0x4cc2b0,self) && v(c(0x4cc2b0,self),0x2c) && rd(v(c(0x4cc2b0,self),0x2c),0x3a4) && rd(self,4)!=rd(player,0x38)){
            bool swap=false;
            if(!rd(other,0x218)){
                unsigned char ref[16];U result=v(other,0x80,{addr(ref)});
                if(c(0x406030,result) && rd(other,4)!=rd(player,0x38))swap=true;
            }
            if(swap){
                U pc=v(sc(0x4ae750,{rd(player,0x38)}),0x30);
                if(pc && rd(pc,0xa88)){
                    if(rd(pc,0x4e0) && r8(pc,0xac0)==1){c(0x62ec60,c(0x5ed690,client()),{0});return literal(0x745dc4);}
                    U first=c(0x6346c0,c(0x5ed8b0,client()),{0});
                    U first_server=sc(0x4ae770,{sc(0x4aea30,{rd(first,4)})});
                    Vector first_pos=read_vector(first_server+0x90);
                    c(0x63d470,first);c(0x4ccd80,first_server,{1});
                    for(U index=1,offset=0x88;static_cast<I>(offset)<0x198;++index,offset+=0x88){
                        U member=c(0x6346c0,c(0x5ed8b0,client()),{index});
                        if(!member)continue;
                        U member_server=sc(0x4ae750,{sc(0x4aea30,{rd(member,4)})});
                        if(member_server && v(member_server,0x30)){c(0x4ccd80,v(member_server,0x30),{1});c(0x63d470,member);}
                        Vector delta;c(0x423bc0,0,{addr(&delta),addr(&first_pos),member+0x24});
                        if(party_distance_at_least(delta)){ //unordered is original early skip
                            U party=c(0x5ed8b0,client());Vector facing=read_vector(party+offset+0x5c),position=read_vector(party+offset+0x4c);
                            U path=c(0x634cc0,party+0x2dc,{rd(party+offset,0x58)});
                            float degrees=read_float(path+0xc);Vector axis{0,0,1};alignas(4) unsigned char quaternion[16];
                            const float radians=static_cast<float>(degree_radians(degrees));
                            c(0x4ac960,addr(quaternion),{bits(axis.x),bits(axis.y),bits(axis.z),bits(radians)});
                            Vector rotated,desired;c(0x4a9bb0,0,{addr(&rotated),addr(quaternion),addr(&facing)});
                            c(0x423b90,0,{addr(&desired),addr(&position),addr(&rotated)});
                            sc(0x4aeb60,{addr(&desired),0x41200000,rd(member_server,4),0,addr(&desired)});
                            v(member,0x8c,{addr(&desired)});
                        }
                    }
                    Vector black{0,0,0};c(0x62abf0,c(0x5ed690,client()),{0,0,0x3f400000,addr(&black)});
                    c(0x5df540,input(),{0});wr(c(0x5ed690,client()),0xb98,1);p4c=rd(first_server,4);
                    if(c(0x4ef890,pc)){
                        U memory=allocate(0x8c);U effect=0;
                        if(memory){try{effect=c(0x503e40,memory,{1});}catch(...){release(memory);throw;}}
                        /* 0057ADDC: effect used unchecked as original */
                        w16(effect,8,literal(0x74668c));w16(effect,0xa,(r16(effect,0xa)&0xfff8)|literal(0x7473b8));
                        c(0x4d14f0,pc,{effect,0,0});wr(pc,0xf0,1);
                    }
                    requeue(pc,3);return literal(0x745dc4);
                }
            }else if(rd(other,4)==rd(player,0x38) && c(0x4cc2b0,other) && v(c(0x4cc2b0,other),0x2c) && rd(v(c(0x4cc2b0,other),0x2c),0x3a4)){
                cc(0x5edc30,{0xfffffffdu,0});
            }
        }
    }else{
        if(c(0x62ac60,c(0x5ed690,client()))){requeue(self,3);return literal(0x745dc4);}
        U replacement=sc(0x4ae750,{p4c});
        if(replacement && v(replacement,0x30)){
            Vector old=read_vector(self+0x90),position=read_vector(v(replacement,0x30)+0x90),orientation=read_vector(v(replacement,0x30)+0x9c);
            U other=sc(0x4ae750,{target});
            if(other && v(other,0x10))c(0x423bc0,0,{addr(&orientation),v(other,0x10)+0x90,addr(&position)});
            c(0x4cd1a0,self,{addr(&position),1,1,0});c(0x4cd170,self,{addr(&orientation)});
            c(0x4cd1a0,v(replacement,0x30),{addr(&old),1,1,0});
            if(c(0x4cc2b0,self) && v(c(0x4cc2b0,self),0x2c)){
                v(v(c(0x4cc2b0,self),0x2c),0x8c,{addr(&position)});
                v(v(c(0x4cc2b0,self),0x2c),0x88,{addr(&orientation)});
            }
            (void)cc(0x5ed5b0,{cc(0x5eda50,{rd(v(replacement,0x30),4)})});
            if(v(replacement,0x30) && c(0x4efb20,v(replacement,0x30)))v(c(0x4efb20,v(replacement,0x30)),0x8c,{addr(&old)});
            (void)sc(0x4ae910,{c(0x4aea40,server())});cc(0x5edc30,{0xfffffffdu,0});
            wr(c(0x5ed690,client()),0xba4,p4c);c(0x5df540,input(),{1});
        }
    }
    U memory=allocate(0x34),event=0;
    if(memory){try{event=c(0x4d7540,memory);}catch(...){release(memory);throw;}}
    /* 0057B156: event used unchecked as original */
    w16(event,0,7);c(0x4d76b0,event,{0,0xffffffff});c(0x4d76b0,event,{1,0xffffffff});
    c(0x4d76b0,event,{2,p40});c(0x4d76b0,event,{3,p48});c(0x4d6ae0,event,{0,addr(&text)});
    const U source=rd(self,4);c(0x4b08d0,c(0x4aed80,server()),{0,0,source,target,literal(0x7441dc),event});
    return literal(0x745dc0);
}
