#ifndef MOVIE_INCLUDED
#define MOVIE_INCLUDED
using namespace std;

#include <string>
#include <vector>

class Movie
{
  public:
    Movie(const std::string& id, const std::string& title,
          const std::string& release_year,
          const std::vector<std::string>& directors,
          const std::vector<std::string>& actors,
          const std::vector<std::string>& genres, float rating);
    std::string get_id() const;
    std::string get_title() const;
    std::string get_release_year() const;
    float get_rating() const;
    std::vector<std::string> get_directors() const;
    std::vector<std::string> get_actors() const;
    std::vector<std::string> get_genres() const;

  private:
      string m_id;
      string m_title;
      string m_release_year;
      float m_rating;
      vector<string> m_directors;
      vector<string> m_actors;
      vector<string> m_genres;
};

#endif // MOVIE_INCLUDED
