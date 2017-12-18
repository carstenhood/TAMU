#include "std_lib_facilities_3.h"

int main() {
    string valids [] = {"Mon", "Monday", "Tues", "Tuesday", "Wed", "Wednesday", "Thurs", "Thursday", "Fri", "Friday", "Sat", "Saturday", "Sun", "Sunday"};
    cout << "Enter weekdays and values. Terminate with a lone period \" . \".\n";
    
    string temp_day;
    int temp_num;
    vector<string> days;
    vector<int> nums;
    
    int rejects=0;
    int sum=0;
    
    while (cin>>temp_day) {
        if (temp_day==".") {
            break;
        }
        bool invalid = true;
        for (int i=0; i<sizeof(valids); i++) {
            if (temp_day == valids[i]) {
                invalid=false;
                break;
            }
        }
        cin>>temp_num;
        if (!cin || invalid) {
            rejects++;
        }
        else {
            days.push_back(temp_day);
            nums.push_back(temp_num);
        }
    }
    for (int i=0; i<nums.size(); i++) {
        cout << days[i] << ", " << nums[i] << "; ";
        sum += nums[i];
    }
    cout << "\nRejected entries: " << rejects;
    cout << "\nSum of values: " << sum;
    
    return 0;
}

