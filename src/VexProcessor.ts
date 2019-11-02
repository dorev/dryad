import * as Vex from "vexflow";
// tslint:disable: no-unused-expression


export const VF = Vex.Flow;

interface VexStaffConnectionJson { connect: number[]; type: string; }
interface VexTickableJson { type: string; data: string[]; }
interface VexVoiceJson { tickables: VexTickableJson[]; }
interface VexStaffJson { clef: string; timesig: string; voices: VexVoiceJson[]; }

export interface VexScoreJson {
  stavesConnections: VexStaffConnectionJson[];
  staves: VexStaffJson[];
}
/*
{
  "staveConnections" : [],
  "staves" : [
    {
      "clef" : "treble",
      "timesig" : "4/4",
      "voices" : [
        {
          "tickables" : [
            { "type": "note", "data" : ["treble", "c/4,e/4,g/4", "4"]},
            { "type": "note", "data" : ["treble", "f/4,a/4,c/5", "2"]},
            { "type": "note", "data" : ["treble", "c/4,e/4,g/4", "4"]}
          ]
        },
        {
          "tickables" : [
            { "type": "note", "data" : ["treble", "c/2", "2"]},
            { "type": "note", "data" : ["treble", "f/2", "2"]}
          ]
        }
      ]
    }
  ]
}
*/

export class VexScore {

  public renderingWidth: number;
  public renderingHeight: number;
  public scoreWidth: number;
  private staves: VexStaff[];
  private renderer: Vex.Flow.Renderer;
  private context: Vex.IRenderContext;

  constructor(
    hostElement: HTMLElement,
    renderingWidth: number,
    renderingHeight: number,
    scoreWidth: number) {

    this.renderingWidth = renderingWidth;
    this.renderingHeight = renderingHeight;
    this.scoreWidth = scoreWidth;

    this.renderer = new VF.Renderer(hostElement, VF.Renderer.Backends.SVG);
    this.renderer.resize(this.renderingWidth, this.renderingHeight);
    const context = this.renderer.getContext();
  }

  public resizeRenderer(renderingWidth: number, renderingHeight: number): void {
    this.renderer.resize(renderingWidth, renderingHeight);
    this.renderingWidth = renderingWidth;
    this.renderingHeight = renderingHeight;
  }

  public render(scoreJson: VexScoreJson): string  {

    // draw all staves
    if (!scoreJson.hasOwnProperty("staves")) {
      return "no staves";
    }

    scoreJson.staves.forEach((staff) => {

    });



    // draw beams
    // draw all connections

    return "all good";

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
