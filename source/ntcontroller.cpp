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

	// NoteNumberParameter型のパラメータを追加
	noteNumberParam_ = new NoteNumberParameter(STR("NoteNumber"), kNoteNumberId);
	parameters.addParameter(noteNumberParam_);

	return result;
}

//------------------------------------------------------------------------
IPlugView* PLUGIN_API NotifyTestController::createView(FIDString name)
{
	if (FIDStringsEqual(name, Vst::ViewType::kEditor))
	{
		// MyEditor型のビューを生成
		auto* view = new MyEditor(this, "view", "nteditor.uidesc");
		return view;
	}
	return nullptr;
}

tresult PLUGIN_API NotifyTestController::notify(Vst::IMessage* message)
{
	auto msgID = message->getMessageID();

	// Processorから通知されたときに実行される処理
	if (strcmp(msgID, "notestat") == 0)
	{
		uint32 size;
		const void* p;
		// データのポインタとサイズを取得
		// 今回の例の場合、サイズは128が取得できるはず
		message->getAttributes()->getBinary("data", p, size);
		// void*型だと扱いにくいのでキャスト
		char8* data = (char8*)p;
		int noteNumber = -1;
		// 配列を上から（高音側）からスキャンして、NoteOnステータスの
		// ノートナンバーが見つかったらブレイク
		for (int i = size - 1; i >= 0; i--) {
			if (data[i] > 0) {
				noteNumber = i;
				break;
			}
		}
		// パラメータ状態を変更して画面に表示
		noteNumberParam_->setNoteNumber(noteNumber);

		return kResultTrue;
	}

	// "notestat"メッセージ以外の処理は親クラスに任せる
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
