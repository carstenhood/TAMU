//hw4 pr1
//Carsten Hood
//movie file in and out

//http://www.cplusplus.com/forum/beginner/48568/

#include "std_lib_facilities_3.h"

struct movie {
    string title;
    int year;
    vector<string> actors;
    string genre;
    vector<double> scores;
};
//load information from file
void read(const string file_name, vector<movie> &movies) {
    ifstream in;
    in.open(file_name);
    string input;
    vector<movie> movies_read;
    while (true) {
        getline(in, input);
        if (input != "Start") break;
        movie m;
        
        getline(in, m.title);
        
        getline(in, input);
        stringstream year_stream(input);
        year_stream >> m.year;
        
        vector<string> actors;
        while (true) {
            getline(in, input);
            if (input == "") break;
            actors.push_back(input);
        }
        m.actors = actors;
        
        getline(in, m.genre);
        
        vector<double> scores;
        while (true) {
            double score;
            getline(in, input);
            if (input == "") break;
            stringstream score_stream(input);
            score_stream >> score;
            scores.push_back(score);
        }
        m.scores = scores;
        
        movies_read.push_back(m);
    }
    movies = movies_read;
}
//save new information to file
void write(const string file_name, vector<movie> &movies) {
    ofstream out;
    out.open(file_name);
    for (int i=0; i<movies.size(); ++i) {
        out << "Start\n";
        movie m = movies[i];
        out << m.title << "\n";
        out << m.year << "\n";
        for (int k=0; k<m.actors.size(); ++k) {
            out << m.actors[k] << "\n";
        }
        out << "\n";
        out << m.genre << "\n";
        for (int k=0; k<m.scores.size(); ++k) {
            out << m.scores[k] << "\n";
        }
        out << "\n";
    }
}
//methods for updating/entering movie information
void set_title(movie &m) {
    while (true) {
        cout << "Title: ";
        getline (cin,m.title);
        if (m.title != "") break;
        cout << "Try again. ";
    }
}
void set_year(movie &m) {
    while (true) {
        string input;
        int new_year;
        cout << "Year: ";
        getline(cin, input);
        stringstream line(input);
        if (line >> new_year && (1900<new_year && new_year<=2020)) {
            m.year=new_year;
            break;
        }
        cout << "Enter year between 1900 and 2020. ";
    }
}
void set_actors(movie &m) {
    vector<string> new_actors;
    cout << "ACTORS (on separate lines, enter nothing when finished)\n";
    while (true) {
        cout << "Add actor: ";
        string new_actor;
        getline (cin,new_actor);
        if (new_actor == "") {
            break;
        }
        new_actors.push_back(new_actor);
    }
    m.actors = new_actors;
}
void set_genre(movie &m) {
    while (true) {
        cout << "Set genre: ";
        getline (cin,m.genre);
        if (m.genre != "") break;
        cout << "Try again. ";
    }
}
void set_scores(movie &m) {
    vector<double> new_scores;
    cout << "SCORES (on separate lines, enter nothing when finished)\n";
    while (true) {
        cout << "Add score: ";
        string input;
        getline(cin, input);
        stringstream line(input);
        if (input=="") {
            m.scores = new_scores;
            break;
        }
        double new_score;
        if (line >> new_score && (new_score>=0 && new_score<=10)) {
            new_scores.push_back(new_score);
            continue;
        }
        cout << "Enter a score between 1 and 10. ";
    }
}

//ensures there is at least 1 movie
bool any_movies(vector<movie> &movies) {
    if (movies.size()>0) {
        return true;
    }
    cout << "\nNo Movies. Add a movie first.";
    return false;
}
//user chooses an available movie to operate on
int select_movie(vector<movie> &movies) {
    if (!any_movies(movies)) {
        return -1;
    }
    string avail = " movies available: ";
    if (movies.size()==1) {
        avail = " movie available: ";
    }
    cout << "\n" << movies.size() << avail << movies[0].title;
    for (int i=1; i<movies.size(); ++i) {
        cout << ", " << movies[i].title;
    }
    while (true) {
        cout << "\nEnter name of movie (enter nothing to cancel): ";
        string movie_title;
        getline(cin, movie_title);
        if (movie_title == "") {
            cout << "Cancelling operation.";
            return -1;
        }
        for (int i=0; i<movies.size(); ++i) {
            if (movies[i].title == movie_title) {
                return i;
            }
        }
        cout << "No matches. Try again. ";
    }
}
//make a new movie
void add_movie(vector<movie> &movies) {
    movie new_movie;
    set_title(new_movie);
    set_year(new_movie);
    set_actors(new_movie);
    set_genre(new_movie);
    set_scores(new_movie);
    movies.push_back(new_movie);
    cout << "New movie created!";
}
//view a particular movie
void view_movie(vector<movie> &movies) {
    int i = select_movie(movies);
    if (i!=-1) {
        movie &m = movies[i];
        cout << "Title: " << m.title
            << "\nYear: " << m.year
            << "\nActors: ";
        string punctuation=", ";
        for (int k=0; k<m.actors.size(); ++k) {
            if (k==m.actors.size()-1) {
                punctuation="";
            }
            cout << m.actors[k] << punctuation;
        }
        cout << "\nGenre: " << movies[i].genre
            << "\nScores: ";
        punctuation = ", ";
        for (int k=0; k<movies[i].scores.size(); k++) {
            if (k==movies[i].scores.size()-1) {
                punctuation="";
            }
            cout << movies[i].scores[k] << punctuation;
        }
    }
}
//make changes to a movie's information
void update_movie(vector<movie> &movies) {
    int i = select_movie(movies);
    if (i!=-1) {
        bool looping=true;
        while (looping) {
            looping=false;
            movie &m = movies[i];
            cout << "Do you want to update..."
            << "\nThe title? Enter 't'"
            << "\nThe year? Enter 'y'"
            << "\nThe actors? Enter 'a'"
            << "\nThe genre? Enter 'g'"
            << "\nThe scores? Enter 's'"
            << "\nNothing? Enter nothing to cancel."
            << "\nEnter character: ";
            string input;
            getline(cin, input);
            if (input == "") return;
            stringstream line(input);
            char command;
            line >> command;
            switch (command) {
                case 't':set_title(m); break;
                case 'y':set_year(m); break;
                case 'a':set_actors(m); break;
                case 'g':set_genre(m); break;
                case 's':set_scores(m); break;
                default:
                    looping=true;
                    cout << "Invalid Entry. Try again. ";
                    break;
            }
            cout << "Movie updated successfully.";
        }
    }
}
//view score statistics for a movie
void view_score(vector<movie> &movies) {
    int i = select_movie(movies);
    if (i!=-1) {
        movie &m = movies[i];
        vector<double> values = m.scores;
        if (values.size()<1) {
            cout << "No scores. Cancelling operation.";
            return;
        }
        double mean=0;
        double min=0;
        double max=0;
        double median=0;
        for (int k=1; k<values.size(); ++k) { //order scores
            double prev = values[k-1];
            double curr = values[k];
            if (prev>curr) {
                values[k]=prev;
                values[k-1]=curr;
                k=0;
            }
        }
        min = values[0];
        max = values[values.size()-1];
        
        double total_sum = 0; //sum values to determine mean
        for (int i=0; i<values.size(); ++i) {
            total_sum+=values[i];
        }
        mean = total_sum/values.size();
        
        int middle = values.size()/2; //precision loss is desirable
        median = values[middle];
        if (values.size()%2==0) { //if 2 values in the middle, average them
            int middle2 = values.size()/2-1;
            median = (median + values[middle2])/2;
        }
        cout << "Average: " << mean << "; Min: " << min
        << "; Max: " << max << "; Median: " << median << ";";
    }
}
//view movies with an actor
void view_actor(vector<movie> &movies) {
    if (!any_movies(movies)) {
        return;
    }
    while (true) {
        cout << "\nEnter name of actor (enter nothing to cancel): ";
        string actor;
        getline(cin, actor);
        if (actor == "") {
            cout << "Cancelling operation.";
            return;
        }
        vector<string> actor_movies;
        for (int i=0; i<movies.size(); ++i) {
            for (int k=0; k<movies[i].actors.size(); ++k) {
                if (movies[i].actors[k] == actor) {
                    actor_movies.push_back(movies[i].title);
                    break;
                }
            }
        }
        if (actor_movies.size()>0) {
            cout << "Movies with " << actor << ": ";
            string punctuation = ", ";
            for (int i=0; i<actor_movies.size(); i++) {
                if (i==actor_movies.size()-1) {
                    punctuation = "";
                }
                cout << actor_movies[i] << punctuation;
            }
            return;
        }
        cout << "No matches. Try again. ";
    }
}
//view movies of a certain year
void view_year(vector<movie> &movies) {
    if (!any_movies(movies)) {
        return;
    }
    while (true) {
        cout << "\nEnter year (enter nothing to cancel): ";
        string input;
        int year;
        cout << "Year: ";
        getline(cin, input);
        stringstream line(input);
        if (input == "") {
            cout << "Cancelling operation.";
            return;
        }
        if (line >> year) {
            vector<string> year_movies;
            for (int i=0; i<movies.size(); ++i) {
                if (movies[i].year == year) {
                    year_movies.push_back(movies[i].title);
                    break;
                }
            }
            if (year_movies.size()>0) {
                cout << "Movies released in " << year << ": ";
                string punctuation = ", ";
                for (int i=0; i<year_movies.size(); i++) {
                    if (i==year_movies.size()-1) {
                        punctuation = "";
                    }
                    cout << year_movies[i] << punctuation;
                }
                return;
            }
        }
        cout << "No matches. Try again. ";
    }
}

int main() {
  try {
    string file_name = "MyMDB.txt"; //already created file in same folder
    while (true) {
        cout << "Enter file name (Enter \"MyMDB.txt\"): ";
        getline (cin,file_name);
        cout << "Loading file...";
        ifstream in;
        in.open(file_name);
        if (in.is_open()) {
            cout << "\nFile opened successfully.";
            break;
        }
        cout << "\nCould not open file. Try again.\n";
    }
    
    vector<movie> movies;
    read(file_name,movies); //read data from file
    while (true) {
        cout << "\n\nDo you want to..."
            << "\nEnter a new movie and its information? Enter 'n'"
            << "\nView all information about an existing movie? Enter 'v'"
            << "\nUpdate the information about an existing movie? Enter 'u'"
            << "\nDisplay the average/min/max/median score for a movie? Enter 's'"
            << "\nDisplay the titles of all movies starring a particular actor? Enter 'a'"
            << "\nDisplay the titles of all movies made in a particular year? Enter 'y'"
            << "\nSave values and quit the program? Enter 'q'"
            << "\nEnter character: ";
        string input;
        getline(cin, input);
        stringstream line(input);
        char command;
        line >> command;
        switch (command) {
            case 'n':add_movie(movies); break;
            case 'v':view_movie(movies); break;
            case 'u':update_movie(movies); break;
            case 's':view_score(movies); break;
            case 'a':view_actor(movies); break;
            case 'y':view_year(movies); break;
            case 'q':
                cout << "\nSaving data...";
                write(file_name,movies); //write changes to file
                cout << "\nData saved. Program terminating.";
                return 0;
            default:
                cout << "Invalid command. Please try again.";
                continue;
        }
        cout << "\n\nEnter anything to continue... ";
        getline(cin, input);
    }
  } catch(...) {
      cout << "Error! Program terminating.";
  }
}