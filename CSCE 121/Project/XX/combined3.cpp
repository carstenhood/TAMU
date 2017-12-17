#include "std_lib_facilities_3.h"
#include"Graph.h"
#include"GUI.h"
#include"Window.h"
#include"Point.h"

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
    : poster(p), title(tl), year(y), tags(tg), actors(a), scores(s), avg(av) {}
};

//------------------------------------------------------------------------------

struct Actor {
    vector<string> names;
    string photo;
    
    Actor() {}
	Actor(vector<string> n, string p) : names(n), photo(p) {}
    //copy constructor
    Actor(const Actor& other) : photo(other.photo) {
        names = other.names;
    }
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

//returns a vector of Movies as read from a file
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

//writes information from a vector of movies to a file
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


//------------------------------------------------------------------------------

//global variables
vector<Movie> movie_vec = get_movies_from_file();
vector<Actor> actor_vec;

//------------------------------------------------------------------------------

//search by actor function
Actor Search_Movies_By_Actor(string Actors)
{
    Actor blank_actor;
    blank_actor.photo = "\n"; //space represent no actor found
    vector<string> blank_names;
    blank_names.push_back(" ");
    blank_actor.names = blank_names;
    try {
        if (Actors == "")
            return blank_actor;
		else {
			vector<string> actor_names;
			string cur_name;
			istringstream Actor_stream(Actors);		//Converts the string into a whitespace seperated vector
			while (Actor_stream >> cur_name) {
				actor_names.push_back(cur_name);
			}
            
            for (int a=0; a<actor_names.size(); ++a) {	//Checking actors
                for (int b=0; b<movie_vec.size(); ++b) {      //Checks movies
                    for (int z=0; z<movie_vec[b].actors.size(); ++z) {       //Checks EACH actor
                        for (int q=0; q<movie_vec[b].actors[z].names.size(); ++q) {		//Checks EACH name
                            if ((movie_vec[b].actors[z].names[q]) == actor_names[a]) {
                                return movie_vec[b].actors[z]; //returns the first actor match to serve as source of search
                            }
                        }
                    }
                }
            }
		}
    } catch (exception& e) {
        return blank_actor;
    }
}

//------------------------------------------------------------------------------

//search function
//returns a vector<int> with indexes of resulting movies corresponding to movie_vector
vector<int> Search_Movies(string Actors, string Title, string Yearstring,
                          string Tags, const vector<Movie>& movie_vec)
{
    vector<int> int_vector;     //stores the indexes in movie_vec of all relevant movies
    try
    {
        while(true) //edit - why is this in a while loop? (same question goes for all of the searches below)
        {
            if(Yearstring == "")
                break;
            istringstream buffer(Yearstring);
            int Year;
            buffer >> Year;
            
            //edit - removed check for invalid year; no need to throw an error and terminate the program just cuz the user is dumb
            
            for (int i=0; i<movie_vec.size(); ++i)			//Adds matching movies to the vector of ints
                if (movie_vec[i].year == Year)
                    int_vector.push_back(i);
            break;
        }
        
        //Leave actors as strings
        //Check which movies in the vector contain those actors
        while (true) //edit - why is this in a while loop?
        {
            if(Actors == "")
                break;
            else {
                vector<string> actor_names;
                string cur_name;
                istringstream Actor_stream(Actors);		//Converts the string into a whitespace seperated vector
                while (Actor_stream >> cur_name) {
                    actor_names.push_back(cur_name);
                }
                
                for (int a=0; a<actor_names.size(); ++a) {	//Checking actors
                    for (int b=0; b<movie_vec.size(); ++b) {      //Checks movies
                        for (int z=0; z<movie_vec[b].actors.size(); ++z) {       //Checks EACH actor
                            for (int q=0; q<movie_vec[b].actors[z].names.size(); ++q) {		//Checks EACH name
                                if ((movie_vec[b].actors[z].names[q]) == actor_names[a]) {
                                    int_vector.push_back(b);
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            break;
        }
        
        //Check for matches with the title field
        while (true)
        {
            if(Title == "")
                break;
            
            for (int k=0; k<movie_vec.size(); ++k) {
                if (movie_vec[k].title == Title)
                    int_vector.push_back(k);
            }
            
            break; //edit - added a break cuz was getting an infinite loop; not sure why there's a loop in the first place
        }
        
        //-----------------------------------------------
        
        //Checks Tags
        while (true)
        {
            if(Tags == "")
                break;
            else {
                vector<string> tags_vec;
                string cur_tag;
                istringstream Tag_stream(Tags);	//Converts the string into a whitespace seperated vector
                while (Tag_stream >> cur_tag)
                {
                    tags_vec.push_back(cur_tag);
                    for (int b=0; b<movie_vec.size(); ++b)	//Adds matching movies to the vector of ints
                    {
                        for (int z=0; z<movie_vec[b].tags.size(); ++z) //edit - removed a loop
                        {
                            if (movie_vec[b].tags[z] == cur_tag) {	//Needs to be a double depth check to check all actors
                                int_vector.push_back(b);
                                break;
                            }
                        }
                    }
                    break;
                }
            }
            break; //edit - again added break as was getting stuck in a loop
        }
        //End of tag checker
        
        //-----------------------------------------------
        
        //Check scores
        
        //Check for duplicates in the int vector
        sort(int_vector.begin(), int_vector.end()); //edit: made changes; this function caused problems; now works perfectly
        for (int q=1; q<int_vector.size(); ++q) { //start at 1, otherwise q-1 = -1 and causes problems
            if (int_vector[q]==int_vector[q-1]) {
                int_vector.erase(int_vector.begin()+q-1);
                q--; //edit: subtract from iterator so that it doesn't skip more than 2 duplicates
            }
        }
        
        return int_vector;
    }
    catch (exception& e)
    {
        vector<int> blank_vec;
        return blank_vec;				//Returns a blank vector if an error is thrown
    }
    
}


//------------------------------------------------------------------------------

struct Movie_window : Graph_lib::Window
{
    Movie_window(Point xy, int w, int h, const string& title);
    
private:
    // data
    Vector_ref<Shape> shapes;
    Vector_ref<Point> points;
    
    // widgets
	Button clear_button;
	Button edit_button;
    Button quit_button;
    
	Button search_button;
	In_box search_title;
    In_box search_year;
    In_box search_actor;
    In_box search_tag;
	
	Button search_by_actor_button;
	
	Button add_button;
	In_box add_title;
	In_box add_poster;
	In_box add_year;
	In_box add_actors;
	In_box add_tags;
	Button create_button;
	Button update_button;
	Button cancel_button;
	
	In_box movie_selected;
    Button select_button;
    
    // actions invoked by callbacks:
    void search();
	void add();
	void clear();
	void edit();
	void select();
    void quit();
	void create();
	void cancel();
	void update();
	void search_by_actor();
    
    //callbacks
    static void cb_select(Address, Address);
    static void cb_search(Address, Address);
	static void cb_add(Address, Address);
	static void cb_clear(Address, Address);
	static void cb_edit(Address, Address);
    static void cb_quit(Address, Address);
	static void cb_create(Address, Address);
	static void cb_cancel(Address, Address);
	static void cb_update(Address, Address);
	static void cb_search_by_actor(Address, Address);
    
    //adding and editing
    void update_poster(Movie &m);
    void update_title(Movie &m);
    void update_year(Movie &m);
    void update_actors(Movie &m);
    void update_tags(Movie &m);
    
    
    // display declarations:
    // show actor info in search by actor
    Image *actor_photo;
    Text *actor_name;
    
    int rows; // can be 1-4; its square is the # of movies on the screen
    int display_index;
    vector<int> int_vector;     //stores the indexes in movie_vec of all relevant movies
    vector<Movie_display*> movie_displays;
    
    // location & dimensions of movie display area
    double display_x;
    double display_y;
    double display_width;
    double display_height;
    
    // widgets
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
    void clear_search();
    void draw_movie(Movie &m, Point p, int w, int h);
    void update_display_boxes();
    void set_rows(int r);
    void display_movies(int index);
    void display_actor(const Actor& a);
};

//------------------------------------------------------------------------------

Movie_window::Movie_window(Point xy, int w, int h, const string& title)
:Window(xy,w,h,title),
clear_button(Point(230,500), 70,20, "Clear", cb_clear),
edit_button(Point(70,690), 100, 20, "Edit movie", cb_edit),
quit_button(Point(x_max()-100,0), 70, 20, "Quit", cb_quit),

search_button(Point(50,500), 70, 20, "Search", cb_search),
search_title(Point(50,100), 150, 20, "Title:"),
search_year(Point(50,200), 150, 20, "Year:"),
search_actor(Point(50,300), 150, 20, "Actor:"),
search_tag(Point(50,400), 150, 20, "Tag:"),

add_button(Point(140,500), 70,20, "Add", cb_add),
add_title(Point(300,100), 150, 20, "Title:"),
add_poster(Point(300,170), 150, 20, "File name of poster:"),
add_year(Point(300,240), 150, 20, "Year of creation:"),
add_actors(Point(300,310), 150, 20, "Enter actor and file name of picture:\nEx.: mattdamon.jpg Matt Damon, \ncharliesheen.jpg Charlie Sheen"),
add_tags(Point(300,380), 150, 20, "Enter tags: \nEx.: romance, comedy, drama"),
create_button(Point(300,450), 70, 20, "Create", cb_create),
cancel_button(Point(380,450), 70, 20, "Cancel", cb_cancel),
update_button(Point(300,450), 70, 20, "Update", cb_update),

search_by_actor_button(Point(70,350), 110, 20, "Search by Actor", cb_search_by_actor),

select_button(Point(70,650), 100, 20, "Select movie", cb_select),
movie_selected(Point(100,600), 200, 20, "Title of movie:"),

// display initializations:
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

rows_out(Point(display_x+display_width+120, h-120), 50, 20, "Current rows:"),
rows_box(Point(display_x+display_width+120, h-90), 50, 20, "Update rows:"),
rows_button(Point(display_x+display_width+50, h-60), 100, 25, "Update Rows", cb_rows)
{
    attach(search_button);
    attach(quit_button);
    attach(add_button);
    attach(clear_button);
    attach(edit_button);
    attach(search_title);
    attach(search_year);
    attach(search_actor);
    attach(search_tag);
    attach(select_button);
    attach(movie_selected);
	attach(search_by_actor_button);
	
	attach(add_title);
	attach(add_poster);
	attach(add_year);
	attach(add_actors);
	attach(add_tags);
	attach(create_button);
	attach(cancel_button);
	attach(update_button);
	add_title.hide();
	add_poster.hide();
	add_year.hide();
	add_actors.hide();
	add_tags.hide();
	create_button.hide();
	cancel_button.hide();
	update_button.hide();
    
    attach(prev_button);
    attach(next_button);
    attach(rows_out);
    attach(rows_button);
    attach(rows_box);
    attach(result_box);
    attach(page_box);
    
    stringstream ss;
    ss << rows;
    rows_out.put(ss.str());
    
    check_nav_buttons();
}

//------------------------------------------------------------------------------

void Movie_window::cb_quit(Address, Address pw)
{
    reference_to<Movie_window>(pw).quit();
}

void Movie_window::cb_add(Address, Address pw)
{
    reference_to<Movie_window>(pw).add();
}

void Movie_window::cb_clear(Address, Address pw)
{
    reference_to<Movie_window>(pw).clear();
}

void Movie_window::cb_edit(Address, Address pw)
{
    reference_to<Movie_window>(pw).edit();
}

void Movie_window::cb_select(Address, Address pw)
{
    reference_to<Movie_window>(pw).select();
}

void Movie_window::cb_search(Address, Address pw)
{
    reference_to<Movie_window>(pw).search();
}

void Movie_window::cb_create(Address, Address pw)
{
    reference_to<Movie_window>(pw).create();
}

void Movie_window::cb_cancel(Address, Address pw)
{
    reference_to<Movie_window>(pw).cancel();
}

void Movie_window::cb_update(Address, Address pw)
{
    reference_to<Movie_window>(pw).update();
}

void Movie_window::cb_search_by_actor(Address, Address pw)
{
    reference_to<Movie_window>(pw).search_by_actor();
}

//------------------------------------------------------------------------------

void Movie_window::quit()
{
	save_movies_to_file(movie_vec);
	hide();
}

void Movie_window::search()
{
    //run search function
	int_vector = Search_Movies (search_actor.get_string(), search_title.get_string(),
                                search_year.get_string(), search_tag.get_string(), movie_vec);
    
    //display results starting at index 0
    clear_actor();
    display_movies(0);
}

void Movie_window::search_by_actor()
{
    cout << "\n1\n";
    Actor actor(Search_Movies_By_Actor(search_actor.get_string()));
    cout << "\n2\n";
    if (actor.photo != "\n")
    {
        cout << "\n2B\n";
        //create actor input
        stringstream actor_stream;
        for (int i=0; i<actor.names.size(); ++i)
            actor_stream << actor.names[i] << " ";
        cout << "\n3\n";
        //run search function with only the actor
        int_vector = Search_Movies(actor_stream.str(), "", "", "", movie_vec); // only actor matters
        cout << "\n4\n";
        //display results starting at index 0
        display_movies(0);
        display_actor(actor);
        cout << "\n5\n";
    }
}


void Movie_window::add()
{
	search_title.hide();
	search_year.hide();
	search_actor.hide();
	search_tag.hide();
	add_button.hide();
	search_button.hide();
	movie_selected.hide();
	select_button.hide();
	edit_button.hide();
	search_by_actor_button.hide();
	
	add_title.show();
	add_poster.show();
	add_year.show();
	add_actors.show();
	add_tags.show();
	create_button.show();
	cancel_button.show();
    
    clear_search();
}

void Movie_window::clear()
{
	//clear all In_boxes
	search_title.show();
	search_year.show();
	search_actor.show();
	search_tag.show();
	add_button.show();
	search_button.show();
	movie_selected.show();
	select_button.show();
	edit_button.show();
	search_by_actor_button.show();
	
	add_title.hide();
	add_poster.hide();
	add_year.hide();
	add_actors.hide();
	add_tags.hide();
	create_button.hide();
	cancel_button.hide();
	update_button.hide();
    
    clear_search();
}

void Movie_window::select() {
    
}

void Movie_window::edit()
{
    string title = movie_selected.get_string();
    if (title!="") {
        for (int i=0; i<movie_vec.size(); ++i) {
            if (title == movie_vec[i].title) {
                clear_search();

                set_rows(1); //show movie full screen
                int_vector.push_back(i);
                display_movies(0);
                
                search_title.hide();
                search_year.hide();
                search_actor.hide();
                search_tag.hide();
                add_button.hide();
                search_button.hide();
                movie_selected.hide();
                select_button.hide();
                edit_button.hide();
                search_by_actor_button.hide();
                
                add_title.show();
                add_poster.show();
                add_year.show();
                add_actors.show();
                add_tags.show();
                update_button.show();
                cancel_button.show();
            }
        }
    }
}

//functions used for updating or adding different movie attributes
//------------------------------------------------------------------------------

void Movie_window::update_poster(Movie &m) {
	m.poster = add_poster.get_string();
}

void Movie_window::update_title(Movie &m) {
    m.title = add_title.get_string();
}

void Movie_window::update_year(Movie &m) {
    m.year = add_year.get_int();
}

void Movie_window::update_actors(Movie &m) {
    Actor actor;
	string temp = add_actors.get_string();
	string name="";
	bool pic = false;
	for (int i=0; i<temp.length(); ++i) {
        
		if (temp[i]!=' ' && temp[i]!=',')
            name+=temp[i];
        
		if (temp[i]==' ' && (i!=0 && temp[i-1]!=',')) {
			for (int j=0; j<name.length(); ++j) {
				if (name[j]=='.') pic = true;
			}
			
			if (pic) actor.photo = name;
			else actor.names.push_back(name);
			
			pic=false;
			name="";
		}
		if (temp[i]==',' || i==temp.length()-1) {
			for (int j=0; j<name.length(); ++j) {
				if (name[j]=='.') pic=true;
			}
			
			if (pic) actor.photo=name;
			else actor.names.push_back(name);
			
			m.actors.push_back(actor);
			actor.names.clear();
			actor.photo="";
			name="";
		}
	}
}

void Movie_window::update_tags(Movie &m) {
    string tag_temp = add_tags.get_string();
	string tag="";
	for (int i=0; i<tag_temp.length(); ++i) {
		if (tag_temp[i]!=' ' && tag_temp[i]!=',')
            tag+=tag_temp[i];
		if (tag_temp[i]==',' || i==tag_temp.length()-1) {
			m.tags.push_back(tag);
			tag="";
		}
	}
}

//------------------------------------------------------------------------------

void Movie_window::create()
{
	Movie m;
    update_poster(m);
    update_title(m);
    update_year(m);
    update_actors(m);
    update_tags(m);
    movie_vec.push_back(m);
    
    //display the new movie
    int_vector.push_back(movie_vec.size()-1);
    display_movies(0);
}

void Movie_window::update()
{
	Movie &m = movie_vec[int_vector[0]];
    if (add_poster.get_string()!="") update_poster(m);
	if (add_title.get_string()!="") update_title(m);
	if (add_year.get_int()!=-999999) update_year(m);
    if (add_actors.get_string()!="") {
        m.actors.clear();
        update_actors(m);
    }
    if (add_tags.get_string()!="") {
        m.tags.clear();
        update_tags(m);
    }
    
    //update display with new information
    display_movies(0);
}

void Movie_window::cancel()
{
    search_title.show();
	search_year.show();
	search_actor.show();
	search_tag.show();
	add_button.show();
	search_button.show();
	movie_selected.show();
	select_button.show();
	edit_button.show();
	search_by_actor_button.show();
	
	add_title.hide();
	add_poster.hide();
	add_year.hide();
	add_actors.hide();
	add_tags.hide();
	create_button.hide();
	update_button.hide();
	cancel_button.hide();
}

//display methods
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
    
    int line_width = w/7;
    int line_height = 10+(5-rows)*3;
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

void Movie_window::clear_search() {
    int_vector.clear();
    clear_actor();
    clear_movies();
    update_display_boxes();
    redraw();
}


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

void Movie_window::set_rows(int r)
{
    rows = r;
    stringstream ss;
    ss << rows;
    rows_out.put(ss.str());
}

// display current page number and number of results
void Movie_window::update_display_boxes()
{
    stringstream result_stream;
    result_stream << int_vector.size();
    result_box.put(result_stream.str());
    
    stringstream page_stream;
    int page = 1+(display_index)/(rows*rows);
    int pages = 1+(int_vector.size()-1)/(rows*rows);
    if (!int_vector.size()>0)
        page = pages = 0;
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
                draw_movie(movie_vec[int_vector[index]],
                Point(x+display_x, y+display_y), xspread, yspread);
            }
            x+=xspread;
            ++index;
        }
        y+=yspread;
    }
    
    check_nav_buttons();
    redraw();
}

void Movie_window::display_actor(const Actor& a)
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

// actions invoked by callback functions
void Movie_window::rows_button_touched()
{
    int r=0;
    stringstream ii;
    ii << rows_box.get_string();
    ii >> r;
    if (!ii || !r) return;
    if (r<1) r=1;
    else if (r>4) r=4;
    if (r==rows) return;
    set_rows(r);
    display_index=0;
    display_movies(display_index);
}

void Movie_window::next_button_touched()
{
    clear_movies();
    display_index+=rows*rows;
    display_movies(display_index);
    save_movies_to_file(movie_vec);
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

int main() {
try {
	movie_vec = get_movies_from_file();
    Movie_window win(Point(0,0), 1300, 1000, "Movie Database");
    return gui_main();
}
catch(exception& e) {
    cerr << "exception: " << e.what() << '\n';
    return 1;
    }
    catch (...) {
        cerr << "Some exception\n";
        return 2;
    }
}

    //------------------------------------------------------------------------------
    
