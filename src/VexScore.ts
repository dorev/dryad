import {
  VexScoreJson,
  VexStaff,
  VF,
} from "./Vex";
import { VexJsonValidator } from "./VexJsonValidator";

// tslint:disable: no-unused-expression



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

  public render(scoreJson: VexScoreJson): string {

    const validationResult: string = VexJsonValidator.validate(scoreJson);
    console.log(validationResult);

    if (validationResult.length !== 0) {
      return validationResult;
    }

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
