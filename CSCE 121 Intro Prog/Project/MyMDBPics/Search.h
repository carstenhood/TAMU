
//Search.h
//contains functions for search and search by actor

#ifndef project_Search_h
#define project_Search_h

//------------------------------------------------------------------------------

//search by actor function
Actor* Search_Movies_By_Actor(string Actors, const vector<Movie>& all_movies)
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
                for (int b=0; b<all_movies.size(); ++b) {      //Checks movies
                    for (int z=0; z<all_movies[b].actors.size(); ++z) {       //Checks EACH actor
                        for (int q=0; q<all_movies[b].actors[z].names.size(); ++q) {		//Checks EACH name
                            if ((all_movies[b].actors[z].names[q]) == actor_names[a]) {
                                Actor *actor = new Actor(all_movies[b].actors[z]);
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
//returns a vector<int> with indexes of resulting movies corresponding to all_moviestor
vector<int> Search_Movies(string Actors, string Title, string Yearstring,
                          string Tags, const vector<Movie>& all_movies)
{
    vector<int> int_vector;     //stores the indexes in all_movies of all relevant movies
    try
    {
        while(true)
        {
            if(Yearstring == "")
                break;
            istringstream buffer(Yearstring);
            int Year;
            buffer >> Year;
            
            for (int i=0; i<all_movies.size(); ++i)	//Adds matching movies to the vector of ints
                if (all_movies[i].year == Year)
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
                    for (int b=0; b<all_movies.size(); ++b) {      //Checks movies
                        for (int z=0; z<all_movies[b].actors.size(); ++z) {       //Checks EACH actor
                            for (int q=0; q<all_movies[b].actors[z].names.size(); ++q) {		//Checks EACH name
                                if ((all_movies[b].actors[z].names[q]) == actor_names[a]) {
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
            
            for (int k=0; k<all_movies.size(); ++k) {
                if (all_movies[k].title == Title)
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
                    for (int b=0; b<all_movies.size(); ++b)	//Adds matching movies to the vector of ints
                    {
                        for (int z=0; z<all_movies[b].tags.size(); ++z)
                        {
                            if (all_movies[b].tags[z] == cur_tag) {
                                int_vector.push_back(b);
                                break;
                            }
                        }
                    }
                }
            }
            break;
        }
        
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