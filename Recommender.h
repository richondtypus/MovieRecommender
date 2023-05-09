#ifndef RECOMMENDER_INCLUDED
#define RECOMMENDER_INCLUDED

#include <string>
#include <vector>

using namespace std;

class UserDatabase;
class MovieDatabase;

struct MovieAndRank
{
    MovieAndRank(const std::string& id, int score)
        : movie_id(id), compatibility_score(score)
    {}

    std::string movie_id;
    int compatibility_score;
};

class Recommender
{
public:
    Recommender(const UserDatabase& user_database,
        const MovieDatabase& movie_database);
    std::vector<MovieAndRank> recommend_movies(const std::string& user_email,
        int movie_count) const;
    ~Recommender() {}
private:
    const UserDatabase* m_userdata;
    const MovieDatabase* m_moviedata;
    bool alreadywatched(string s, vector<string>v) const {
        for (int i = 0; i < v.size(); i++) {
            if (s == v[i]) {
                return true;
            }
        }
        return false;
    }
};

#endif // RECOMMENDER_INCLUDED
