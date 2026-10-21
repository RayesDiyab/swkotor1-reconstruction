#pragma once
#include "c534281817-startup_string.h"
#include <cstdio>
namespace file_internal {
using Word=std::uint32_t;using String=kotor_reconstruction::StartupString;
// Full observed Internal20 used by every admitted reconstructed file operation.
// The modern hosting boundary owns a CRT FILE; historical FILE fields are never used.
struct Internal {std::FILE* stream;String name;String mode;};
static_assert(sizeof(Internal)==20&&alignof(Internal)==4&&offsetof(Internal,stream)==0&&offsetof(Internal,name)==4&&offsetof(Internal,mode)==12,"Internal20");
}
