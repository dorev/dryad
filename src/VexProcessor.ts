import * as Vex from "vexflow";
// tslint:disable: no-unused-expression

export const VF = Vex.Flow;

interface VexStaffConnectionJson { connect: number[]; type: string; }
interface VexTickableJson { tickableType: string; data: string[]; }
type VexVoiceJson = VexTickableJson[];
type VexStaffJson = VexVoiceJson[];

export interface VexScoreJson {
  stavesConnections: VexStaffConnectionJson[];
  staves: VexStaffJson[];
}

export class VexScore {

  private staves: VexStaff[];

  constructor(
    hostElement: HTMLElement,
    renderingWidth: number,
    renderingHeight: number,
    scoreWidth: number) {
    //
    /*
    const div = document.querySelector("#vex") as HTMLElement;
    const renderer = new VF.Renderer(div, VF.Renderer.Backends.SVG);

    // Size our svg:
    renderer.resize(420, 200);

    // And get a drawing context:
    const context = renderer.getContext();
    */
  }

  public render(scoreJson: VexScoreJson): void  {

    // draw all staves
    // draw beams
    // draw all connections

  }

  private addStave(stave: VexStaff): void {
    this.staves.push(stave);
  }

  private connectStaves(firstStaveId: number, secondStaveId: number, connectionType: string): void {
    //
  }
}

enum Tickable {
  StaveNote,
  TimeSigNote,
}

class VexTickable {
  public type: Tickable;
  public data: any[];
}

class VexVoice {

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

class VexStaff {
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
