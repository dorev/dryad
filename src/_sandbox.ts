/*
"connect": [0,1],
*/

const testJson = JSON.parse(`{
  "stavesConnections" : [
    {
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
`);

const result: string = validateJson(testJson as VexScoreJson);


const r2: string = result;




interface VexStaffConnectionJson { connect: number[]; type: string; }
interface VexTickableJson { type: string; data: string[]; }
interface VexVoiceJson { tickables: VexTickableJson[]; }
interface VexStaffJson { clef: string; timesig: string; voices: VexVoiceJson[]; }

export interface VexScoreJson {
  stavesConnections: VexStaffConnectionJson[];
  staves: VexStaffJson[];
}

function validateJson(scoreJson: VexScoreJson): string {

    // validate score json
    for (const prop of ["staves", "stavesConnections"]) {

      if (!scoreJson.hasOwnProperty(prop)) {
        return `missing ${prop} property`;
      }
    }

    // validate staff json
    for (let staff = 0; staff < scoreJson.staves.length; ++staff) {

      for (const prop of ["clef", "timesig", "voices"]) {
        if (!scoreJson.staves[staff].hasOwnProperty(prop)) {
          return `staff ${staff} has no ${prop}`;
        }
      }
    }

    // validate staff connection json
    for (let connection = 0; connection < scoreJson.stavesConnections.length; ++connection) {

      for (const prop of ["connect", "type"]) { // properties to inspect
        if (!scoreJson.stavesConnections[connection].hasOwnProperty(prop)) {
          return `connection ${connection} has no ${prop}`;
        }
      }
    }
    // validate staff sub-objects
    for (let staff = 0; staff < scoreJson.staves.length; ++staff) {

      const currentStaff = scoreJson.staves[staff];

      // validate voices
      for (let voice = 0; voice < currentStaff.voices.length; ++voice) {

        for ( const prop of ["tickables"]) { // properties to inspect

        const currentVoice = currentStaff.voices[voice];

        if (!currentVoice.hasOwnProperty(prop)) {
          return `voice ${voice} has no ${prop}`;
        }

        // validate tickables
        if (prop === "tickables") {
            for (let tickable = 0; tickable < currentVoice.tickables.length; ++tickable) {

              const currentTickable = currentVoice.tickables[tickable];

              for (const tickableProp of ["type", "data"]) {
                if (!currentTickable.hasOwnProperty(tickableProp)) {
                  return `tickable ${tickable} of voice ${voice} in staff ${staff} has no ${tickableProp}`;
                }
              }
            }
          } // end of tickable validation

        }

      } // end of voices validation

    } // end of staves validation

    return "valid";

}


