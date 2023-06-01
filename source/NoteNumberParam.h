// ノート番号表示用パラメータクラス

#pragma once

#include "public.sdk/source/vst/vstparameters.h"

using namespace Steinberg;
using namespace Vst;

class NoteNumberParameter : public Parameter
{
public:
	NoteNumberParameter(TChar* title, int32 id);
	void toString(ParamValue normValue, String128 string) const SMTG_OVERRIDE;
	bool fromString(const TChar* string, ParamValue& normValue) const SMTG_OVERRIDE;

	// ノート番号のセットと表示
	void setNoteNumber(int noteNumber);
protected:
	int noteNumber_;
};
