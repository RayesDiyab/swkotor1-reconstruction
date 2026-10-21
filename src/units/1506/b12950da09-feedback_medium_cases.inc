    case 31: object_feedback(c,0x592); return true; // 005FF989
    case 32: object_feedback(c,0); return true; // 005FFA35
    case 35: object_feedback(c,0); return true; // 005FFBB8
    case 36: object_feedback(c,0); return true; // 005FFC61
    case 37: object_feedback(c,0); return true; // 005FFD0A
    case 38: object_feedback(c,0x591); return true; // 005FFDB3
    case 39: object_feedback(c,0); return true; // 005FFE5F
    case 40: object_feedback(c,0); return true; // 005FFF08
    case 41: object_feedback(c,0); return true; // 005FFFB1
    case 42: object_feedback(c,0); return true; // 0060005A
    case 43: object_feedback(c,0); return true; // 00600103
    case 44: object_feedback(c,0x590); return true; // 006001AC
    case 46: object_feedback(c,0); return true; // 005FD1D7
    case 48: object_feedback(c,0); return true; // 005FD2B6
    case 125: object_feedback(c,0x5c7); return true; // 00600B44
    case 126: object_feedback(c,0x5c8); return true; // 00600BF0
    case 127: object_feedback(c,0); return true; // 00600C9C
    case 128: object_feedback(c,0x5c9); return true; // 00600D45
    case 129: object_feedback(c,0x5ca); return true; // 00600E9D
    case 130: object_feedback(c,0x5cb); return true; // 00600F49
    case 131: object_feedback(c,0x5cc); return true; // 00600FF5
    case 132: object_feedback(c,0x5cd); return true; // 006010A1
    case 133: object_feedback(c,0x5ce); return true; // 0060114D
    case 134: object_feedback(c,0x5cf); return true; // 006011F9
    case 135: object_feedback(c,0x5d0); return true; // 006012A5
    case 136: object_feedback(c,0); return true; // 00601351
    case 137: object_feedback(c,0x5d1); return true; // 006013FA
    case 138: object_feedback(c,0x5d2); return true; // 006014A6
    case 139: object_feedback(c,0); return true; // 00601551
    case 140: object_feedback(c,0); return true; // 006015F9
    case 141: object_feedback(c,0); return true; // 006016A1
    case 144: object_feedback(c,0x5d3); return true; // 00601749
    case 145: object_feedback(c,0); return true; // 006017F4
    case 146: object_feedback(c,0x5d4); return true; // 0060189C
    case 202: object_feedback(c,0); return true; // 006002FA
    case 227: object_feedback(c,0xa623); return true; // 00600DF1

    case 1: //005FCDF8
        integer_token(0,integer(c,0)); assign_fetch(c,0x59c); return true;
    case 4:case 47: //005FCF1C,005FCFFE
        name_token(c,object(c,0),0); string_token(c,1,0); assign_fetch(c,0); return true;
    case 5: { //005FD0E0 reads the integer before object-name resolution.
        const U id=object(c,0); const U value=integer(c,0);
        name_token(c,id,1); integer_token(0,value); assign_fetch(c,0); return true;
    }
    case 34: { //005FFB14: fallback does not set token0.
        const U id=object(c,0);
        if(resolve_name(c,id,&c.object_name)!=0) token(0,&c.object_name);
        else { ReturnedString temporary; c.object_name.Assign005e5c50(lookup(c,0,temporary)); }
        assign_fetch(c,0); return true;
    }
    case 50:case 51:
        string_token(c,0,0); assign_fetch(c,c.selector==50?0x5a9u:0x5aau); return true;
    case 63:case 64:case 69: { //005FEF86,005FF1C0,005FF6E3
        const U id=object(c,0); const U value=integer(c,0);
        name_token(c,id,0);
        if(c.selector==69) {
            ReturnedString temporary;
            kotor_fastcall_0041db50(kotor_cell_tlk_client_007a3a0c,nullptr,1,from_integer(static_cast<I>(value),temporary));
        } else { integer_token(1,value); }
        assign_fetch(c,c.selector==63?0x5aeu:c.selector==64?0x5afu:0u); return true;
    }
    case 66:case 67:case 70: { // two integer reads precede the name resolution.
        const U id=object(c,0); const U first=integer(c,0); const U second=integer(c,1);
        name_token(c,id,0);
        if(c.selector==70) {
            { ReturnedString temporary; kotor_fastcall_0041db50(kotor_cell_tlk_client_007a3a0c,nullptr,1,
                                                               from_integer(static_cast<I>(first),temporary)); }
            { ReturnedString temporary; kotor_fastcall_0041db50(kotor_cell_tlk_client_007a3a0c,nullptr,2,
                                                               from_integer(static_cast<I>(second),temporary)); }
        } else { integer_token(1,first); integer_token(2,second); }
        assign_fetch(c,c.selector==66?0x5b0u:c.selector==67?0x5b1u:0u); return true;
    }
    case 68: { //005FF5D8: the original dereferences a null spell if GetSpell fails.
        const U id=object(c,0); const U spell_id=integer(c,0);
        void* const spell=kotor_m15b_spell_array_get_spell_0059b6d0(read<void*>(kotor_cell_rules_007a3a28,0x8c),nullptr,static_cast<I>(spell_id));
        name_token(c,id,0);
        { ReturnedString temporary; token(1,temporary.accept(feedback_spell_name_0059b940(spell,nullptr,temporary.storage()))); }
        assign_fetch(c,0x5b2); return true;
    }
    case 71: { //005FE253: token0 receives index1 and token1 receives index0.
        const U first=integer(c,0); const U second=integer(c,1);
        integer_token(0,second); integer_token(1,first); assign_fetch(c,0); return true;
    }
    case 72: { //005FDFE9, including the two assignments in the missing-player branch.
        if(c.player!=nullptr) {
            if(resolve_name(c,read<U>(c.player,4),&c.object_name)!=0) token(0,&c.object_name);
        } else {
            {
                ReturnedString temporary;
                String* const result=c.output.Assign005e5c50(lookup(c,0,temporary));
                c.object_name.Assign005e5c50(result);
            }
            token(0,&c.object_name);
        }
        assign_fetch(c,0); return true;
    }
    case 93: assign_fetch(c,integer(c,0)); return true;
    case 108: { //006009FB signed comparisons; outer lookup survives both inner strings.
        const I value=static_cast<I>(integer(c,1));
        const U reference=value>=10?0xa3c1u:value<5?0xa3c3u:0xa3c2u;
        ReturnedString outer;
        token(0,lookup(c,reference,outer));
        String category;
        { ReturnedString temporary; category.Assign005e5c50(lookup(c,integer(c,0)==2?0xa3c4u:0xa3c5u,temporary)); }
        token(1,&category); assign_fetch(c,0x5bf); return true;
    }
    case 142: { //005FF3FA: float getter -> exact dword spill -> x87 truncation helper.
        const U id=object(c,0); const U kind=integer(c,0); const U value=integer(c,1);
        const float amount=feedback_get_float_004d64a0(c.data,nullptr,0);
        name_token(c,id,0);
        const U references[6]={0xd3,0xd4,0xd5,0xd7,0xd6,0xd8};
        const U reference=kind<=5u?references[kind]:0u;
        ReturnedString category;
        token(1,fetch(reference,nullptr,category));
        integer_token(2,value);
        integer_token(3,static_cast<U>(truncate_amount(amount)));
        assign_fetch(c,0xa5a8); return true;
    }
    case 143: integer_token(0,integer(c,0)); assign_fetch(c,0xa5c6); return true;
    case 148:case 149:
        integer_token(0,integer(c,0)); assign_fetch(c,c.selector==148?0x5d5u:0x5d6u); return true;
    case 151:case 228:
        name_token(c,object(c,0),0); integer_token(1,integer(c,0));
        assign_fetch(c,c.selector==151?0x5d7u:0x407u); return true;
    case 182:case 183: { //005FD84A/005FD8A5.
        const U value=integer(c,0);
        ReturnedString temporary;
        String* const localized=lookup(c,c.selector==182?0x600u:0x601u,temporary);
        const char* const text=cstr(localized);
        format(&c.output,"%s %i",text,static_cast<I>(value)); return true;
    }
    case 184: { //005FD900: both hidden-return strings survive formatting; lookup is destroyed first.
        ReturnedString message;
        (void)message_string(c,0,message);
        ReturnedString localized;
        String* const result=lookup(c,0x7e28,localized); // immutable original constant00752C10.
        const char* const message_text=cstr(message.storage());
        const char* const localized_text=cstr(result);
        format(&c.output,"%s %s",localized_text,message_text); return true;
    }
    case 222: { //00600553: only this spell-name case checks the spell lookup result.
        String spell_text;
        const U spell_id=integer(c,0);
        void* const spell=kotor_m15b_spell_array_get_spell_0059b6d0(read<void*>(kotor_cell_rules_007a3a28,0x8c),nullptr,static_cast<I>(spell_id));
        if(spell!=nullptr) {
            ReturnedString temporary;
            spell_text.Assign005e5c50(temporary.accept(feedback_spell_name_0059b940(spell,nullptr,temporary.storage())));
        }
        token(0,&spell_text); assign_fetch(c,0); return true;
    }
