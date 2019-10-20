import { Beat } from "./Beat";
import { MusicalEvent } from "./Events";
import { Note } from "./Note";

export interface InstantAddress {
  offset: Beat;
  id: number;
}

export class Instant {
  public readonly beat: Beat;
  public events: MusicalEvent[] = [];
  public notesResonating: Note[] = [];

  constructor(event: MusicalEvent, beat: Beat) {
    this.events.push(event);
    this.beat = beat;
  }
}
