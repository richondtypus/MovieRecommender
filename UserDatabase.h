#ifndef USERDATABASE_INCLUDED
#define USERDATABASE_INCLUDED

using namespace std;
#include "treemm.h"
#include <string>

class User;

class UserDatabase
{
  public:
    UserDatabase();
    ~UserDatabase();
    bool load(const std::string& filename);
    User* get_user_from_email(const std::string& email) const;

  private:
      TreeMultimap<string, User*> usertree;
      vector<string>user_emails;
      void makelower(string& s) {
          for (int i = 0; i < s.size(); i++) {
              s[i] = tolower(s[i]);
          }
      }
};

#endif // USERDATABASE_INCLUDED
