#ifndef MOVIEDATABASE_INCLUDED
#define MOVIEDATABASE_INCLUDED

#include "treemm.h"
#include <string>
#include <vector>

class Movie;

class MovieDatabase
{
  public:
    MovieDatabase();
    ~MovieDatabase();
    bool load(const std::string& filename);
    Movie* get_movie_from_id(const std::string& id) const;
    std::vector<Movie*> get_movies_with_director(const std::string& director) const;
    std::vector<Movie*> get_movies_with_actor(const std::string& actor) const;
    std::vector<Movie*> get_movies_with_genre(const std::string& genre) const;

  private:
      TreeMultimap<string, Movie*>id_tree;
      TreeMultimap<string, Movie*>director_tree;
      TreeMultimap<string, Movie*>actor_tree;
      TreeMultimap<string, Movie*>genre_tree;
      vector<string>ids;
      vector<string>directors;
      vector<string>actors;
      vector<string>genres;
      void makelower(string& s) {
          for (int i = 0; i < s.size(); i++) {
              s[i] = tolower(s[i]);
          }
      }
      string lowercasecopy(const string& s) const {
          string temp = "";
          for (int i = 0; i < s.size(); i++) {
              temp += tolower(s[i]);
          }
          return temp;
      }
};

#endif // MOVIEDATABASE_INCLUDED
