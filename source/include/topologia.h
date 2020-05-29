#include <iostream>
#include <vector>

using namespace std;
typedef struct new_key{
    string site;
    string email;
    string pass;
    void set(string s, string e, string p){
        site=s;
        email=e;
        pass=p;
        return;
    }
    string get_site(){
        return site;
    }
    string get_email(){
        return email;
    }
    string get_pass(){
        return pass;
    }
}new_key;

typedef struct Bank{
    string ID;
    string local_pass;
    string master_pass;
    vector <new_key> keys;
    void set(string name, string local, string master){
        ID=name;
        local_pass=local;
        master_pass=master;
    }
    string get_ID(){
        return ID;
    }
    string get_pass_local(){
        return local_pass;
    }
    string get_pass_master(){
        return master_pass;
    }
    void set_key(new_key input){
        keys.push_back(input);
        return;
    }
    void erase_key(int poss){
        if (poss < 0) 
            return;
        if (poss>keys.size())
            return;
        keys.erase(keys.begin()+poss-1);
        return;
    }
    void pop_key(){
        keys.pop_back();
        return;
    }
    
}Bank;