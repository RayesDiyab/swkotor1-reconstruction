// lane-m1-opt-sound-gfx: link-time joins (D9 /alternatename mechanism, same ABI) for the two single-owner bodies this
// lane keeps (coordinator ruling): CSWGuiOptionsGraphics::OnExitSlider 006DED90 and
// CSWGuiOptionsGraphicsAdvanced::SetDescription 006DF390. lane-m1-opt-gameplay consumers spell them
// kotor_m1_opt::FoldedHandlers::OnExitSlider_006ded90 (thiscall member, RET 4) and
// @kotor_m1_opt_graphics_adv_set_description_006df390@12 (fastcall, RET 4); its unit m1-opt-folded is dropped and the
// spellings resolve here. An alternate name is applied only while the left symbol stays undefined, so these rows are
// inert if that unit were still linked (the composer must then still keep exactly one body per address).
#pragma comment(linker, "/alternatename:?OnExitSlider_006ded90@FoldedHandlers@kotor_m1_opt@@QAEXPAUControl@kotor_input_hover@@@Z=?OnExitSlider_006ded90@OptionsSliderHandlers@kotor_m1_options@@QAEXPAUControl@kotor_input_hover@@@Z")
#pragma comment(linker, "/alternatename:@kotor_m1_opt_graphics_adv_set_description_006df390@12=@kotor_m1_options_graphics_adv_set_description_006df390@12")
