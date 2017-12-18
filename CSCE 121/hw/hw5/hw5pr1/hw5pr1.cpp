
#include "std_lib_facilities_3.h"
#include "Chrono.h"

//classes for handling errors
class Unavailable { };
class Bad_Date { };

struct Reservation {
    string cust_name;
    int room;
    Chrono::Date start_day;
    Chrono::Date end_day;
    
    Reservation(string cn, int r, Chrono::Date sd, Chrono::Date ed) :
    cust_name(cn), room(r), start_day(sd), end_day(ed) {};
};

class Hotel {
    string name;
    int num_rooms;
    bool has_pool;
    vector<Reservation> bookings;
    
    int find_available_room(Chrono::Date sd, Chrono::Date ed) {
        vector<int> taken_rooms;
        for (int k=1; k<num_rooms+1; ++k) {
            bool booked = false;
            for (int i=0; i<bookings.size(); ++i) {
                int room_num = bookings[i].room;
                if (k==room_num) {
                    if (((sd<=bookings[i].end_day && sd>=bookings[i].start_day)
                            || (ed>=bookings[i].start_day && ed<=bookings[i].end_day))
                        || ((bookings[i].end_day>=sd && bookings[i].end_day<=ed)
                            || (bookings[i].start_day>=sd && bookings[i].start_day<=ed))) {
                        booked = true;
                        break;
                    }
                }
            }
            if (!booked) { return k; }
        }
        throw Unavailable();
    }
    
 public:
    //getter functions
    string get_name() {
        return name;
    }
    int get_rooms() {
        return num_rooms;
    }
    int get_bookings() {
        return bookings.size();
    }
    
    //other public functions
    Hotel(string nm, int nr, bool pool) :
        name(nm), num_rooms(nr), has_pool(pool) {}
    
    void make_reservation(string cn, Chrono::Date sd, Chrono::Date ed) {
        int room = find_available_room(sd, ed);
        Reservation res(cn, room, sd, ed);
        bookings.push_back(res);
    }
};

//gets the user to input a valid start or end date and handles errors
Chrono::Date input_date(string which_date) {
    while (true) {
        cout << "Enter a valid " << which_date << " for your 2014 reservation: ";
        string input;
        getline(cin, input);
        stringstream line(input);
        try {
            char separator;
            int y, d, m_num;
            Chrono::Date::Month m;
            line >> m_num; if(!line) throw Bad_Date();
            line >> separator;
            line >> d; if(!line) throw Bad_Date();
            line >> separator;
            line >> y; if(!line) throw Bad_Date();
            switch (m_num) { //use the enumerated months in the date class
                case 1: m = Chrono::Date::jan; break;
                case 2: m = Chrono::Date::feb; break;
                case 3: m = Chrono::Date::mar; break;
                case 4: m = Chrono::Date::apr; break;
                case 5: m = Chrono::Date::may; break;
                case 6: m = Chrono::Date::jun; break;
                case 7: m = Chrono::Date::jul; break;
                case 8: m = Chrono::Date::aug; break;
                case 9: m = Chrono::Date::sep; break;
                case 10: m = Chrono::Date::oct; break;
                case 11: m = Chrono::Date::nov; break;
                case 12: m = Chrono::Date::dec; break;
                default: throw Bad_Date();
            }
            return Chrono::Date(y,m,d);
        } catch (Bad_Date) {
            cout << "Invalid date format. Try again.\n";
        } catch (Chrono::Date::Invalid) {
            cout << "Invalid date. Try again.\n";
        }
    }
}

string input_reservation_name() {
    cout << "Enter a name for your reservation: ";
    string reservation_name;
    getline(cin, reservation_name);
    return reservation_name;
}

void choose_reservation(Hotel &hotel) {
    cout << "\nEnter your desired dates.\n- Dates follow the format \"mm,dd,2014\". Separate values with any char. ('/', '\\', '.', etc.)\n- Separate reservations cannot start and end on the same day, to allow time for the rooms to be cleaned.\n";
    while (true) {
        try {
            Chrono::Date start_date = input_date("START date");
            Chrono::Date end_date = input_date("END date");
            if (start_date>=end_date) { throw Bad_Date(); } //end date should come after the start date
            string reservation_name = input_reservation_name();
            hotel.make_reservation(reservation_name, start_date, end_date);
            cout << "\nYour reservation \"" << reservation_name << "\" has been booked.\n\n";
            return; //reservation successful, return to main
        } catch (Chrono::Date::Invalid) {
            cout << "\nInvalid date. Try again.\n";
        } catch (Unavailable) {
            cout << "\nNo rooms available at " << hotel.get_name() << " on those dates. Please allow a day between reservations. Try again.\n";
        } catch (Bad_Date) {
            cout << "\nEnd dates must come after start dates. Please try again.\n";
        }
    }
}

int choose_hotel(vector<Hotel> &hotels) {
    while (true) {
        cout << "\nAvailable hotels:\n"; //display hotels and number of rooms & bookings
        for (int i=0; i<hotels.size(); i++) {
            Hotel &h = hotels[i];
            string ending = "s, "; //adjusts for singular or plural # of bookings
            if (h.get_bookings()==1) { ending=", "; }
            cout << h.get_name()
                << " (" << h.get_bookings() << " booking" << ending << h.get_rooms() << " rooms)\n";
        }
        
        cout << "\nEnter name of desired hotel: ";
        string hotel_name;
        getline(cin, hotel_name);        
        for (int i=0; i<hotels.size(); i++) {
            if (hotels[i].get_name()==hotel_name) {
                return i;
            }
        }

        cout << "There is no hotel named \"" << hotel_name << "\". Try again.\n";
    }
}

int main() {
    vector<Hotel> hotels; //init hotels and hotel properties in main
    hotels.push_back(Hotel("Hotel Alpha", 8, true));
    hotels.push_back(Hotel("Bob's", 2, true));
    hotels.push_back(Hotel("Southmont Inn", 22, false));
    hotels.push_back(Hotel("The Crag", 4, false));
    
    while (true) {
        try {
            cout << "Book a reservation for the year 2014...\n";
            choose_reservation(hotels[choose_hotel(hotels)]);
        } catch (...) {
            cout << "\nSomething went wrong. Please try again.\n";
        }
    }
    
    return 0;
}
