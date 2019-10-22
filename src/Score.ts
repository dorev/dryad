import { Beat } from "./Beat";
import { MusicalEvent } from "./MusicalEvent";
import { ScorePosition } from "./ScorePosition";

export interface ScoreSearchResult {
    found: boolean;
    index: number;
}

export interface ScoreEvent {
    event: MusicalEvent;
    beat: Beat;
}

export class Score {

    public score: ScorePosition[] = [];

    constructor() {
        //
    }

    public addEvent(scoreEvent: ScoreEvent): void {

        if (this.score.length === 0) {
            this.score.push(new ScorePosition(scoreEvent));
        }

        const searchResult: ScoreSearchResult = this.findBeat(scoreEvent.beat);

        if (searchResult.found) {
            // add the event to that position
            this.score[searchResult.index].events.push(scoreEvent.event);
        } else {
            // insert new position with event in it
            this.score.splice(searchResult.index, 0, new ScorePosition(scoreEvent));
        }
    }

    // Assumes that this.score is already sorted
    // Returns the index of identical beat
    // or the index of the beat which would be right after
    public findBeat(beat: Beat): ScoreSearchResult {
        let i = 0;
        for (; i < this.score.length; ++i) {

            const compare = Beat.compare(beat, this.score[i].beat);
            if (compare === 0) {
                return {found : true, index: i};
            } else if (compare === -1) { break; }
        }
        return {found : false, index: i};
    }
}
