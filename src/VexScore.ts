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

    if (validationResult.length !== 0) {
      return validationResult;
    }

    // Load JSON into objects
    const staves: Vex.Flow.Stave[] = [];

    for ( const staveJson of scoreJson.staves) {
      const voices: Vex.Flow.Voice[] = [];

      for (const voiceJson of staveJson.voices) {
        const tickables: Vex.Flow.Note[] = [];

        for (const tickableJson of voiceJson.tickables) {
          // Create  Vex.Flow.Note instances
          const data = tickableJson.data;

          switch (tickableJson.type) {
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
            default :
              throw new Error(`Not Implemented >>> tickable type : ${tickableJson.type} `);
          }

          voices.push(new VF.Voice({}).addTickables(tickables));

          // Generate beams
          const beams: Vex.Flow.Beam[] = VF.Beam.generateBeams(
            tickables.filter((t) => t instanceof Vex.Flow.StemmableNote) as Vex.Flow.StemmableNote[]);

          for (const beam of beams) {
            beam.setContext(this.context).draw();
          }

        }
        const formatter = new VF.Formatter().joinVoices(voices).format(voices, this.scoreWidth);
      }

      // Render stave
      const stave = new VF.Stave(0, 0, this.renderingWidth);
      stave.addClef(staveJson.clef).addTimeSignature(staveJson.timesig);
      stave.setContext(this.context);
      staves.push(stave);
      stave.draw();

      // Render voices
      for (const voice of voices) {
        voice.draw(this.context, stave);
      }
    }

    for (const connectionJson of scoreJson.stavesConnections) {
      const connection = new VF.StaveConnector(
        staves[connectionJson.connect[0]],
        staves[connectionJson.connect[1]]);

      let connectionType: Vex.Flow.StaveConnector.type = null;

      switch (connectionJson.type) {
        case "single"  : connectionType = VF.StaveConnector.type.SINGLE; break;
        case "double"  : connectionType = VF.StaveConnector.type.DOUBLE; break;
        case "bracket" : connectionType = VF.StaveConnector.type.BRACKET; break;
        case "brace"   : connectionType = VF.StaveConnector.type.BRACE; break;
        default :
          throw new Error(`Not Implemented >>> stave connector type : ${connectionJson.type} `);
      }

      connection.setType(connectionType);
      connection.draw();
    }


    // Finish score rendering

    // draw beams

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
