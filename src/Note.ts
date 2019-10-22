import { Beat } from "./Beat";

export class Note {
  public readonly value: number;
  public readonly duration: Beat;

  constructor(value: number, duration: Beat = new Beat(1)) {
    this.value = value;
    this.duration = duration;
  }
}
