import * as Vex from "vexflow";

export const VF = Vex.Flow;

export interface VexStaffConnectionJson { connect: number[]; type: string; }
export interface VexTickableJson { type: string; data: string[]; }
export interface VexVoiceJson { tickables: VexTickableJson[]; }
export interface VexStaffJson { clef: string; timesig: string; voices: VexVoiceJson[]; }

export interface VexScoreJson {
  stavesConnections: VexStaffConnectionJson[];
  staves: VexStaffJson[];
}


export enum Tickable {
  StaveNote,
  TimeSigNote,
}

export class VexTickable {
  public type: Tickable;
  public data: any[];
}

export class VexVoice {
  public tickables: VexTickable[];
  constructor(voiceJson: VexVoiceJson) {
    //
  }

  public getVoice() {

    // ???? NEEDED ????
    // Create a voice in 4/4 and add above notes
    const voice = new VF.Voice({num_beats: 4,  beat_value: 4});
    //
    //
    const vexTickables: Vex.Flow.Note[] = [];

    for (const tickable of this.tickables) {
      switch (tickable.type) {
        case Tickable.StaveNote :
          vexTickables.push(new VF.StaveNote({
              clef: tickable.data[0],
              keys: tickable.data[1],
              duration: tickable.data[2],
            }));
          break;

        case Tickable.TimeSigNote :
          vexTickables.push(new VF.TimeSigNote(tickable.data[0], 0));
          break;
      }
    }
    return voice.addTickables(vexTickables);
  }
}

export class VexStaff {
  public timeSignature: string;
  public clef: string;
  public text: string;
  public voices: Vex.Flow.Voice[];

  constructor(staffJson: VexStaffJson) {
    //
  }

  public format(width: number) {
    new VF.Formatter().joinVoices(this.voices).format(this.voices, width);
  }
}
