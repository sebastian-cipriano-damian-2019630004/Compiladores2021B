#include<bits/stdc++.h>

using namespace std;

struct conjunto{
	vector<string> kernel;
	vector<string> prod;
};


string extension;
vector<string> producciones;
set<string> noTerminales;
set<string> Terminales;
vector<pair<string,vector<string>>> gramatica;
vector<pair<int,conjunto>> conjuntos;
set<vector<string>> kernels;

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

int numConjunto=1;

void obtenerCerradura(vector<string> kernel){
	set<string> agregadas;
	vector<string> respuesta;
	for(int i=0;i<kernel.size();i++){
		respuesta.push_back(kernel[i]);
		agregadas.insert(kernel[i]);
	}
	cout<<"Si llegué 4.1"<<endl;
	for(int i=0;i<respuesta.size();i++){
		int j=0;
		while(respuesta[i][j]!='.'){
			j++;
		}
		cout<<"Si llegué 4.2"<<endl;
		if(j+1<respuesta[i].size()){
			string auxStr;
			auxStr+=respuesta[i][j+1];
			if(noTerminales.count(auxStr)==1){
				for(int k=0; k<producciones.size()-1;k++){
					if(respuesta[i][j+1]==producciones[k][0] and agregadas.count(producciones[k])==0){
						agregadas.insert(producciones[k]);
						respuesta.push_back(producciones[k]);
					}
				}
			}
		}
	}
	conjunto conj;
	conj.kernel=kernel;
	conj.prod=respuesta;
	conjuntos.push_back(make_pair(numConjunto,conj));
	numConjunto++;
}

vector<string> mover(string movercon, vector<string> vecProducciones){
	vector<string> respuesta;
	for(int i=0;i<vecProducciones.size();i++){
		cout<<"."<<endl;
		int j=0;
		while(vecProducciones[i][j]!='.'){
			j++;
		}
		if(j+1<vecProducciones[i].size()){
			string auxStr;
			auxStr+=vecProducciones[i][j+1];
			if(movercon==auxStr){
				swap(vecProducciones[i][j],vecProducciones[i][j+1]);
				respuesta.push_back(vecProducciones[i]);
			}
		}
	}
	cout<<"Si llegué"<<endl;
	return respuesta;
} 

void crearConjuntos(){
	vector<pair<string,vector<string>>> res;
	for(int i=0;i<conjuntos.size();i++){
		cout<<conjuntos.size()<<endl;
		for(auto j:Terminales){
			vector<string> nuevoKernel;
			nuevoKernel=mover(j,conjuntos[i].second.prod);
			cout<<"Si llegué 2"<<endl;
			sort(nuevoKernel.begin(),nuevoKernel.end());
			if(nuevoKernel.size()>0 and kernels.count(nuevoKernel)==0){
				cout<<"Si llegué 3"<<endl;
				kernels.insert(nuevoKernel);
				res.push_back(make_pair(j,nuevoKernel));
				obtenerCerradura(nuevoKernel);
				cout<<"Si llegué 4"<<endl;
			}
		}
		for(auto j:noTerminales){
			vector<string> nuevoKernel;
			nuevoKernel=mover(j,conjuntos[i].second.prod);
			sort(nuevoKernel.begin(),nuevoKernel.end());
			if(nuevoKernel.size()>0 and kernels.count(nuevoKernel)==0){
				kernels.insert(nuevoKernel);
				res.push_back(make_pair(j,nuevoKernel));
				obtenerCerradura(nuevoKernel);
			}
		}
	}
}

void primerCerradura(string produccion){
	set<string> agregadas;
	agregadas.insert(produccion);
	vector<string> cerradura;
	cerradura.push_back(produccion);
	for(int i=0; i<cerradura.size();i++){
		for(int j=0; j<cerradura[i].size();j++){
			if(cerradura[i][j]=='.'){
				if(j+1<cerradura[i].size()){
					string auxStr;
					auxStr+=cerradura[i][j+1];
					if(noTerminales.count(auxStr)==1){
						for(int k=0; k<gramatica.size();k++){
							if(gramatica[k].first==auxStr and agregadas.count(producciones[k-1])==0){
								cerradura.push_back(producciones[k-1]);
								agregadas.insert(producciones[k-1]);
							}
						}
					}
				}
			}
		}
	}
	conjunto conj;
	conj.kernel=cerradura;
	conj.prod=cerradura;
	conjuntos.push_back(make_pair(0,conj));
	crearConjuntos();
}

int main(){
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
	for(int i=0;i<producciones.size();i++){
		producciones[i]=reescribir(producciones[i]);
	}
	primerCerradura(producciones[producciones.size()-1]);
	cout<<conjuntos.size()<<endl;
	return 0;
}