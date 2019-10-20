import { Beat } from "./Beat";
import { InstantAddress } from "./Instant";
import { Note } from "./Note";

export class MusicalEvent {
  private static nextId: number = 0;
  public readonly id: number;

  constructor() {
    this.id = MusicalEvent.nextId++;
  }
}

export class NoteOn extends MusicalEvent {
  public readonly note: Note;

  constructor(note: Note) {
    super();
    this.note = note;
  }
}

export class NoteOff extends MusicalEvent {
  public readonly noteRef: InstantAddress;

  constructor(noteRef: InstantAddress) {
    super();
    this.noteRef = noteRef;
  }
}

export interface EventData {
  event: MusicalEvent;
  beat: Beat;
}
