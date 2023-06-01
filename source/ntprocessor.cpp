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

	// �ύX�t���O��������
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
	// ����̏ꍇ�A�p�����[�^�ύX�C�x���g�����͕s�v
	
	// MIDI�C�x���g����
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
				// Note On�C�x���g
				case Vst::Event::kNoteOnEvent:
					if (event.noteOn.velocity == 0.f)
					{
						// �x���V�e�B0��NoteOn��NoteOff����
						notestat[event.noteOn.pitch].store(0);
					}
					else
					{
						// NoteOn�̓X�e�[�^�X��1�ɂ���
						notestat[event.noteOn.pitch].store(1);
					}
					// �ύX�t���O�𗧂Ă�
					changed.store(true);
					break;

				// Note Off�C�x���g
				case Vst::Event::kNoteOffEvent:
					// NoteOff�̓X�e�[�^�X��0�ɂ���
					notestat[event.noteOff.pitch].store(0);
					// �ύX�t���O�𗧂Ă�
					changed.store(true);
					break;
				}
			}
		}
	}


	// ����̗�͖����̃v���O�C���Ȃ̂ŁA�m�C�Y���o�Ȃ��悤�O�̂��߃o�b�t�@���[���N���A
	uint32 sampleFramesSize = getSampleFramesSizeInBytes(processSetup, data.numSamples);
	void** out = getChannelBuffersPointer(processSetup, data.outputs[0]);
	memset(out[0], 0, sampleFramesSize);
	memset(out[1], 0, sampleFramesSize);
	// �T�C�����X�t���O�����ĂĂ�����DAW�ɗD����
	data.outputs[0].silenceFlags = 0b11;

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API NotifyTestProcessor::notify(Vst::IMessage* message)
{
	auto msgID = message->getMessageID();

	// Editor�̃^�C�}�[�C�x���g����iController�o�R�Łj�ʒm���ꂽ�Ƃ��̏���
	// process()�Ƃ͕ʃX���b�h�Ŏ��s�����
	if (strcmp(msgID, "timer") == 0)
	{
		// �f�[�^�ɕύX������Ƃ��̂ݎ��s
		if (changed.load())
		{
			// �ύX�t���O�����ɖ߂�
			changed.store(false);
			// ���M�p�̃��b�Z�[�W���쐬
			auto res = allocateMessage();
			// ID�͎��R�Ȃ̂ŁA�����"notestat"�Ƃ���
			res->setMessageID("notestat");
			const uint32 size = 128;
			char8 data[size];
			// ���M�p�o�b�t�@�ɔz����R�s�[
			for (uint32 i = 0; i < size; i++)
				data[i] = notestat[i].load();
			// ���b�Z�[�W�Ƀf�[�^���Z�b�g
			res->getAttributes()->setBinary("data", data, size);
			// Processor��sendMessage��Controller�ɒʒm�����
			sendMessage(res);
			// ���b�Z�[�W�̃����������
			res->release();
		}
		return kResultTrue;
	}

	// "timer"���b�Z�[�W�ȊO�̏����͐e�N���X�ɔC����
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

