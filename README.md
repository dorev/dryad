## TODO

* Make a DryadGUI "Hello World!"

Dryad, again!

## Another rework?

The idea would be to make Dryad more of an harmony analyzer and incremental score ledger/stream.
The point is also really to minimize the API surface, limit multithreading risks.

How different would that be from the current setup?

Leveraging preexisting progressions would be amazing, and bypass the editor aspect for now.

It would do the following:
* Maintain a score, with already committed and pending notes
* Manage 3 types of voice:
  * Chords
  * Melodies
  * Percs (TBD)

* User API:
    * Data
        * RegisterProgression(Progression{})
        * RegisterMelody(Melody{})
        * RegisterVoice(Voice{})
    * Communication
        * CommitBeats()
        * PopNewCommittedEvent() --> ScoreEvent
    * Changes
        * SetupTransition(Transition)
        * SetChordsVoice(VoiceId)
        * SetVoiceMelody(MelodyId) // null == kill
    * Info
        * GetPendingTransition()
        * GetCurrentProgression()
        * GetCurrentMelodies()
    * Callbacks
        * OnTransition (to change voices, melodies, etc..)
        * OnBeat
        * OnBar
        * OnProgression

constexpr int MaxNameSize = 128;
constexpr int MaxNotesPerMelody = 128;
constexpr int MaxChordsPerProgression = 32;

enum class EventType
{
    Invalid,
    NoteOn,
    NoteOff
};

struct ScoreEvent
{
    int voiceId;
    int value;
    EventType event;
    int scorePosition;
};

struct Voice
{
    int id;
    char name[MaxNameSize];
    int rangeTop;
    int rangeBottom;
};

enum class HarmonicAnchor
{
    Invalid,
    Chord,
    Scale
};

enum class RythmicAnchor
{
    Invalid,
    AnyBeat,
    StrongBeat,
    WeakBeat
};

struct MelodyNote
{
    int offset;
    int position;
    int duration;
};

struct Melody
{
    int id;
    char name[MaxNameSize];
    HarmonicAnchor harmonicAnchor;
    RythmicAnchor rythmicAnchor;
    MelodyNote notes[MaxNotesPerMelody];
};

enum class Quality
{
    Default = 0,
    Minor = 1,
    Major = 2,
    Diminished = 3,
    Augmented = 4,
    Sus2 = 5,
    Sus4 = 6,
    DiminishedSeventh = 7,
    Seventh = 1 >> 4,
    MajorSeventh = 1 >> 5
};

struct Chord
{
    int degree;
    int accidental;
    Quality quality;
    Duration;
};

struct Progression
{
    int id;
    char name[MaxNameSize];
    Chord chords[MaxChordsPerProgression];
};

enum class TransitionPosition
{
    EndOfProgression,
    EndOfBar,
    NextBeat
};

struct Transition
{
    int userTag;
    int progressionId;
    int root;
    TransitionPosition type;
};

// Internal

Score
{
    RegisterVoice();
    RegisterMelody();
    RegisterProgression();
}