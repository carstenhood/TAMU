var n = 10;

var fib = 0;
if (n == 1) {
    fib = 1;
} else {
    var v1 = 0;
    var v2 = 1;
    var i = 1;
    while (i < n) {
        fib = v1 + v2;
        v1 = v2;
        v2 = fib;
        i = i + 1;
    }
}

print "result is ";
print fib;
print "\n";
