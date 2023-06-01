VST3NotifyExample
===

## はじめに

VST3プラグインでは、信号処理用のProcessorとパラメータ管理・UI管理用のControllerは別コンポーネントとなっており、両者の間でデータをやりとりするときはParameterChangeまたはMessageを使うことになっている。
ParameterChangeの仕組みは実装例が豊富であるものの、扱える情報は基本的に0.0～1.0の正規化された実数だけであり、整数、配列、文字列といったデータは扱いづらい。  
一方でMessageの実装例は少なく、特にProcessor側からControllerへ伝えるシンプルな例は現状ほとんどない。

オーディオ情報やMIDI情報はProcessorのprocess()関数で検知する。そのためこれらを画面表示するためにはControllerへ渡す必要がある。ただし、process()関数はリアルタイムスレッドで動いており、このスレッドで時間のかかるメッセージ送信をしてはならず、代わりにタイマースレッドを使うようドキュメントには書いてある。

[FAQ - How should I communicate between the 'Processing' and the 'User Interface'?](https://steinbergmedia.github.io/vst3_dev_portal/pages/FAQ/Communication.html#q-how-should-i-communicate-between-the-processing-and-the-user-interface)

[Communication between the components - Private communication](https://steinbergmedia.github.io/vst3_dev_portal/pages/Technical+Documentation/API+Documentation/Index.html#private-communication)

そこで、タイマースレッドでProcessorからメッセージ送信する、もっともシンプルなVST3/VSTGUIプラグイン実装例を書いてみた。

## プラグイン仕様

- VST InstrumentプラグインとしてDAWから起動できる
- MIDIキーボードやピアノロールからノートオンを受信すると、押されたノート番号をプラグイン画面に表示する
- 複数の鍵盤を押した場合、一番高い音のノート番号を表示する。

## データ送信～表示の仕組み

1. EditorクラスではUI更新用に定期的なタイマーイベントが動いている。ここでProcessorに毎回空のメッセージを送信する
2. ProcessorはNoteOn/NoteOffイベントのたびに各鍵盤の状態を配列に記憶しておく
3. メッセージを受信したProcessorは、鍵盤状態に変更があれば鍵盤状態配列をメッセージデータとしてControllerに返信する
4. Controllerはノート番号表示用パラメータクラスに、押されている鍵盤のうち最も高い音のノート番号を渡す
5. ノート番号表示用パラメータクラスは自身の状態を変更すると自動的に画面に反映される

## LICENSE

VST3NotifyExample program is licensed under MIT License.  
Copyright 2023, aike (@aike1000)

