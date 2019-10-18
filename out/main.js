"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
var Beat_1 = require("./Beat");
var beat1 = new Beat_1.Beat(1, 3, 4);
var beat2 = new Beat_1.Beat(2, 5, 6);
var beat = beat1.add(beat2);
var num = beat.num;
var num2 = num + 2;
//# sourceMappingURL=main.js.map