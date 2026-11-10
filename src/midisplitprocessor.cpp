#include "public.sdk/source/main/pluginfactory.h"
#include "public.sdk/source/vst/vstaudioeffect.h"
#include "public.sdk/source/vst/vsteditcontroller.h"
#include "pluginterfaces/vst/ivstevents.h"

namespace MIDISplit {

using namespace Steinberg;
using namespace Steinberg::Vst;

static const FUID ProcessorUID(0x6B57F5A1, 0x9E7D4A32, 0xB1F68C20, 0xD4A3E917);
static const FUID ControllerUID(0x2F6B1C84, 0xA09347DE, 0xBE55219A, 0x73C804F6);

class Processor final : public AudioEffect {
public:
    static FUnknown* createInstance(void*) {
        return static_cast<IAudioProcessor*>(new Processor());
    }

    tresult PLUGIN_API initialize(FUnknown* context) override {
        const tresult result = AudioEffect::initialize(context);
        if (result != kResultOk) {
            return result;
        }

        setControllerClass(ControllerUID);
        addEventInput(STR16("MIDI Input"), 16);
        addEventOutput(STR16("MIDI Output 1"), 16);
        addEventOutput(STR16("MIDI Output 2"), 16);
        return kResultOk;
    }

    tresult PLUGIN_API process(ProcessData& data) override {
        if (data.inputEvents == nullptr || data.outputEvents == nullptr) {
            return kResultOk;
        }

        const int32 eventCount = data.inputEvents->getEventCount();
        for (int32 index = 0; index < eventCount; ++index) {
            Event event{};
            if (data.inputEvents->getEvent(index, event) != kResultOk) {
                continue;
            }

            event.busIndex = 0;
            data.outputEvents->addEvent(event);

            event.busIndex = 1;
            data.outputEvents->addEvent(event);
        }

        return kResultOk;
    }
};

class Controller final : public EditController {
public:
    static FUnknown* createInstance(void*) {
        return static_cast<IEditController*>(new Controller());
    }
};

} // namespace MIDISplit

BEGIN_FACTORY_DEF("VST-MIDISplit", "", "")

DEF_CLASS2(
    INLINE_UID_FROM_FUID(MIDISplit::ProcessorUID),
    PClassInfo::kManyInstances,
    kVstAudioEffectClass,
    "VST-MIDISplit",
    Vst::kDistributable,
    "Fx",
    "1.0.0",
    kVstVersionString,
    MIDISplit::Processor::createInstance)

DEF_CLASS2(
    INLINE_UID_FROM_FUID(MIDISplit::ControllerUID),
    PClassInfo::kManyInstances,
    kVstComponentControllerClass,
    "VST-MIDISplit Controller",
    0,
    "",
    "1.0.0",
    kVstVersionString,
    MIDISplit::Controller::createInstance)

END_FACTORY