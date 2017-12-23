#include "std_lib_facilities_3.h"
#include"Graph.h"   
#include"GUI.h"
#include"Window.h"
#include"Point.h"
using namespace Graph_lib;
using namespace std;



struct Actor {
    vector<string> names;
    string photo;
};

struct Movie {					//THIS REQUIRES A COPY CONSTRUCTOR IN ORDER TO WORK.
    string title;
    string poster;
    int year;
    vector<Actor> actors;
    vector<string> tags;
    vector<double> scores;
	
	Movie(){};
	
	Movie(string t, string p, int y){
		title=t;
		poster=p;
		year=y;
	};
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
//SEARCH

vector<Movie> movie_vec=get_movies_from_file();

//--------------------------------------------------------------------------------------------------------------------
//This search returns a vector containing integers representing all movies
//--------------------------------------------------------------------------------------------------------------------
vector<int> Search_Movies (string Actors, string Title, string Yearstring, string Tags, const vector<Movie>& movie_vector) {
try
{
	vector<int> int_vector;				//Stores the number of all relevant movies

	while(true)
	{
		if(Yearstring == "")
			break;
		istringstream buffer(Yearstring);
		int Year;								//Converts to int
		buffer >> Year;
		if ((Year<=1700)||(Year>=2100))			//Checks if year is valid
			error("Invalid Year");
		for (int i=0; i<movie_vector.size(); ++i)			//Adds matching movies to the vector of ints
			if (movie_vector[i].year == Year)
				int_vector.push_back(i);
		break;


	}

			

	//leave actors as strings
		//Check which movies in the vector contain those actors
	while (true)
	{
		if(Actors == "")
			break;
		else {
			vector<string> actor_names;
			string cur_name;
			istringstream Actor_stream(Actors);			//Converts the string into a whitespace seperated vector
			while (Actor_stream >> cur_name){
				actor_names.push_back(cur_name);
			}

		for (int a=0; a<actor_names.size(); ++a)			//Checking actors
		{
			for (int b=0; b<movie_vector.size(); ++b)			//Checks movies
			{
				for (int z=0; z<movie_vector[b].actors.size(); ++z){			//Checks EACH actor
					for (int q=0; q<movie_vector[b].actors[z].names.size(); ++q)			//Checks EACH actor
					{
					if ((movie_vector[b].actors[z].names[q]) == actor_names[a]) {	
						int_vector.push_back(b);
						break;		
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

		for (int k=0; k<movie_vector.size(); ++k)
		{
			if (movie_vector[k].title == Title)
				int_vector.push_back(k);
		}
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
				istringstream Tag_stream(Tags);			//Converts the string into a whitespace seperated vector
				while (Tag_stream >> cur_tag){
					tags_vec.push_back(cur_tag);

				for (int b=0; b<movie_vector.size(); ++b)			//Adds matching movies to the vector of ints
				{
					for (int z=0; z<movie_vector[b].tags.size(); ++z)
					{
						for (int q=0; q<movie_vector[b].tags[z].size(); ++q)
						{
						if (movie_vector[b].tags[z] == tags_vec[q]) {				//Needs to be a double depth check to check all actors.
							int_vector.push_back(b);
							break;			
							}
						}
					}					
				}
			}
		break;
		}
	}
	}
	//End of tag checker
	//-----------------------------------------------




	//CHECKS FOR SCORES



	//Check for duplicates in the int vector
	sort(int_vector.begin(), int_vector.end());
	for (int q=0; q<int_vector.size(); ++q)
		if (int_vector[q]==int_vector[q-1])
			int_vector.erase(int_vector.begin()+q-1);					
	return int_vector;
}
catch (exception& e) 
{  
    vector<int> blank_vec;
    return blank_vec;				//Returns a blank vector if an error is thrown
}
}


//------------------------------------------------------------------------------
 
struct Lines_window : Graph_lib::Window {
    Lines_window(Point xy, int w, int h, const string& title );
     
private:
    // Data:
    Vector_ref<Shape> shapes;
    Vector_ref<Point> points;
 
    // Widgets:
	Button clear_button;
	Button edit_button;
    Button quit_button;
    
	Button search_button;
	In_box search_title;
    In_box search_year;
    In_box search_actor;
    In_box search_tag;
	
	Button add_button;
	In_box add_title;
	In_box add_poster;
	In_box add_year;
	In_box add_actors;
	In_box add_tags;
	Button create_button;
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
 
  
    static void cb_select(Address, Address);
    static void cb_search(Address, Address);
	static void cb_add(Address, Address);
	static void cb_clear(Address, Address);
	static void cb_edit(Address, Address);
    static void cb_quit(Address, Address);
	static void cb_create(Address, Address);
	static void cb_cancel(Address, Address);
};
 
//------------------------------------------------------------------------------
 
Lines_window::Lines_window(Point xy, int w, int h, const string& title)
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
	add_poster(Point(300,150), 150, 20, "File name of poster:"),
	add_year(Point(300,200), 150, 20, "Year of creation:"),
	add_actors(Point(300,250), 150, 20, "Enter actor names separated by a comma (,):"),
	add_tags(Point(300,300), 150, 20, "Enter tags separated by a comma (,):"),
	create_button(Point(300,350), 70, 20, "Create", cb_create),
	cancel_button(Point(380,350), 70, 20, "Cancel", cb_cancel),
	
	select_button(Point(70,650), 100, 20, "Select movie", cb_select),
    movie_selected(Point(100,600), 200, 20, "Title of movie:")
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
	
	attach(add_title);
	attach(add_poster);
	attach(add_year);
	attach(add_actors);
	attach(add_tags);
	attach(create_button);
	attach(cancel_button);
	add_title.hide();
	add_poster.hide();
	add_year.hide();
	add_actors.hide();
	add_tags.hide();
	create_button.hide();
	cancel_button.hide();
     
}
 
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

 
void Lines_window::cb_quit(Address, Address pw)    // "the usual"
{  
       reference_to<Lines_window>(pw).quit();
} 

void Lines_window::cb_add(Address, Address pw)
{
		reference_to<Lines_window>(pw).add();
}

void Lines_window::cb_clear(Address, Address pw)
{
		reference_to<Lines_window>(pw).clear();
}
 
void Lines_window::cb_edit(Address, Address pw)
{
		reference_to<Lines_window>(pw).edit();
}

void Lines_window::cb_select(Address, Address pw)
{
    reference_to<Lines_window>(pw).select();
}

void Lines_window::cb_search(Address, Address pw)     // "the usual"
{  
       reference_to<Lines_window>(pw).search();
} 

void Lines_window::cb_create(Address, Address pw)
{
    reference_to<Lines_window>(pw).create();
}

void Lines_window::cb_cancel(Address, Address pw)
{
    reference_to<Lines_window>(pw).cancel();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

 
void Lines_window::quit()
{
    hide();        // curious FLTK idiom for delete window
}
 
void Lines_window::search()
{
	
	Search_Movies (search_actor.get_string(),search_title.get_string(), search_year.get_string(),search_tag.get_string(),movie_vec);
    //run search function
    //for each movie returned, create a new text object with its name
    //on the same vertical level, display a jpeg of the movie poster
 
      redraw();
}

void Lines_window::add()
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
	
	add_title.show();
	add_poster.show();
	add_year.show();
	add_actors.show();
	add_tags.show();
	create_button.show();
	cancel_button.show();

}

void Lines_window::clear()
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
	
	add_title.hide();
	add_poster.hide();
	add_year.hide();
	add_actors.hide();
	add_tags.hide();
	create_button.hide();
	cancel_button.hide();
}

void Lines_window::select()
{


}

void Lines_window::edit()
{


}

void Lines_window::create(){
	Movie m;
	m.title=add_title.get_string();
	m.poster=add_poster.get_string();
	m.year=add_poster.get_int();
	
	string temp=add_actors.get_string();
	string name;
	for(int i=0;i<temp.length();++i){
		if(temp[i]!=',') name+=temp[i];
		if(temp[i]==','){
			//m.actors.push_back(name);
			cout<<name;
			name="";
		}
	}
	
	movie_vec.push_back(m);

}

void Lines_window::cancel(){
	
	search_title.show();
	search_year.show();
	search_actor.show();
	search_tag.show();
	add_button.show();
	search_button.show();
	movie_selected.show();
	select_button.show();
	edit_button.show();
	
	add_title.hide();
	add_poster.hide();
	add_year.hide();
	add_actors.hide();
	add_tags.hide();
	create_button.hide();
	cancel_button.hide();

}
 
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
 
int main()
try {
	movie_vec = get_movies_from_file();
    Lines_window win(Point(0,0),1300,1000,"lines");
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

//------------------------------------------------------------------------------

