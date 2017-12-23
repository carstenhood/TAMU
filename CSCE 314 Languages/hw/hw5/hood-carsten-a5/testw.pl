#!/usr/bin/perl
# run all tests; compare output to expected output; mark as "fail" or "pass".

$tested = 0; $succeeded = 0; $failed = 0; $intentional_error = 0;

# test case 1 should succeed
$tested += 1;
$output = `./w factorial.w`;
if( $output eq "result is 120\n" ) {
$succeeded += 1;
}else{
$failed += 1;
}
# test case 2 should succeed
$tested += 1;
$output = `./w empty-example.w`;
if( $output eq "Testing...\n" ) {
$succeeded += 1;
}else{
$failed += 1;
}
# test case 3 is for intentional error
$tested += 1;
$output = `./w factorial-wrong.w 2>&1 1>/dev/null`;
@words = split " ", $output;
if( $words[1] eq "Unused" && $words[4] eq "while") {
$intentional_error += 1;
}else{
$failed += 1;
}

# add more test cases of yours ...

print "$tested tested\n";
print "$succeeded + $intentional_error passed and $failed failed\n";

# to run this script, type the following at the terminal command line prompt
# > perl testw.pl
# then the result should be the following two lines
# 3 tested
# 2 + 1 passed and 0 failed