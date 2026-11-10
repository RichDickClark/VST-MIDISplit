# VST-MIDISplit

A VST3 MIDI utility with one MIDI input and two MIDI outputs. Every incoming event is copied unchanged to both outputs, including note, controller, pitch-bend, and other MIDI events.

## Build

Requirements: CMake 3.25 or newer, a C++17 compiler, Git, and an internet connection for the first configure (the Steinberg VST3 SDK is fetched automatically).

```sh
cmake -S . -B build
cmake --build build
```

The VST3 bundle is generated in `build/VST3/Release/VST-MIDISplit.vst3` by default (the configuration folder varies on multi-configuration generators). Enable both output ports in the host and connect each as needed.
