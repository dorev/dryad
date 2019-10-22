enum EventType {
  Invalid,
  NoteOn,
  NoteOff,
}

export class MusicalEvent {
  private static nextId: number = 0;
  public readonly id: number;
  public readonly type: EventType;
  public readonly value: any;

  constructor(eventType: EventType = EventType.Invalid, value?: any) {
    this.id = MusicalEvent.nextId++;
    this.type = eventType;
    this.value = value;
  }
}
