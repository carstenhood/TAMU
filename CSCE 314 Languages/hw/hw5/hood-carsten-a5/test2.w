// test logic

var a = True;
var b = False;

// following are true
var c = a || b;
var d = c && !((a || b) && !(a || b));
var e = !false && !False && true && True && d;

print e;