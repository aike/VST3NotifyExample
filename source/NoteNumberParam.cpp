// �m�[�g�ԍ��\���p�p�����[�^�N���X

#include "NoteNumberParam.h"
#include "pluginterfaces/base/ustring.h"

NoteNumberParameter::NoteNumberParameter(TChar* title, int32 id)
{
	noteNumber_ = -1;

	UString(info.title, str16BufferSize(String128)).assign(title);
	info.flags = ParameterInfo::kCanAutomate;
	info.id = id;
	info.stepCount = 0;
	info.defaultNormalizedValue = 0.f;
	info.unitId = kRootUnitId;
}

void NoteNumberParameter::toString(ParamValue normValue, String128 string) const
{
	// �f�[�^�\�����ɌĂ΂�鏈��

	// �\������m�[�g�i���o�[���Ȃ��ꍇ�A�߂�l�p�ϐ�string��"-"���Z�b�g
	if (noteNumber_ < 0) {
		// String128�^�́A����128��char16�^��������Ӗ�����
		// ���̂���char16�p�̊֐��ŕ�������R�s�[����
		strcpy16(string, STR("-"));
		return;
	}

	char text[32];
	// int�^�̃����o�ϐ�noteNumber_�̒l��char������ɕϊ�
	itoa(noteNumber_, text, 10);
	// char�������char16������ɕϊ�����ɂ�UString�N���X���g���Ǝ����葁��
	// �߂�l�p�ϐ�string�Ƀm�[�g�i���o�[��������Z�b�g
	Steinberg::UString(string, 128).fromAscii(text);
}

bool NoteNumberParameter::fromString(const TChar* string, ParamValue& normValue) const
{
	return false;
}

void NoteNumberParameter::setNoteNumber(int noteNumber)
{
	// �f�[�^�ێ��p�����o�ϐ����X�V
	noteNumber_ = noteNumber;
	// valueNormalized�ɕύX���Ȃ��ƕ\�����X�V����Ȃ��̂œK���ɒl��ς���
	setNormalized(1.0 - valueNormalized);
}
