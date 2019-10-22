import { Beat } from "./Beat";
import { MusicalEvent } from "./MusicalEvent";

declare interface ScoreEvent {
  event: MusicalEvent;
  beat: Beat;
}

export class ScorePosition {
  public readonly beat: Beat;
  public events: MusicalEvent[] = [];
  public notesResonating: number[] = [];

  constructor(scoreEvent: ScoreEvent) {
    this.events.push(scoreEvent.event);
    this.beat = scoreEvent.beat;
  }
}
