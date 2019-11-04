import { VexScoreJson, VF } from "./Vex";
import { VexJsonValidator } from "./VexJsonValidator";

export class VexScore {

  public renderingWidth: number;
  public renderingHeight: number;
  public scoreWidth: number;
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
    this.context = this.renderer.getContext();
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
    const staves: Vex.Flow.Stave[] = [];

    for ( const jsonStaff of scoreJson.staves) {
      const voices: Vex.Flow.Voice[] = [];

      for (const jsonVoice of jsonStaff.voices) {
        const tickables: Vex.Flow.Note[] = [];

        for (const jsonTickable of jsonVoice.tickables) {
          // Create  Vex.Flow.Note instances
          const data = jsonTickable.data;

          switch (jsonTickable.type) {
            case "StaveNote" :
                tickables.push(new VF.StaveNote({
                  clef: data[0],
                  keys: data[1].split(",").map(Function.prototype.call, String.prototype.trim), // trim js black magic
                  duration: data[2],
                }));
                break;

            case "TimeSigNote" :
              const customPadding: number = parseInt(data[1]) || 0;
              tickables.push(new VF.TimeSigNote(data[0], customPadding));
              break;
          }
          voices.push(new VF.Voice({}).addTickables(tickables));
        }
        const formatter = new VF.Formatter().joinVoices(voices).format(voices, this.scoreWidth);
      }

      // Render staff
      const staff = new VF.Stave(0, 0, this.renderingWidth);
      staff.addClef(jsonStaff.clef).addTimeSignature(jsonStaff.timesig);
      staff.setContext(this.context).draw();

      // Render voice
      for (const voice of voices) {
        voice.draw(this.context, staff);
      }

      staves.push(staff);
    }

    // Bind staves

    // Finish score rendering

    // draw beams
    // draw all connections

    return "all good";
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
