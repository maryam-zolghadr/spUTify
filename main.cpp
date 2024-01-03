#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#define REQUEST_ERR "Bad Request"
#define NOT_FOUND "Not Found"
#define PERMISSION_DENIED "Permission Denied"
#define EMPTY "Empty"
using namespace std;
class Song
{
public:
    vector<string> song_info()
    {
        string tmp = to_string(song_id);
        vector<string> info(4);
        info[0] = tmp;
        info[1] = this->song_name;
        info[2] = this->artist;
        return info;
    }
    vector<string> get_full_info()
    {
        vector<string> info(8);
        string tmp, tmp2;
        tmp = to_string(song_id);
        tmp2 = to_string(year);
        info[0] = tmp;
        info[1] = this->song_name;
        info[2] = this->artist;
        info[3] = tmp2;
        info[4] = this->album;
        info[5] = this->tmp_tag;
        info[6] = this->duration;
        return info;
    }
    void set_song(const string& title, const string& path, const string& year, const string& album, const string& tags, const string& s_duration, const int& id, const string& artist)
    {
        this->song_name = title;
        this->song_id = id;
        this->year = stoi(year);
        this->path = path;
        this->album = album;
        this->artist = artist;
        stringstream ss(tags);
        string token;
        this->tmp_tag = tags;
        while (getline(ss, token, ';')) 
        {
            this->tags.push_back(token);
        }
        this->duration = s_duration; // edit this
    }
    int get_id() {return song_id;};
    private:
    string tmp_tag;
    string song_name;
    string artist;
    int song_id;
    int year;
    string album;
    string duration;
    string path;
    vector<string> tags;
};

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
    vector<string> user_info()
    {
        string tmp = to_string(tot_song_o_playlist());
        vector<string> info(5);
        info[0] = to_string(user_id);
        info[1] = this->mode;
        info[2] = this->user_name;
        info[3] = tmp;
        return info;
    }
    string user_name;
    int user_id;
    string mode;
    int status = 0;
    string get_password() {return password;};
    void set_password(string password) {this -> password = password;};
    vector<string> user_info_to_print()
    {
        vector<string> res(4); 
        res[0] = to_string(this->user_id); 
        res[1] = this->mode;
        res[2] = this->user_name;
        res[3] = to_string(this->tot_song_o_playlist()); 
        return res;
    }
    string song_O_playlist;
    virtual int tot_song_o_playlist() = 0;
private:
    string password;
};

class RegularUsr : public User
{
public:
    RegularUsr() {song_O_playlist = "Playlists";}
    int tot_song_o_playlist() override
    {
        return playlist_num;
    }
private:
    int playlist_num = 0;
};

class Artist : public User
{
public:
    Artist() {song_O_playlist = "Songs";}
    Song add_song(const string& title, const string& path, const string& year, const string& album, const string& tags, const string& duration, const int& id)
    {
        Song song;
        song.set_song(title, path, year, album, tags, duration, id, user_name);
        singer_songs.push_back(song);
        tot_songs += 1;
        return song;
    }
    int tot_song_o_playlist() override
    {
        return tot_songs;
    }
private:
    int tot_songs = 0;
    vector<Song> singer_songs;
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
                throw REQUEST_ERR;
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
            throw REQUEST_ERR;
        }
        new_user->set_new_user(user_name, password, mode, next_user_id);
        next_user_id += 1;
        new_user->status = 1;
        users.push_back(new_user);
        current_user = new_user;
        throw string("OK");
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
                    user->status = 1;
                    current_user = user;
                    throw string("OK");
                }
            }
        }
        if(tmp == 0)
        {
            throw NOT_FOUND;
        }
        if(tmp == 1)
        {
            throw PERMISSION_DENIED;
        }
    }

    void log_out()
    {
        current_user = nullptr;
        throw string("OK");
    }


    void handle_read_input(string input) 
    {
        tokens.clear();
        string block = "";
        bool in_block = false;
        for (char c : input) 
        {
            if (c == '<') 
            {
                in_block = true;
            } else if (c == '>') 
            {
                in_block = false;
                tokens.push_back(block);
                block = "";
            } else if (c == ' ' && !in_block) 
            {
                if (block != "") 
                {
                    tokens.push_back(block);
                    block = "";
                }
            } 
            else 
            {
                block += c;
            }
        }
        if (block != "") 
        {
            tokens.push_back(block);
        }
    }

    string declare_variables(string variable)
    {
        for(int i = 0; i < tokens.size(); i++)
        {
            if(tokens[i] == variable)
            {
                if(i + 1 != tokens.size())
                {
                    return tokens[i + 1];
                }
            }
        }
        throw REQUEST_ERR;
    }
    void handle_command_post_type(const string& order)
    {
        if(order == "logout")
        {
            if(current_user == nullptr)
            {
                throw PERMISSION_DENIED;
            }
            log_out();
        }

        if(order == "music")
        {
            if(current_user == nullptr)
            {
                throw PERMISSION_DENIED;
            }
            if(current_user->mode != "artist")
            {
                throw PERMISSION_DENIED;
            }
            add_music();
        }

        if(order == "signup")
        {
            if(current_user != nullptr)
            {
                throw PERMISSION_DENIED;
            }
            sign_up(declare_variables("username"), declare_variables("password"), declare_variables("mode"));
        }

        if(order == "login")
        {
            if(current_user != nullptr)
            {
                throw PERMISSION_DENIED;
            }        
            login(declare_variables("username"), declare_variables("password"));
        }
    }
    void add_music()
    {
        if (dynamic_cast<Artist*>(current_user) != nullptr) 
        {
            Artist* artist = dynamic_cast<Artist*>(current_user);
            Song tmp_song = artist->add_song(declare_variables("title"), declare_variables("path"), declare_variables("year"), declare_variables("album"), declare_variables("tags"), declare_variables("duration"), next_song_id);
            next_song_id += 1;
            songs.push_back(tmp_song);
            throw string("OK");
        }
    }
    void handle_command_get_type(const string& order)
    {
        bool all = true;
        if(current_user == nullptr)
        {
            throw PERMISSION_DENIED;
        }
        if(tokens.size() > 3)
        {
            all = false;
            int id = stoi(declare_variables("id"));
        }
        if(order == "musics")
        {
            if(all == true)
            {
                print_all_songs();
            }
            else
            {
                print_wanted_song(declare_variables("id"));
            }
        }
        if(order == "users")
        {
            if(users.size() == 0)
                throw EMPTY;
            if(all == true)
            {
                print_all_users();
            }
            else
            {

            }
        }
    }

    void print_wanted_song(const string& id)
    {
        int check = 0;
        cout << "ID, Name, Artist, Year, Album, Tags, Duration" << endl;
        for(Song& song : songs)
        {
            if(song.get_id() == stoi(id))
            {
                check = 1;
                vector<string> info = song.get_full_info();
                for(int i = 0; i < info.size() - 1; i++)
                {
                    cout << info[i];
                    if(i != info.size() - 2)
                        cout << ", ";
                }
                cout << endl;
                return;
            }
        }
        throw NOT_FOUND;
    }
    void process_command()
    {
        int check = 0;
        if(tokens[0] == "POST")
        {
            for(const string& order : post_orders)
            {
                if(tokens[1] == order)
                {
                    check = 1;
                    handle_command_post_type(order);
                }
            }
            if(check == 0)
            {
                throw NOT_FOUND;
            }
        }

        else if(tokens[0] == "GET")
        {
            for(const string& order : get_orders)
            {
                if(tokens[1] == order)
                {
                    check = 1;
                    handle_command_get_type(order);
                }
            }   
            if(check == 0)
            {
                throw NOT_FOUND;
            }    
        }

        else if(tokens[0] == "DELETE")
        {

        }

        else if(tokens[0] == "PUT")
        {

        }

        else
            throw REQUEST_ERR;
    }
    // void print_user(int id)
    // {
    //     int n = id - 1;
    //     cout << "ID: " << id;
    //     cout << "Mode: " << users[n]->mode;
    //     cout << "Username: " << users[n]->user_name;
    //     cout << users[n]->song_O_playlist << ": ";
    // }
    void print_all_users()
    {
        cout << "ID, Mode, Username, Playlists_number/Songs_number" << endl;
        for(User* user : users)
        {
            vector<string> info = user->user_info();
            for(int i = 0; i < info.size() - 1; i++)
            {
                cout << info[i];
                if(i != info.size() - 2)
                {
                    cout << ", ";
                }
            }
            cout << endl;
        }
    }
    void print_all_songs()
    {
        cout << "ID, Name, Artist" << endl;
        for(auto& song : songs)
        {
            vector<string> info = song.song_info();
            for(int i = 0; i < info.size() - 1; i++)
            {
                cout << info[i];
                if(i != info.size() - 2)
                {
                    cout << ", ";
                }
            }
            cout << endl;
        }
    }

    void set_orders()
    {
        post_orders.push_back("signup");
        post_orders.push_back("login");
        post_orders.push_back("logout");
        post_orders.push_back("playlist");
        post_orders.push_back("music");

        get_orders.push_back("musics");
        get_orders.push_back("users");

    }
    
    void run(const string& command)
    {
        if(command == " ")
            return;
        handle_read_input(command);
        try
        {
            process_command();
        }
        catch(const string& EX_msg)
        {
            cout << EX_msg << endl;
        }
        catch(const char* ex)
        {
            cout << ex << endl;
        }
        catch(...)
        {
            cout << "An unexpected error occurred." << endl;
        }
        for(int i = 0; i < users.size(); i++)
        {
            cout << users[i]->user_name << " " << users[i]->get_password() << " " << users[i]->user_id << " " << users[i]->mode << endl;
            if (current_user != nullptr)
            cout << "----------- " << current_user->user_name << " -----------" << endl;
        }
        if(songs.size() != 0)
        {
                for(int i = 0; i < songs.size(); i++)
                {               
                    vector<string> shit = songs[i].song_info();
                    cout << songs[i].song_info()[0] << " " << songs[i].song_info()[1] << " "  << songs[i].song_info()[2] <<  endl;
                    cout << "-------------------------" << endl;
                }
        }
    }
private:
    string command_type;
    int song_id;
    vector<User*> users;
    int next_user_id = 1;
    int next_song_id = 1;
    User* current_user = nullptr;
    vector<string> post_orders;
    vector<string> get_orders;
    vector<string> delete_orders;
    vector<string> put_ordres;
    vector<Song> songs;
    vector<string> tokens;
};
 
int main(int argc, char* argv[])
{
    string command;
    CommandManagement command_management;
    command_management.set_orders();
    while(true)
    {
        getline(cin, command);
        command_management.run(command);
        if(command.empty())
        {
            return 0;
        }
    }
}
