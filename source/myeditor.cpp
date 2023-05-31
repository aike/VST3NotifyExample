#include "myeditor.h"


MyEditor::MyEditor(Steinberg::Vst::EditController* controller, UTF8StringPtr templateName, UTF8StringPtr xmlFile)
	: VST3Editor(controller, templateName, xmlFile)
{
	// �^�C�}�[�Ԋu��10msec�ɕύX����(�f�t�H���g��100msec)
	setIdleRate(10);
}

MyEditor::~MyEditor()
{}


CMessageResult MyEditor::notify(CBaseObject* sender, const char* message)
{
	// 10msec���Ɏ��s����鏈��
	if (message == CVSTGUITimer::kMsgTimer)
	{
		// ���M�p�̃��b�Z�[�W���쐬
		auto message = controller->allocateMessage();
		// ID�͎��R�Ȃ̂ŁA�����"timer"�Ƃ���
		message->setMessageID("timer");
		// ���ɑ��M����f�[�^�͂Ȃ����߃f�[�^���Z�b�g���鏈���͕s�v

		// Controller��sendMessage���ĂԂ�Processor�ɒʒm�����
		controller->sendMessage(message);
		// ���b�Z�[�W�̃����������
		message->release();
	}

	return VST3Editor::notify(sender, message);
}
