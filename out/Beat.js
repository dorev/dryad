"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
var Utils_1 = require("./Utils");
var Beat = (function () {
    function Beat(beat, num, denom) {
        if (num === void 0) { num = null; }
        if (denom === void 0) { denom = null; }
        this.beat = 0;
        this.num = null;
        this.denom = null;
        if (beat < 0 || beat === null || beat === undefined) {
            throw new Error("Beat value must be a positive integer");
        }
        this.beat = beat;
        if (!num || num === denom) {
            this.num = null;
            this.denom = null;
            return;
        }
        else if (!denom) {
            throw new Error("Beat fraction denominator cannot be null with non-null numerator");
        }
        this.num = num;
        this.denom = denom;
    }
    Beat.prototype.add = function (other) {
        var beat = 0;
        var num = 0;
        if (other.denom === null && this.denom === null) {
            return new Beat(this.beat + other.beat);
        }
        if (!this.denom) {
            this.denom = other.denom;
        }
        else if (!other.denom) {
            other.denom = this.denom;
        }
        if (this.denom === other.denom) {
            beat = this.beat + other.beat + Math.floor((this.num + other.num) / this.denom);
            num = ((this.num + other.num) / this.denom) > 1
                ? (this.num + other.num) % this.denom
                : this.num + other.num;
            return new Beat(beat, num, this.denom);
        }
        else {
            var beatFraction = Utils_1.reduceFraction({
                denom: this.denom === other.denom ? this.denom : this.denom * other.denom,
                num: this.denom === other.denom ? this.num + other.num : this.num * other.denom + other.num * this.denom,
            });
            beat = this.beat + other.beat + Math.floor(beatFraction.num / beatFraction.denom);
            num = (beatFraction.num / beatFraction.denom) > 1 ? beatFraction.num % this.denom : beatFraction.num;
            return new Beat(beat, num, beatFraction.denom);
        }
    };
    Beat.prototype.substract = function (other) {
        var beat = 0;
        var num = 0;
        if (other.denom === null && this.denom === null) {
            return new Beat(this.beat - other.beat);
        }
        if (!this.denom) {
            this.denom = other.denom;
        }
        else if (!other.denom) {
            other.denom = this.denom;
        }
        if (this.denom === other.denom) {
            beat = this.beat - other.beat + Math.floor((this.num - other.num) / this.denom);
            num = ((this.num - other.num) / this.denom) < 0
                ? (this.num - other.num) % this.denom
                : this.num - other.num;
            return new Beat(beat, num, this.denom);
        }
        else {
            var beatFraction = Utils_1.reduceFraction({
                denom: this.denom === other.denom ? this.denom : this.denom * other.denom,
                num: this.denom === other.denom ? this.num - other.num : this.num * other.denom - other.num * this.denom,
            });
            beat = this.beat - other.beat - Math.floor(beatFraction.num / beatFraction.denom);
            num = Math.floor(beatFraction.num / beatFraction.denom) < 0 ? beatFraction.num % this.denom : beatFraction.num;
            return new Beat(beat, num, beatFraction.denom);
        }
    };
    return Beat;
}());
exports.Beat = Beat;
//# sourceMappingURL=Beat.js.map