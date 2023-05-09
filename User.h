#ifndef USER_INCLUDED
#define USER_INCLUDED

#include <string>
#include <vector>


using namespace std;

class User
{
  public:
    User(const std::string& full_name, const std::string& email,
         const std::vector<std::string>& watch_history);
    std::string get_full_name() const;
    std::string get_email() const;
    std::vector<std::string> get_watch_history() const;

  private:
      string m_name;
      string m_email;
      vector<string>movie_history;

};

#endif // USER_INCLUDED
