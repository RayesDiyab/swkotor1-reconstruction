#pragma once
namespace recovered_gff_scratch {
constexpr unsigned list_bytes=20,struct_bytes=4,ref_bytes=16;
constexpr unsigned encounters=0x70,stores=0x70,effects=0x50,triggers=0xd0,cameras=0x78;
static_assert(0x54+list_bytes<=encounters && 0x58+list_bytes<=stores && 0x38+list_bytes<=effects);
static_assert(0xb0+list_bytes<=triggers && 0x5c+list_bytes<=triggers);
static_assert(0x50+list_bytes<=cameras && 0x6c+struct_bytes<=cameras);
static_assert(0x5c+list_bytes<=0x90 && 0x84+struct_bytes<=0x90 && 0x4c+ref_bytes<=0x90);
static_assert(0x44+list_bytes<=0x74 && 0x6c+struct_bytes<=0x74 && 0x34+ref_bytes<=0x74);
static_assert(0x30+list_bytes<=0x60 && 0x58+struct_bytes<=0x60);
static_assert(0x20+list_bytes<=0x50 && 0x48+struct_bytes<=0x50);
static_assert(0x48+list_bytes<=0x78 && 0x70+struct_bytes<=0x78 && 0x38+ref_bytes<=0x78);
static_assert(0x60+list_bytes<=0x90 && 0x88+struct_bytes<=0x90 && 0x40+ref_bytes<=0x90);
}
