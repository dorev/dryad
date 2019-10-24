enum EventType {
  Empty,
  NoteOn,
  NoteOff,
  TimeSignature,
  Tempo,
}

export class MusicalEvent {
  private static nextId: number = 0;
  public readonly id: number;
  public readonly type: EventType;
  public readonly value: any;

  constructor(eventType: EventType = EventType.NoteOn, value: any = 60) {
    this.id = MusicalEvent.nextId++;
    this.type = eventType;
    this.value = value;
  }
}
