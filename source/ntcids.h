//------------------------------------------------------------------------
// Copyright(c) 2023 aikelab.
//------------------------------------------------------------------------

#pragma once

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/vsttypes.h"

namespace aikelab {
//------------------------------------------------------------------------
static const Steinberg::FUID kNotifyTestProcessorUID (0x44541D02, 0xB51C5961, 0xB34E5117, 0xA5CAE759);
static const Steinberg::FUID kNotifyTestControllerUID (0x9AF808E9, 0xBC475B80, 0x863A8098, 0x824D268E);

#define NotifyTestVST3Category "Instrument"

//------------------------------------------------------------------------
} // namespace aikelab
