#pragma once

#include "vstgui/plugin-bindings/vst3editor.h"

using namespace VSTGUI;

class MyEditor : public VST3Editor
{
public:
	MyEditor(Steinberg::Vst::EditController* controller, UTF8StringPtr templateName, UTF8StringPtr xmlFile);
	CMessageResult notify(CBaseObject* sender, const char* message) SMTG_OVERRIDE;
protected:
	~MyEditor() override;

	DELEGATE_REFCOUNT(VST3Editor)
};
