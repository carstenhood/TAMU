//Data.h
//contains structs for movie and actor data
//contains functions for saving and loading data from a file

#ifndef project_Data_h
#define project_Data_h

//------------------------------------------------------------------------------

struct Actor {
    vector<string> names;
    string photo;
    
    Actor() {}
	Actor(vector<string> n, string p) : names(n), photo(p) {}
};

struct Movie {
    string title;
    string poster;
    int year;
    vector<Actor> actors;
    vector<string> tags;
    vector<double> scores;
	
	Movie() {}
	Movie(string t, string p, int y) : title(t), poster(p), year(y) {}
};

//------------------------------------------------------------------------------

vector<Movie> get_movies_from_file()
{
    ifstream in;
    in.open("mdb.txt");
    
    string input;
    vector<Movie> movies_read;
    
    while (true) {
        getline(in, input);
        if (input != "-NEXT MOVIE") break;
        
        Movie m;
        getline(in, m.title);
        
        getline(in, input);
        if (input!="") {
            stringstream year_stream(input);
            year_stream >> m.year;
        } else
            m.year = 0000;
        
        getline(in, m.poster);
        
        getline(in, input);
        while (true) {
            getline(in, input);
            if (input != "ACTOR") break;
            
            Actor actor;
            getline(in, actor.photo);
            
            string name;
            while (true) {
                getline(in, name);
                if (name == "") break;
                actor.names.push_back(name);
            }
            
            m.actors.push_back(actor);
        }
        
        while (true) {
            string tag;
            getline(in, tag);
            if (tag == "" || !in) break;
            m.tags.push_back(tag);
        }
        
        while (true) {
            double score;
            getline(in, input);
            if (input == "" || !in) break;
            stringstream score_stream(input);
            score_stream >> score;
            m.scores.push_back(score);
        }
        
        movies_read.push_back(m);
        getline(in, input);
    }
    return movies_read;
}

void save_movies_to_file(vector<Movie> &movies)
{
    ofstream out;
    out.open("mdb.txt");
    
    for (int i=0; i<movies.size(); ++i) {
        Movie m = movies[i];
        out << "-NEXT MOVIE\n";
        
        out << m.title << "\n";
        out << m.year << "\n";
        out << m.poster << "\n";
        out << "\n";
        
        for (int k=0; k<m.actors.size(); ++k) {
            out << "ACTOR\n";
            out << m.actors[k].photo << "\n";
            for (int n=0; n<m.actors[k].names.size(); ++n) {
                out << m.actors[k].names[n] << "\n";
            }
            out << "\n";
        }
        out << "\n";
        
        for (int k=0; k<m.tags.size(); ++k) {
            out << m.tags[k] << "\n";
        }
        out << "\n";
        
        for (int k=0; k<m.scores.size(); ++k) {
            out << m.scores[k] << "\n";
        }
        out << "\n\n";
    }
}

#endif
