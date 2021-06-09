#include<bits/stdc++.h>

using namespace std;

map<int,vector<int>> tabla;
vector<pair<int,char>> saltos;
set<char> setSimbolos;
vector<char> simbolos;

struct Arbolito {
	char simbolo;
	bool anulable;
	int num;
	vector<int> iniciales;
	vector<int> finales;
    Arbolito *left;
	Arbolito *right;
};

struct transicion{
	char simbolo;
	int estadoA;
	int estadoB;
};

class AFD{
	public:
	vector<transicion> transiciones;
	vector<pair<int,vector<int>>> estados;
	vector<char> simbolos;
	vector<int> finales;
	int final;

	void nuevaTransicion(int estadoA, int estadoB, char simbolo){
		transicion aux;
		aux.estadoA=estadoA;
		aux.estadoB=estadoB;
		aux.simbolo=simbolo;
		transiciones.push_back(aux);
	}

	set<vector<int>> setEstadosCreados;
	int numEstado=0 ;

	void establecerFinales(){
		for(int i=0; i<estados.size(); i++){
			if(estados[i].second.size()!=0){
				int n = estados[i].second.size()-1;
				if(estados[i].second[n]==final){
					finales.push_back(estados[i].first);
					break;
				}
			}
		}
	}

	void nuevoEstado(vector<int> vectorEstado,int numeroEstado){
		estados.push_back(make_pair(numeroEstado,vectorEstado));
	}

	void primerEstado(Arbolito *raiz){
		setEstadosCreados.insert(raiz->iniciales);
		nuevoEstado(raiz->iniciales,numEstado);
	}

	void comprobarRepeticion(int estadoA, vector<int> estadoRepetido, char simbolo){
		set<vector<int>> analizados;
		for(int i=0; i<estados.size();i++){
			analizados.insert(estados[i].second);
			if(analizados.count(estadoRepetido)==1){
				nuevaTransicion(estadoA,estados[i].first,simbolo);
				break;
			}
		}
	}

	void crearEstados(){
		for(int i=0; i<estados.size();i++){
			for(int j=0; j<simbolos.size()-1;j++){
				vector<int> aux;
				for(int l=0; l<estados[i].second.size();l++){
					for(int k=0; k<saltos.size();k++){
						if(estados[i].second[l]==saltos[k].first and saltos[k].second==simbolos[j]){
							for(int n=0; n<tabla[saltos[k].first].size();n++){
								aux.push_back(tabla[saltos[k].first][n]);
							}
						}
					}
				}
				if(setEstadosCreados.count(aux)==0 and aux.size()>0){
					setEstadosCreados.insert(aux);
					numEstado++;
					nuevoEstado(aux,numEstado);
					nuevaTransicion(estados[i].first,numEstado,simbolos[j]);
				}
				else if(aux.size()>0){
					comprobarRepeticion(estados[i].first,aux,simbolos[j]);
				}
			}
		}
	}
	void mostrar(){
		transicion trans;
		cout<<"digraph AFN{\nrankdir = LR;"<<endl;
		cout<<"node [shape = doublecircle];";
		for(int i=0; i<int(finales.size());i++){
			cout<<" "<<finales[i];
		}
		cout<<";"<<endl;
		cout<<"node [shape = point ];"<<" qi"<<endl;
		cout<<"node [shape = circle];"<<endl;
		cout<<"qi"<<" -> "<<"0"<<endl;
		for(int i = 0; i < int(transiciones.size()); i++) {
			trans = transiciones[i];
			cout<<trans.estadoA<<" -> "<<trans.estadoB<<" [label =\""<<trans.simbolo<<"\"];"<<endl;
		}
		cout<<"}"<<endl;
	}
};

int num=1;

AFD construirAFD(Arbolito *arbol){
	AFD *nuevoAFD = new AFD;
	nuevoAFD->simbolos=simbolos;
	nuevoAFD->primerEstado(arbol);
	nuevoAFD->crearEstados();
	nuevoAFD->final = num-1;
	nuevoAFD->establecerFinales();
	return *nuevoAFD;
}

void mostrar(Arbolito *arbol, int cont){
	if(arbol==NULL){
		return;
	}
	else{
		mostrar(arbol->right,cont+1);
		for(int i=0; i<cont; i++){
			cout<<"    ";
		}
		cout<<arbol->simbolo;
		for(int i=0; i<arbol->finales.size(); i++){
			cout<<arbol->finales[i];
		}
		cout<<endl;
		mostrar(arbol->left,cont+1);
	}
}

bool esAnulable(char simbolo, bool anulable1, bool anulable2){
	if(simbolo == '.'){
		if(anulable1==true and anulable2==true){
			return true;
		}
		else{
			return false;
		}
	}
	else{
		if(anulable1==true or anulable2==true){
			return true;
		}
		else{
			return false;
		}
	}	
}

Arbolito *concatenacion(Arbolito* hijo1, Arbolito* hijo2){
	Arbolito *respuesta= new Arbolito;
	respuesta->simbolo='.';
	respuesta->left=hijo1;
	respuesta->right=hijo2;
	respuesta->anulable=esAnulable('.',hijo1->anulable,hijo2->anulable);
	respuesta->iniciales=hijo1->iniciales;

	if(hijo1->anulable==true){
		for(int i=0; i<hijo2->iniciales.size();i++){
			respuesta->iniciales.push_back(hijo2->iniciales[i]);
		}
	}

	if(hijo2->anulable==true){
		respuesta->finales=hijo1->finales;
	}
	else{
		respuesta->finales=hijo2->finales;
	}

	for(int i=0; i<hijo1->finales.size();i++){
		for(int j=0; j<hijo2->iniciales.size();j++){
			tabla[hijo1->finales[i]].push_back(hijo2->iniciales[j]);
		}
	}

	return respuesta;
}

Arbolito *operadorOr(Arbolito* hijo1, Arbolito* hijo2){
	Arbolito *respuesta= new Arbolito;
	respuesta->simbolo='|';
	respuesta->left=hijo1;
	respuesta->right=hijo2;
	respuesta->anulable=esAnulable('|',hijo1->anulable,hijo2->anulable);
	respuesta->iniciales=hijo1->iniciales;
	for(int i=0; i<hijo2->iniciales.size();i++){
		respuesta->iniciales.push_back(hijo2->iniciales[i]);
	}
	respuesta->finales=hijo1->finales;
	for(int i=0; i<hijo2->finales.size();i++){
		respuesta->finales.push_back(hijo2->finales[i]);
	}
	return respuesta;
}

Arbolito *operadorKleene(Arbolito* hijo){
	Arbolito *respuesta= new Arbolito;
	respuesta->simbolo='*';
	respuesta->left=hijo;
	respuesta->right=NULL;
	respuesta->anulable = true;
	respuesta->iniciales=hijo->iniciales;
	respuesta->finales=hijo->finales;
	for(int i=0; i<respuesta->finales.size();i++){
		for(int j=0; j<respuesta->iniciales.size();j++){
			tabla[respuesta->finales[i]].push_back(respuesta->iniciales[j]);
		}
	}
	return respuesta;
}




Arbolito *crearArbol(string regex){
	stack<Arbolito*> pila;
	Arbolito *hijo1;
	Arbolito *hijo2;
	Arbolito *respuesta;
	
	for(char i:regex){
		Arbolito* aux = new Arbolito;
		if(i=='.'){
			hijo2=pila.top();
			pila.pop();
			hijo1=pila.top();
			pila.pop();
			respuesta=concatenacion(hijo1,hijo2);
			pila.push(respuesta);
		}
		else if(i=='*'){
			hijo1=pila.top();
			pila.pop();
			respuesta=operadorKleene(hijo1);
			pila.push(respuesta);
		}
		else if(i=='|'){
			hijo2=pila.top();
			pila.pop();
			hijo1=pila.top();
			pila.pop();
			respuesta=operadorOr(hijo1,hijo2);
			pila.push(respuesta);
		}
		else{
			aux->simbolo=i;
			aux->left=aux->right=NULL;
			if(i!='e'){
				aux->anulable=false;
			}
			else{
				aux->anulable=true;
			}
			aux->num=num;
			aux->iniciales.push_back(num);
			aux->finales=aux->iniciales;
			if(setSimbolos.count(i)==0){
				setSimbolos.insert(i);
				simbolos.push_back(i);
			}
			saltos.push_back(make_pair(num,i));
			num++;
			pila.push(aux);
		}
	}
	return pila.top();
}

int precedencia(char c) {
    if(c == '*'){
 		return 3;
	}
    else if(c == '.'){
		return 2;
	}
    else if(c == '|'){
		return 1;
	}   
    else{
		return 0;
	}
}

string postfija(string regex) {
    stack<char> pila;
    string post;
    for(char i: regex){
        if((i!='(') and (i!=')') and (i!='.') and (i!='*') and (i!='|')){
        	post+=i;
		}
        else if(i == '('){
        	pila.push('(');
		}
        else if(i == ')'){
            while(!pila.empty() && pila.top() != '('){
                post += pila.top();
                pila.pop();
            }
            pila.pop();
        }
        else {
            while(!pila.empty() && precedencia(i) <= precedencia(pila.top())) {
                post+= pila.top();
                pila.pop();
            }
            pila.push(i);
        }
    }
    while(!pila.empty()) {
        post+= pila.top();
        pila.pop();
    }
 	return post;
}

string definirConcatenaciones(string regex){
	string nuevoRegex;
	for(int i=0; i<regex.size();i++){
		nuevoRegex+=regex[i];
		if(regex[i]==')' or regex[i]=='*'){
			if(i+1<regex.size()){
				if((regex[i+1]!='*') and (regex[i+1]!='|') and (regex[i+1]!=')')){
					nuevoRegex+='.';
				}
			}
		}
		else{
			if((regex[i]>='a' and regex[i]<='z') or (regex[i]>='A' and regex[i]<='Z')){
				if((regex[i+1]!='*') and (regex[i+1]!='|') and (regex[i+1]!=')') and i+1<regex.size()){
					nuevoRegex+='.';
				}
			}
		}
	}
	return nuevoRegex;
}

void mostrarTabla(){
	int j=0;

	tabla[num-1].push_back(0);
	for(auto i:tabla){
		cout<<i.first<<" "<<saltos[j].second<<"  ";
		j++;
		for(auto j: i.second){
			cout<<j;
		}
		cout<<endl;
	}
}

int main(){
	Arbolito *respuesta;
	string regex;
	cin>>regex;
	regex=definirConcatenaciones(regex);
	string post;
	post=postfija(regex);
	respuesta=crearArbol(post);
	AFD respuestaAFD;
	respuestaAFD=construirAFD(respuesta);
	respuestaAFD.mostrar();
	return 0;
}
