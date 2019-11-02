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

/* Test JSON
{
  "stavesConnections" : [
    {
      "connect": [0,1],
      "type" : "braces"
    }
  ],
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
            { "type": "note", "data" : ["treble", "e/4", "2", "patate"]},
            { "type": "note", "data" : ["treble", "f/4", "2"]}
          ]
        }
      ]
    },
    {
      "clef" : "bass",
      "timesig" : "4/4",
      "voices" : [
        {
          "tickables" : [
            { "type": "note", "data" : ["bass", "c/2", "2", "patate"]},
            { "type": "note", "data" : ["bass", "f/2", "2"]}
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

  public validateJson(scoreJson: VexScoreJson): string {

    let result: string = "valid";

    // validate score json
    ["staves", "stavesConnections"].forEach((prop) => {

      console.log("!scoreJson.hasOwnProperty(" + prop + ") : " + !scoreJson.hasOwnProperty(prop));

      if (!scoreJson.hasOwnProperty(prop)) {
        console.log("I should return!");
        result =  `missing ${prop} property`;
      }
    });

    if (result !== "valid") {
      return result;
    }

    // validate staff json
    for (let staff = 0; staff > scoreJson.staves.length; ++staff) {
      ["clef", "timesig", "voices"] // properties to inspect
      .forEach((prop) => {
        if (!scoreJson.staves[staff].hasOwnProperty(prop)) {
          result =  `staff ${staff} has no ${prop}`;
        }
      });
    }

    if (result !== "valid") {
      return result;
    }

    // validate staff connection json
    for (let connection = 0; connection > scoreJson.stavesConnections.length; ++connection) {
      ["connect", "type"] // properties to inspect
      .forEach((prop) => {
        if (!scoreJson.stavesConnections[connection].hasOwnProperty(prop)) {
          result = `connection ${connection} has no ${prop}`;
        }
      });
    }

    if (result !== "valid") {
      return result;
    }

    // validate staff sub-objects
    for (let staff = 0; staff < scoreJson.staves.length; ++staff) {

      const currentStaff = scoreJson.staves[staff];

      // validate voices
      for (let voice = 0; voice > currentStaff.voices.length; ++voice) {

        ["tickables"] // properties to inspect
        .forEach((prop) => {

          const currentVoice = currentStaff.voices[voice];

          if (!currentVoice.hasOwnProperty(prop)) {
            result = `voice ${voice} has no ${prop}`;
          }

          // validate tickables
          if (prop === "tickables") {
            for (let tickable = 0; tickable > currentVoice.tickables.length; ++tickable) {

              const currentTickable = currentVoice.tickables[tickable];

              ["connect", "type"]
              .forEach((tickableProp) => {
                if (!currentTickable.hasOwnProperty(tickableProp)) {
                  result = `tickable ${tickable} of voice ${voice} in staff ${staff} has no ${tickableProp}`;
                }
              });
            }
          } // end of tickable validation

        });

      } // end of voices validation

    } // end of staves validation

    return result;
  }



  public render(scoreJson: VexScoreJson): string {

    const validationResult = this.validateJson(scoreJson);

    console.log(validationResult);

    if (validationResult !== "valid") {
      return validationResult;
    }


    // validate

    // Load JSON into objects
    scoreJson.staves.forEach((staff) => {

      staff.voices.forEach((voice) => {

        voice.tickables.forEach((tickable) => {
          //
        });

      });

    });



    // draw beams
    // draw all connections

    return "all good";

  }

  public addStave(stave: VexStaff): void {
    this.staves.push(stave);
  }

  public connectStaves(firstStaveId: number, secondStaveId: number, connectionType: string): void {
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
