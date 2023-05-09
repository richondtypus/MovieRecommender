#include "Recommender.h"
#include "UserDatabase.h"
#include "User.h"
#include "Movie.h"
#include "MovieDatabase.h"
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <cstring>
using namespace std;


Recommender::Recommender(const UserDatabase& user_database,
    const MovieDatabase& movie_database) :m_userdata(&user_database), m_moviedata(&movie_database)
{
    // Replace this line with correct code.
}


vector<MovieAndRank> Recommender::recommend_movies(const string& user_email, int movie_count) const
{
    const User* u;
    vector<MovieAndRank>v;
    //if negative act as it were 0
    if (movie_count <= 0) {
        return v;
    }
    u = m_userdata->get_user_from_email(user_email);
    vector<string>movies_watched;
    //unordered sets prevent duplicates
    movies_watched = u->get_watch_history();
    unordered_map<string, int>compatabile_map;
    for (int i = 0; i < movies_watched.size(); i++) {
        unordered_set<string>actors_watched;
        unordered_set<string>directors_watched;
        unordered_set<string>genres_watched;
        const Movie* m = m_moviedata->get_movie_from_id(movies_watched[i]);
        vector<string>actors = m->get_actors();
        for (int k = 0; k < actors.size(); k++) {
            actors_watched.insert(actors[k]);
        }
        vector<string>directors = m->get_directors();
        for (int k = 0; k < directors.size(); k++) {
            directors_watched.insert(directors[k]);
        }
        vector<string>genres = m->get_genres();
        for (int k = 0; k < genres.size(); k++) {
            genres_watched.insert(genres[k]);
        }

        for (auto it = actors_watched.begin(); it != actors_watched.end(); it++) {
            vector<Movie*>movies_with_actor = m_moviedata->get_movies_with_actor(*it);
            for (int i = 0; i < movies_with_actor.size(); i++) {
                //if user has already watched don't count
                if (alreadywatched(movies_with_actor[i]->get_id(), movies_watched)) {
                    continue;
                }
                else {
                    unordered_map<string, int>::const_iterator got = compatabile_map.find(movies_with_actor[i]->get_id());
                    if (got == compatabile_map.end()) {
                        //insert new movie
                        compatabile_map.insert(make_pair(movies_with_actor[i]->get_id(), 30));
                    }
                    else {
                        //add 30 to the total score
                        compatabile_map[movies_with_actor[i]->get_id()] += 30;
                    }
                }
            }
        }

        //check for directors
        for (auto it = directors_watched.begin(); it != directors_watched.end(); it++) {
            vector<Movie*>movies_with_director = m_moviedata->get_movies_with_director(*it);
            for (int i = 0; i < movies_with_director.size(); i++) {
                if (alreadywatched(movies_with_director[i]->get_id(), movies_watched)) {
                    continue;
                }
                else {
                    unordered_map<string, int>::const_iterator got = compatabile_map.find(movies_with_director[i]->get_id());
                    if (got == compatabile_map.end()) {
                        //insert new movie
                        compatabile_map.insert(make_pair(movies_with_director[i]->get_id(), 20));
                    }
                    else {
                        //add 20 to the total score
                        compatabile_map[movies_with_director[i]->get_id()] += 20;
                    }

                }
            }
        }
        //check for genres
        for (auto it = genres_watched.begin(); it != genres_watched.end(); it++) {
            vector<Movie*>movies_with_genre = m_moviedata->get_movies_with_genre(*it);
            for (int i = 0; i < movies_with_genre.size(); i++) {
                if (alreadywatched(movies_with_genre[i]->get_id(), movies_watched)) {
                    continue;
                }
                else {
                    unordered_map<string, int>::const_iterator got = compatabile_map.find(movies_with_genre[i]->get_id());
                    if (got == compatabile_map.end()) {
                        //insert new movie
                        compatabile_map.insert(make_pair(movies_with_genre[i]->get_id(), 1));
                    }
                    else {
                        //add 1 to existing score
                        compatabile_map[movies_with_genre[i]->get_id()] += 1;
                    }
                }
            }
        }
    }

    //having created the ordered sets let us now search them

    vector<pair<string, int>> top_movie_helper(movie_count);
    partial_sort_copy(compatabile_map.begin(), compatabile_map.end(),
        top_movie_helper.begin(), top_movie_helper.end(),
        [](const pair<string, int>& a, const pair<string, int>& b)
        {
            return b.second < a.second;
        });
    int lowest_comptability = top_movie_helper[movie_count - 1].second;

    //add remaining movies of same lowest_compatability rating to vector
    int count = count_if(compatabile_map.begin(), compatabile_map.end(),
        [&](auto const& elem) {
            return elem.second == lowest_comptability &&
                find_if(top_movie_helper.begin(), top_movie_helper.end(),
                    [&](auto const& p) { return p.first == elem.first; })
                == top_movie_helper.end();
        });
    //now create final vector that includes all compatability scores
    vector<pair<string, int>> top_movies(movie_count + count);
    partial_sort_copy(compatabile_map.begin(), compatabile_map.end(),
        top_movies.begin(), top_movies.end(),
        [](const pair<string, int>& a, const pair<string, int>& b)
        {
            return b.second < a.second;
        });
    //now copy unordered set to vector
    //now sort movies with equal compatability scores based on rating
    for (int i = 0; i < top_movies.size() - 1; i++) {
        for (int k = i + 1; k < top_movies.size(); k++) {
            if (top_movies[i].second == top_movies[k].second) {
                if (m_moviedata->get_movie_from_id(top_movies[k].first)->get_rating() > m_moviedata->get_movie_from_id(top_movies[i].first)->get_rating()) {
                    //swap positions
                    pair<string, int>temp = top_movies[i];
                    top_movies[i] = top_movies[k];
                    top_movies[k] = temp;
                }

            }
        }
    }
    //finaly sort movies with equal rating by alphabetical order
    for (int i = 0; i < top_movies.size() - 1; i++) {
        for (int k = i + 1; k < top_movies.size(); k++) {
            if ((top_movies[i].second == top_movies[k].second) &&
                (m_moviedata->get_movie_from_id(top_movies[i].first)->get_rating() == m_moviedata->get_movie_from_id(top_movies[k].first)->get_rating()) &&
                (strcmp(top_movies[i].first.c_str(), top_movies[k].first.c_str()) > 0)) {
                //swap positions
                pair<string, int>temp = top_movies[i];
                top_movies[i] = top_movies[k];
                top_movies[k] = temp;
            }

        }
    }

    //now add to movie rank vector
    for (int i = 0; i < movie_count; i++) {
        v.push_back(MovieAndRank(top_movies[i].first, top_movies[i].second));
    }

    return v;
}


