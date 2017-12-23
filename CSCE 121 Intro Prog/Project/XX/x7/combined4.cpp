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

//------------------------------------------------------------------------------

//search by actor function
Actor* Search_Movies_By_Actor(string Actors)
{
    vector<string> blank_names;
    Actor* blank_actor = new Actor(blank_names, "none");

    try {
        if (Actors == "")
            return blank_actor;
		else {
			vector<string> actor_names;
			string cur_name;
			istringstream Actor_stream(Actors);	//Converts the string into a whitespace seperated vector
			while (Actor_stream >> cur_name) {
				actor_names.push_back(cur_name);
			}
            
            for (int a=0; a<actor_names.size(); ++a) {	//Checking actors
                for (int b=0; b<movie_vec.size(); ++b) {      //Checks movies
                    for (int z=0; z<movie_vec[b].actors.size(); ++z) {       //Checks EACH actor
                        for (int q=0; q<movie_vec[b].actors[z].names.size(); ++q) {		//Checks EACH name
                            if ((movie_vec[b].actors[z].names[q]) == actor_names[a]) {
                                Actor *actor = new Actor(movie_vec[b].actors[z]);
                                return actor; //returns the first actor match to serve as source of search
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
        while(true)
        {
            if(Yearstring == "")
                break;
            istringstream buffer(Yearstring);
            int Year;
            buffer >> Year;
            
            for (int i=0; i<movie_vec.size(); ++i)	//Adds matching movies to the vector of ints
                if (movie_vec[i].year == Year)
                    int_vector.push_back(i);
            break;
        }
        
        //Leave actors as strings
        //Check which movies in the vector contain those actors
        while (true)
        {
            if(Actors == "")
                break;
            else {
                vector<string> actor_names;
                string cur_name;
                istringstream Actor_stream(Actors);	//Converts the string into a whitespace seperated vector
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
            
            break;
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
                        for (int z=0; z<movie_vec[b].tags.size(); ++z)
                        {
                            if (movie_vec[b].tags[z] == cur_tag) {
                                int_vector.push_back(b);
                                break;
                            }
                        }
                    }
                    break;
                }
            }
            break;
        }
        //End of tag checker
        
        //-----------------------------------------------
        
        //Check for duplicates in the int vector
        sort(int_vector.begin(), int_vector.end());
        for (int q=1; q<int_vector.size(); ++q) {
            if (int_vector[q]==int_vector[q-1]) {
                int_vector.erase(int_vector.begin()+q-1);
                q--;
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
    vector<string> all_tags;

    // data
    Vector_ref<Shape> shapes;
    Vector_ref<Point> points;
    
    // widgets
    Button quit_button;
    
	In_box search_title;
    In_box search_year;
    In_box search_actor;
    In_box search_tag;
    
    Button search_by_title_button;
    Button search_by_year_button;
    Button search_by_actor_button;
    Button search_by_tags_button;
	Button search_button;
    Button display_all_button;
    Button clear_button;
	
    Button edit_button;
    Button add_button;
	In_box add_title;
	In_box add_poster;
	In_box add_year;
	In_box add_actors;
	In_box add_tags;
    In_box add_scores;
	Button create_button;
	Button update_button;
	Button end_edit_button;
	
	In_box movie_selected;

    void init_labels();
            
    // actions invoked by callbacks:
    void search();
    void display_all();
	void add();
	void clear();
	void edit();
    void quit();
	void create();
	void end_edit();
	void update();
    
    void search_by_title();
    void search_by_year();
	void search_by_actor();
    void search_by_tags();
    
    // hide and show appropriate buttons
    void edit_mode();
    void search_mode();
    
    //callbacks
    static void cb_search(Address, Address);
    static void cb_display_all(Address, Address);
	static void cb_add(Address, Address);
	static void cb_clear(Address, Address);
	static void cb_edit(Address, Address);
    static void cb_quit(Address, Address);
	static void cb_create(Address, Address);
	static void cb_end_edit(Address, Address);
	static void cb_update(Address, Address);
    static void cb_search_by_title(Address, Address);
    static void cb_search_by_year(Address, Address);
	static void cb_search_by_actor(Address, Address);
    static void cb_search_by_tags(Address, Address);
    
    //adding and editing
    void update_poster(Movie &m);
    void update_title(Movie &m);
    void update_year(Movie &m);
    void update_actors(Movie &m);
    void update_tags(Movie &m);
    void update_scores(Movie &m);
    
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
    
    Text rows_text;
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

quit_button(Point(x_max()-100,0), 70, 20, "Quit", cb_quit),

search_title(Point(100,100), 150, 25, "Title:"),
search_by_title_button(Point(270,100), 150, 25, "Search by Title", cb_search_by_title),
search_year(Point(100,200), 150, 25, "Year:"),
search_by_year_button(Point(270,200), 150, 25, "Search by Year", cb_search_by_year),
search_actor(Point(100,300), 150, 25, "Actor(s):\n(use spaces)"),
search_by_actor_button(Point(270,300), 150, 25, "Search by Actor", cb_search_by_actor),
search_tag(Point(100,400), 150, 25, "Tag(s):\n(use spaces)"),
search_by_tags_button(Point(270,400), 150, 25, "Search by Tags", cb_search_by_tags),

search_button(Point(270,460), 150, 50, "General Search", cb_search),
display_all_button(Point(270,520), 150, 25, "Display All", cb_display_all),
clear_button(Point(270,560), 150, 25, "Clear Display", cb_clear),

movie_selected(Point(100,650), 150, 25, "Edit Movie:"),
edit_button(Point(270,650), 150, 40, "Edit Movie", cb_edit),
add_button(Point(270,700), 150, 40, "New Movie", cb_add),

add_title(Point(300,100), 150, 25, "Movie Title:"),
add_poster(Point(300,170), 150, 25, "Poster Filename:"),
add_year(Point(300,240), 150, 25, "Release Year:"),
add_actors(Point(300,310), 150, 25, "Actor Name and Photo:\nEx.: matt_damon.jpg Matt Damon, \ncharlie_sheen.jpg Charlie Sheen"),
add_tags(Point(300,380), 150, 25, "Tags:\nEx.: Thriller, Action"),
add_scores(Point(300,450), 150, 25, "Scores:\nSeparate with spaces.\nEx.: 6.7 8.4 7.8"),

create_button(Point(300,520), 150, 25, "Create", cb_create),
update_button(Point(300,520), 150, 25, "Update", cb_update),
end_edit_button(Point(300,550), 150, 25, "Done", cb_end_edit),

// display initializations:
display_index(0),
display_x(500),
display_y(10),
display_width(600),
display_height(h-100),

prev_button(Point(display_x+10, h-60), 70, 30, "<--", cb_prev),
next_button(Point(display_x+display_width-80, h-60), 70, 30, "-->", cb_next),
page_box(Point(display_x+display_width/2-100, h-60), 50, 20, "Page:"),
result_box(Point(display_x+display_width/2+100, h-60), 50, 20, "Results:"),

rows_text(Point(display_x+display_width+40, h-260), "Display Sizing"),
rows_out(Point(display_x+display_width+130, h-120), 50, 20, "Current rows:"),
rows_box(Point(display_x+display_width+130, h-90), 50, 20, "Update rows (1-4):"),
rows_button(Point(display_x+display_width+50, h-60), 100, 25, "Update Rows", cb_rows)
{
    attach(search_button);
    attach(display_all_button);
    attach(quit_button);
    attach(add_button);
    attach(clear_button);
    attach(edit_button);
    attach(search_title);
    attach(search_year);
    attach(search_actor);
    attach(search_tag);
    attach(movie_selected);
    
    attach(search_by_title_button);
    attach(search_by_year_button);
	attach(search_by_actor_button);
    attach(search_by_tags_button);

	attach(add_title);
	attach(add_poster);
	attach(add_year);
	attach(add_actors);
	attach(add_tags);
    attach(add_scores);
	attach(create_button);
	attach(end_edit_button);
	attach(update_button);

    attach(prev_button);
    attach(next_button);
    rows_text.set_font_size(18);
    attach(rows_text);
    attach(rows_out);
    attach(rows_button);
    attach(rows_box);
    attach(result_box);
    attach(page_box);
    
    init_labels();
    set_rows(2);
    search_mode();
    check_nav_buttons();
    update_display_boxes();
}

void Movie_window::init_labels()
{
    all_tags.push_back("Comedy");
    all_tags.push_back("Romance");
    all_tags.push_back("Horror");
    all_tags.push_back("Thriller");
    all_tags.push_back("Action");
    all_tags.push_back("Superhero");
    all_tags.push_back("Medieval");
    all_tags.push_back("Fantasy");
    
    stringstream tags_stream;
    tags_stream << "All Tags: ";
    string punctuation = ", ";
    for (int i=0; i<all_tags.size(); ++i) {
        if (i==all_tags.size()-1) punctuation = "";
        tags_stream << all_tags[i] << punctuation;
    }
    Text *all_tags_text = new Text(Point(20, 820), tags_stream.str());
    all_tags_text->set_font_size(12);
    attach(*all_tags_text);
    
    Text *title_text = new Text(Point(150, 45), "Movie Database");
    title_text->set_font_size(28);
    attach(*title_text);
}


//------------------------------------------------------------------------------

void Movie_window::cb_quit(Address, Address pw) {
    reference_to<Movie_window>(pw).quit();
}

void Movie_window::cb_add(Address, Address pw) {
    reference_to<Movie_window>(pw).add();
}

void Movie_window::cb_clear(Address, Address pw) {
    reference_to<Movie_window>(pw).clear();
}

void Movie_window::cb_edit(Address, Address pw) {
    reference_to<Movie_window>(pw).edit();
}

void Movie_window::cb_search(Address, Address pw) {
    reference_to<Movie_window>(pw).search();
}

void Movie_window::cb_display_all(Address, Address pw) {
    reference_to<Movie_window>(pw).display_all();
}

void Movie_window::cb_create(Address, Address pw) {
    reference_to<Movie_window>(pw).create();
}

void Movie_window::cb_end_edit(Address, Address pw) {
    reference_to<Movie_window>(pw).end_edit();
}

void Movie_window::cb_update(Address, Address pw) {
    reference_to<Movie_window>(pw).update();
}

void Movie_window::cb_search_by_title(Address, Address pw) {
    reference_to<Movie_window>(pw).search_by_title();
}

void Movie_window::cb_search_by_year(Address, Address pw) {
    reference_to<Movie_window>(pw).search_by_year();
}

void Movie_window::cb_search_by_actor(Address, Address pw) {
    reference_to<Movie_window>(pw).search_by_actor();
}

void Movie_window::cb_search_by_tags(Address, Address pw) {
    reference_to<Movie_window>(pw).search_by_tags();
}

//------------------------------------------------------------------------------

void Movie_window::edit_mode()
{
    search_button.hide();
    display_all_button.hide();
    search_title.hide();
	search_year.hide();
	search_actor.hide();
	search_tag.hide();
	add_button.hide();
	movie_selected.hide();
	edit_button.hide();
    clear_button.hide();
    
    search_by_title_button.hide();
    search_by_year_button.hide();
    search_by_actor_button.hide();
    search_by_tags_button.hide();
	
	add_title.show();
	add_poster.show();
	add_year.show();
	add_actors.show();
	add_tags.show();
    add_scores.show();
	end_edit_button.show();
}

void Movie_window::search_mode()
{
    search_title.show();
    display_all_button.show();
	search_year.show();
	search_actor.show();
	search_tag.show();
	add_button.show();
	search_button.show();
	movie_selected.show();
	edit_button.show();
	search_by_actor_button.show();
    clear_button.show();
    
    search_by_title_button.show();
    search_by_year_button.show();
    search_by_actor_button.show();
    search_by_tags_button.show();
	
	add_title.hide();
	add_poster.hide();
	add_year.hide();
	add_actors.hide();
	add_tags.hide();
    add_scores.hide();
	create_button.hide();
	update_button.hide();
	end_edit_button.hide();
}

//------------------------------------------------------------------------------


void Movie_window::quit()
{
	save_movies_to_file(movie_vec);
	hide();
}

void Movie_window::search()
{
    clear_search();
	int_vector = Search_Movies (search_actor.get_string(), search_title.get_string(),
                                search_year.get_string(), search_tag.get_string(), movie_vec);
    display_movies(0);
}

void Movie_window::display_all()
{
    clear_search();
	int_vector.clear();
    for (int i=0; i<movie_vec.size(); ++i)
        int_vector.push_back(i);
    display_movies(0);
}

void Movie_window::search_by_title()
{
    clear_search();
    int_vector = Search_Movies ("", search_title.get_string(), "", "", movie_vec);
    display_movies(0);
}

void Movie_window::search_by_year()
{
    clear_search();
    int_vector = Search_Movies ("", "", search_year.get_string(), "", movie_vec);
    display_movies(0);
}

void Movie_window::search_by_actor()
{
    clear_search();
    try {
        Actor *actor = (Search_Movies_By_Actor(search_actor.get_string()));
        if ((*actor).photo=="none" || (*actor).names.size()<1)
            return;
        
        stringstream actor_stream;
        for (int i=0; i<(*actor).names.size(); ++i)
            actor_stream << (*actor).names[i] << " ";
        
        int_vector = Search_Movies(actor_stream.str(), "", "", "", movie_vec); // only actor matters
        display_movies(0);
        display_actor(*actor);
    } catch (...) {
        return;
    }
}

void Movie_window::search_by_tags()
{
    clear_search();
    int_vector = Search_Movies ("", "", "", search_tag.get_string(), movie_vec);
    display_movies(0);
}

void Movie_window::add()
{
	edit_mode();
    create_button.show();
    clear_search();
}

void Movie_window::clear()
{
    clear_search();
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
                
                edit_mode();
                update_button.show();
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
    if (m.year==-999999) m.year=0000;
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
            for (int k=0; k<all_tags.size(); ++k) { //check tag is available
                if (all_tags[k]==tag) {
                    bool repeat=false;
                    for (int v=0; v<m.tags.size(); ++v) { //check for repeats
                        if (m.tags[v]==tag) {
                            repeat=true;
                            break;
                        }
                    }
                    if (!repeat) {
                        m.tags.push_back(tag);
                    }
                    break;
                }
            }
			tag="";
		}
	}
}

void Movie_window::update_scores(Movie &m) {
    stringstream score_stream(add_scores.get_string());
    double next_score;
    char break_char;
    while (true) {
        if (score_stream >> next_score) {
            m.scores.push_back(next_score);
        } else if (score_stream) {
            score_stream >> break_char;
            continue;
        } else
            break;
    }
}

//------------------------------------------------------------------------------

void Movie_window::create()
{
    if (add_title.get_string()=="")
        return;
	
    Movie m;
    update_poster(m);
    update_title(m);
    update_year(m);
    update_actors(m);
    update_tags(m);
    update_scores(m);
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
    if (add_scores.get_string()!="") {
        m.scores.clear();
        update_scores(m);
    }
    
    //update display with new information
    display_movies(0);
}

void Movie_window::end_edit()
{
    search_mode();
}

//display methods
//------------------------------------------------------------------------------

// deal with hiding and revealing page navigation buttons
bool Movie_window::can_next() {
    if (int_vector.size()<1) return false;
    return display_index + rows*rows < int_vector.size();
}

bool Movie_window::can_prev() {
    if (int_vector.size()<1) return false;
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
    if (int_vector.size()==0)
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
    
