//------------------------------------------------------------------------
// Copyright(c) 2023 aikelab.
//------------------------------------------------------------------------

#include "ntcontroller.h"
#include "ntcids.h"
//#include "vstgui/plugin-bindings/vst3editor.h"
#include "myeditor.h"

#define kNoteNumberId 100

using namespace Steinberg;

namespace aikelab {

//------------------------------------------------------------------------
// NotifyTestController Implementation
//------------------------------------------------------------------------
tresult PLUGIN_API NotifyTestController::initialize (FUnknown* context)
{
	tresult result = EditControllerEx1::initialize (context);
	if (result != kResultOk)
	{
		return result;
	}

	// NoteNumberParameter�^�̃p�����[�^��ǉ�
	noteNumberParam_ = new NoteNumberParameter(STR("NoteNumber"), kNoteNumberId);
	parameters.addParameter(noteNumberParam_);

	return result;
}

//------------------------------------------------------------------------
IPlugView* PLUGIN_API NotifyTestController::createView(FIDString name)
{
	if (FIDStringsEqual(name, Vst::ViewType::kEditor))
	{
		// MyEditor�^�̃r���[�𐶐�
		auto* view = new MyEditor(this, "view", "nteditor.uidesc");
		return view;
	}
	return nullptr;
}

tresult PLUGIN_API NotifyTestController::notify(Vst::IMessage* message)
{
	auto msgID = message->getMessageID();

	// Processor����ʒm���ꂽ�Ƃ��Ɏ��s����鏈��
	if (strcmp(msgID, "notestat") == 0)
	{
		uint32 size;
		const void* p;
		// �f�[�^�̃|�C���^�ƃT�C�Y���擾
		// ����̗�̏ꍇ�A�T�C�Y��128���擾�ł���͂�
		message->getAttributes()->getBinary("data", p, size);
		// void*�^���ƈ����ɂ����̂ŃL���X�g
		char8* data = (char8*)p;
		int noteNumber = -1;
		// �z����ォ��i�������j����X�L�������āANoteOn�X�e�[�^�X��
		// �m�[�g�i���o�[������������u���C�N
		for (int i = size - 1; i >= 0; i--) {
			if (data[i] > 0) {
				noteNumber = i;
				break;
			}
		}
		// �p�����[�^��Ԃ�ύX���ĉ�ʂɕ\��
		noteNumberParam_->setNoteNumber(noteNumber);

		return kResultTrue;
	}

	// "notestat"���b�Z�[�W�ȊO�̏����͐e�N���X�ɔC����
	return Vst::EditControllerEx1::notify(message);
}

//------------------------------------------------------------------------
tresult PLUGIN_API NotifyTestController::terminate ()
{
	return EditControllerEx1::terminate ();
}

//------------------------------------------------------------------------
tresult PLUGIN_API NotifyTestController::setComponentState (IBStream* state)
{
	if (!state)
		return kResultFalse;

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API NotifyTestController::setState (IBStream* state)
{
	return kResultTrue;
}

//------------------------------------------------------------------------
tresult PLUGIN_API NotifyTestController::getState (IBStream* state)
{
	return kResultTrue;
}

//------------------------------------------------------------------------
tresult PLUGIN_API NotifyTestController::setParamNormalized (Vst::ParamID tag, Vst::ParamValue value)
{
	tresult result = EditControllerEx1::setParamNormalized (tag, value);
	return result;
}

//------------------------------------------------------------------------
tresult PLUGIN_API NotifyTestController::getParamStringByValue (Vst::ParamID tag, Vst::ParamValue valueNormalized, Vst::String128 string)
{
	return EditControllerEx1::getParamStringByValue (tag, valueNormalized, string);
}

//------------------------------------------------------------------------
tresult PLUGIN_API NotifyTestController::getParamValueByString (Vst::ParamID tag, Vst::TChar* string, Vst::ParamValue& valueNormalized)
{
	return EditControllerEx1::getParamValueByString (tag, string, valueNormalized);
}

//------------------------------------------------------------------------
} // namespace aikelab
