#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct ItemDestructPorts{virtual ~ItemDestructPorts()=default;virtual void tables(Word)=0;virtual Word client_id(Word,Word)=0;virtual Word client_item(Word,Word)=0;virtual Word client_objects(Word)=0;virtual Word remove_client_object(Word,Word,Word)=0;virtual void destroy(Word,Word)=0;virtual Word module(Word)=0;virtual void remove_tag(Word,Word,Word)=0;virtual void release(Word)=0;virtual Word area(Word)=0;virtual void detach(Word)=0;virtual Word master(Word)=0;virtual void remove_ai(Word,Word)=0;virtual void localized_destroy(Word)=0;virtual void object_destroy(Word)=0;virtual void base_destroy(Word)=0;};
void item_destruct_0055ec70(Memory&,ItemDestructPorts&,Word,Word);
}
