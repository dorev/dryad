import { VexScoreJson } from "./Vex";

export class VexJsonValidator {

  // Returns "valid" or an indication
  // of what is missing in the object
  public static validate(scoreJson: VexScoreJson) {

    // validate score json
    for (const prop of ["staves", "stavesConnections"]) {

      if (!scoreJson.hasOwnProperty(prop)) {
        return `main object missing ${prop} property`;
      }
    }

    // validate staff json
    for (let staff = 0; staff < scoreJson.staves.length; ++staff) {

      for (const prop of ["clef", "timesig", "voices"]) {
        if (!scoreJson.staves[staff].hasOwnProperty(prop)) {
          return `staff ${staff + 1} has no ${prop}`;
        }
      }
    }

    // validate staff connection json
    for (let connection = 0; connection < scoreJson.stavesConnections.length; ++connection) {

      for (const prop of ["connect", "type"]) {
        if (!scoreJson.stavesConnections[connection].hasOwnProperty(prop)) {
          return `connection ${connection + 1} has no ${prop}`;
        }
      }
    }
    // validate staff sub-objects
    for (let staff = 0; staff < scoreJson.staves.length; ++staff) {

      const currentStaff = scoreJson.staves[staff];

      // validate voices
      for (let voice = 0; voice < currentStaff.voices.length; ++voice) {

        for ( const prop of ["tickables"]) {
          const currentVoice = currentStaff.voices[voice];

          if (!currentVoice.hasOwnProperty(prop)) {
            return `voice ${voice + 1} of staff ${staff + 1} has no ${prop}`;
          }

          // validate tickables
          if (prop === "tickables") {
            for (let tickable = 0; tickable < currentVoice.tickables.length; ++tickable) {

              const currentTickable = currentVoice.tickables[tickable];

              for (const tickableProp of ["type", "data"]) {
                if (!currentTickable.hasOwnProperty(tickableProp)) {
                  return `tickable ${tickable + 1} of voice ${voice + 1} in staff ${staff + 1} has no ${tickableProp + 1}`;
                }
              }
            }
          } // end of tickable validation
        } // end of for voice validation
      } // end of voices and tickables validation
    } // end of staves validation

    return "valid";
  }

}
