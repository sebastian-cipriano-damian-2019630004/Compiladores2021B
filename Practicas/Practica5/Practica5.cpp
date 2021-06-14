#include<bits/stdc++.h>

using namespace std;

void A(const string& s, int);
void B(const string& s, int);

void error(){
    cout << "No le sabes"<< endl;
    exit(1);
}

void consume(char x, const string s, int id){
    if(s[id] == x){
        id++;
    }
    else{
        error();
    }
}

void Grammar(string s){
    int id = 0;
    A(s, id);
    cout<<"Si pertenece"<<endl;
}
void A(const string& s, int id){
    consume('a', s, id); 
	B(s, id); 
	consume('a', s, id);
}

void B(const string& s, int id){
    switch(s[id]){
        case 'b':
            consume('b', s, id);
			A(s, id); 
			consume('b', s, id);
            break;
        case 'a':
            consume('a', s, id); 
            break;
        default:
            error();
    }
}

int main(){
    while(true){
        string s; 
		cin>>s;
        Grammar(s);
    }
	return 0;
}
