#pragma once

#include "graph.h"
#include "voice.h"
#include "chord.h"

namespace Dryad
{

    class MotifNote;
    class Scale;
    class Score;

    class ScoreFrame : public Node
    {
    public:
        DRYAD_NODE_CLASS_ID(ScoreFrame);

        ScoreFrame(Time relativePosition = 0);

        // Comparison helper to automate the ordering in a set
        struct compare_by_position
        {
            bool operator()(const ScoreFrame* a, const ScoreFrame* b) const
            {
                return a->relativePosition < b->relativePosition;
            }
        };

        Score* getScore();
        NoteValue getCurrentRoot();
        Scale* getCurrentScale();
        Chord getCurrentChord();
        Error addMotifNote(MotifNote* motifNote);

        Time relativePosition;
        bool committed;
    };

    class Progression;
    class Scale;
    struct SerializedScore;

    class Score : public Graph
    {
    public:
        DRYAD_NODE_CLASS_ID(Score);

        Score();

        Voice* addVoice(int id, String name);
        Error addMotifInstance(Voice* voice, Motif* motif, Time position, MotifInstance*& instance);

        // Calling this will set in stone the notes within the specified duration,
        // effectively appending 'real' notes to the score.
        //
        // The steps of this process are:
        //
        // - Check that all motifs in all voices are generated at least until the end of
        //   the committed duration (include the duration appended by this call)
        //
        // - Each newly committed frame will then evaluate its notes based on the associated
        //   motif parameters and considering the scale and progression chord of the frame
        Error commit(Time durationToCommit);

        Error dump(SerializedScore& serializedScore);
        ScoreFrame* getOrCreateFrame(Time relativePosition);
        ScoreFrame* findFrameAtPosition(Time relativePosition);
        ScoreFrame* findLastCommittedFrame();

        NoteValue currentRoot;
        Progression* currentProgression;
        Scale* currentScale;
        Set<Voice*, Voice::compareByID> cachedVoices;
        Set<ScoreFrame*, ScoreFrame::compare_by_position> cached_frames;
    };

} // namespace Dryad
