# Dryad API for JUCE

Dryad is a small harmony engine that turns chord progressions and motivic fragments
into streams of note events.  This repository provides an imagined developer-friendly
C++ API that can be embedded in a JUCE application to drive a sequencer, a game
engine or any other target that consumes note information.

## Building

```bash
cmake -S . -B build
cmake --build build
```

The build produces a static library `libdryad.a` and a small test binary.

### JUCE GUI demo

Enable `DRYAD_BUILD_JUCE_DEMO` during configuration to compile a minimal JUCE
application for experimenting with Dryad. The demo offers a node-based
progression editor, a piano-roll motif sequencer, and a simple built-in
synthesiser for auditioning the results.

## Core Concepts

* **Voice** – a playable range of notes, e.g. "Bass" or "Lead".
* **Motif** – a sequence of `MotifNote` objects aligned to harmonic and
  rhythmic anchors.
* **Progression** – a list of `Chord` definitions describing the harmonic flow.
* **Transition** – tells the engine when and how to move to a new progression.
* **ScoreEvent** – a note-on or note-off that should be forwarded to a MIDI
  buffer, a synthesiser or a game event queue.

## C++ API

Dryad defines aliases for its core types in `dryad/src/overrides.h`.  They map to
standard library containers by default but can be changed to custom
implementations before building the library.

```cpp
namespace dryad
{

struct MotifNote
{
    int relativeValue;    // semitone offset from the anchor
    int relativePosition; // beat position within the motif
    int duration;         // beats
};

enum class HarmonicAnchor
{
    Scale,
    Chord
};

enum class RhythmicAnchor
{
    ChordBeginning,
    StrongBeat,
    WeakBeat,
    AnyBeat
};

enum class NoteIntervalType
{
    Diatonic,
    Chromatic
};

struct Motif
{
    int id;
    String name;
    HarmonicAnchor harmonic;
    RhythmicAnchor rhythmic;
    NoteIntervalType intervalType;
    int duration;             // beats including trailing silence
    Vector<MotifNote> notes;
};

struct Voice
{
    int id;
    String name;
    Vector<Motif*> motifs;    // motifs owned by this voice
};

enum class Degree
{
    Tonic = 0,
    Supertonic,
    Mediant,
    Subdominant,
    Dominant,
    Submediant,
    LeadingTone
};

enum class Accidental
{
    Flat = -1,
    Natural = 0,
    Sharp = 1
};

enum class ChordQuality
{
    None            = 0,
    Minor           = 1 << 0,
    Major           = 1 << 1,
    HalfDiminished  = 1 << 2,
    Augmented       = 1 << 3,
    Sus2            = 1 << 4,
    Sus4            = 1 << 5,
    Diminished      = 1 << 6
};

struct Chord
{
    Degree degree;
    ChordQuality qualities; // flag set
    Accidental accidental;
};

struct ProgressionChord
{
    Chord chord;
    int duration;    // beats
};

struct Progression
{
    int id;
    String name;
    Vector<ProgressionChord> chords;
};

enum class EventType
{
    NoteOn,
    NoteOff
};

struct ScoreEvent
{
    int voiceId;
    int value;           // MIDI note number
    EventType event;
    int position;        // sample position within the block
};

struct Transition
{
    int userTag;         // forwarded to callbacks
    int progressionId;
    int root;            // MIDI note number of the tonic
};

class Engine
{
public:
    using Callback = void (*)(int);

    void registerVoice(const Voice&);
    void registerMotif(const Motif&);
    void registerProgression(const Progression&);

    void setChordsVoice(int voiceId);
    void setVoiceMotif(int voiceId, int motifId); // motifId < 0 disables

    void queueTransition(const Transition&);
    void commitBeats(int beats);          // advance the timeline
    bool popEvent(ScoreEvent&);           // fetch generated notes

    void onBeat(Callback);       // called every beat
    void onBar(Callback);        // called every bar
    void onProgression(Callback); // called on progression change
};

} // namespace dryad
```

## Using the Engine in JUCE

```cpp
class MyPluginProcessor : public juce::AudioProcessor
{
    dryad::Engine engine;

public:
    void prepareToPlay(double, int) override
    {
        engine.registerVoice({1, "Lead", {}});
        engine.registerProgression(makeProgression());
        engine.setChordsVoice(1);
        engine.queueTransition({0, /*progId*/1, /*root*/60});
    }

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer& midi) override
    {
        engine.commitBeats(getBeatsThisBlock());
        dryad::ScoreEvent ev;
        while (engine.popEvent(ev))
        {
            if (ev.event == dryad::EventType::NoteOn)
            {
                midi.addEvent(juce::MidiMessage::noteOn(1, ev.value, (juce::uint8)100), ev.position);
            }
            else
            {
                midi.addEvent(juce::MidiMessage::noteOff(1, ev.value), ev.position);
            }
        }
    }
};
```

The same `Engine` can feed note events into a game engine by converting each
`ScoreEvent` into whatever event system the game uses.  Callbacks such as
`onBeat` or `onProgression` are useful to synchronise visual effects with the
music.

This API description is conceptual and intended as a guide for building a
friendly interface around the Dryad harmony core.
