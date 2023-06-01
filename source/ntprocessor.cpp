//------------------------------------------------------------------------
// Copyright(c) 2023 aikelab.
//------------------------------------------------------------------------

#include "ntprocessor.h"
#include "ntcids.h"

#include "base/source/fstreamer.h"
#include "public.sdk/source/vst/hosting/eventlist.h"
#include "public.sdk/source/vst/vstaudioprocessoralgo.h"

using namespace Steinberg;

namespace aikelab {
//------------------------------------------------------------------------
// NotifyTestProcessor
//------------------------------------------------------------------------
NotifyTestProcessor::NotifyTestProcessor ()
{
	setControllerClass (kNotifyTestControllerUID);

	// 変更フラグを初期化
	changed.store(false);
}

//------------------------------------------------------------------------
NotifyTestProcessor::~NotifyTestProcessor ()
{}

//------------------------------------------------------------------------
tresult PLUGIN_API NotifyTestProcessor::initialize (FUnknown* context)
{
	tresult result = AudioEffect::initialize (context);
	if (result != kResultOk)
	{
		return result;
	}

	addAudioInput (STR16 ("Stereo In"), Steinberg::Vst::SpeakerArr::kStereo);
	addAudioOutput (STR16 ("Stereo Out"), Steinberg::Vst::SpeakerArr::kStereo);
	addEventInput (STR16 ("Event In"), 1);

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API NotifyTestProcessor::terminate ()
{
	return AudioEffect::terminate ();
}

//------------------------------------------------------------------------
tresult PLUGIN_API NotifyTestProcessor::setActive (TBool state)
{
	return AudioEffect::setActive (state);
}

//------------------------------------------------------------------------
tresult PLUGIN_API NotifyTestProcessor::process (Vst::ProcessData& data)
{
	// 今回の場合、パラメータ変更イベント処理は不要
	
	// MIDIイベント処理
	Vst::IEventList* events = data.inputEvents;

	if (events != NULL)
	{
		int32 numEvent = events->getEventCount();
		for (int32 i = 0; i < numEvent; i++)
		{
			Vst::Event event;
			if (events->getEvent(i, event) == kResultOk)
			{
				switch (event.type)
				{
				// Note Onイベント
				case Vst::Event::kNoteOnEvent:
					if (event.noteOn.velocity == 0.f)
					{
						// ベロシティ0のNoteOnはNoteOff扱い
						notestat[event.noteOn.pitch].store(0);
					}
					else
					{
						// NoteOnはステータスを1にする
						notestat[event.noteOn.pitch].store(1);
					}
					// 変更フラグを立てる
					changed.store(true);
					break;

				// Note Offイベント
				case Vst::Event::kNoteOffEvent:
					// NoteOffはステータスを0にする
					notestat[event.noteOff.pitch].store(0);
					// 変更フラグを立てる
					changed.store(true);
					break;
				}
			}
		}
	}


	// 今回の例は無音のプラグインなので、ノイズが出ないよう念のためバッファをゼロクリア
	uint32 sampleFramesSize = getSampleFramesSizeInBytes(processSetup, data.numSamples);
	void** out = getChannelBuffersPointer(processSetup, data.outputs[0]);
	memset(out[0], 0, sampleFramesSize);
	memset(out[1], 0, sampleFramesSize);
	// サイレンスフラグも立てておくとDAWに優しい
	data.outputs[0].silenceFlags = 0b11;

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API NotifyTestProcessor::notify(Vst::IMessage* message)
{
	auto msgID = message->getMessageID();

	// Editorのタイマーイベントから（Controller経由で）通知されたときの処理
	// process()とは別スレッドで実行される
	if (strcmp(msgID, "timer") == 0)
	{
		// データに変更があるときのみ実行
		if (changed.load())
		{
			// 変更フラグを元に戻す
			changed.store(false);
			// 送信用のメッセージを作成
			auto res = allocateMessage();
			// IDは自由なので、今回は"notestat"とした
			res->setMessageID("notestat");
			const uint32 size = 128;
			char8 data[size];
			// 送信用バッファに配列をコピー
			for (uint32 i = 0; i < size; i++)
				data[i] = notestat[i].load();
			// メッセージにデータをセット
			res->getAttributes()->setBinary("data", data, size);
			// ProcessorのsendMessageはControllerに通知される
			sendMessage(res);
			// メッセージのメモリを解放
			res->release();
		}
		return kResultTrue;
	}

	// "timer"メッセージ以外の処理は親クラスに任せる
	return AudioEffect::notify(message);
}

//------------------------------------------------------------------------
tresult PLUGIN_API NotifyTestProcessor::setupProcessing (Vst::ProcessSetup& newSetup)
{
	return AudioEffect::setupProcessing (newSetup);
}

//------------------------------------------------------------------------
tresult PLUGIN_API NotifyTestProcessor::canProcessSampleSize (int32 symbolicSampleSize)
{
	if (symbolicSampleSize == Vst::kSample32)
		return kResultTrue;
	return kResultFalse;
}

//------------------------------------------------------------------------
tresult PLUGIN_API NotifyTestProcessor::setState (IBStream* state)
{
	IBStreamer streamer (state, kLittleEndian);
	
	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API NotifyTestProcessor::getState (IBStream* state)
{
	IBStreamer streamer (state, kLittleEndian);

	return kResultOk;
}

//------------------------------------------------------------------------
} // namespace aikelab

