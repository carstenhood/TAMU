
#include <iostream>
#include <sstream>
#include "Graph.h"
#include "GUI.h"
#include "Window.h"
#include "std_lib_facilities_3.h"

#include "Data.h"

using namespace Graph_lib;
using namespace std;

//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

struct Movie_window : Graph_lib::Window
{
    //-----------------------------------------------------------------
    // delete these; my substitutes for things already in the file
    Movie_window(Point xy, int w, int h, const string& title );

    vector<int> int_vector;
    vector<Movie> movie_vector;
    void populate_vectors()  //creates a fake vector of movies for testing
    {
        movie_vector = get_movies_from_file();
        for (int i=0; i<movie_vector.size(); ++i) {
            int_vector.push_back(i);
        }
    }
    void quit();
    static void cb_quit(Address, Address);
    // end of delete these section
    //-----------------------------------------------------------------
    
    
    Vector_ref<Shape> shapes;

    // show actor info in search by actor
    Image *actor_photo;
    Text *actor_name;
    
    int rows; // can be 1-4; its square is the # of movies on the screen
    int display_index;
    vector<Movie_display*> movie_displays;
    
    // location & dimensions of movie display area
    double display_x;
    double display_y;
    double display_width;
    double display_height;
    
    // widgets
    Button quit_button;
    Button prev_button;
    Button next_button;
    
    Button rows_button;
    In_box rows_box;
    Out_box rows_out;
    
    Out_box page_box;
    Out_box result_box;

    // actions invoked by callbacks
    void rows_button_touched();
    void prev_button_touched();
    void next_button_touched();
    
    // callback functions
    static void cb_prev(Address, Address);
    static void cb_next(Address, Address);
    static void cb_rows(Address, Address);
    
    // display methods
    bool can_next();
    bool can_prev();
    void check_nav_buttons();
    void clear_movies();
    void clear_actor();
    void draw_movie(Movie &m, Point p, int w, int h);
    void update_display_boxes();
    void display_movies(int index);
    void display_actor(Actor a);
};

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

// window constructor to be merged
Movie_window::Movie_window(Point xy, int w, int h, const string& title)
:Window(xy,w,h,title),

rows(2),
display_index(0),
display_x(500),
display_y(10),
display_width(600),
display_height(h-100),

prev_button(Point(display_x+10, h-60), 70, 30, "<--", cb_prev),
next_button(Point(display_x+display_width-80, h-60), 70, 30, "-->", cb_next),
page_box(Point(display_x+display_width/2-100, h-60), 50, 20, "Page:"),
result_box(Point(display_x+display_width/2+100, h-60), 50, 20, "Results:"),

rows_out(Point(display_x+display_width+120, h-120), 50, 20, "Rows displayed:"),
rows_box(Point(display_x+display_width+120, h-90), 50, 20, "Update (1-4):"),
rows_button(Point(display_x+display_width+50, h-60), 100, 25, "Update Rows", cb_rows),

quit_button(Point(10,10), 70, 20, "Quit", cb_quit) //delete this

{
    attach(prev_button);
    attach(next_button);
    attach(rows_out);
    attach(rows_button);
    attach(rows_box);
    attach(quit_button); //delete this
    attach(result_box);
    attach(page_box);
    
    stringstream ss;
    ss << rows;
    rows_out.put(ss.str());
    
    check_nav_buttons();
}

//------------------------------------------------------------------------------

// deal with hiding and revealing page navigation buttons
bool Movie_window::can_next() {
    return display_index + rows*rows < int_vector.size();
}

bool Movie_window::can_prev() {
    return display_index - rows*rows >= 0;
}

void Movie_window::check_nav_buttons()
{
    if (can_next()) {
        next_button.hide();
        next_button.show();
    } else {
        next_button.show();
        next_button.hide();
    }
    if (can_prev()) {
        prev_button.hide();
        prev_button.show();
    } else {
        prev_button.show();
        prev_button.hide();
    }
}

//------------------------------------------------------------------------------

//draw a single movie given the location and dimensions of its display area
void Movie_window::draw_movie(Movie &m, Point p, int w, int h)
{
    double poster_side_length = h/2;
    if (poster_side_length > w)
        poster_side_length = w;
    
    int line_width=w/7;
    int line_height=10+(5-rows)*3;
    Point text_point(p.x, p.y+h/2+line_height);
    
    //display poster
    Image *poster = new Image(p, m.poster);
    poster->set_mask(Point(0,0), w-10, h/2);
    
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
    
    //create display shapes
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
    
    //attach shapes to window
    attach(*poster);
    attach(*title);
    attach(*year);
    attach(*tags);
    attach(*actors);
    attach(*scores);
    attach(*avg);
    
    Movie_display *m_display = new Movie_display(poster, title, year, tags, actors, scores, avg);
    movie_displays.push_back(m_display);
}

//------------------------------------------------------------------------------

void Movie_window::clear_movies()
{
    for (int i=0; i<movie_displays.size(); ++i) {
        detach(*movie_displays[i]->poster);
        detach(*movie_displays[i]->title);
        detach(*movie_displays[i]->year);
        detach(*movie_displays[i]->tags);
        detach(*movie_displays[i]->actors);
        detach(*movie_displays[i]->scores);
        detach(*movie_displays[i]->avg);
    }
    
    movie_displays.clear();
}

void Movie_window::clear_actor()
{
    detach(*actor_name);
    detach(*actor_photo);
}

//------------------------------------------------------------------------------

// display current page number and number of results
void Movie_window::update_display_boxes()
{
    stringstream result_stream;
    result_stream << int_vector.size();
    result_box.put(result_stream.str());
    
    stringstream page_stream;
    int page = 1+(display_index)/(rows*rows);
    int pages = 1+(int_vector.size()-1)/(rows*rows);
    page_stream << page << "/" << pages;
    page_box.put(page_stream.str());
}

//------------------------------------------------------------------------------

void Movie_window::display_movies(int index)
{
    clear_movies();
    update_display_boxes();
    
    double xspread = display_width/rows;
    double yspread = display_height/rows;
    double y = 0;
    
    for (int row=0; row<rows; ++row)
    {
        double x = 0;
        for (int col=0; col<rows; ++col) {
            if (index < int_vector.size()) {
                draw_movie(movie_vector[int_vector[index]],
                           Point(x+display_x, y+display_y), xspread, yspread);
            }
            x+=xspread;
            ++index;
        }
        y+=yspread;
    }
    
    check_nav_buttons();
}

void Movie_window::display_actor(Actor a)
{
    stringstream actor_stream;
    for (int k=0; k<a.names.size(); ++k)
        actor_stream << a.names[k] << " ";
    
    Point actor_point(display_x+display_width, display_y+20);
    actor_name = new Text(actor_point, actor_stream.str());
    actor_name->set_font_size(14);
    
    actor_photo = new Image(Point(actor_point.x, actor_point.y+20), a.photo);
    actor_photo->set_mask(Point(10,0), 190, 400);
    
    attach(*actor_name);
    attach(*actor_photo);
}

//------------------------------------------------------------------------------

//delete these; quit button for testing
void Movie_window::cb_quit(Address, Address pw) {
    reference_to<Movie_window>(pw).quit();
}

void Movie_window::quit() {
    hide();
}
//end of delete these section

//------------------------------------------------------------------------------

// actions invoked by callback functions
void Movie_window::rows_button_touched()
{
    int old_rows = rows;
    stringstream ii;
    ii << rows_box.get_string();
    ii >> rows;
    if (!ii || !rows) return;
    if (rows<1) rows=1;
    else if (rows>4) rows=4;
    if (rows == old_rows) return;
    stringstream ss;
    ss << rows;
    rows_out.put(ss.str());
    display_index=0;
    display_movies(display_index);
}

void Movie_window::next_button_touched()
{
    clear_movies();
    display_index+=rows*rows;
    display_movies(display_index);
    save_movies_to_file(movie_vector);   
}
void Movie_window::prev_button_touched()
{
    clear_movies();
    display_index -= rows*rows;
    display_movies(display_index);
}

//------------------------------------------------------------------------------

// callback functions
void Movie_window::cb_prev(Address, Address pw) {
    reference_to<Movie_window>(pw).prev_button_touched();
}

void Movie_window::cb_next(Address, Address pw) {
    reference_to<Movie_window>(pw).next_button_touched();
}

void Movie_window::cb_rows(Address, Address pw) {
    reference_to<Movie_window>(pw).rows_button_touched();
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

int main()
{
    try {
        Movie_window win1(Point(100,200), 1300, 900, "Testing");
        
        //begin testing
        win1.populate_vectors();
        win1.display_movies(0);
        
        vector<string> an;
        an.push_back("Sean");
        an.push_back("Bean");
        Actor actor;
        actor.names = an;
        actor.photo = "sean_bean.jpg";
        
        win1.display_actor(actor);
        //end testing
        
        return gui_main();
    } catch(exception& e) {
        cerr << "Exception: " << e.what() << '\n';
        return 1;
    } catch (...) {
        cerr << "Some exception\n";
        return 1;
    }
    
    return 0;
}


