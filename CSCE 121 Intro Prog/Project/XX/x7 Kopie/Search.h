
#ifndef project_Search_h
#define project_Search_h

//------------------------------------------------------------------------------

//global variables
vector<Movie> movie_vec;

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

#endif