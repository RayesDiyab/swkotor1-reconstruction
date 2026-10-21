    edx = (edx & 0xFFFFFF00u) | ld<U8>(self, 0xC6);   // 0069F650 mov dl,byte ptr [ecx+000000C6h]
    eax = animation;   // 0069F656 mov eax,dword ptr [esp+4]
    ;   // 0069F65A push ebx
    ebx = (ebx & 0xFFFFFF00u) | ld<U8>(self, 0xC5);   // 0069F65B mov bl,byte ptr [ecx+000000C5h]
    fa = edx & 0xFFu; fb = 0x1u;   // 0069F661 cmp dl,1
    edx = eax & 0xFFFFu;   // 0069F664 movzx edx,ax
    if (fa != fb) goto L_0069FCBB;   // 0069F667 jne 0069FCBB
    fa = edx; fb = 0x2727u;   // 0069F66D cmp edx,2727h
    if (static_cast<I>(fa) > static_cast<I>(fb)) goto L_0069F7BE;   // 0069F673 jg 0069F7BE
    if (fa == fb) goto L_0069F7B5;   // 0069F679 je 0069F7B5
    edx -= 0x2710u; fa = edx; fb = 0u;   // 0069F67F sub edx,2710h
    fa = edx; fb = 0x13u;   // 0069F685 cmp edx,13h
    if (fa > fb) goto L_0069FED3;   // 0069F688 ja 0069FED3
    switch (edx) {   // 0069F68E jmp dword ptr [edx*4+0069FED8h]
    case 0: goto L_0069FA4E;
    case 1: goto L_0069F9CE;
    case 2: goto L_0069F695;
    case 3: goto L_0069F695;
    case 4: goto L_0069F6F6;
    case 5: goto L_0069FED3;
    case 6: goto L_0069F776;
    case 7: goto L_0069FED3;
    case 8: goto L_0069F77F;
    case 9: goto L_0069FA2A;
    case 10: goto L_0069FED3;
    case 11: goto L_0069F7AC;
    case 12: goto L_0069F7A3;
    case 13: goto L_0069FED3;
    case 14: goto L_0069F8BC;
    case 15: goto L_0069F764;
    case 16: goto L_0069F76D;
    case 17: goto L_0069F788;
    case 18: goto L_0069F791;
    case 19: goto L_0069F79A;
    default: __assume(0);   // index bounded by the preceding cmp/ja
    }
L_0069F695:
    fa = ebx & 0xFFu; fb = 0x2u;   // 0069F695 cmp bl,2
    if (fa != fb) goto L_0069F6AD;   // 0069F698 jne 0069F6AD
    eax = (eax & 0xFFFFFF00u) | ld<U8>(self, 0xDC);   // 0069F69A mov al,byte ptr [ecx+000000DCh]
    fa = (eax & 0xFFu); fb = 0u;   // 0069F6A0 test al,al
    if (fa <= fb) goto L_0069F6F0;   // 0069F6A2 jbe 0069F6F0
    eax = 0x152u;   // 0069F6A4 mov eax,152h
    ;   // 0069F6A9 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F6AA ret 4
L_0069F6AD:
    fa = ebx & 0xFFu; fb = 0x4u;   // 0069F6AD cmp bl,4
    if (fa != fb) goto L_0069F6C5;   // 0069F6B0 jne 0069F6C5
    eax = (eax & 0xFFFFFF00u) | ld<U8>(self, 0xDC);   // 0069F6B2 mov al,byte ptr [ecx+000000DCh]
    fa = (eax & 0xFFu); fb = 0u;   // 0069F6B8 test al,al
    if (fa <= fb) goto L_0069F6F0;   // 0069F6BA jbe 0069F6F0
    eax = 0x153u;   // 0069F6BC mov eax,153h
    ;   // 0069F6C1 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F6C2 ret 4
L_0069F6C5:
    fa = ebx & 0xFFu; fb = 0x3u;   // 0069F6C5 cmp bl,3
    if (fa != fb) goto L_0069F6DD;   // 0069F6C8 jne 0069F6DD
    eax = (eax & 0xFFFFFF00u) | ld<U8>(self, 0xDC);   // 0069F6CA mov al,byte ptr [ecx+000000DCh]
    fa = (eax & 0xFFu); fb = 0u;   // 0069F6D0 test al,al
    if (fa <= fb) goto L_0069F6F0;   // 0069F6D2 jbe 0069F6F0
    eax = 0x155u;   // 0069F6D4 mov eax,155h
    ;   // 0069F6D9 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F6DA ret 4
L_0069F6DD:
    fa = ebx & 0xFFu; fb = 0x7u;   // 0069F6DD cmp bl,7
    if (fa != fb) goto L_0069F6EB;   // 0069F6E0 jne 0069F6EB
L_0069F6E2:
    eax = 0x154u;   // 0069F6E2 mov eax,154h
    ;   // 0069F6E7 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F6E8 ret 4
L_0069F6EB:
    fa = ebx & 0xFFu; fb = 0x9u;   // 0069F6EB cmp bl,9
    if (fa == fb) goto L_0069F6E2;   // 0069F6EE je 0069F6E2
L_0069F6F0:
    eax = 0u;   // 0069F6F0 xor eax,eax
    ;   // 0069F6F2 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F6F3 ret 4
L_0069F6F6:
    eax = ld<U>(self, 0xCC);   // 0069F6F6 mov eax,dword ptr [ecx+000000CCh]
    fa = eax; fb = 0u;   // 0069F6FC test eax,eax
    if (fa != fb) goto L_0069F6F0;   // 0069F6FE jne 0069F6F0
    fa = ebx & 0xFFu; fb = 0x2u;   // 0069F700 cmp bl,2
    if (fa != fb) goto L_0069F718;   // 0069F703 jne 0069F718
    eax = (eax & 0xFFFFFF00u) | ld<U8>(self, 0xDC);   // 0069F705 mov al,byte ptr [ecx+000000DCh]
    fa = (eax & 0xFFu); fb = 0u;   // 0069F70B test al,al
    if (fa <= fb) goto L_0069F75B;   // 0069F70D jbe 0069F75B
    eax = 0x159u;   // 0069F70F mov eax,159h
    ;   // 0069F714 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F715 ret 4
L_0069F718:
    fa = ebx & 0xFFu; fb = 0x4u;   // 0069F718 cmp bl,4
    if (fa != fb) goto L_0069F730;   // 0069F71B jne 0069F730
    eax = (eax & 0xFFFFFF00u) | ld<U8>(self, 0xDC);   // 0069F71D mov al,byte ptr [ecx+000000DCh]
    fa = (eax & 0xFFu); fb = 0u;   // 0069F723 test al,al
    if (fa <= fb) goto L_0069F75B;   // 0069F725 jbe 0069F75B
    eax = 0x3u;   // 0069F727 mov eax,3
    ;   // 0069F72C pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F72D ret 4
L_0069F730:
    fa = ebx & 0xFFu; fb = 0x3u;   // 0069F730 cmp bl,3
    if (fa != fb) goto L_0069F748;   // 0069F733 jne 0069F748
    eax = (eax & 0xFFFFFF00u) | ld<U8>(self, 0xDC);   // 0069F735 mov al,byte ptr [ecx+000000DCh]
    fa = (eax & 0xFFu); fb = 0u;   // 0069F73B test al,al
    if (fa <= fb) goto L_0069F75B;   // 0069F73D jbe 0069F75B
    eax = 0x157u;   // 0069F73F mov eax,157h
    ;   // 0069F744 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F745 ret 4
L_0069F748:
    fa = ebx & 0xFFu; fb = 0x7u;   // 0069F748 cmp bl,7
    if (fa != fb) goto L_0069F756;   // 0069F74B jne 0069F756
L_0069F74D:
    eax = 0x156u;   // 0069F74D mov eax,156h
    ;   // 0069F752 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F753 ret 4
L_0069F756:
    fa = ebx & 0xFFu; fb = 0x9u;   // 0069F756 cmp bl,9
    if (fa == fb) goto L_0069F74D;   // 0069F759 je 0069F74D
L_0069F75B:
    eax = 0x2u;   // 0069F75B mov eax,2
    ;   // 0069F760 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F761 ret 4
L_0069F764:
    eax = 0x3Eu;   // 0069F764 mov eax,3Eh
    ;   // 0069F769 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F76A ret 4
L_0069F76D:
    eax = 0x40u;   // 0069F76D mov eax,40h
    ;   // 0069F772 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F773 ret 4
L_0069F776:
    eax = 0x51u;   // 0069F776 mov eax,51h
    ;   // 0069F77B pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F77C ret 4
L_0069F77F:
    eax = 0x53u;   // 0069F77F mov eax,53h
    ;   // 0069F784 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F785 ret 4
L_0069F788:
    eax = 0x3Fu;   // 0069F788 mov eax,3Fh
    ;   // 0069F78D pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F78E ret 4
L_0069F791:
    eax = 0x41u;   // 0069F791 mov eax,41h
    ;   // 0069F796 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F797 ret 4
L_0069F79A:
    eax = 0x43u;   // 0069F79A mov eax,43h
    ;   // 0069F79F pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F7A0 ret 4
L_0069F7A3:
    eax = 0x12Du;   // 0069F7A3 mov eax,12Dh
    ;   // 0069F7A8 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F7A9 ret 4
L_0069F7AC:
    eax = 0x12Eu;   // 0069F7AC mov eax,12Eh
    ;   // 0069F7B1 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F7B2 ret 4
L_0069F7B5:
    eax = 0x4Du;   // 0069F7B5 mov eax,4Dh
    ;   // 0069F7BA pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F7BB ret 4
L_0069F7BE:
    fa = edx; fb = 0x275Du;   // 0069F7BE cmp edx,275Dh
    if (static_cast<I>(fa) > static_cast<I>(fb)) goto L_0069F8C5;   // 0069F7C4 jg 0069F8C5
    if (fa == fb) goto L_0069F8BC;   // 0069F7CA je 0069F8BC
    edx -= 0x272Cu; fa = edx; fb = 0u;   // 0069F7D0 sub edx,272Ch
    fa = edx; fb = 0x2Au;   // 0069F7D6 cmp edx,2Ah
    if (fa > fb) goto L_0069FED3;   // 0069F7D9 ja 0069FED3
    ecx = kIndex_0069FF88[edx];   // 0069F7DF movzx ecx,byte ptr [edx+0069FF88h]
    switch (ecx) {   // 0069F7E6 jmp dword ptr [ecx*4+0069FF28h]
    case 0: goto L_0069F82C;
    case 1: goto L_0069F8A1;
    case 2: goto L_0069F8AA;
    case 3: goto L_0069F83E;
    case 4: goto L_0069F8B3;
    case 5: goto L_0069F847;
    case 6: goto L_0069F88F;
    case 7: goto L_0069F86B;
    case 8: goto L_0069F874;
    case 9: goto L_0069F87D;
    case 10: goto L_0069F886;
    case 11: goto L_0069F835;
    case 12: goto L_0069F7ED;
    case 13: goto L_0069F823;
    case 14: goto L_0069F81A;
    case 15: goto L_0069F7F6;
    case 16: goto L_0069F7FF;
    case 17: goto L_0069F808;
    case 18: goto L_0069F811;
    case 19: goto L_0069F850;
    case 20: goto L_0069F859;
    case 21: goto L_0069F862;
    case 22: goto L_0069F898;
    case 23: goto L_0069FED3;
    default: __assume(0);   // index bounded by the preceding cmp/ja
    }
L_0069F7ED:
    eax = 0x7u;   // 0069F7ED mov eax,7
    ;   // 0069F7F2 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F7F3 ret 4
L_0069F7F6:
    eax = 0xCu;   // 0069F7F6 mov eax,0Ch
    ;   // 0069F7FB pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F7FC ret 4
L_0069F7FF:
    eax = 0xDu;   // 0069F7FF mov eax,0Dh
    ;   // 0069F804 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F805 ret 4
L_0069F808:
    eax = 0xEu;   // 0069F808 mov eax,0Eh
    ;   // 0069F80D pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F80E ret 4
L_0069F811:
    eax = 0xFu;   // 0069F811 mov eax,0Fh
    ;   // 0069F816 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F817 ret 4
L_0069F81A:
    eax = 0xAu;   // 0069F81A mov eax,0Ah
    ;   // 0069F81F pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F820 ret 4
L_0069F823:
    eax = 0xBu;   // 0069F823 mov eax,0Bh
    ;   // 0069F828 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F829 ret 4
L_0069F82C:
    eax = 0x21u;   // 0069F82C mov eax,21h
    ;   // 0069F831 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F832 ret 4
L_0069F835:
    eax = 0x11u;   // 0069F835 mov eax,11h
    ;   // 0069F83A pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F83B ret 4
L_0069F83E:
    eax = 0x18u;   // 0069F83E mov eax,18h
    ;   // 0069F843 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F844 ret 4
L_0069F847:
    eax = 0x13u;   // 0069F847 mov eax,13h
    ;   // 0069F84C pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F84D ret 4
L_0069F850:
    eax = 0x28u;   // 0069F850 mov eax,28h
    ;   // 0069F855 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F856 ret 4
L_0069F859:
    eax = 0x29u;   // 0069F859 mov eax,29h
    ;   // 0069F85E pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F85F ret 4
L_0069F862:
    eax = 0x46u;   // 0069F862 mov eax,46h
    ;   // 0069F867 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F868 ret 4
L_0069F86B:
    eax = 0x1Bu;   // 0069F86B mov eax,1Bh
    ;   // 0069F870 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F871 ret 4
L_0069F874:
    eax = 0x1Au;   // 0069F874 mov eax,1Ah
    ;   // 0069F879 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F87A ret 4
L_0069F87D:
    eax = 0x1Du;   // 0069F87D mov eax,1Dh
    ;   // 0069F882 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F883 ret 4
L_0069F886:
    eax = 0x1Cu;   // 0069F886 mov eax,1Ch
    ;   // 0069F88B pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F88C ret 4
L_0069F88F:
    eax = 0x19u;   // 0069F88F mov eax,19h
    ;   // 0069F894 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F895 ret 4
L_0069F898:
    eax = 0x25u;   // 0069F898 mov eax,25h
    ;   // 0069F89D pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F89E ret 4
L_0069F8A1:
    eax = 0x1Fu;   // 0069F8A1 mov eax,1Fh
    ;   // 0069F8A6 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F8A7 ret 4
L_0069F8AA:
    eax = 0x12u;   // 0069F8AA mov eax,12h
    ;   // 0069F8AF pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F8B0 ret 4
L_0069F8B3:
    eax = 0x10u;   // 0069F8B3 mov eax,10h
    ;   // 0069F8B8 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F8B9 ret 4
L_0069F8BC:
    eax = 0x12Fu;   // 0069F8BC mov eax,12Fh
    ;   // 0069F8C1 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F8C2 ret 4
L_0069F8C5:
    fa = edx; fb = 0x27A5u;   // 0069F8C5 cmp edx,27A5h
    if (static_cast<I>(fa) > static_cast<I>(fb)) goto L_0069FA57;   // 0069F8CB jg 0069FA57
    if (fa == fb) goto L_0069FA4E;   // 0069F8D1 je 0069FA4E
    edx -= 0x276Cu; fa = edx; fb = 0u;   // 0069F8D7 sub edx,276Ch
    fa = edx; fb = 0x38u;   // 0069F8DD cmp edx,38h
    if (fa > fb) goto L_0069FED3;   // 0069F8E0 ja 0069FED3
    edx = kIndex_006A0028[edx];   // 0069F8E6 movzx edx,byte ptr [edx+006A0028h]
    switch (edx) {   // 0069F8ED jmp dword ptr [edx*4+0069FFB4h]
    case 0: goto L_0069F91A;
    case 1: goto L_0069F8F4;
    case 2: goto L_0069F8FD;
    case 3: goto L_0069FA2A;
    case 4: goto L_0069F935;
    case 5: goto L_0069F97D;
    case 6: goto L_0069F93E;
    case 7: goto L_0069F947;
    case 8: goto L_0069F950;
    case 9: goto L_0069F959;
    case 10: goto L_0069F986;
    case 11: goto L_0069F962;
    case 12: goto L_0069F96B;
    case 13: goto L_0069F974;
    case 14: goto L_0069F98F;
    case 15: goto L_0069F923;
    case 16: goto L_0069F92C;
    case 17: goto L_0069F998;
    case 18: goto L_0069F9A1;
    case 19: goto L_0069F911;
    case 20: goto L_0069F9B3;
    case 21: goto L_0069F9BC;
    case 22: goto L_0069F9C5;
    case 23: goto L_0069F9AA;
    case 24: goto L_0069FA33;
    case 25: goto L_0069FA3C;
    case 26: goto L_0069FA45;
    case 27: goto L_0069F9CE;
    case 28: goto L_0069FED3;
    default: __assume(0);   // index bounded by the preceding cmp/ja
    }
L_0069F8F4:
    eax = 0x1u;   // 0069F8F4 mov eax,1
    ;   // 0069F8F9 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F8FA ret 4
L_0069F8FD:
    eax = ld<U>(self, 0xCC);   // 0069F8FD mov eax,dword ptr [ecx+000000CCh]
    cf = eax != 0u; eax = 0u - eax;   // 0069F903 neg eax
    eax = cf ? 0xFFFFFFFFu : 0u;   // 0069F905 sbb eax,eax
    eax &= 0xFFFFFFFDu;   // 0069F907 and eax,0FFFFFFFDh
    eax += 0x4u; fa = eax; fb = 0u;   // 0069F90A add eax,4
    ;   // 0069F90D pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F90E ret 4
L_0069F911:
    eax = 0x5u;   // 0069F911 mov eax,5
    ;   // 0069F916 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F917 ret 4
L_0069F91A:
    eax = 0x8u;   // 0069F91A mov eax,8
    ;   // 0069F91F pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F920 ret 4
L_0069F923:
    eax = 0x39u;   // 0069F923 mov eax,39h
    ;   // 0069F928 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F929 ret 4
L_0069F92C:
    eax = 0x3Au;   // 0069F92C mov eax,3Ah
    ;   // 0069F931 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F932 ret 4
L_0069F935:
    eax = 0x4Bu;   // 0069F935 mov eax,4Bh
    ;   // 0069F93A pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F93B ret 4
L_0069F93E:
    eax = 0x20u;   // 0069F93E mov eax,20h
    ;   // 0069F943 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F944 ret 4
L_0069F947:
    eax = 0x2Cu;   // 0069F947 mov eax,2Ch
    ;   // 0069F94C pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F94D ret 4
L_0069F950:
    eax = 0x35u;   // 0069F950 mov eax,35h
    ;   // 0069F955 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F956 ret 4
L_0069F959:
    eax = 0x36u;   // 0069F959 mov eax,36h
    ;   // 0069F95E pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F95F ret 4
L_0069F962:
    eax = 0x2Bu;   // 0069F962 mov eax,2Bh
    ;   // 0069F967 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F968 ret 4
L_0069F96B:
    eax = 0x44u;   // 0069F96B mov eax,44h
    ;   // 0069F970 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F971 ret 4
L_0069F974:
    eax = 0x26u;   // 0069F974 mov eax,26h
    ;   // 0069F979 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F97A ret 4
L_0069F97D:
    eax = 0x10Eu;   // 0069F97D mov eax,10Eh
    ;   // 0069F982 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F983 ret 4
L_0069F986:
    eax = 0x4Au;   // 0069F986 mov eax,4Ah
    ;   // 0069F98B pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F98C ret 4
L_0069F98F:
    eax = 0x2Fu;   // 0069F98F mov eax,2Fh
    ;   // 0069F994 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F995 ret 4
L_0069F998:
    eax = 0x30u;   // 0069F998 mov eax,30h
    ;   // 0069F99D pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F99E ret 4
L_0069F9A1:
    eax = 0x33u;   // 0069F9A1 mov eax,33h
    ;   // 0069F9A6 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F9A7 ret 4
L_0069F9AA:
    eax = 0x34u;   // 0069F9AA mov eax,34h
    ;   // 0069F9AF pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F9B0 ret 4
L_0069F9B3:
    eax = 0x4Cu;   // 0069F9B3 mov eax,4Ch
    ;   // 0069F9B8 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F9B9 ret 4
L_0069F9BC:
    eax = 0x4Eu;   // 0069F9BC mov eax,4Eh
    ;   // 0069F9C1 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F9C2 ret 4
L_0069F9C5:
    eax = 0x4Fu;   // 0069F9C5 mov eax,4Fh
    ;   // 0069F9CA pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F9CB ret 4
L_0069F9CE:
    ecx = ebx & 0xFFu;   // 0069F9CE movzx ecx,bl
    ecx -= 1u; fa = ecx; fb = 0u;   // 0069F9D1 dec ecx
    fa = ecx; fb = 0x8u;   // 0069F9D2 cmp ecx,8
    if (fa > fb) goto L_0069FED3;   // 0069F9D5 ja 0069FED3
    switch (ecx) {   // 0069F9DB jmp dword ptr [ecx*4+006A0064h]
    case 0: goto L_0069F9E2;
    case 1: goto L_0069F9EB;
    case 2: goto L_0069F9F4;
    case 3: goto L_0069F9FD;
    case 4: goto L_0069FA06;
    case 5: goto L_0069FA0F;
    case 6: goto L_0069FA21;
    case 7: goto L_0069FA18;
    case 8: goto L_0069FA21;
    default: __assume(0);   // index bounded by the preceding cmp/ja
    }
L_0069F9E2:
    eax = 0x5Cu;   // 0069F9E2 mov eax,5Ch
    ;   // 0069F9E7 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F9E8 ret 4
L_0069F9EB:
    eax = 0x85u;   // 0069F9EB mov eax,85h
    ;   // 0069F9F0 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F9F1 ret 4
L_0069F9F4:
    eax = 0xAEu;   // 0069F9F4 mov eax,0AEh
    ;   // 0069F9F9 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069F9FA ret 4
L_0069F9FD:
    eax = 0xD7u;   // 0069F9FD mov eax,0D7h
    ;   // 0069FA02 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FA03 ret 4
L_0069FA06:
    eax = 0xDFu;   // 0069FA06 mov eax,0DFh
    ;   // 0069FA0B pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FA0C ret 4
L_0069FA0F:
    eax = 0xEDu;   // 0069FA0F mov eax,0EDh
    ;   // 0069FA14 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FA15 ret 4
L_0069FA18:
    eax = 0xF9u;   // 0069FA18 mov eax,0F9h
    ;   // 0069FA1D pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FA1E ret 4
L_0069FA21:
    eax = 0xF5u;   // 0069FA21 mov eax,0F5h
    ;   // 0069FA26 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FA27 ret 4
L_0069FA2A:
    eax = 0x12Cu;   // 0069FA2A mov eax,12Ch
    ;   // 0069FA2F pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FA30 ret 4
L_0069FA33:
    eax = 0x15Au;   // 0069FA33 mov eax,15Ah
    ;   // 0069FA38 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FA39 ret 4
L_0069FA3C:
    eax = 0x163u;   // 0069FA3C mov eax,163h
    ;   // 0069FA41 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FA42 ret 4
L_0069FA45:
    eax = 0x165u;   // 0069FA45 mov eax,165h
    ;   // 0069FA4A pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FA4B ret 4
L_0069FA4E:
    eax = 0x6u;   // 0069FA4E mov eax,6
    ;   // 0069FA53 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FA54 ret 4
L_0069FA57:
    fa = edx; fb = 0x27ECu;   // 0069FA57 cmp edx,27ECh
    if (static_cast<I>(fa) > static_cast<I>(fb)) goto L_0069FB4A;   // 0069FA5D jg 0069FB4A
    fa = edx; fb = 0x27EBu;   // 0069FA63 cmp edx,27EBh
    if (static_cast<I>(fa) >= static_cast<I>(fb)) goto L_0069FB41;   // 0069FA69 jge 0069FB41
    edx -= 0x27A6u; fa = edx; fb = 0u;   // 0069FA6F sub edx,27A6h
    fa = edx; fb = 0xEu;   // 0069FA75 cmp edx,0Eh
    if (fa > fb) goto L_0069FED3;   // 0069FA78 ja 0069FED3
    switch (edx) {   // 0069FA7E jmp dword ptr [edx*4+006A0088h]
    case 0: goto L_0069FA85;
    case 1: goto L_0069FA97;
    case 2: goto L_0069FAA0;
    case 3: goto L_0069FED3;
    case 4: goto L_0069FAB2;
    case 5: goto L_0069FABB;
    case 6: goto L_0069FAA9;
    case 7: goto L_0069FAC4;
    case 8: goto L_0069FAF1;
    case 9: goto L_0069FB14;
    case 10: goto L_0069FB1D;
    case 11: goto L_0069FB26;
    case 12: goto L_0069FA8E;
    case 13: goto L_0069FB2F;
    case 14: goto L_0069FB38;
    default: __assume(0);   // index bounded by the preceding cmp/ja
    }
L_0069FA85:
    eax = 0x48u;   // 0069FA85 mov eax,48h
    ;   // 0069FA8A pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FA8B ret 4
L_0069FA8E:
    eax = 0x45u;   // 0069FA8E mov eax,45h
    ;   // 0069FA93 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FA94 ret 4
L_0069FA97:
    eax = 0x167u;   // 0069FA97 mov eax,167h
    ;   // 0069FA9C pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FA9D ret 4
L_0069FAA0:
    eax = 0x168u;   // 0069FAA0 mov eax,168h
    ;   // 0069FAA5 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FAA6 ret 4
L_0069FAA9:
    eax = 0x177u;   // 0069FAA9 mov eax,177h
    ;   // 0069FAAE pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FAAF ret 4
L_0069FAB2:
    eax = 0x172u;   // 0069FAB2 mov eax,172h
    ;   // 0069FAB7 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FAB8 ret 4
L_0069FABB:
    eax = 0x173u;   // 0069FABB mov eax,173h
    ;   // 0069FAC0 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FAC1 ret 4
L_0069FAC4:
    ecx = ebx & 0xFFu;   // 0069FAC4 movzx ecx,bl
    ecx -= 0x2u; fa = ecx; fb = 0u;   // 0069FAC7 sub ecx,2
    if (fa == fb) goto L_0069FAE8;   // 0069FACA je 0069FAE8
    ecx -= 1u; fa = ecx; fb = 0u;   // 0069FACC dec ecx
    if (fa == fb) goto L_0069FADF;   // 0069FACD je 0069FADF
    ecx -= 1u; fa = ecx; fb = 0u;   // 0069FACF dec ecx
    if (fa != fb) goto L_0069FED3;   // 0069FAD0 jne 0069FED3
    eax = 0x178u;   // 0069FAD6 mov eax,178h
    ;   // 0069FADB pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FADC ret 4
L_0069FADF:
    eax = 0x17Au;   // 0069FADF mov eax,17Ah
    ;   // 0069FAE4 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FAE5 ret 4
L_0069FAE8:
    eax = 0x179u;   // 0069FAE8 mov eax,179h
    ;   // 0069FAED pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FAEE ret 4
L_0069FAF1:
    ecx = ebx & 0xFFu;   // 0069FAF1 movzx ecx,bl
    ecx -= 0x2u; fa = ecx; fb = 0u;   // 0069FAF4 sub ecx,2
    if (fa == fb) goto L_0069FBB1;   // 0069FAF7 je 0069FBB1
    ecx -= 1u; fa = ecx; fb = 0u;   // 0069FAFD dec ecx
    if (fa == fb) goto L_0069FE60;   // 0069FAFE je 0069FE60
    ecx -= 1u; fa = ecx; fb = 0u;   // 0069FB04 dec ecx
    if (fa != fb) goto L_0069FED3;   // 0069FB05 jne 0069FED3
L_0069FB0B:
    eax = 0xD6u;   // 0069FB0B mov eax,0D6h
    ;   // 0069FB10 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FB11 ret 4
L_0069FB14:
    eax = 0x22u;   // 0069FB14 mov eax,22h
    ;   // 0069FB19 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FB1A ret 4
L_0069FB1D:
    eax = 0x23u;   // 0069FB1D mov eax,23h
    ;   // 0069FB22 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FB23 ret 4
L_0069FB26:
    eax = 0x47u;   // 0069FB26 mov eax,47h
    ;   // 0069FB2B pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FB2C ret 4
L_0069FB2F:
    eax = 0x180u;   // 0069FB2F mov eax,180h
    ;   // 0069FB34 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FB35 ret 4
L_0069FB38:
    eax = 0x181u;   // 0069FB38 mov eax,181h
    ;   // 0069FB3D pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FB3E ret 4
L_0069FB41:
    eax = 0x55u;   // 0069FB41 mov eax,55h
    ;   // 0069FB46 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FB47 ret 4
L_0069FB4A:
    fa = edx; fb = 0x283Du;   // 0069FB4A cmp edx,283Dh
    if (static_cast<I>(fa) > static_cast<I>(fb)) goto L_0069FC13;   // 0069FB50 jg 0069FC13
    if (fa == fb) goto L_0069FC02;   // 0069FB56 je 0069FC02
    edx -= 0x27EDu; fa = edx; fb = 0u;   // 0069FB5C sub edx,27EDh
    fa = edx; fb = 0x4Fu;   // 0069FB62 cmp edx,4Fh
    if (fa > fb) goto L_0069FED3;   // 0069FB65 ja 0069FED3
    ecx = kIndex_006A00E8[edx];   // 0069FB6B movzx ecx,byte ptr [edx+006A00E8h]
    switch (ecx) {   // 0069FB72 jmp dword ptr [ecx*4+006A00C4h]
    case 0: goto L_0069FB79;
    case 1: goto L_0069FB82;
    case 2: goto L_0069FB8B;
    case 3: goto L_0069FB94;
    case 4: goto L_0069FBF0;
    case 5: goto L_0069FB9D;
    case 6: goto L_0069FBF9;
    case 7: goto L_0069FBC3;
    case 8: goto L_0069FED3;
    default: __assume(0);   // index bounded by the preceding cmp/ja
    }
L_0069FB79:
    eax = 0x50u;   // 0069FB79 mov eax,50h
    ;   // 0069FB7E pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FB7F ret 4
L_0069FB82:
    eax = 0x52u;   // 0069FB82 mov eax,52h
    ;   // 0069FB87 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FB88 ret 4
L_0069FB8B:
    eax = 0x17Du;   // 0069FB8B mov eax,17Dh
    ;   // 0069FB90 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FB91 ret 4
L_0069FB94:
    eax = 0x17Eu;   // 0069FB94 mov eax,17Eh
    ;   // 0069FB99 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FB9A ret 4
L_0069FB9D:
    ecx = ebx & 0xFFu;   // 0069FB9D movzx ecx,bl
    ecx -= 1u; fa = ecx; fb = 0u;   // 0069FBA0 dec ecx
    fa = ecx; fb = 0x8u;   // 0069FBA1 cmp ecx,8
    if (fa > fb) goto L_0069FED3;   // 0069FBA4 ja 0069FED3
    switch (ecx) {   // 0069FBAA jmp dword ptr [ecx*4+006A0138h]
    case 0: goto L_0069FE57;
    case 1: goto L_0069FBB1;
    case 2: goto L_0069FE60;
    case 3: goto L_0069FB0B;
    case 4: goto L_0069FE69;
    case 5: goto L_0069FE72;
    case 6: goto L_0069FE7B;
    case 7: goto L_0069FBBA;
    case 8: goto L_0069FE7B;
    default: __assume(0);   // index bounded by the preceding cmp/ja
    }
L_0069FBB1:
    eax = 0x84u;   // 0069FBB1 mov eax,84h
    ;   // 0069FBB6 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FBB7 ret 4
L_0069FBBA:
    eax = 0x175u;   // 0069FBBA mov eax,175h
    ;   // 0069FBBF pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FBC0 ret 4
L_0069FBC3:
    ecx = ebx & 0xFFu;   // 0069FBC3 movzx ecx,bl
    ecx -= 0x2u; fa = ecx; fb = 0u;   // 0069FBC6 sub ecx,2
    if (fa == fb) goto L_0069FBE7;   // 0069FBC9 je 0069FBE7
L_0069FBCB:
    ecx -= 1u; fa = ecx; fb = 0u;   // 0069FBCB dec ecx
    if (fa == fb) goto L_0069FBDE;   // 0069FBCC je 0069FBDE
    ecx -= 1u; fa = ecx; fb = 0u;   // 0069FBCE dec ecx
    if (fa != fb) goto L_0069FED3;   // 0069FBCF jne 0069FED3
    eax = 0xC0u;   // 0069FBD5 mov eax,0C0h
    ;   // 0069FBDA pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FBDB ret 4
L_0069FBDE:
    eax = 0x97u;   // 0069FBDE mov eax,97h
    ;   // 0069FBE3 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FBE4 ret 4
L_0069FBE7:
    eax = 0x6Du;   // 0069FBE7 mov eax,6Dh
    ;   // 0069FBEC pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FBED ret 4
L_0069FBF0:
    eax = 0x17u;   // 0069FBF0 mov eax,17h
    ;   // 0069FBF5 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FBF6 ret 4
L_0069FBF9:
    eax = 0x17Fu;   // 0069FBF9 mov eax,17Fh
    ;   // 0069FBFE pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FBFF ret 4
L_0069FC02:
    ecx = ebx & 0xFFu;   // 0069FC02 movzx ecx,bl
    ecx -= 0x2u; fa = ecx; fb = 0u;   // 0069FC05 sub ecx,2
    if (fa != fb) goto L_0069FBCB;   // 0069FC08 jne 0069FBCB
    eax = 0x6Eu;   // 0069FC0A mov eax,6Eh
    ;   // 0069FC0F pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FC10 ret 4
L_0069FC13:
    fa = edx; fb = 0x28A2u;   // 0069FC13 cmp edx,28A2h
    if (static_cast<I>(fa) > static_cast<I>(fb)) goto L_0069FC98;   // 0069FC19 jg 0069FC98
    if (fa == fb) goto L_0069FC3A;   // 0069FC1B je 0069FC3A
    edx -= 0x283Eu; fa = edx; fb = 0u;   // 0069FC1D sub edx,283Eh
    if (fa == fb) goto L_0069FC43;   // 0069FC23 je 0069FC43
    edx -= 0x62u; fa = edx; fb = 0u;   // 0069FC25 sub edx,62h
    if (fa == fb) goto L_0069FC3A;   // 0069FC28 je 0069FC3A
    edx -= 1u; fa = edx; fb = 0u;   // 0069FC2A dec edx
    if (fa != fb) goto L_0069FED3;   // 0069FC2B jne 0069FED3
L_0069FC31:
    eax = 0x73u;   // 0069FC31 mov eax,73h
    ;   // 0069FC36 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FC37 ret 4
L_0069FC3A:
    eax = 0x54u;   // 0069FC3A mov eax,54h
    ;   // 0069FC3F pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FC40 ret 4
L_0069FC43:
    ecx = ebx & 0xFFu;   // 0069FC43 movzx ecx,bl
    ecx += 0xFFFFFFFEu; fa = ecx; fb = 0u;   // 0069FC46 add ecx,0FFFFFFFEh
    fa = ecx; fb = 0x7u;   // 0069FC49 cmp ecx,7
    if (fa > fb) goto L_0069FED3;   // 0069FC4C ja 0069FED3
    switch (ecx) {   // 0069FC52 jmp dword ptr [ecx*4+006A015Ch]
    case 0: goto L_0069FC59;
    case 1: goto L_0069FC6B;
    case 2: goto L_0069FC62;
    case 3: goto L_0069FC74;
    case 4: goto L_0069FC7D;
    case 5: goto L_0069FC8F;
    case 6: goto L_0069FC86;
    case 7: goto L_0069FC8F;
    default: __assume(0);   // index bounded by the preceding cmp/ja
    }
L_0069FC59:
    eax = 0x7Cu;   // 0069FC59 mov eax,7Ch
    ;   // 0069FC5E pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FC5F ret 4
L_0069FC62:
    eax = 0xA5u;   // 0069FC62 mov eax,0A5h
    ;   // 0069FC67 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FC68 ret 4
L_0069FC6B:
    eax = 0xCEu;   // 0069FC6B mov eax,0CEh
    ;   // 0069FC70 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FC71 ret 4
L_0069FC74:
    eax = 0xDCu;   // 0069FC74 mov eax,0DCh
    ;   // 0069FC79 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FC7A ret 4
L_0069FC7D:
    eax = 0xEAu;   // 0069FC7D mov eax,0EAh
    ;   // 0069FC82 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FC83 ret 4
L_0069FC86:
    eax = 0x118u;   // 0069FC86 mov eax,118h
    ;   // 0069FC8B pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FC8C ret 4
L_0069FC8F:
    eax = 0xF2u;   // 0069FC8F mov eax,0F2h
    ;   // 0069FC94 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FC95 ret 4
L_0069FC98:
    edx -= 0x2AF8u; fa = edx; fb = 0u;   // 0069FC98 sub edx,2AF8h
    if (fa == fb) goto L_0069FCB2;   // 0069FC9E je 0069FCB2
    edx -= 1u; fa = edx; fb = 0u;   // 0069FCA0 dec edx
    if (fa == fb) goto L_0069FCA9;   // 0069FCA1 je 0069FCA9
    edx -= 1u; fa = edx; fb = 0u;   // 0069FCA3 dec edx
    goto L_0069FECC;   // 0069FCA4 jmp 0069FECC
L_0069FCA9:
    eax = 0x188u;   // 0069FCA9 mov eax,188h
    ;   // 0069FCAE pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FCAF ret 4
L_0069FCB2:
    eax = 0x42u;   // 0069FCB2 mov eax,42h
    ;   // 0069FCB7 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FCB8 ret 4
L_0069FCBB:
    fa = edx; fb = 0x273Cu;   // 0069FCBB cmp edx,273Ch
    if (static_cast<I>(fa) > static_cast<I>(fb)) goto L_0069FD0F;   // 0069FCC1 jg 0069FD0F
    if (fa == fb) goto L_0069FD06;   // 0069FCC3 je 0069FD06
    edx -= 0x2710u; fa = edx; fb = 0u;   // 0069FCC5 sub edx,2710h
    fa = edx; fb = 0x2Au;   // 0069FCCB cmp edx,2Ah
    if (fa > fb) goto L_0069FED3;   // 0069FCCE ja 0069FED3
    edx = kIndex_006A01B8[edx];   // 0069FCD4 movzx edx,byte ptr [edx+006A01B8h]
    switch (edx) {   // 0069FCDB jmp dword ptr [edx*4+006A017Ch]
    case 0: goto L_0069FDD6;
    case 1: goto L_0069FDE8;
    case 2: goto L_0069FD83;
    case 3: goto L_0069FD3E;
    case 4: goto L_0069FDF1;
    case 5: goto L_0069FA2A;
    case 6: goto L_0069F7AC;
    case 7: goto L_0069F7A3;
    case 8: goto L_0069FC86;
    case 9: goto L_0069FCFD;
    case 10: goto L_0069FCEB;
    case 11: goto L_0069FCE2;
    case 12: goto L_0069F8AA;
    case 13: goto L_0069FCF4;
    case 14: goto L_0069FED3;
    default: __assume(0);   // index bounded by the preceding cmp/ja
    }
L_0069FCE2:
    eax = 0x107u;   // 0069FCE2 mov eax,107h
    ;   // 0069FCE7 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FCE8 ret 4
L_0069FCEB:
    eax = 0x10Cu;   // 0069FCEB mov eax,10Ch
    ;   // 0069FCF0 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FCF1 ret 4
L_0069FCF4:
    eax = 0x105u;   // 0069FCF4 mov eax,105h
    ;   // 0069FCF9 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FCFA ret 4
L_0069FCFD:
    eax = 0x17Bu;   // 0069FCFD mov eax,17Bh
    ;   // 0069FD02 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FD03 ret 4
L_0069FD06:
    eax = 0x104u;   // 0069FD06 mov eax,104h
    ;   // 0069FD0B pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FD0C ret 4
L_0069FD0F:
    fa = edx; fb = 0x27A3u;   // 0069FD0F cmp edx,27A3h
    if (static_cast<I>(fa) > static_cast<I>(fb)) goto L_0069FDB0;   // 0069FD15 jg 0069FDB0
    if (fa == fb) goto L_0069FDA7;   // 0069FD1B je 0069FDA7
    edx -= 0x2744u; fa = edx; fb = 0u;   // 0069FD21 sub edx,2744h
    fa = edx; fb = 0x5Au;   // 0069FD27 cmp edx,5Ah
    if (fa > fb) goto L_0069FED3;   // 0069FD2A ja 0069FED3
    edx = kIndex_006A022C[edx];   // 0069FD30 movzx edx,byte ptr [edx+006A022Ch]
    switch (edx) {   // 0069FD37 jmp dword ptr [edx*4+006A01E4h]
    case 0: goto L_0069FD5F;
    case 1: goto L_0069FD71;
    case 2: goto L_0069FD68;
    case 3: goto L_0069FDD6;
    case 4: goto L_0069FD9E;
    case 5: goto L_0069FC86;
    case 6: goto L_0069FD83;
    case 7: goto L_0069FD56;
    case 8: goto L_0069FD3E;
    case 9: goto L_0069FA2A;
    case 10: goto L_0069FD7A;
    case 11: goto L_0069F97D;
    case 12: goto L_0069F986;
    case 13: goto L_0069F962;
    case 14: goto L_0069FD8C;
    case 15: goto L_0069FD95;
    case 16: goto L_0069FA33;
    case 17: goto L_0069FED3;
    default: __assume(0);   // index bounded by the preceding cmp/ja
    }
L_0069FD3E:
    edx = ld<U>(self, 0xCC);   // 0069FD3E mov edx,dword ptr [ecx+000000CCh]
    eax = 0u;   // 0069FD44 xor eax,eax
    fa = edx; fb = 0u;   // 0069FD46 test edx,edx
    eax = (eax & 0xFFFFFF00u) | ((fa == fb) ? 1u : 0u);   // 0069FD48 sete al
    ;   // 0069FD4B pop ebx
    eax = eax + eax + 0xFDu;   // 0069FD4C lea eax,[eax+eax+000000FDh]
    return static_cast<std::uint16_t>(eax);   // 0069FD53 ret 4
L_0069FD56:
    eax = 0xFEu;   // 0069FD56 mov eax,0FEh
    ;   // 0069FD5B pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FD5C ret 4
L_0069FD5F:
    eax = 0x101u;   // 0069FD5F mov eax,101h
    ;   // 0069FD64 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FD65 ret 4
L_0069FD68:
    eax = 0x103u;   // 0069FD68 mov eax,103h
    ;   // 0069FD6D pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FD6E ret 4
L_0069FD71:
    eax = 0x102u;   // 0069FD71 mov eax,102h
    ;   // 0069FD76 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FD77 ret 4
L_0069FD7A:
    eax = 0x10Au;   // 0069FD7A mov eax,10Ah
    ;   // 0069FD7F pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FD80 ret 4
L_0069FD83:
    eax = 0xFDu;   // 0069FD83 mov eax,0FDh
    ;   // 0069FD88 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FD89 ret 4
L_0069FD8C:
    eax = 0x10Bu;   // 0069FD8C mov eax,10Bh
    ;   // 0069FD91 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FD92 ret 4
L_0069FD95:
    eax = 0x10Du;   // 0069FD95 mov eax,10Dh
    ;   // 0069FD9A pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FD9B ret 4
L_0069FD9E:
    eax = 0x12Bu;   // 0069FD9E mov eax,12Bh
    ;   // 0069FDA3 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FDA4 ret 4
L_0069FDA7:
    eax = 0x166u;   // 0069FDA7 mov eax,166h
    ;   // 0069FDAC pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FDAD ret 4
L_0069FDB0:
    fa = edx; fb = 0x27ECu;   // 0069FDB0 cmp edx,27ECh
    if (static_cast<I>(fa) > static_cast<I>(fb)) goto L_0069FE15;   // 0069FDB6 jg 0069FE15
    fa = edx; fb = 0x27EBu;   // 0069FDB8 cmp edx,27EBh
    if (static_cast<I>(fa) >= static_cast<I>(fb)) goto L_0069FE0C;   // 0069FDBE jge 0069FE0C
    edx -= 0x27A4u; fa = edx; fb = 0u;   // 0069FDC0 sub edx,27A4h
    fa = edx; fb = 0x8u;   // 0069FDC6 cmp edx,8
    if (fa > fb) goto L_0069FED3;   // 0069FDC9 ja 0069FED3
    switch (edx) {   // 0069FDCF jmp dword ptr [edx*4+006A0288h]
    case 0: goto L_0069FDE8;
    case 1: goto L_0069FDD6;
    case 2: goto L_0069FDDF;
    case 3: goto L_0069FED3;
    case 4: goto L_0069FE03;
    case 5: goto L_0069FED3;
    case 6: goto L_0069FDFA;
    case 7: goto L_0069F8AA;
    case 8: goto L_0069FDF1;
    default: __assume(0);   // index bounded by the preceding cmp/ja
    }
L_0069FDD6:
    eax = 0x100u;   // 0069FDD6 mov eax,100h
    ;   // 0069FDDB pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FDDC ret 4
L_0069FDDF:
    eax = 0x108u;   // 0069FDDF mov eax,108h
    ;   // 0069FDE4 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FDE5 ret 4
L_0069FDE8:
    eax = 0x116u;   // 0069FDE8 mov eax,116h
    ;   // 0069FDED pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FDEE ret 4
L_0069FDF1:
    eax = 0x113u;   // 0069FDF1 mov eax,113h
    ;   // 0069FDF6 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FDF7 ret 4
L_0069FDFA:
    eax = 0x174u;   // 0069FDFA mov eax,174h
    ;   // 0069FDFF pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FE00 ret 4
L_0069FE03:
    eax = 0x171u;   // 0069FE03 mov eax,171h
    ;   // 0069FE08 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FE09 ret 4
L_0069FE0C:
    eax = 0x110u;   // 0069FE0C mov eax,110h
    ;   // 0069FE11 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FE12 ret 4
L_0069FE15:
    fa = edx; fb = 0x28A1u;   // 0069FE15 cmp edx,28A1h
    if (static_cast<I>(fa) > static_cast<I>(fb)) goto L_0069FE96;   // 0069FE1B jg 0069FE96
    if (fa == fb) goto L_0069FC31;   // 0069FE1D je 0069FC31
    edx -= 0x27EDu; fa = edx; fb = 0u;   // 0069FE23 sub edx,27EDh
    fa = edx; fb = 0xB3u;   // 0069FE29 cmp edx,0B3h
    if (fa > fb) goto L_0069FED3;   // 0069FE2F ja 0069FED3
    ecx = kIndex_006A02C4[edx];   // 0069FE35 movzx ecx,byte ptr [edx+006A02C4h]
    switch (ecx) {   // 0069FE3C jmp dword ptr [ecx*4+006A02ACh]
    case 0: goto L_0069FE84;
    case 1: goto L_0069FE8D;
    case 2: goto L_0069FE43;
    case 3: goto L_0069FC86;
    case 4: goto L_0069FEBD;
    case 5: goto L_0069FED3;
    default: __assume(0);   // index bounded by the preceding cmp/ja
    }
L_0069FE43:
    ecx = ebx & 0xFFu;   // 0069FE43 movzx ecx,bl
    ecx -= 1u; fa = ecx; fb = 0u;   // 0069FE46 dec ecx
    fa = ecx; fb = 0x8u;   // 0069FE47 cmp ecx,8
    if (fa > fb) goto L_0069FED3;   // 0069FE4A ja 0069FED3
    switch (ecx) {   // 0069FE50 jmp dword ptr [ecx*4+006A0378h]
    case 0: goto L_0069FE57;
    case 1: goto L_0069FBB1;
    case 2: goto L_0069FE60;
    case 3: goto L_0069FB0B;
    case 4: goto L_0069FE69;
    case 5: goto L_0069FE72;
    case 6: goto L_0069FE7B;
    case 7: goto L_0069FED3;
    case 8: goto L_0069FE7B;
    default: __assume(0);   // index bounded by the preceding cmp/ja
    }
L_0069FE57:
    eax = 0x5Bu;   // 0069FE57 mov eax,5Bh
    ;   // 0069FE5C pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FE5D ret 4
L_0069FE60:
    eax = 0xADu;   // 0069FE60 mov eax,0ADh
    ;   // 0069FE65 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FE66 ret 4
L_0069FE69:
    eax = 0xDEu;   // 0069FE69 mov eax,0DEh
    ;   // 0069FE6E pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FE6F ret 4
L_0069FE72:
    eax = 0xECu;   // 0069FE72 mov eax,0ECh
    ;   // 0069FE77 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FE78 ret 4
L_0069FE7B:
    eax = 0xF4u;   // 0069FE7B mov eax,0F4h
    ;   // 0069FE80 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FE81 ret 4
L_0069FE84:
    eax = 0x112u;   // 0069FE84 mov eax,112h
    ;   // 0069FE89 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FE8A ret 4
L_0069FE8D:
    eax = 0x111u;   // 0069FE8D mov eax,111h
    ;   // 0069FE92 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FE93 ret 4
L_0069FE96:
    fa = edx; fb = 0x2AF9u;   // 0069FE96 cmp edx,2AF9h
    if (static_cast<I>(fa) > static_cast<I>(fb)) goto L_0069FEC6;   // 0069FE9C jg 0069FEC6
    if (fa == fb) goto L_0069FCA9;   // 0069FE9E je 0069FCA9
    fa = edx; fb = 0x28A2u;   // 0069FEA4 cmp edx,28A2h
    if (fa == fb) goto L_0069FEBD;   // 0069FEAA je 0069FEBD
    fa = edx; fb = 0x2AF8u;   // 0069FEAC cmp edx,2AF8h
    if (fa != fb) goto L_0069FED3;   // 0069FEB2 jne 0069FED3
    eax = 0x17Cu;   // 0069FEB4 mov eax,17Ch
    ;   // 0069FEB9 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FEBA ret 4
L_0069FEBD:
    eax = 0x10Fu;   // 0069FEBD mov eax,10Fh
    ;   // 0069FEC2 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FEC3 ret 4
L_0069FEC6:
    fa = edx; fb = 0x2AFAu;   // 0069FEC6 cmp edx,2AFAh
L_0069FECC:
    if (fa != fb) goto L_0069FED3;   // 0069FECC jne 0069FED3
    eax = 0x189u;   // 0069FECE mov eax,189h
L_0069FED3:
    ;   // 0069FED3 pop ebx
    return static_cast<std::uint16_t>(eax);   // 0069FED4 ret 4
