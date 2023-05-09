#include "UserDatabase.h"
#include "User.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

UserDatabase::UserDatabase()
{
    
}

UserDatabase::~UserDatabase() {
    for (int i = 0; i < user_emails.size(); i++) {
        TreeMultimap<string, User*>::Iterator it = usertree.find(user_emails[i]);
        while (it.is_valid()) {
            delete it.get_value();
            it.get_value() = nullptr;
            it.advance();
        }
    }
}

bool UserDatabase::load(const string& filename)
{
    
    ifstream f(filename);
    if (!f) {
        return false;
    }

    //while not end of file
    while(!f.eof()) {
        string name, email, number, id, skip;
        getline(f, name);
        getline(f, email);
        getline(f, number);
        int n = stoi(number);
        vector<string>moviesWatched;
        for (int i = 0; i < n; i++) {
            getline(f, id);
            makelower(id);
            moviesWatched.push_back(id);
        }
        usertree.insert(email, new User(name, email, moviesWatched));
        user_emails.push_back(email);
        //skip last line
        getline(f, skip);

    }
    f.close();
    return true;
}

User* UserDatabase::get_user_from_email(const string& email) const
{
    TreeMultimap<string, User*>::Iterator it = usertree.find(email);
    if (it.is_valid()) {
        return it.get_value();
    }
    
    return nullptr;
}
