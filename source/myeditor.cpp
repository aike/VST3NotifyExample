#include "myeditor.h"


MyEditor::MyEditor(Steinberg::Vst::EditController* controller, UTF8StringPtr templateName, UTF8StringPtr xmlFile)
	: VST3Editor(controller, templateName, xmlFile)
{
	// タイマー間隔を10msecに変更する(デフォルトは100msec)
	setIdleRate(10);
}

MyEditor::~MyEditor()
{}


CMessageResult MyEditor::notify(CBaseObject* sender, const char* message)
{
	// 10msec毎に実行される処理
	if (message == CVSTGUITimer::kMsgTimer)
	{
		// 送信用のメッセージを作成
		auto message = controller->allocateMessage();
		// IDは自由なので、今回は"timer"とした
		message->setMessageID("timer");
		// 特に送信するデータはないためデータをセットする処理は不要

		// ControllerのsendMessageを呼ぶとProcessorに通知される
		controller->sendMessage(message);
		// メッセージのメモリを解放
		message->release();
	}

	return VST3Editor::notify(sender, message);
}
