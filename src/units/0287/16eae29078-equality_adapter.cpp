#include "d3d1816205-providers.h"
#include "13c249825e-operations.h"
#include <string.h>
// Carrier/data adaptation only: the comparison algorithm is the unchanged,
// already-owned task11 source. No new004060B0 reconstruction credit is claimed.
extern "C" int __fastcall resource_equal_004060b0(menu_resources::Ref* a,void*,menu_resources::Ref* b){
 kotor_reconstruction::CResRef left,right;memcpy(left.bytes,a,16);memcpy(right.bytes,b,16);return left.operator==(&right);
}
