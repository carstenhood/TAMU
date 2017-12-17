//
//  main.cpp
//  x_test
//
//  Created by Carsten Hood on 11/15/13.
//  Copyright (c) 2013 Carsten Hood. All rights reserved.
//

#include <iostream>

#include "std_lib_facilities_3.h"
#include "Simple_window.h"
#include "Graph.h"
#include "Data.h"

struct Movie_display
{
    Image *poster;
    Text *title;
    Text *year;
    Text *tags;
    Text *actors;
    Text *scores;
    Text *avg;
    
    Movie_display(Image *p, Text *tl, Text *y, Text *tg, Text *a, Text *s, Text *av)
    : poster(p), title(tl), year(y), tags(tg), actors(a), scores(s), avg(av) { }
};

Simple_window win1(Point(100,200),600,600,"Testing");
vector<int> int_vector;
vector<Movie> movie_vector;

void populate_vectors()  //creates a fake vector of movies for testing
{
    movie_vector = get_movies_from_file();
    for (int i=0; i<movie_vector.size(); ++i) {
        int_vector.push_back(i);
    }
}
//testing code ends
//-----------------------------------------------------------------


//global variables
int rows=2; //global variable can be 1-4; its square is the # of movies on the screen at a time;
int display_index;
vector<Movie_display*> movie_displays;

//draw an individual movie given the location and dimensions of the display area
void draw_movie(Movie &m, Point p, int w, int h)
{
    double poster_side_length = h/2;
    if (poster_side_length > w)
        poster_side_length = w;
    
    int line_width=w/7; // less than 100 characters in 600 pixels
    int line_height=10+(5-rows)*3;
    Point text_point(p.x, p.y + h/2 + line_height);
    
    //display poster
    Image *poster = new Image(p, m.poster);
    poster->set_mask(Point(0,0), w-10, h/2); //fltk
    
    //display title
    stringstream year_stream;
    year_stream << "(" << m.year << ")";
    
    //display tags
    stringstream tags_stream;
    if (rows < 5) {
        tags_stream << "Tags: ";
        for (int i=0; i<m.tags.size(); ++i) {
            if (tags_stream.str().length()+m.tags[i].length() > line_width) {
                tags_stream << "...";
                break;
            }
            
            if (i!=0) tags_stream << ", ";
            tags_stream << m.tags[i];
            if (i == m.tags.size()-1)
                tags_stream << ";";
        }
    }
    
    //display actors
    stringstream actors_stream;
    if (rows < 5) {
        actors_stream << "Actors:";
        for (int i=0; i<m.actors.size(); ++i) {
            stringstream next_actor;
            for (int k=0; k<m.actors[i].names.size(); ++k)
                next_actor << " " << m.actors[i].names[k];
            
            if (actors_stream.str().length()+next_actor.str().length() > line_width) {
                actors_stream << "...";
                break;
            }
            
            if (i!=0) actors_stream << ",";
            actors_stream << next_actor.str();
            if (i == m.actors.size()-1)
                actors_stream << ";";
        }
    }
    
    //display scores
    stringstream scores_stream;
    if (rows < 5) {
        scores_stream << "Ratings: ";
        for (int i=0; i<m.scores.size(); ++i) {
            stringstream next_score;
            next_score << m.scores[i];
            if (scores_stream.str().length()+next_score.str().length() > line_width) {
                scores_stream << "...";
                break;
            }
            
            if (i!=0) scores_stream << ", ";
            scores_stream << m.scores[i];
            if (i == m.scores.size()-1)
                scores_stream << ";";
        }
    }
    
    //display average score
    stringstream avg_stream;
    if (m.scores.size()>0) {
        double mean=0;
        double total_sum = 0; //sum values to determine mean
        for (int i=0; i<m.scores.size(); ++i)
            total_sum+=m.scores[i];
        mean = total_sum/m.scores.size();

        avg_stream << "Average rating: " << mean << " / 10.0";
    }

    Text *title = new Text(text_point, m.title);
    title->set_font_size(22-rows*2);
    
    Text *year = new Text(Point(text_point.x, text_point.y+line_height), year_stream.str());
    year->set_font_size(20-rows*2);
    
    Text *tags = new Text(Point(text_point.x, text_point.y+line_height*2), tags_stream.str());
    tags->set_font_size(16-rows);
    
    Text *actors = new Text(Point(text_point.x, text_point.y+line_height*2.9), actors_stream.str());
    actors->set_font_size(14-rows);
    
    Text *scores = new Text(Point(text_point.x, text_point.y+line_height*3.7), scores_stream.str());
    scores->set_font_size(14-rows);
    
    Text *avg = new Text(Point(text_point.x, text_point.y+line_height*4.5), avg_stream.str());
    avg->set_font_size(14-rows);
    
    win1.attach(*poster);
    win1.attach(*title);
    win1.attach(*year);
    win1.attach(*tags);
    win1.attach(*actors);
    win1.attach(*scores);
    win1.attach(*avg);
    
    Movie_display *m_display = new Movie_display(poster, title, year, tags, actors, scores, avg);
    movie_displays.push_back(m_display);
}

void clear_movies()
{
    for (int i=0; i<movie_displays.size(); ++i) {
        win1.detach(*movie_displays[i]->poster);
        win1.detach(*movie_displays[i]->title);
        win1.detach(*movie_displays[i]->year);
        win1.detach(*movie_displays[i]->tags);
        win1.detach(*movie_displays[i]->actors);
        win1.detach(*movie_displays[i]->scores);
        win1.detach(*movie_displays[i]->avg);
    }
    movie_displays.clear();
}

void display_movies(int index) //the index in int_vector of the first movie to be displayed on this screen
{
    clear_movies();
    //position and dimensions of the movies display within the window
    // "display" refers to the region where the movies will be
    // displayed on the right side of the window
    double display_x = 10; //leftmost point of display (probs around 300)
    double display_y = 10; //margin of top of display (probs around 10)
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

bool can_next() { //checks if the next button should be visible
    return display_index + rows*rows < int_vector.size();
}

bool can_prev() { //checks if the prev button should be visible
    return display_index - rows*rows >= 0;
}

void check_nav_buttons()
{
    if (can_next()) {
        //next_button.show();
    } else {
        //next_button.hide();
    }
    if (can_prev()) {
        //prev_button.show();
    } else {
        //prev_button.hide();
    }
}

void rows_button_touched()
{
    //rows = rows_box.input or whatever (get an int from a "number of rows" input box)
    rows=2; //testing
    if (rows<1) rows=1; // min movies that can be displayed is 1^2 (1 movie)
    else if (rows>4) rows=4; //max # of movies that can be displayed is 4^2 (16 movies)
    display_index=0;
    display_movies(display_index);
}

void next_button_touched()
{
    clear_movies();
    display_index+=rows*rows;
    display_movies(display_index);
}
void prev_button_touched()
{
    clear_movies();
    display_index -= rows*rows;
    display_movies(display_index);
}

int main()
{
    try {
        populate_vectors();
        display_movies(0);
        win1.wait_for_button(); //fltk
    } catch(exception& e) {
        cerr << "Exception: " << e.what() << '\n';
        return 1;
    } catch (...) {
        cerr << "Some exception\n";
        return 1;
    }
    
    return 0;
}
