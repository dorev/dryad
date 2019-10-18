import { Beat, Note } from "./BasicElements";

export interface InstantAddress {
  offset: Beat;
  id: number;
}

export class Instant {
  public readonly offset: Beat;
  public events: MusicalEvent[];
  public notesResonating: Note[];
}

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
