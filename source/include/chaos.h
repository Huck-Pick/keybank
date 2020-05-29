#define PI 3.14159265358979323846
double chaos_1_LTL(double mu, double xn){
    double trash;
    if (xn<0.5)
        return modf(8.0*mu*mu*xn*(1.0-2.0*mu*xn)+(4.0-4.0*mu)*xn*(1.0-xn),&trash);
    return modf(8.0*mu*mu*(1.0-xn)*(1.0-2.0*mu*(1.0-xn))+(4.0-4.0*mu)*xn*(1.0-xn),&trash);
}
double chaos_2_STL(double mu, double xn){
    double trash;
    if (xn<0.5)
        return modf(mu*sin(PI*2.0*mu*xn)+(4.0-4.0*mu)*xn*(1.0-xn),&trash);
    return modf(mu*sin(PI*2.0*mu*(1.0-xn))+(4.0-4.0*mu)*xn*(1.0-xn),&trash);
}
double chaos_3_LLT(double mu, double xn){
    double trash;
    if (xn<0.5)
        return modf(4.0*mu*(4.0*mu*xn*(1.0-xn))*(1.0-4.0*mu*xn*(1.0-xn))+(2.0-2.0*mu)*xn,&trash);
    return modf(4.0*mu*(4.0*mu*xn*(1.0-xn))*(1.0-4.0*mu*xn*(1.0-xn))+(2.0-2.0*mu)*(1.0-xn),&trash);
}
double chaos_4_LTS(double mu, double xn){
    double trash;
    if (xn<0.5)
        return modf(8.0*mu*mu*xn*(1.0-2.0*mu*xn)+(1.0-mu)*sin(PI*xn),&trash);
    return modf(8.0*mu*mu*(1.0-xn)*(1.0-2.0*mu*(1.0-xn))+(1.0-mu)*sin(PI*xn),&trash);
}
double chaos_5_LLS(double mu, double xn){
    double trash;
    return modf(8.0*mu*mu*xn*(1.0-xn)*(1.0-4.0*mu*xn*(1.0-xn))+(1.0-mu)*sin(PI*xn),&trash);
}
double chaos_6_STS(double mu, double xn){
    double trash;
    if (xn<0.5)
        return modf(mu*sin(PI*2.0*mu*xn)+(1.0-mu)*sin(PI*xn),&trash);
    return modf(mu*sin(PI*2.0*mu*(1.0-xn))+(1.0-mu)*sin(PI*xn),&trash);
}
unsigned char * chaos_string(double mu, double xt, int tipe){
    unsigned char *str;
    str= new unsigned char [m*m*m];
    double xn=xt;
    double pot=pow(2,16);
    unsigned int temp;
    for (size_t i=0; i<m*m*m; i++ ){
        if (tipe%m==0)
            xn=chaos_1_LTL(mu, xn);
        if (tipe%m==1)
            xn=chaos_2_STL(mu, xn);
        if (tipe%m==2)
            xn=chaos_3_LLT(mu, xn);
        if (tipe%m==3)
            xn=chaos_4_LTS(mu, xn);
        if (tipe%m==4)
            xn=chaos_5_LLS(mu, xn);
        if (tipe%m==5)
            xn=chaos_6_STS(mu, xn);
        temp=xn*pot;
        str[i]=(unsigned char)(temp%256);

    }
    return str;
}


unsigned char * string_2_char(string input){
    unsigned char *str;
    str=new unsigned char [m*m*m];
    for (size_t i=0; i<input.size(); i++)
        str[i]=(unsigned char)(input[i]);
    return str;
}
string char_2_string(unsigned char * str){
    string out="";
    for (int i=0; i<m*m*m; i++){
        out+=(char)str[i];
    }
    return out;
}

string encrypt_dat(string input, string master_key, int tipe){
    chaos_value chaos_v=inti_var(sha256(master_key));
    unsigned char *chaos=chaos_string(chaos_v.mu_crypt, chaos_v.xt_crypt,tipe);
    unsigned char *change= chaos_string(chaos_v.mu_shuffle,chaos_v.xt_shuffle,tipe);
    unsigned char *data=string_2_char(input);
    for (size_t i=0; i<m*m*m; i++)
        data[i]^=change[i];

    for (size_t i=0; i<m*m*m-1; i++){
        data[i+1]=(data[i+1]^chaos[i+1]^data[i]);
    }
    
    input=char_2_string(data);
    return input;
}
string decrypt_dat(string input, string master_key,int tipe){
    chaos_value chaos_v=inti_var(sha256(master_key));
    unsigned char *chaos=chaos_string(chaos_v.mu_crypt, chaos_v.xt_crypt,tipe);
    unsigned char *change= chaos_string(chaos_v.mu_shuffle,chaos_v.xt_shuffle, tipe);
    
    unsigned char *data=string_2_char(input);
    for (size_t i=m*m*m-1; i>0; i--)
        data[i]=(data[i]^chaos[i]^data[i-1]);

    for (size_t i=0; i<m*m*m; i++)
        data[i]^=change[i];

    input=char_2_string(data);
    return input;
}
string crypt_w_local_pass(string input, string local_pass){
    vector <rotations> r;
    for (size_t i=0; i<m*m/2; i++){
        r=shuffle_instructions(local_pass+char(i));
        input=shuffle_forwards(input,r);
        input=encrypt_dat(input,local_pass+char(i),i);
    }
    return input;
}
string decrypt_w_local_pass(string input, string local_pass){
    vector <rotations> r;
    for (size_t i=m*m/2-1; i>=0; i--){
        input=decrypt_dat(input,local_pass+char(i),i);
        r=shuffle_instructions(local_pass+char(i));
        input=shuffle_backwards(input,r);
    }
    return input;
}
string crypt_w_master_pass(string input, string master_pass){
    vector <rotations> r;
    for (size_t i=0; i<m*m*m; i++){
        r=shuffle_instructions(sha256(master_pass+char(i)));
        input=shuffle_forwards(input,r);
        input=encrypt_dat(input,sha256(master_pass+char(i)),i);
    }
    return input; 
}
string decrypt_w_master_pass(string input, string master_pass){
    vector <rotations> r;
    for (size_t i=m*m*m-1; i>=0; i--){
        input=decrypt_dat(input,sha256(master_pass+char(i)),i);
        r=shuffle_instructions(sha256(master_pass+char(i)));
        input=shuffle_backwards(input,r);
    }
    return input;
}