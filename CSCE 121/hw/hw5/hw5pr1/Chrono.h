//
//  Header.h
//  hw5pr1
//
//  Created by Carsten Hood on 10/2/13.
//  Copyright (c) 2013 Carsten Hood. All rights reserved.
//

#ifndef hw5pr1_Chrono_h
#define hw5pr1_Chrono_h

using namespace std;

// Date Class
namespace Chrono {
    
    //------------------------------------------------------------------------------
    
    class Date {
    public:
        enum Month {
            jan=1, feb, mar, apr, may, jun, jul, aug, sep, oct, nov, dec
        };
        
        class Invalid { };                 // to throw as exception
        
        Date(int y, Month m, int d);       // check for valid date and initialize
        Date();                            // default constructor
        // the default copy operations are fine
        
        // non-modifying operations:
        int   day()   const { return d; }
        Month month() const { return m; }
        int   year()  const { return y; }
        
        // modifying operations:
        void add_day(int n);
        void add_month(int n);
        void add_year(int n);
    private:
        int   y;
        Month m;
        int   d;
    };
    
    //------------------------------------------------------------------------------
    
    bool is_date(int y, Date::Month m, int d); // true for valid date
    
    //------------------------------------------------------------------------------
    
    bool leapyear(int y);                  // true if y is a leap year
    
    //------------------------------------------------------------------------------
    
    bool operator==(const Date& a, const Date& b);
    bool operator!=(const Date& a, const Date& b);
    bool operator<=(const Date& a, const Date& b);
    bool operator>=(const Date& a, const Date& b);
    
    //------------------------------------------------------------------------------
    
    ostream& operator<<(ostream& os, const Date& d);
    istream& operator>>(istream& is, Date& dd);
    
    //------------------------------------------------------------------------------
    
}

// Chrono
//------------------------------------------------------------------------------
namespace Chrono {
    
    Date::Date(int yy, Month mm, int dd)
    : y(yy), m(mm), d(dd) {
        if (!is_date(yy,mm,dd)) throw Invalid();
    }
    
    const Date& default_date() {
        static const Date dd(2014,Date::jan,1); // start of 21st century
        return dd;
    }
    
    Date::Date()
    :y(default_date().year()),
    m(default_date().month()),
    d(default_date().day()) {
    }
    
    void Date::add_year(int n) {
        if (m==feb && d==29 && !leapyear(y+n)) { // beware of leap years!
            m = mar;        // use March 1 instead of February 29
            d = 1;
        }
        y+=n;
    }
    
    bool is_date(int y, Date::Month  m, int d) {
        if (y!=2014) return false;
        
        if (d<=0) return false;            // d must be positive
        
        int days_in_month = 31;            // most months have 31 days
        switch (m) {
            case Date::feb:                        // the length of February varies
                days_in_month = (leapyear(y))?29:28;
                break;
            case Date::apr: case Date::jun: case Date::sep: case Date::nov:
                days_in_month = 30;                // the rest have 30 days
                break;
        }
        if (d>days_in_month) return false;
        
        return true;
    }
    
    bool leapyear(int y) {
        if (y%4==0) {
            return true;
        }
        return false;
    }
    
    //operators
    bool operator==(const Date& a, const Date& b) {
        return a.year()==b.year()
        && a.month()==b.month()
        && a.day()==b.day();
    }

    bool operator!=(const Date& a, const Date& b) {
        return !(a==b);
    }
    
    bool operator<=(const Date& a, const Date& b) {
        bool less = true;
        if (a.year()>b.year()) { less = false; }
        else if (a.year()==b.year()) {
            if (a.month()>b.month()) { less = false; }
            else if (a.month()==b.month()) {
                if (a.day()>b.day()) { less = false; }
            }
        }
        return less;
    }
    
    bool operator>=(const Date& a, const Date& b) {
        return !(a<=b) || a==b;
    }
    
    ostream& operator<<(ostream& os, const Date& d) {
        return os << '(' << d.year()
        << ',' << d.month()
        << ',' << d.day()
        << ')';
    }
    
    istream& operator>>(istream& is, Date& dd) {
        int y, m, d;
        char ch1, ch2, ch3, ch4;
        is >> ch1 >> y >> ch2 >> m >> ch3 >> d >> ch4;
        if (!is) return is;
        if (ch1!='(' || ch2!=',' || ch3!=',' || ch4!=')') { // oops: format error
            is.clear(ios_base::failbit);                    // set the fail bit
            return is;
        }
        dd = Date(y,Date::Month(m),d);     // update dd
        return is;
    }
    
    enum Day {
        sunday, monday, tuesday, wednesday, thursday, friday, saturday
    };
} // Chrono
//-----------------------------------------------------------------------------


#endif
