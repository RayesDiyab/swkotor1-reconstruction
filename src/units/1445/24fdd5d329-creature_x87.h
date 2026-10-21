#pragma once
// Narrow instruction helpers for the original m32 stores, unordered branches and
// operand order. No call is hidden from the compiler inside inline assembly.
namespace creature_x87 {
inline float store_m32(double value) { float result;
    __asm { fld value
        fstp result
    }
    return result;
}
inline float difference(float left,float right) { float result;
    __asm { fld left
        fsub right
        fstp result
    }
    return result;
}
inline float add_length(double magnitude_value,float distance) { float result;
    __asm { fld magnitude_value
        fadd distance
        fstp result
    }
    return result;
}
inline unsigned compare(float left,float right) { unsigned short status;
    __asm { fld left
        fcomp right
        fnstsw status
    }
    return static_cast<unsigned>(status>>8);
}
inline unsigned compare(double left,float right) { unsigned short status;
    __asm { fld left
        fcomp right
        fnstsw status
    }
    return static_cast<unsigned>(status>>8);
}
inline float load(float value) { float result;
    __asm { fld value
        fstp result
    }
    return result;
}
inline float times(float left,float right) { float result;
    __asm { fld left
        fmul right
        fstp result
    }
    return result;
}
inline float ratio(float rate,float table,float duration,bool duration_first=false) {
    // TEST AH,44/JNP: only equal skips; unordered passes.
    if(duration_first) {
        if((compare(duration,0.0f)&0x44u)==0x40u || (compare(table,0.0f)&0x44u)==0x40u) return 0.0f;
    } else {
        if((compare(table,0.0f)&0x44u)==0x40u || (compare(duration,0.0f)&0x44u)==0x40u) return 0.0f;
    }
    const float scale=0.0010000000474974513f; float result;
    __asm { fld rate
        fmul scale
        fld table
        fdiv duration
        fdivp st(1),st(0)
        fstp result
    }
    return result;
}
inline float special_time(float rate,double denominator,float length_value) {
    const float scale=0.0010000000474974513f;float result;
    __asm { fld denominator
        fdivr rate
        fmul length_value
        fmul scale
        fstp result
    }
    return result;
}
inline float normal_time(double length_value,float elapsed,float duration) {
    const float scale=0.0010000000474974513f;float result;
    __asm { fld length_value
        fld elapsed
        fdiv duration
        fmulp st(1),st(0)
        fmul scale
        fstp result
    }
    return result;
}
}
