#include "7102fe53f2-startup_string.h"
namespace kotor_reconstruction {
int StartupString::BorrowedEquals005e5290(const char* compare) {



    const char* left = data;
    const char* right = compare;
    if (!left) return !right || *right == 0;
    if (!right) return *left == 0;
    for (;;) {
        if (*left != *right) return 0;
        if (*left == 0) return 1;
        ++left;
        ++right;
    }

}
std::int32_t StartupString::BorrowedLength005e5790() {



    if (data == nullptr) return 0;
    const char* end = data;
    while (*end != '\0') ++end;
    return static_cast<int>(end - data);

}
}
