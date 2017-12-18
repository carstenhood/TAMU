// test comparisons

// the following are all false
var a = 3 >= 7;
var b = 7 <= 3;
var c = 3 > 7;
var d = 7 < 3;
var e = 7 == 3;
var allFalse = a || b || c || d || e;

// the following are all true
var f = 3 <= 7;
var g = 3 < 7;
var h = 7 >= 3;
var i = 7 > 3;
var j = 3 != 7;
var allTrue = f && g && h && i && j;

// logic with integer comparisons
var x = 7;
var y = 3;
var alwaysTrue = (x < y) || (x >= y);

// test boolean equality
var k = (True != False);
var l = (True == False);
var alwaysFalse = k == l;

var comp = !allFalse && allTrue && alwaysTrue && !alwaysFalse;

print comp;