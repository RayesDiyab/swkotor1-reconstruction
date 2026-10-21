#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include "ea06fcb788-locale.h"
namespace kotor_functional {
bool source_heap_identity() noexcept;
class LocaleScope {
 CRITICAL_SECTION lock_{};
 void* threads_=nullptr;
 DWORD owner_=0;
 bool claimed_=false;
 unsigned classification_mode_=0,mapping_mode_=0,codepage_=0,locale_id_=0;
public:
 bool claim() noexcept;
 bool close() noexcept; // Other participating threads must have exited/joined.
 bool identity() const noexcept;
 void enter(int);
 void leave(int);
 kotor_locale::Ptd* thread();
 void detach(void*) noexcept;
 LocaleScope()=default;
 ~LocaleScope();
 LocaleScope(const LocaleScope&)=delete;
 LocaleScope& operator=(const LocaleScope&)=delete;
};
bool locale_identity() noexcept;
}
