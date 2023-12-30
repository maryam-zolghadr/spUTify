#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#define REQUEST_ERR "Bad Request"
using namespace std;
class User
{
public:
    void set_new_user(const string& user_name, const string& password, const string& mode, const int&id)
    {
        this ->user_name = user_name;
        this ->set_password(password);
        this ->mode = mode;
        this ->user_id = id;
        this ->status = 1;        
    }
    string user_name;
    int user_id;
    string mode;
    int status = 0;
    string get_password() {return password;};
    void set_password(string password) {this -> password = password;};
private:
    string password;
};

class RegularUsr : public User
{

};

class Artist : public User
{

};

class Song
{
public:
    void print_song()
    {
        cout << song_id << ", " << song_name << ", " << artist << endl;
    }
private:
    string song_name;
    string artist;
    int song_id;
};

class CommandManagement
{
public:

    void sign_up(const string& user_name, const string& password, const string& mode)
    {
        for(const auto& user : users)
        {
            if(user_name == user->user_name)
            {
                cout << REQUEST_ERR << endl;
                return;
            }
        }
        User* new_user;
        if(mode == "user")
        {
            new_user = new RegularUsr;
        }
        else if(mode == "artist")
        {
            new_user = new Artist;
        }
        else
        {
            cout << REQUEST_ERR << endl;
            return;
        }
        new_user->set_new_user(user_name, password, mode, next_user_id);
        next_user_id += 1;
        new_user->status = 1;
        users.push_back(new_user);
        current_user = new_user;
        cout << "OK" << endl;
    }

    void login(const string& user_name, const string& password)
    {
        int tmp = 0;
        for(const auto& user : users)
        {
            if(user->user_name == user_name)
            {
                tmp = 1;
                if(user->get_password() == password)
                {
                    tmp = 2;
                    if(user->status == 0)
                    {
                        user->status = 1;
                        current_user = user;
                        cout << "OK" << endl;
                        return;
                    }
                }
            }
        }
        if(tmp == 0)
        {
            cout << "Not Found" << endl;
            return;
        }
        if(tmp == 1 || tmp == 2)
        {
            cout << "Permission Denied" << endl;
            return;
        }
    }

    void log_out()
    {
        if(current_user == nullptr)
        {
            cout << "Permission Denied" << endl;
            return;
        }
        current_user = nullptr;
    }

    void handle_read_input(const string& command)
    {
        istringstream iss(command);
        vector<string> tokens;
        string token;
        while (getline(iss, token, ' ')) 
        {
            tokens.push_back(token);
        }        
        handle_input(tokens);
    }
    void handle_command_passing(string order, vector<string> tokens)
    {
        if(order == "signup")
        {
            
        }
    }
    void handle_input(vector<string> tokens)
    {
        if(tokens[0] == "POST")
        {
            for(const string& order : post_orders)
            {
                if(tokens[1] == order)
                {
                    handle_command_passing(order, tokens);
                }
            }
        }
        else if(tokens[0] == "GET")
        {

        }
        else if(tokens[0] == "DELETE")
        {

        }
        else if(tokens[0] == "PUT")
        {

        }
        else
            cout << "Permission Denied" << endl;

    }

    void print_all_songs()
    {
        for(auto& song : songs)
        {
            song.print_song();
        }
    }

    void set_orders()
    {
        post_orders.push_back("signup");
        post_orders.push_back("login");
        post_orders.push_back("logout");
        post_orders.push_back("playlist");
        post_orders.push_back("music");
    }
private:
    string command_type;
    int song_id;
    vector<User*> users;
    int next_user_id = 1;
    User* current_user = nullptr;
    vector<string> post_orders;
    vector<string> get_orders;
    vector<string> delete_orders;
    vector<string> put_ordres;
    vector<Song> songs;
};
 
int main(int argc, char* argv[])
{
    string command;
    CommandManagement command_management;
    command_management.set_orders();
    while(true)
    {
        getline(cin, command);
        command_management.handle_read_input(command);
        if(command.empty())
        {
            return 0;
        }
    }
}