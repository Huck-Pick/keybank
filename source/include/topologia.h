#ifndef __TOPOLOGY__
#define __TOPOLOGY__
#include <string>
#include <vector>
class new_key{
    private:
        std::string site;
        std::string email;
        std::string pass;
    public:
        void set(std::string s, std::string e, std::string p){
            site=s;
            email=e;
            pass=p;
            return;
        }
        std::string get_site(){
            return site;
        }
        std::string get_email(){
            return email;
        }
        std::string get_pass(){
            return pass;
        }
};
class Bank{
    private:
        std::string ID;
        std::string local_pass;
        std::string master_pass;
        std::vector <new_key> keys;
    public:
        void set(std::string name, std::string local, std::string master){
            ID=name;
            local_pass=local;
            master_pass=master;
        }
        std::string get_ID(){
            return ID;
        }
        std::string get_pass_local(){
            return local_pass;
        }
        std::string get_pass_master(){
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
        unsigned int get_keys_size(){
            return static_cast<unsigned int>(keys.size());
        }
        std::string get_key_site(size_t pos){
            return keys[pos].get_site();
        }
        std::string get_key_email(size_t pos){
            return keys[pos].get_email();
        }
        std::string get_key_pass(size_t pos){
            return keys[pos].get_pass();
        }
        void set_key_changes(size_t pos, std::string site_, std::string email_, std::string pass_){
            keys[pos].set(site_,email_, pass_);
            return;
        }
};
#endif