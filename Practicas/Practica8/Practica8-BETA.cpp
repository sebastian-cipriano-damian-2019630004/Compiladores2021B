#include<bits/stdc++.h>

using namespace std;

struct conjunto{
	vector<string> kernel;
	vector<string> prod;
};

struct irA{
	int a;
	int b;
	string sim;
};

struct salto{
	int a;
	int b;
	string sim;
};

string extension;
vector<string> producciones;
set<string> noTerminales;
set<string> Terminales;
vector<pair<string,vector<string>>> gramatica;
vector<pair<int,conjunto>> conjuntos;
set<vector<string>> kernels;
vector<irA> virA;
vector<salto> vSaltos;

string reescribir(string produccion){
	int j=0;
	while(produccion[j]!='>'){
		j++;
	}
	j++;
	produccion.insert(j,".");
	return produccion;
}

void definirNoTerminales(string noTer){
	noTerminales.insert(noTer);
}

void definirTerminales(vector<string> posiblesTerminales){
	for(int i=0; i<posiblesTerminales.size();i++){
		if(noTerminales.count(posiblesTerminales[i])==0){
			Terminales.insert(posiblesTerminales[i]);
		}
	}
}

void definirGramatica(string produccion){
	string noTer;
	vector<string> simbolos;
	int i=0;
	while(produccion[i]!='-'){
		noTer+=produccion[i];
		i++;
	}
	i+=1;
	for(int j=i+1;j<produccion.size();j++){
		string aux;
			aux+=produccion[j];
			simbolos.push_back(aux);
	}
	gramatica.push_back(make_pair(noTer,simbolos));
}

int main(){

	//NO ESTÁ TERMINADA

	int cantidad;
	string produccion;
	cin>>cantidad;
	for(int i=0; i<cantidad ;i++){
		cin>>produccion;
		producciones.push_back(produccion);
	}
	extension+="S";
	extension+='-';
	extension+='>';
	extension+=producciones[0][0];
	producciones.push_back(extension);
	definirGramatica(extension);
	for(int i=0;i<producciones.size();i++){
		definirGramatica(producciones[i]);
	}
	for(int i=0;i<gramatica.size();i++){
		definirNoTerminales(gramatica[i].first);
	}
	for(int i=0;i<gramatica.size();i++){
		definirTerminales(gramatica[i].second);
	}
	noTerminales.erase("S");
	for(int i=0;i<producciones.size();i++){
		producciones[i]=reescribir(producciones[i]);
	}

	cout<<"  ir_A"<<endl;

	for(int j=0;j<virA.size();j++){
		cout<<virA[j].a<<"--"<<virA[j].sim<<"-->"<<virA[j].b<<endl;
	}
	cout<<endl;
	cout<<" Saltos"<<endl;

	for(int j=0;j<vSaltos.size();j++){
		cout<<vSaltos[j].a<<"--"<<vSaltos[j].sim<<"-->"<<vSaltos[j].b<<endl;
	}
	extension+='.';
	for(int i=0;i<conjuntos.size();i++){
		for(int j=0;j<conjuntos[i].second.prod.size();j++){
			if(conjuntos[i].second.prod[j]==extension){
				cout<<conjuntos[i].first<<"--$-->AC"<<endl;
			}
		}
	}

	return 0;
}