#include<bits/stdc++.h>

using namespace std;

map<int,vector<int>> tabla;
vector<pair<int,char>> saltos;
set<char> setSimbolos;
vector<char> simbolos;
vector<set<int>> setTabla;

struct Arbolito{
	char simbolo;
	bool anulable;
	int num;
	int posicion;
	vector<int> primeros;
	vector<int> ultimos;
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
	vector<transicion> nuevasTransiciones;
	vector<pair<int,vector<int>>> estados;
	vector<pair<int,vector<int>>> nuevosEstados;
	vector<char> simbolos;
	vector<int> finales;
	vector<int> nuevosFinales;
	vector<int> nofinales;
	vector<vector<vector<int>>> particiones;
	set<int> setFinales;
	int final;
	int inicial;

	vector<pair<char,int>> encontrarEstadosAlcanzados(int estado){
		vector<pair<char,int>> respuesta;
		for(int i=0;i<transiciones.size();i++){
			transicion trans;
			trans=transiciones[i];
			if(trans.estadoA==estado){
				respuesta.push_back(make_pair(trans.simbolo,trans.estadoB));
			}
		}
		sort(respuesta.begin(),respuesta.end());
		return respuesta;
	}

	vector<int> encontrarParticionesAlcanzadas(vector<pair<char,int>> estadosAlcanzados,int numParticion){
		vector<int> respuesta;
		for(int i=0; i<estadosAlcanzados.size();i++){
			for(int j=0;j<particiones[numParticion].size();j++){
				for(int k=0;k<particiones[numParticion][j].size();k++){
					if(particiones[numParticion][j][k]==estadosAlcanzados[i].second){
							respuesta.push_back(j);
					}
				}
			}
		}
		return respuesta;
	}

	void obtenerPrimerParticion(){
		vector<vector<int>> primerParticion;
		vector<int> fin;
		vector<int> noFin;
		for(int i=0;i<finales.size();i++){
			setFinales.insert(finales[i]);
			fin.push_back(finales[i]);
		}
		for(int j=0; j<estados.size();j++){
			if(setFinales.count(estados[j].first)==0){
				noFin.push_back(estados[j].first);
			}
		}
		primerParticion.push_back(fin);
		if(noFin.size()>0){
			primerParticion.push_back(noFin);
		}
		particiones.push_back(primerParticion);
	}

	void obtenerNuevoInicial(){
		for(int i=0; i<nuevosEstados.size();i++){
			for(int j=0; j<nuevosEstados[i].second.size();j++){
				if(nuevosEstados[i].second[j]==0){
					inicial=i;
				}
			}
		}
	}

	void reducirAFD(){
		obtenerParticiones();
		obtenerNuevosEstados();
		obtenerNuevosFinales();
		obtenerNuevasTransiciones();
		obtenerNuevoInicial();
	}

	void obtenerNuevasTransiciones(){
		for(int i=0; i<nuevosEstados.size();i++){
			vector<int> particionesAlcanzadasA;
			vector<pair<char,int>> estadosAlcanzadosA;
			int a=nuevosEstados[i].second[0];
			estadosAlcanzadosA=encontrarEstadosAlcanzados(a);
			particionesAlcanzadasA=encontrarParticionesAlcanzadas(estadosAlcanzadosA,particiones.size()-1);
			for(int j=0;j<particionesAlcanzadasA.size();j++){
				nuevaTransicion2(i,particionesAlcanzadasA[j],estadosAlcanzadosA[j].first);
			}
		}
	}

	void obtenerNuevosFinales(){
		set<int> setNuevosFinales;
		for(int i=0; i<nuevosEstados.size();i++){
			for(int j=0; j<nuevosEstados[i].second.size();j++){
				if(setFinales.count(nuevosEstados[i].second[j])==1 and setNuevosFinales.count(nuevosEstados[i].first)==0){
					setNuevosFinales.insert(nuevosEstados[i].first);
					nuevosFinales.push_back(nuevosEstados[i].first);
				}
			}
		}
	}

	void obtenerNuevosEstados(){
		int numEst=0;
		for(int j=0; j<particiones[particiones.size()-1].size();j++){
			vector<int> estado;
			for(auto k:particiones[particiones.size()-1][j]){
				estado.push_back(k);
			}
			nuevosEstados.push_back(make_pair(numEst,estado));
			numEst++;
		}
	}

	void obtenerParticiones(){
		int mx=0;
		obtenerPrimerParticion();
		for(int i=0; i<particiones.size();i++){
			if(particiones[i].size()==mx){
				break;
			}
			mx=particiones[i].size();
			vector<vector<int>> nuevaParticion;
			for(int j=0;j<particiones[i].size();j++){
				vector<int> particionesAlcanzadasA;
				vector<pair<char,int>> estadosAlcanzadosA;
				int a =particiones[i][j][0];
				estadosAlcanzadosA=encontrarEstadosAlcanzados(a);
				particionesAlcanzadasA=encontrarParticionesAlcanzadas(estadosAlcanzadosA,i);
				vector<int> grupoA;
				grupoA.push_back(a);
				vector<int> grupoB;
				for(int k=1;k<particiones[i][j].size();k++){
					vector<int> particionesAlcanzadasB;
					vector<pair<char,int>> estadosAlcanzadosB;
					estadosAlcanzadosB=encontrarEstadosAlcanzados(particiones[i][j][k]);
					particionesAlcanzadasB=encontrarParticionesAlcanzadas(estadosAlcanzadosB,i);
					if(estadosAlcanzadosA.size() == estadosAlcanzadosB.size()){
						bool fl=false;
						for(int l=0; l<particionesAlcanzadasA.size();l++){
							if(particionesAlcanzadasA[l]!=particionesAlcanzadasB[l]){
								grupoB.push_back(particiones[i][j][k]);
								fl=false;
								break;
							}
							else{
								fl=true;
							}
						}
						if(fl){
							grupoA.push_back(particiones[i][j][k]);
						}
					}
					else{
						grupoB.push_back(particiones[i][j][k]);
					}
				}
				if(grupoB.size()>0){
					nuevaParticion.push_back(grupoB);
				}
				if(grupoA.size()>0){
					nuevaParticion.push_back(grupoA);
				}
			}
			if(nuevaParticion.size()>0){
				particiones.push_back(nuevaParticion);
			}
		}
	}

	void nuevaTransicion(int estadoA, int estadoB, char simbolo){
		transicion aux;
		aux.estadoA=estadoA;
		aux.estadoB=estadoB;
		aux.simbolo=simbolo;
		transiciones.push_back(aux);
	}

	void nuevaTransicion2(int estadoA, int estadoB, char simbolo){
		transicion aux;
		aux.estadoA=estadoA;
		aux.estadoB=estadoB;
		aux.simbolo=simbolo;
		nuevasTransiciones.push_back(aux);
	}

	set<vector<int>> setEstadosCreados;
	int numEstado=0 ;

	void establecerFinales(){
		for(int i=0; i<estados.size(); i++){
			if(estados[i].second.size()!=0){
				int n = estados[i].second.size()-1;
				if(estados[i].second[n]==final){
					finales.push_back(estados[i].first);
				}
			}
		}
	}

	void nuevoEstado(vector<int> vectorEstado,int numeroEstado){
		estados.push_back(make_pair(numeroEstado,vectorEstado));
	}

	void primerEstado(Arbolito *raiz){
		setEstadosCreados.insert(raiz->primeros);
		nuevoEstado(raiz->primeros,numEstado);
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
				vector<int> posibleEstado;
				set<int> setPosibleEstado;
				for(int l=0; l<estados[i].second.size();l++){
					for(int k=0; k<saltos.size();k++){
						if(estados[i].second[l]==saltos[k].first and saltos[k].second==simbolos[j]){
							for(int n=0; n<tabla[saltos[k].first].size();n++){
								if(setPosibleEstado.count(tabla[saltos[k].first][n])==0){
									posibleEstado.push_back(tabla[saltos[k].first][n]);
									setPosibleEstado.insert(tabla[saltos[k].first][n]);
								}
							}
						}
					}
				}
				sort(posibleEstado.begin(),posibleEstado.end());
				if(setEstadosCreados.count(posibleEstado)==0 and posibleEstado.size()>0){
					setEstadosCreados.insert(posibleEstado);
					numEstado++;
					nuevoEstado(posibleEstado,numEstado);
					nuevaTransicion(estados[i].first,numEstado,simbolos[j]);
				}
				else if(posibleEstado.size()>0){
					comprobarRepeticion(estados[i].first,posibleEstado,simbolos[j]);
				}
			}
		}
	}
	void mostrar(){
		transicion trans;
		cout<<"digraph AFN{\nrankdir = LR;"<<endl;
		cout<<"node [shape = doublecircle];";
		for(int i=0; i<int(nuevosFinales.size());i++){
			cout<<" "<<nuevosFinales[i];
		}
		cout<<";"<<endl;
		cout<<"node [shape = point ];"<<" qi"<<endl;
		cout<<"node [shape = circle];"<<endl;
		cout<<"qi"<<" -> "<<inicial<<endl;
		for(int i = 0; i < int(nuevasTransiciones.size()); i++) {
			trans = nuevasTransiciones[i];
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

vector<int> obtenerPrimeros(Arbolito* arbol){
	vector<int> respuesta;
	switch (arbol->simbolo){
		case '*':
			respuesta = arbol->left->primeros;
			break;
		case '|':
			respuesta = arbol->left->primeros;
			for(int i=0;i<arbol->right->primeros.size();i++){
					respuesta.push_back(arbol->right->primeros[i]);
			}
			break;
		case '.':
			respuesta = arbol->left->primeros;
			if(arbol->left->anulable){
				for(int i=0;i<arbol->right->primeros.size();i++){
					respuesta.push_back(arbol->right->primeros[i]);
				}
			}
			break;
		default:
			respuesta.push_back(arbol->posicion);
			break;
	}
	return respuesta;
}

vector<int> obtenerUltimos(Arbolito* arbol){
	vector<int> respuesta;
	switch (arbol->simbolo){
		case '*':
			respuesta = arbol->left->ultimos;
			break;
		case '|':
			respuesta = arbol->left->ultimos;
			for(int i=0;i<arbol->right->ultimos.size();i++){
				respuesta.push_back(arbol->right->ultimos[i]);
			}
			break;
		case '.':
			respuesta = arbol->right->ultimos;
			if(arbol->right->anulable){
				for(int i=0;i<arbol->left->ultimos.size();i++){
					respuesta.push_back(arbol->left->ultimos[i]);
				}
			}
			break;
		default:
			respuesta.push_back(arbol->posicion);
			break;
	}
	return respuesta;
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

void siguientes(Arbolito *arbol){
	vector<int> aux;
	switch(arbol->simbolo){
		case '*':
			for(int i=0;i<arbol->ultimos.size();i++){
				for(int j=0;j<arbol->primeros.size();j++){
					if(setTabla[arbol->ultimos[i]-1].count(arbol->primeros[j])==0){
						setTabla[arbol->ultimos[i]-1].insert(arbol->primeros[j]);
						tabla[arbol->ultimos[i]].push_back(arbol->primeros[j]);
					}
				}
			}
		break;
		case '.':
			for(int i=0;i<arbol->right->primeros.size();i++){
				for(int j=0;j<arbol->left->ultimos.size();j++){
					if(setTabla[arbol->left->ultimos[j]-1].count(arbol->right->primeros[i])==0){
						setTabla[arbol->left->ultimos[j]-1].insert(arbol->right->primeros[i]);
						tabla[arbol->left->ultimos[j]].push_back(arbol->right->primeros[i]);
					}
				}
			}
		break;
	}
}

Arbolito *concatenacion(Arbolito* hijo1, Arbolito* hijo2){
	Arbolito *respuesta= new Arbolito;
	respuesta->simbolo='.';
	respuesta->left=hijo1;
	respuesta->right=hijo2;
	respuesta->anulable=esAnulable('.',hijo1->anulable,hijo2->anulable);
	respuesta->primeros=obtenerPrimeros(respuesta);
	respuesta->ultimos=obtenerUltimos(respuesta);
	sort(respuesta->primeros.begin(),respuesta->primeros.end());
	siguientes(respuesta);
	return respuesta;
}

Arbolito *operadorOr(Arbolito* hijo1, Arbolito* hijo2){
	Arbolito *respuesta= new Arbolito;
	respuesta->simbolo='|';
	respuesta->left=hijo1;
	respuesta->right=hijo2;
	respuesta->anulable=esAnulable('|',hijo1->anulable,hijo2->anulable);
	respuesta->primeros=obtenerPrimeros(respuesta);
	respuesta->ultimos=obtenerUltimos(respuesta);
	sort(respuesta->primeros.begin(),respuesta->primeros.end());
	return respuesta;
}

Arbolito *operadorKleene(Arbolito* hijo){
	Arbolito *respuesta= new Arbolito;
	respuesta->simbolo='*';
	respuesta->left=hijo;
	respuesta->right=NULL;
	respuesta->anulable = true;
	respuesta->primeros=obtenerPrimeros(respuesta);
	respuesta->ultimos=obtenerUltimos(respuesta);
	sort(respuesta->primeros.begin(),respuesta->primeros.end());
	siguientes(respuesta);
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
				if(setSimbolos.count(i)==0){
					setSimbolos.insert(i);
					simbolos.push_back(i);
				}
			}
			else{
				aux->anulable=true;
			}
			aux->num=num;
			set<int> auxset;
			setTabla.push_back(auxset);
			saltos.push_back(make_pair(num,i));
			aux->posicion=num;
			aux->primeros=obtenerPrimeros(aux);
			aux->ultimos=obtenerUltimos(aux);
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
	respuestaAFD.reducirAFD();
	respuestaAFD.mostrar();
	return 0;
}
