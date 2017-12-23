//
//  Display.cpp
//  
//
//  Created by Carsten Hood on 11/11/13.
//
//

#include "Display.h"


struct Movie_display
{
    Image poster;
    Text title;
    Text tags;
    Text actors;
    Text scores;
    
    Movie_display(Image &p, Text &t, Text &l)
    : poster(p), title(t), label(l) { }
};

//global variables
int rows = 2; //global variable can be 1-4; its square is the # of movies on the screen at a time;
vector<Movie_display> movie_displays;

//testing code begins; this will be deleted (substitutes for code I don't have)
struct Actor {
    vector<string> names;
    string photo;
};

struct Movie {
    string title;
    string poster;
    int year;
    vector<Actor> actors;
    vector<string> tags;
    vector<double> scores;
};

vector<int> int_vector;
vector<Movie> movie_vector;
void populate_vectors()
{
    Movie m;
    m.title = "Black Death";
    m.poster = "blackdeath.jpg";
    m.year = 2005;
    
    Actor actor;
    actor.photo = "seanbean.jpg";
    actor.names.push_back("Sean");
    actor.names.push_back("Bean");
    m.actors.push_back(actor);
    
    m.tags.push_back("Horror");
    m.tags.push_back("Thriller");
    
    m.scores.push_back(9);
    m.scores.push_back(8.5);
    
    movie_vector.push_back(m);
    movie_vector.push_back(m);
    
    int_vector.push_back(0);
}
//testing code ends

void display_movies(int index) //the index in int_vector of the first movie to be displayed on this screen
{
    //position and dimensions of the movies display within the window
    // "display" refers to the region where the movies will be
    // displayed on the right side of the window
    double displayw_x = 400; //leftmost point of display
    double display_y = 10; //margin of top of display
    double display_width = 600;
    double display_height = 600;
    
    //distance between individual movies; also determines width of movie
    double xspread = display_width/rows;
    double yspread = display_height/rows;

    //loop through the rows and columns, and call draw_movie on every movie
    double y = 0; //or //double y = yspread/2; //to give the center
    for (int row=0; row<rows; ++row)
    {
        double x = 0;
        for (int col=0; col<rows; ++col) {
            if (index < int_vector.size()) //if we are not through with the movies draw the next one
                draw_movie(movie_vector[int_vector[index]], Point(x+display_x, y+display_y), xspread, yspread);
            x+=xspread;
            ++index;
        }
        
        y+=yspread;
    }
}

void clear_movies()
{
    for (int i=0; i<movie_displays.size(); ++i) {
        //remove movie_displays[i] from window
    }
    movie_displays.clear();

}

//draw an individual movie given the location and dimensions of the display area
void draw_movie(Movie &m, Point p, int w, int h)
{
    double poster_height = h/2;
    if (poster_side_length > w)
        poster_side_length = w;
    
    Image poster = new Image(p, m.poster);
    
    double line_height = 10;
    Point text_point(p.x, p.y + poster_side_length + 5);
    
    stringstream title_stream << m.title << " (" << m.year << ")";
    Text title = new Text(text_point, title_stream.str());

    string punctuation = "";

    //display tags
    stringstream tags_stream;
    if (rows < 4) {
        tags_stream << "Tags: ";
        punctuation = ", ";
        for (int i=0; i<m.tags.size(); ++i) {
            tags_stream << m.tags[i];
            
            if (i == m.tags.size()-1) {
                punctuation = ";";
            } else if (tags_stream.str().length()>20) {
                tags_stream << "...";
                break;
            }
            tags_stream << punctuation;
        }
    }
    
    //display actors
    stringstream actors_stream;
    if (rows < 3) {
        actors_stream << "\nActors: ";
        punctuation = ",";
        for (int i=0; i<m.actors.size(); ++i) {
            for (int k=0; k<m.actors[i].names.size(); ++k)
                actors_stream << " " << m.actors[i].names[k];
            
            if (i == m.tags.size()-1) {
                punctuation = ";";
            } else if (actors_stream.str().length()>30) {
                actors_stream << "...";
                break;
            }
            actors_stream << punctuation;
        }
    }
    
    //display scores
    stringstream scores_stream;
    if (rows < 3) {
        scores_stream << "\nScores: ";
        punctuation = ",";
        for (int i=0; i<m.scores.size(); ++i) {
            scores_stream << m.scores[i];
            
            if (i == m.scores.size()-1) {
                punctuation = ";";
            } else if (scores_stream.str().length()>20) {
                scores_stream << "...";
                break;
            }
            scores_stream << punctuation;
        }
    }

    Text tags = new Text(text_point + line_height, tags_stream.str());
    Text actors = new Text(text_point + line_height*2, actors_stream.str());
    Text scores = new Text(text_point + line_height*3, scores_stream.str());
    
    //window.attach(poster); // idk the syntax for these
    //window.attach(title);
    //window.attach(tags);
    //window.attach(actors);
    //window.attach(scores);
    Movie_display m = new Movie_display(poster, title, tags, actors, scores);
    movie_displays.push_back(m);
}

bool can_next() { //checks if the next button should be visible
    return display_index + rows*rows < int_vector.size();
}
bool can_prev() { //checks if the prev button should be visible
    return display_index - rows*rows >= 0;
}

void update_num_rows ()
{
    //rows = rows_box.input or whatever (get an int from a "number of rows" input box)
    rows=2; //testing
    if (rows<1) rows=1; // min movies that can be displayed is 1^2 (1 movie)
    else if (rows>4) rows=4; //max # of movies that can be displayed is 4^2 (16 movies)
    display_index=0;
    //hide the prev button
    if (!can_next()) {
        //hide the next button
    }
}

void next_button_touched ()
{
    display_index+=rows*rows;
    display_movies(display_index);
    if (can_next()) {
        //hide the next button
    }
    //show the prev button
}

void prev_button_touched ()
{
    display_index -= rows*rows;
    display_movies(display_index);
    if (can_prev()) {
        //hide the prev button
    }
    //show the next button
}