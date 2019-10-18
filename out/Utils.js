"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
function reduceFraction(fraction) {
    var divisor = fraction.denom;
    for (var i = 1; i <= fraction.denom; ++i) {
        if (fraction.num % i === 0 && fraction.denom % i === 0) {
            divisor = i;
        }
    }
    return { num: fraction.num / divisor, denom: fraction.denom / divisor };
}
exports.reduceFraction = reduceFraction;
//# sourceMappingURL=Utils.js.map