"use strict";
var __extends = (this && this.__extends) || (function () {
    var extendStatics = function (d, b) {
        extendStatics = Object.setPrototypeOf ||
            ({ __proto__: [] } instanceof Array && function (d, b) { d.__proto__ = b; }) ||
            function (d, b) { for (var p in b) if (b.hasOwnProperty(p)) d[p] = b[p]; };
        return extendStatics(d, b);
    };
    return function (d, b) {
        extendStatics(d, b);
        function __() { this.constructor = d; }
        d.prototype = b === null ? Object.create(b) : (__.prototype = b.prototype, new __());
    };
})();
Object.defineProperty(exports, "__esModule", { value: true });
var MusicalEvent = (function () {
    function MusicalEvent() {
        this.id = MusicalEvent.nextId++;
    }
    MusicalEvent.nextId = 0;
    return MusicalEvent;
}());
exports.MusicalEvent = MusicalEvent;
var NoteOn = (function (_super) {
    __extends(NoteOn, _super);
    function NoteOn(note) {
        var _this = _super.call(this) || this;
        _this.note = note;
        return _this;
    }
    return NoteOn;
}(MusicalEvent));
exports.NoteOn = NoteOn;
var NoteOff = (function (_super) {
    __extends(NoteOff, _super);
    function NoteOff(noteRef) {
        var _this = _super.call(this) || this;
        _this.noteRef = noteRef;
        return _this;
    }
    return NoteOff;
}(MusicalEvent));
exports.NoteOff = NoteOff;
//# sourceMappingURL=Events.js.map