import { Beat } from "./Beat";
import { MusicalEvent } from "./Events";
import { Note } from "./Note";

export interface InstantAddress {
  offset: Beat;
  id: number;
}

export class Instant {
  public readonly offset: Beat;
  public events: MusicalEvent[];
  public notesResonating: Note[];

  constructor(...events: MusicalEvent[]) {
    this.events = [...this.events, ...events];
  }
}
