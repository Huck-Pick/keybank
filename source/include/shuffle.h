const unsigned int m=6;
using namespace std;

string add2string(string str){
    string test;
    char a=(char)(str.size()+33);
    test=a;
    test+=str;
    for (size_t i=test.size(); i<(m*m*m); i++)
        test+=a;
    return test;
}

unsigned char ***string_matrix_char(string input){
    unsigned char ***A;
    A= new unsigned char**[m];
    for (size_t i=0; i<m; i++)
        A[i]=new unsigned char*[m];
    
    for (size_t i=0; i<m;i++)
        for (size_t j=0; j<m; j++)
            A[i][j]=new unsigned char[m];
    size_t k=0;
    for(size_t i=0; i<m; i++)
        for (size_t j=0; j<m; j++)
            for (size_t w=0; w<m; w++){
                A[j][i][w]=(unsigned char)input[k];
                k++;
            }
    return A;
}
string matrix_char_string (unsigned char ***A){
    string output;
    for(size_t i=0; i<m; i++)
        for (size_t j=0; j<m; j++)
            for (size_t w=0; w<m; w++){
                output+=(char)A[j][i][w];
            }
    return output;
}
string recoverstring(string str){
    size_t tam=(int)str[0]-33;
    return str.substr(1,tam);
}

void Swap(unsigned char &a, unsigned char &b){
    a = a + b;
    b = a - b;
    a = a - b;
    return;
}
void Rotate_K3(unsigned char *** dat,unsigned int pos, unsigned int dir){
    unsigned int level=0;
    unsigned int last=m-1;
    unsigned int tot_levels=m/2;
    if (dir==1){ // rotate right 
        while (level < tot_levels){
            for (size_t i=level; i< last ; i++){
                Swap(dat[pos][level][i],dat[pos][i][last]);
                Swap(dat[pos][level][i],dat[pos][last][last-i+level]);
                Swap(dat[pos][level][i],dat[pos][last-i+level][level]);
            
            }
            level++;
            last--;
        }
        return;
    }
    else{ // ratate left
        while (level < tot_levels){
            for (size_t i=last; i>level ; i--){
                Swap(dat[pos][level][i],dat[pos][last-i+level][level]);
                Swap(dat[pos][level][i],dat[pos][last][last-i+level]);
                Swap(dat[pos][level][i],dat[pos][i][last]);
            }
            level++;
            last--;
        }
        return;
    }
}
void Rotate_I3(unsigned char ***dat, unsigned int pos, unsigned int dir){
    unsigned int level=0;
    unsigned int last=m-1;
    unsigned int tot_levels=m/2;
    if (dir==1){ // rotate right 
        while (level < tot_levels){
            for (size_t i=level; i< last ; i++){
                Swap(dat[level][pos][i],           dat[i][pos][last]);
                Swap(dat[level][pos][i],        dat[last][pos][last-i+level]);
                Swap(dat[level][pos][i],dat[last-i+level][pos][level]);
            
            }
            level++;
            last--;
        }
        return;
    }
    else{ // ratate left
        while (level < tot_levels){
            for (size_t i=last; i>level ; i--){
                Swap(dat[level][pos][i],dat[last-i+level][pos][level]);
                Swap(dat[level][pos][i],        dat[last][pos][last-i+level]);
                Swap(dat[level][pos][i],           dat[i][pos][last]);
            }
            level++;
            last--;
        }
        return;
    }
}
void Rotate_J3(unsigned char ***dat, unsigned int pos, unsigned int dir){
    unsigned int level=0;
    unsigned int last=m-1;
    unsigned int tot_levels=m/2;
    if (dir==1){ // rotate right 
        while (level < tot_levels){
            for (size_t i=level; i< last ; i++){
                Swap(dat[level][i][pos],            dat[i][last][pos]);
                Swap(dat[level][i][pos], dat[last][last-i+level][pos]);
                Swap(dat[level][i][pos],dat[last-i+level][level][pos]);
            
            }
            level++;
            last--;
        }
        return;
    }
    else{ // ratate left
        while (level < tot_levels){
            for (size_t i=last; i>level ; i--){
                Swap(dat[level][i][pos],dat[last-i+level][level][pos]);
                Swap(dat[level][i][pos], dat[last][last-i+level][pos]);
                Swap(dat[level][i][pos],            dat[i][last][pos]);
            }
            level++;
            last--;
        }
        return;
    }
}

typedef struct ratations{
    char dir;
    int  pos;
    int  spin;
    void set(char d, int p, int s){
        dir=d;
        pos=p;
        spin=s;
    }

}rotations;
const char DD[3]={'I','J','K'};
vector<rotations> shuffle_instructions(string str){
    str=sha256(str);
    vector <rotations> r;
    rotations s;
    for (size_t i=0; i<str.size()-3; i+=3){
        s.set(DD[(int)str[i]%3],(int)str[i+1]%m,(int)str[i+2]%2);
        r.push_back(s);
    }
    return r;
}
void matrix_shuffle_forwards(unsigned char ***A, vector<rotations> r){
    for (size_t i=0; i<r.size(); i++){
        if (r[i].dir=='I')
            Rotate_I3(A,r[i].pos,r[i].spin);
        if (r[i].dir=='J')
            Rotate_J3(A,r[i].pos,r[i].spin);
        if (r[i].dir=='K')
            Rotate_K3(A,r[i].pos,r[i].spin);
    }
    return ;
}

void matrix_shuffle_backwards(unsigned char ***A, vector<rotations> r){
    for (size_t i=r.size()-1; i>=0; i--){
        if (r[i].dir=='I')
            Rotate_I3(A,r[i].pos,(r[i].spin+1)%2);
        if (r[i].dir=='J')
            Rotate_J3(A,r[i].pos,(r[i].spin+1)%2);
        if (r[i].dir=='K')
            Rotate_K3(A,r[i].pos,(r[i].spin+1)%2);
    }
    return ;
}
string shuffle_forwards(string input, vector<rotations> r){
    unsigned char ***A;
    A=string_matrix_char(input);
    for (size_t i=0; i<r.size(); i++){
        if (r[i].dir=='I')
            Rotate_I3(A,r[i].pos,r[i].spin);
        if (r[i].dir=='J')
            Rotate_J3(A,r[i].pos,r[i].spin);
        if (r[i].dir=='K')
            Rotate_K3(A,r[i].pos,r[i].spin);
    }
    input=matrix_char_string (A);
    return input;
}

string shuffle_backwards(string input, vector<rotations> r){
    unsigned char ***A;
    A=string_matrix_char(input);
    for (size_t i=r.size()-1; i>=0; i--){
        if (r[i].dir=='I')
            Rotate_I3(A,r[i].pos,(r[i].spin+1)%2);
        if (r[i].dir=='J')
            Rotate_J3(A,r[i].pos,(r[i].spin+1)%2);
        if (r[i].dir=='K')
            Rotate_K3(A,r[i].pos,(r[i].spin+1)%2);
    }
    input=matrix_char_string (A);
    return input;
}