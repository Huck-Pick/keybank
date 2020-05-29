using namespace std;
typedef struct chaos_value{
    double mu_shuffle;
    double xt_shuffle;
    double mu_crypt;
    double xt_crypt;
    void set(double m_s, double x_s, double m_c, double x_c){
        mu_shuffle=m_s;
        xt_shuffle=x_s;
        mu_crypt=m_c;
        xt_crypt=x_c;
    }
}chaos_value;

double double_substring(string a){
    for (size_t i=0;i<a.size(); i++)
        a[i]=(char)(a[i]%10+48);
    a="0."+a;
    return stod(a);
}

chaos_value inti_var(string sub){
    sub= sha256(sub);
    double mu_shuffle=double_substring( sub.substr(1,15));
    double xt_shuffle=double_substring( sub.substr(15,15));
    double mu_crypt=double_substring( sub.substr(30,15));
    double xt_crypt=double_substring( sub.substr(45,15));
    chaos_value s;
    s.set(0.4+mu_shuffle*0.6 , xt_shuffle,0.4+mu_crypt*0.6, xt_crypt);
    return s;
}