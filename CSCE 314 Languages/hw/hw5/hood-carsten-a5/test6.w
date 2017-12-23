// double loop to test if and while statements, scope, and complex operations

var counter = 0;
var i = 0;
var n = 10;
while (i != n) {
    i = i + 1;
    var i = 0;    // test var with same name in new scope
    var n = 10;
    while (i < n) {
        i = i + 1;
        if (i == n) {
            counter = counter + i;
        } else {
            // test empty block
        }
    }
}

print counter;
