#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>

using namespace std;
string path;

#include ".\include\sha256.h"
#include ".\include\shuffle.h"
#include ".\include\shadouble.h"
#include ".\include\chaos.h"
#include ".\include\stk_file.h"
#include ".\include\topologia.h"
#include ".\include\support.h"

string init=            "BB54068AEA85FAA7E487530083366BE9962390AF822E4C71EF1ACA7033C83E66";
string add=             "7E9E5AC30F2216FD0FD6F5FAED316F2D5983361A4203C3330CFA46EF65BB4767";
string rm=              "58466EBDD352F801198118E294E38715F864985FD87977F348BFCD7DB62E7C76";
string edit=            "262121C5372BE8AF3AE6FF0D3D138D9E6E1249335222C7F0E02535E35073BB0B";
string clip=            "67905AD3CC2DD52B1F5F6A6D2814DE0396618B29B4238B9AF5207AEB69936E6D";
string edit_u=          "75C176A60370BAE8AD556E416FF53722E61ADF01A54A5FEBFE31E7C49FD443A9";
string erase_user=      "1BDFD402BF43077829B6CFF964641FD710CC304BE35B73EDAEE8FD6A2B594ECB";

int main(int argc, char *argv[]){
    if (argc==1){
        cout_menu();
        return 0;
    }
    path=get_current_path(argv[0]);
    path+="\\Bank\\";
    if (sha256(argv[1])==init){
        create_bank();
        return 0;
    }
    string file_name=path;
    file_name+=sha256(argv[1]);
    file_name+=".stk";
    if (file_exists(file_name)){
        if (argc<=2){
            opciones();
            return 0;
        }
        string op=sha256(argv[2]);
        if (op!=add && op!=edit && op !=rm && op!=clip &&op!=erase_user && op!=edit_u){
            opciones();
            return 0;
        }
        Bank *USER;
        USER=bank_from_file(file_name);
            if (op==erase_user){
                erase_user_bank(USER,file_name);
                free(USER);
                return 0;
            }
            if (op==add){
                add_key_to_bank(USER);
            }
            if (op==edit){
                edit_key_from_bank(USER);
            }
            if (op==rm){
                rm_key_from_bank(USER);
            }
            if (op==clip){
                key_bank_to_clip(USER);
            }
            if (op==edit_u){
                file_name=edit_user_bank(USER,file_name);
            }
        save_bank(file_name,USER);
        free(USER);
        return 0;
    }
    else{
        cout<<endl<<"Usuario no existente, puede crearlo con\n\n>.\\keybank.exe init"<<endl<<endl;
        return 0;
    }
    return 0;
}