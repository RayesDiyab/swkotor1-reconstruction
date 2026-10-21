#include "8b79ab8567-feedback_context.h"

namespace feedback {

bool case62(Context& c) {
    String description; // 005FE886, original state 106.
    const U object_id = object(c, 0);
    (void)integer(c, 0);
    const U flags = integer(c, 1);
    if ((flags & 7u) != 0) {
        { String clear(""); token(0, &clear); } // 107 -> 106.
        ReturnedString temporary;
        description.Assign005e5c50(fetch(0x58f, nullptr, temporary)); // 108.
    } else if ((flags & 8u) != 0) {
        { String clear(""); token(0, &clear); } // 109 -> 106.
        ReturnedString temporary;
        description.Assign005e5c50(fetch(0x58e, nullptr, temporary)); // 110.
    } else if ((flags & 0x10u) != 0) {
        { String clear(""); token(0, &clear); } // 111 -> 106.
        ReturnedString temporary;
        description.Assign005e5c50(fetch(0x5a0, nullptr, temporary)); // 112.
    } else if ((flags & 0x20u) != 0) {
        { String clear(""); token(0, &clear); } // 113 -> 106.
        ReturnedString temporary;
        description.Assign005e5c50(fetch(0x5a1, nullptr, temporary)); // 114.
    } else if ((flags & 0x40u) != 0) {
        { String clear(""); token(0, &clear); } // 115 -> 106.
        ReturnedString temporary;
        description.Assign005e5c50(fetch(0x5a2, nullptr, temporary)); // 116.
    } else if ((flags & 0x80u) != 0) {
        { String clear(""); token(0, &clear); } // 117 -> 106.
        ReturnedString temporary;
        description.Assign005e5c50(fetch(0x5a3, nullptr, temporary)); // 118.
    } else if ((flags & 0x100u) != 0) {
        { String clear(""); token(0, &clear); } // 119 -> 106.
        ReturnedString temporary;
        description.Assign005e5c50(fetch(0x5a4, nullptr, temporary)); // 120.
    } else if ((flags & 0x200u) != 0) {
        { String clear(""); token(0, &clear); } // 121 -> 106.
        ReturnedString temporary;
        description.Assign005e5c50(fetch(0x5a5, nullptr, temporary)); // 122.
    } else if ((flags & 0x400u) != 0) {
        { String clear(""); token(0, &clear); } // 123 -> 106.
        ReturnedString temporary;
        description.Assign005e5c50(fetch(0x5a6, nullptr, temporary)); // 124.
    } else if ((flags & 0x800u) != 0) {
        { String clear(""); token(0, &clear); } // 125 -> 106.
        ReturnedString temporary;
        description.Assign005e5c50(fetch(0x5a7, nullptr, temporary)); // 126.
    } else if ((flags & 0x1000u) != 0) {
        { String clear(""); token(0, &clear); } // 127 -> 106.
        ReturnedString temporary;
        description.Assign005e5c50(fetch(0x5a8, nullptr, temporary)); // 128.
    } else if ((flags & 0x2000u) != 0) {
        { String clear(""); token(0, &clear); } // 129 -> 106.
        ReturnedString temporary;
        description.Assign005e5c50(fetch(0xa3ae, nullptr, temporary)); // 130.
    }

    if (resolve_name(c, object_id, &c.object_name) == 0) {
        ReturnedString temporary;
        c.object_name.Assign005e5c50(lookup(c, 0, temporary)); // 131.
    }
    token(0, &c.object_name);
    token(1, &description);
    {
        ReturnedString temporary;
        c.output.Assign005e5c50(fetch(0x5b2, nullptr, temporary)); // 132.
    }
    return true; // description is destroyed before 006021F8.
}

bool case160_165(Context& c) {
    if (c.selector == 160) {
        String first; // 270: [esp+30].
        String second; // 271: [esp+28].
        const U object_id = object(c, 0);
        {
            ReturnedString temporary;
            first.Assign005e5c50(message_string(c, 0, temporary)); // 272.
        }
        {
            ReturnedString temporary;
            second.Assign005e5c50(message_string(c, 1, temporary)); // 273.
        }
        const U reference = integer(c, 0);
        ReturnedString lookup_temporary;
        String* localized = lookup(c, reference, lookup_temporary); // 274.
        String formatted; // 275: [esp+20].
        const char* localized_text = cstr(localized);
        const char* second_text = cstr(&second);
        format(&formatted, "OID: %08x, Tag: %s, %s",
               object_id, second_text, localized_text);
        token(0, &formatted);
        token(1, &first);
        {
            ReturnedString temporary;
            c.output.Assign005e5c50(fetch(0x5d8, nullptr, temporary)); // 276.
        }
        return true; // formatted, lookup result, second, first, then normal suffix.
    }

    if (c.selector == 161 || c.selector == 164 ||
        c.selector == 162 || c.selector == 165) {
        String unused; // 277 / 283: [esp+38], intentionally constructed.
        String tag; // 278 / 284: [esp+30].
        const U object_id = object(c, 0);
        {
            ReturnedString temporary;
            tag.Assign005e5c50(message_string(c, 1, temporary)); // 279 / 285.
        }
        const U first = integer(c, 0);
        const U second = integer(c, 1);
        String formatted; // 280 / 286: [esp+40].
        const char* tag_text = cstr(&tag);
        format(&formatted, "(%d > %d) OID: %08x, Tag: %s",
               static_cast<I>(first), static_cast<I>(second), object_id, tag_text);
        token(0, &formatted);
        {
            ReturnedString temporary;
            if (c.selector == 164) {
                c.output.Assign005e5c50(fetch(0x5dc, nullptr, temporary)); // 281.
            } else if (c.selector == 161) {
                c.output.Assign005e5c50(fetch(0x5d9, nullptr, temporary)); // 282.
            } else if (c.selector == 165) {
                c.output.Assign005e5c50(fetch(0x5dd, nullptr, temporary)); // 287.
            } else {
                c.output.Assign005e5c50(fetch(0x5da, nullptr, temporary)); // 288.
            }
        }
        (void)cstr(&c.output);
        if (c.selector == 162 || c.selector == 165) {
            (void)cstr(&c.output);
            (void)cstr(&formatted);
            (void)cstr(&c.output);
            (void)cstr(&formatted);
        }
        return false; // formatted, tag, unused, then 00602251 (no log append).
    }

    String unused; // 289: [esp+880], intentionally constructed.
    String tag; // 290: [esp+38].
    String scripts; // 291: [esp+48].
    const U object_id = object(c, 0);
    {
        ReturnedString temporary;
        tag.Assign005e5c50(message_string(c, 1, temporary)); // 292.
    }
    {
        ReturnedString temporary;
        scripts.Assign005e5c50(message_string(c, 2, temporary)); // 293.
    }
    const U first = integer(c, 0);
    const U second = integer(c, 1);
    const U action = integer(c, 2);
    const U flags = integer(c, 3);
    String formatted; // 294: [esp+40].
    const char* tag_text = cstr(&tag);
    const char* scripts_text = cstr(&scripts);
    format(&formatted,
           "(%d > %d) Scripts: (%s), Action: %d [Flags: %02x], OID: %08x, Tag: %s",
           static_cast<I>(first), static_cast<I>(second), scripts_text,
           static_cast<I>(action), flags, object_id, tag_text);
    token(0, &formatted);
    {
        ReturnedString temporary;
        c.output.Assign005e5c50(fetch(0x5db, nullptr, temporary)); // 295.
    }
    (void)cstr(&c.output);
    return false; // formatted, scripts, tag, unused, then 00602251.
}

} // namespace feedback
