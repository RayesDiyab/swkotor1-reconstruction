#pragma once
// Lane B (B-cerf-quicksave) exact export contracts.
//
// These are the callable names D's 006323A0 and the other consumers bind. The
// decoration is produced from the C linkage and calling convention; it is not a
// numeric-address alias. See exports.txt for the exact decorated list.
//
// ABI per accepted PLAN:
//   0062FBE0 2 stack args, RET 8        -> __stdcall _...@8
//   005DD9C0 ECX=this, 0, RET 0, EAX=this -> __fastcall @4
//   005DCE50 ECX=this, 1 arg, RET 4    -> __fastcall @12
//   005DD3C0 ECX=this, 1 arg, RET 4    -> __fastcall @12
//   005DD710 ECX=this, 6 args, RET 24  -> __fastcall @32
//   005DE3A0 ECX=this, 0, RET 0        -> __fastcall @4
//   005E6A20 ECX=this, 0, RET 0        -> __fastcall @4
//   005DD2B0 ECX=this, 1 arg, RET 4    -> __fastcall @12
//   005DD170 ECX=this, 3 args, RET 12  -> __fastcall @20
//   005DD350 ECX=this, 0, RET 0, EAX=this -> __fastcall @4
//   005DE2C0 ECX=this, 0, RET 0        -> __fastcall @4
//   005DD240 ECX=this, 1 arg, RET 4    -> __fastcall @12
//   005E6970 ECX=file, 2 args, RET 8   -> __fastcall @16
//   005EB9F0 ECX=file, 2 args, RET 8   -> __fastcall @16
//   004010A0 ECX=client, 0, RET 0      -> __fastcall @4
//   006C8750 4 stack args, RET 0       -> __cdecl _name

extern "C" {

int __stdcall cerf_copy_quick_save_game_to_future_game_0062fbe0(
    int save_id, void* name);

void* __fastcall cerf_construct_cerf_file_005dd9c0(void* self);
int __fastcall cerf_read_cerf_file_005dce50(
    void* self, void* unused, void* path);
int __fastcall cerf_read_header_variance_005dd3c0(
    void* self, void* unused, void* path);
void __fastcall cerf_export_files_from_erf_005dd710(
    void* self, void* unused, void* path, void* out_name, int flags,
    void* callback, int ctx1, int ctx2);
void __fastcall cerf_destroy_cerf_file_005de3a0(void* self);

int __fastcall cerf_c_exofile_flush_005e6a20(void* self);
int __fastcall cerf_c_exofile_read_string_record_005dd2b0(
    void* self, void* unused, void* file);
int __fastcall cerf_cerf_res_copy_to_file_005dd170(
    void* self, void* unused, void* in_file, void* out_file, void* unused3);
void* __fastcall cerf_list_construct_005dd350(void* self);
int __fastcall cerf_cerf_file_reset_005de2c0(void* self);
int __fastcall cerf_cerf_string_set_text_005dd240(
    void* self, void* unused, void* src);

int __fastcall cerf_c_exofile_read_string_005e6970(
    void* file, void* unused, void* out, int length);
int __fastcall cerf_c_exofile_internal_read_string_005eb9f0(
    void* file, void* unused, void* out, int length);
void __fastcall cerf_alloc_large_temp_buffer_004010a0(void* self);

int __cdecl cerf_load_screen_update_006c8750(int a, int b, int c, int d);

}  // extern "C"
