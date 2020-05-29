#include <windows.h>
#include <stdlib.h>
using namespace std;
string same=            "08B441C927C339CF9997764AAC3280A0037985867F83D26EAD0B0C7DA10EAE63";
bool file_exists(string filename){
  ifstream ifile(filename);
  return ifile.good();
}
void create_bank(){
    system("cls");
    string user_name, user_local_pass, user_master_pass;
    string file_name=path;
    cout<<"<NOMBRE_USUARIO>: ";
    getline(cin,user_name);
    user_name=sha256(user_name);
    file_name+=user_name+".stk";
    if (file_exists(file_name)){
        cout<<"Nombre de usuario existente"<<endl;
        return;
    }
    user_name=add2string(user_name);
    system("cls");
    cout<<"User local pass: ";
    getline(cin,user_local_pass);
    user_local_pass=sha256(user_local_pass);
    string temp_l=user_local_pass;
    user_local_pass=add2string(user_local_pass);
    user_local_pass=crypt_w_local_pass(user_local_pass,temp_l);
    system("cls");
    cout<<"User master pass: ";
    getline(cin,user_master_pass);
    user_master_pass=sha256(user_master_pass);
    system("cls");
    string temp_m=user_master_pass;
    user_master_pass=add2string(user_master_pass);
    user_master_pass=crypt_w_master_pass(user_master_pass,temp_m);
    user_master_pass=crypt_w_local_pass(user_master_pass,temp_l);
    temp_l.clear();
    temp_m.clear();
    stk_file F_stk(m*m*m,1);
    F_stk.clear();
    dat_t c;
    for (size_t i=0; i<F_stk.width(); i++){
        c.first=(unsigned char)user_name[i];
        c.second=(unsigned char)user_local_pass[i];
        c.third=(unsigned char)user_master_pass[i];
        F_stk.set_info(i,0,c);
    }
    user_name.clear();
    user_local_pass.clear();
    user_master_pass.clear();
    c.first=0;
    c.second=0;
    c.third=0;
    F_stk.save_stk_file(file_name);
    return;
}
Bank * bank_from_file(string file_name){
    Bank *data;
    data = new Bank;
    stk_file F_stk(file_name);
    string user,local_pass,master_pass;
    dat_t c;
    user.clear();
    local_pass.clear();
    master_pass.clear();
    for (size_t i=0; i<F_stk.width(); i++){
        F_stk.get_info(i,0,c);
        user+=char(c.first);
        local_pass+=char(c.second);
        master_pass+=char(c.third);
    }
    data->set(user,local_pass,master_pass);
    user.clear();
    local_pass.clear();
    master_pass.clear();
    string site="", email="", pass="";
    for (size_t y=1; y<F_stk.height(); y++){
        new_key key;
        site.clear();
        email.clear();
        pass.clear();
        for (size_t x=0; x<F_stk.width(); x++){
            F_stk.get_info(x,y,c);
            site+=char(c.first);
            email+=char(c.second);
            pass+=char(c.third);
        }
        key.set(site,email,pass);
        data->set_key(key);
    }
    site.clear();
    email.clear();
    pass.clear();
    return data;
}
void save_bank(string file_name, Bank * USER){
    stk_file F_stk(m*m*m,1+USER->keys.size());
    F_stk.clear();
    dat_t c;
    string id, pass_l, pass_m;
    id=USER->get_ID();
    pass_l=USER->get_pass_local();
    pass_m=USER->get_pass_master();
    for (size_t i=0; i<F_stk.width(); i++){
        c.first=(unsigned char)id[i];
        c.second=(unsigned char)pass_l[i];
        c.third=(unsigned char) pass_m[i];
        F_stk.set_info(i,0,c);
    }
    pass_l.clear();
    pass_m.clear();
    string site, email, pass;
    for (size_t y=1; y<F_stk.height(); y++){
        site=USER->keys[y-1].get_site();
        email=USER->keys[y-1].get_email();
        pass=USER->keys[y-1].get_pass();
        for (size_t x=0; x<F_stk.width(); x++){
            c.first=(unsigned char)site[x];
            c.second=(unsigned char)email[x];
            c.third=(unsigned char)pass[x];
            F_stk.set_info(x,y,c);
        }
        site.clear();
        email.clear();
        pass.clear();
    }
    F_stk.save_stk_file(file_name);
}

void add_key_to_bank(Bank *USER){
    system("cls");
    string local_pass, master_pass;
    cout<<"local pass: ";
    getline(cin,local_pass);
    local_pass=sha256(local_pass);
    if( add2string(local_pass)!=decrypt_w_local_pass(USER->get_pass_local(),local_pass)){
        return ;
    }
    system("cls");
    cout<<"master pass: ";
    getline(cin,master_pass);
    master_pass=sha256(master_pass);
    if (crypt_w_master_pass(add2string(master_pass),master_pass)!=decrypt_w_local_pass(USER->get_pass_master(),local_pass)){
        local_pass.clear();
        return;
    }
    system("cls");
    string site, email, pass;
    cout<<"Sitio: ";
    getline(cin, site);
    site=add2string(site);
    site=crypt_w_local_pass(site,local_pass);
    cout<<"Email: ";
    getline(cin, email);
    email=add2string(email);
    email=crypt_w_local_pass(email,local_pass);
    system("cls");
    cout<<"Password: "; 
    getline(cin, pass);
    pass=add2string(pass);
    pass=crypt_w_master_pass(pass,master_pass);
    pass=crypt_w_local_pass(pass,local_pass);
    local_pass.clear();
    master_pass.clear();
    new_key key;
    key.set(site,email,pass);
    USER->set_key(key);
    site.clear();
    email.clear();
    pass.clear();
    system("cls");
    return;
}

void edit_key_from_bank(Bank *USER){
    system("cls");
    string local_pass, master_pass;
    cout<<"local pass: ";
    getline(cin,local_pass);
    local_pass=sha256(local_pass);
    system("cls");
    if( add2string(local_pass)!=decrypt_w_local_pass(USER->get_pass_local(),local_pass)){
        return ;
    }
    for (size_t i=0; i<USER->keys.size(); i++)
        cout<<i+1<<")\t"<<recoverstring( decrypt_w_local_pass( USER->keys[i].get_site(), local_pass) )<<endl;
    cout<<endl<<USER->keys.size()+1<<")\tExit"<<endl;
    int op;
    do{
        cout<<"op: ";
        while (!(cin >> op)){
            cin.clear();
            while (cin.get() != '\n') continue;
            cout<<"op: ";
        }
    }while(op<1 || op>USER->keys.size()+1);

    if (op==USER->keys.size()+1 ){
        system("cls");
        return;
    }
    cout<<"\\= mantiene el mismo valor"<<endl;
    string site, email, pass;
    fflush(stdin);
    cout<<"Sitio: ";
    fflush(stdin);
    getline(cin, site);
    if (sha256(site)==same){
        site=USER->keys[op-1].get_site();
    }
    else{
        site=add2string(site);
        site=crypt_w_local_pass(site,local_pass);
    }
    cout<<"Email: ";
    getline(cin, email);
    if (sha256(email)==same){
        email=USER->keys[op-1].get_email();
    }
    else{
        email=add2string(email);
        email=crypt_w_local_pass(email,local_pass);
    }
    cout<<"Password: "; 
    getline(cin, pass);
    system("cls");
    if (sha256(pass)==same){
        pass=USER->keys[op-1].get_pass();
    }
    else{
        cout<<"master pass: ";
        getline(cin,master_pass);
        master_pass=sha256(master_pass);
        system("cls");
        if (crypt_w_master_pass(add2string(master_pass),master_pass)==decrypt_w_local_pass(USER->get_pass_master(),local_pass)){
            pass=add2string(pass);
            pass=crypt_w_master_pass(pass,master_pass);
            pass=crypt_w_local_pass(pass,local_pass);
            cout<<"Pasword cambiado"<<endl;
        }
        else{
            pass=USER->keys[op-1].get_pass();
        }
    }
    master_pass.clear();
    local_pass.clear();
    USER->keys[op-1].set(site,email,pass);
    site.clear();
    email.clear();
    pass.clear();
    return;
}

void rm_key_from_bank(Bank *USER){
    system("cls");
    string local_pass, master_pass;
    cout<<"local pass: ";
    getline(cin,local_pass);
    local_pass=sha256(local_pass);
    system("cls");
    if( add2string(local_pass)!=decrypt_w_local_pass(USER->get_pass_local(),local_pass)){
        return ;
    }
    for (size_t i=0; i<USER->keys.size(); i++)
        cout<<i+1<<")\t"<<recoverstring( decrypt_w_local_pass( USER->keys[i].get_site(), local_pass) )<<endl;
    cout<<endl<<USER->keys.size()+1<<")\tExit"<<endl;
    int op;
    do{
        cout<<"op: ";
        while (!(cin >> op)){
            cin.clear();
            while (cin.get() != '\n') continue;
            cout<<"op: ";
        }
    }while(op<1 || op>USER->keys.size()+1);
    if (op==USER->keys.size()+1 ){
        system("cls");
        return;
    }
    fflush(stdin);
    system("cls");
    cout<<"master pass: ";
    getline(cin,master_pass);
    master_pass=sha256(master_pass);
    system("cls");
    if (crypt_w_master_pass(add2string(master_pass),master_pass)!=decrypt_w_local_pass(USER->get_pass_master(),local_pass)){
        local_pass.clear();
        cout<<"Error"<<endl;
        return;
    }
    master_pass.clear();
    local_pass.clear();
    USER->erase_key(op);
    system("cls");
    cout<<"Llave borrada"<<endl;
    return;
}
void SetToClipboard(string str){
    const size_t len = str.length()+1;
    HGLOBAL hMem =  GlobalAlloc(GMEM_MOVEABLE, len);
    memcpy(GlobalLock(hMem), str.c_str(), len);
    GlobalUnlock(hMem);
    OpenClipboard(0);
    EmptyClipboard();
    SetClipboardData(CF_TEXT, hMem);
    CloseClipboard();
    return;
}

void key_bank_to_clip(Bank *USER){
    system("cls");
    string local_pass, master_pass;
    cout<<"local pass: ";
    getline(cin,local_pass);
    local_pass=sha256(local_pass);
    system("cls");
    if( add2string(local_pass)!=decrypt_w_local_pass(USER->get_pass_local(),local_pass)){
        return ;
    }
    for (size_t i=0; i<USER->keys.size(); i++)
        cout<<i+1<<")\t"<<recoverstring( decrypt_w_local_pass( USER->keys[i].get_site(), local_pass) )<<endl;
    cout<<endl<<USER->keys.size()+1<<")\tExit"<<endl;
    int op;
    do{
        cout<<"op: ";
        while (!(cin >> op)){
            cin.clear();
            while (cin.get() != '\n') continue;
            cout<<"op: ";
        }
    }while(op<1 || op>USER->keys.size()+1);
    if (op==USER->keys.size()+1 ){
        system ("cls");
        return;
    }
    fflush(stdin);
    system("cls");
    cout<<"master pass: ";
    getline(cin,master_pass);
    master_pass=sha256(master_pass);
    system("cls");
    if (crypt_w_master_pass(add2string(master_pass),master_pass)!=decrypt_w_local_pass(USER->get_pass_master(),local_pass)){
        local_pass.clear();
        return;
    }
    SetToClipboard( recoverstring( decrypt_w_master_pass( decrypt_w_local_pass( USER->keys[op-1].get_pass(),local_pass),master_pass) ) );
    system("cls");
    cout<<"tienes 30 segundos"<<endl<<endl;
    cout<<"Correo ligado:\t"<<recoverstring( decrypt_w_local_pass( USER->keys[op-1].get_email(), local_pass ))<<endl<<endl;
    for (size_t i=30; i>=0; i-=2){
        cout<<i<<":";
        Sleep(2000);
    }
    SetToClipboard("Nada que mostrar");
    system("cls");
    return;
}

void erase_user_bank(Bank *USER,string file_name){
    string local_pass, master_pass;
    cout<<"local pass: ";
    getline(cin,local_pass);
    local_pass=sha256(local_pass);
    if( add2string(local_pass)!=decrypt_w_local_pass(USER->get_pass_local(),local_pass)){
        return ;
    }
    system("cls");
    cout<<"master pass: ";
    getline(cin,master_pass);
    master_pass=sha256(master_pass);
    if (crypt_w_master_pass(add2string(master_pass),master_pass)!=decrypt_w_local_pass(USER->get_pass_master(),local_pass)){
        local_pass.clear();
        return;
    }
    system("cls");
    master_pass.clear();
    local_pass.clear();
    fflush(stdin);
    int op;
    cout<<"Desea continuar? \n1) Si\n2) No"<<endl; 
    do{
        cout<<"op: ";
        while (!(cin >> op)){
            cin.clear();
            while (cin.get() != '\n') continue;
            cout<<"op: ";
        }
    }while(op<1 || op>2);
    fflush(stdin);
    if (op==2){
        return;
    }
    if( remove(file_name.c_str()) != 0 )
        cout<<"Error"<<endl;
    else
        cout<<"Usuario eliminado"<<endl;

    return;
}

string get_current_path(string input){
    size_t op = input.find("keybank.exe");
    string str = input.substr (0,op-1);
    return str;
}

string edit_user_bank(Bank * USER, string file_name){
    system("cls");
    string local_pass, master_pass;
    cout<<"local pass: ";
    getline(cin,local_pass);
    local_pass=sha256(local_pass);
    if( add2string(local_pass)!=decrypt_w_local_pass(USER->get_pass_local(),local_pass)){
        return file_name;
    }
    system("cls");
    cout<<"master pass: ";
    getline(cin,master_pass);
    master_pass=sha256(master_pass);
    if (crypt_w_master_pass(add2string(master_pass),master_pass)!=decrypt_w_local_pass(USER->get_pass_master(),local_pass)){
        local_pass.clear();
        return file_name;
    }
    system("cls");
    remove(file_name.c_str());
    string new_name, new_local_pass, new_master_pass;
    new_name.clear();
    new_local_pass.clear();
    new_master_pass.clear();
    cout<<"\\=  :  mantiene los valores anteriores"<<endl;
    cout<<"<USER_NAME>: ";
    getline(cin,new_name);
    
    if (sha256(new_name)==same){
        file_name=recoverstring(USER->get_ID());
        new_name=USER->get_ID();
    }
    else{
        file_name=sha256(new_name);
        new_name=add2string(sha256(new_name));
    }
    cout<<"New local pass: ";
    getline(cin,new_local_pass);
    new_local_pass=sha256(new_local_pass);
    if (new_local_pass==same){
        new_local_pass=local_pass;
    }
    cout<<"New master pass: ";
    getline(cin,new_master_pass);
    new_master_pass=sha256(new_master_pass);
    if (new_master_pass==same){
        new_master_pass=master_pass;
    }
    system("cls");
    string temp_l, temp_m;
    temp_l=new_local_pass;
    new_local_pass=add2string(new_local_pass);
    new_local_pass=crypt_w_local_pass(new_local_pass,temp_l);

    temp_m=new_master_pass;
    new_master_pass=add2string(new_master_pass);
    new_master_pass=crypt_w_master_pass(new_master_pass,temp_m);
    new_master_pass=crypt_w_local_pass(new_master_pass,temp_l);

    USER->set(new_name, new_local_pass, new_master_pass);

    string new_site, new_email, new_pass;
    for (size_t i=0; i< USER->keys.size(); i++){
        new_site.clear();
        new_email.clear();
        new_pass.clear();
        new_site= crypt_w_local_pass( decrypt_w_local_pass(USER->keys[i].get_site(),local_pass), temp_l);
        new_email= crypt_w_local_pass( decrypt_w_local_pass(USER->keys[i].get_email() ,local_pass), temp_l);  
        new_pass = crypt_w_local_pass( crypt_w_master_pass( decrypt_w_master_pass( decrypt_w_local_pass(USER->keys[i].get_pass() ,local_pass),master_pass),temp_m), temp_l) ; 
        USER->keys[i].set(new_site, new_email, new_pass);
    }
    master_pass.clear();
    local_pass.clear();
    temp_l.clear();
    temp_m.clear();
    return path+file_name+".stk";
}

void opciones(){
    cout<<endl;
    cout<<"\t<OPCION>"<<endl;
    cout<<"\t add\t\tA"<<char(164)<<"ade nueva llave"<<endl;
    cout<<"\t rm\t\tBorra llave" <<endl;
    cout<<"\t edit\t\tEdita llave"<<endl;
    cout<<"\t clip\t\tGuarda contrase"<<char(164)<<"a a portapapeles por 30 s"<<endl;
    cout<<"\t edit_user\tEdita: nombre, contrase"<<char(164)<<"a local y maestra"<<endl;
    cout<<"\t rm_user\tBorra usuario"<<endl<<endl;

}

void cout_menu(){
    cout<<endl<<"Peque"<<char(164)<<"o gestor de contrase"<<char(164)<<"as"<<endl;
    cout<<"Autor: Huck_Pick"<<endl<<endl;
    cout<<"Como usar:\n\n >.\\keybank.exe init \t\t\tA"<<char(164)<<"ade un nuevo usuario"<<endl;
    cout<<"\t\t\t\t\t\t-Pide un <NOMBRE_USUARIO>"<<endl;
    cout<<"\t\t\t\t\t\t-Pide una constrase"<<char(164)<<"a local"<<endl;
    cout<<"\t\t\t\t\t\t-Pide una constrase"<<char(164)<<"a maestra"<<endl<<endl;
    cout<<">.\\keybank.exe <NOMBRE_USUARIO> <OPCION> "<<endl;
    opciones();
    return;
}
