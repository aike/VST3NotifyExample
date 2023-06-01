// ノート番号表示用パラメータクラス

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
	// データ表示時に呼ばれる処理

	// 表示するノートナンバーがない場合、戻り値用変数stringに"-"をセット
	if (noteNumber_ < 0) {
		// String128型は、長さ128のchar16型文字列を意味する
		// そのためchar16用の関数で文字列をコピーする
		strcpy16(string, STR("-"));
		return;
	}

	char text[32];
	// int型のメンバ変数noteNumber_の値をchar文字列に変換
	itoa(noteNumber_, text, 10);
	// char文字列をchar16文字列に変換するにはUStringクラスを使うと手っ取り早い
	// 戻り値用変数stringにノートナンバー文字列をセット
	Steinberg::UString(string, 128).fromAscii(text);
}

bool NoteNumberParameter::fromString(const TChar* string, ParamValue& normValue) const
{
	return false;
}

void NoteNumberParameter::setNoteNumber(int noteNumber)
{
	// データ保持用メンバ変数を更新
	noteNumber_ = noteNumber;
	// valueNormalizedに変更がないと表示が更新されないので適当に値を変える
	setNormalized(1.0 - valueNormalized);
}
