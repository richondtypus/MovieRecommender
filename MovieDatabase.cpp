#include "MovieDatabase.h"
#include "Movie.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

MovieDatabase::MovieDatabase()
{
    
}

MovieDatabase::~MovieDatabase() {
    for (int i = 0; i < ids.size(); i++) {
        TreeMultimap<string, Movie*>::Iterator it = id_tree.find(ids[i]);
        while (it.is_valid()) {
            delete it.get_value();
            it.get_value() = nullptr;
            it.advance();
        }
    }
    for (int i = 0; i < directors.size(); i++) {
        TreeMultimap<string, Movie*>::Iterator it = director_tree.find(directors[i]);
        while (it.is_valid()) {
            delete it.get_value();
            it.get_value() = nullptr;
            it.advance();
        }
    }
    for (int i = 0; i < actors.size(); i++) {
        TreeMultimap<string, Movie*>::Iterator it = actor_tree.find(actors[i]);
        while (it.is_valid()) {
            delete it.get_value();
            it.get_value() = nullptr;
            it.advance();
        }
    }
    for (int i = 0; i < genres.size(); i++) {
        TreeMultimap<string, Movie*>::Iterator it = genre_tree.find(genres[i]);
        while (it.is_valid()) {
            delete it.get_value();
            it.get_value() = nullptr;
            it.advance();
        }
    }
}

bool MovieDatabase::load(const string& filename)
{
    ifstream f(filename);
    if (!f) {
        return false;
    }
    
    //while not end of file
    vector<string>director_list;
    vector<string>actor_list;
    vector<string>genre_list;
    float r;
    while (!f.eof()) {
        string id, title, year, director, actor, genre, rating, skip;
        director_list.clear();
        actor_list.clear();
        genre_list.clear();
        getline(f, id);
        if (id == "") {
            return true;
        }
        makelower(id);
        getline(f, title);
        //get directors
        getline(f, year);
        getline(f, director);
        makelower(director);
        stringstream ssd(director);
        while (ssd.good()) {
            string substr;
            getline(ssd, substr, ',');
            director_list.push_back(substr);
        }
        getline(f, actor);
        makelower(actor);
        stringstream ssa(actor);
        while (ssa.good()) {
            string substr;
            getline(ssa, substr, ',');
            actor_list.push_back(substr);
        }
        getline(f, genre);
        makelower(genre);
        stringstream ssg(genre);
        while (ssg.good()) {
            string substr;
            getline(ssg, substr, ',');
            genre_list.push_back(substr);
        }
        getline(f, rating);
        r = stof(rating);
        //create id_tree
        id_tree.insert(id, new Movie(id, title, year, director_list, actor_list, genre_list, r));
        ids.push_back(id);
        //create director_tree
        for (int i = 0; i < director_list.size(); i++) {
            director_tree.insert(director_list[i], new Movie(id, title, year, director_list, actor_list, genre_list, r));
            directors.push_back(director_list[i]); 
        }
        //create actor_tree
        for (int i = 0; i < actor_list.size(); i++) {
            actor_tree.insert(actor_list[i], new Movie(id, title, year, director_list, actor_list, genre_list, r));
            actors.push_back(actor_list[i]); 
        }
        //create genre tree
        for (int i = 0; i < genre_list.size(); i++) {
            genre_tree.insert(genre_list[i], new Movie(id, title, year, director_list, actor_list, genre_list, r));
            genres.push_back(genre_list[i]); 
        }
        //skip last line
        getline(f, skip);
    }

    f.close();
    return true;
}

Movie* MovieDatabase::get_movie_from_id(const string& id) const
{
    string id_temp = lowercasecopy(id);
    TreeMultimap<string, Movie*>::Iterator it = id_tree.find(id_temp);
    if (it.is_valid()) {
        return it.get_value();
    }

    return nullptr;
}

vector<Movie*>MovieDatabase::get_movies_with_director(const string & director) const
{
    string director_temp = lowercasecopy(director);
    vector<Movie*>movies_with_director;
    TreeMultimap<string, Movie*>::Iterator it = director_tree.find(director_temp);
    while (it.is_valid()) {
        movies_with_director.push_back(it.get_value());
        it.advance();
    }
    return movies_with_director;
    
}

vector<Movie*> MovieDatabase::get_movies_with_actor(const string& actor) const
{
    string actor_temp = lowercasecopy(actor);
    vector<Movie*>movies_with_actor;
    TreeMultimap<string, Movie*>::Iterator it = actor_tree.find(actor_temp);
    while (it.is_valid()) {
        movies_with_actor.push_back(it.get_value());
        it.advance();
    }
    return movies_with_actor;
}

vector<Movie*> MovieDatabase::get_movies_with_genre(const string& genre) const
{
    string genre_temp = lowercasecopy(genre);
    vector<Movie*>movies_with_genre;
    TreeMultimap<string, Movie*>::Iterator it = genre_tree.find(genre_temp);
    while (it.is_valid()) {
        movies_with_genre.push_back(it.get_value());
        it.advance();
    }
    return movies_with_genre;
}
