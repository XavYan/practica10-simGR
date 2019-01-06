#include "../include/GR.hpp"


GR::GR (void) : alphabet_(), V_(), S_(), P_() {}
GR::GR (const string& nombreFichero, bool& errorApertura) {
  errorApertura = 0;
  if (nombreFichero.rfind(".dfa") != string::npos) {
    DFAtoGR(nombreFichero.data(), errorApertura);
  } else if (nombreFichero.rfind(".grammar") != string::npos) {
    gen_GR(nombreFichero.data(), errorApertura);
  } else {
    cerr << "No se pudo crear la gramatica. El fichero indicado no es valido. Use un fichero 'grammar' o 'dfa'\n";
    errorApertura = 1;
  }
}
GR::~GR (void) {
  alphabet_.clear();
  V_.clear();
  P_.clear();
}

void GR::gen_GR (const char* nombreFichero, bool& errorApertura) {
    ifstream fich;
    fich.open(nombreFichero);
    if (!fich.is_open()) {
      cerr << "Error. El fichero no se ha podido abrir con exito.\n";
      errorApertura = 1;
      return;
    }
    //Leemos el alfabeto (simbolos terminales)
    int all_alphabet;
    fich >> (int &)all_alphabet;
    for (int i = 0; i < all_alphabet; i++) {
      char aux;
      fich >> (char &)aux;
      alphabet_.insert(aux);
    }

    //Ahora simbolos no terminales
    int all_symbols;
    fich >> (int &)all_symbols;
    fich >> (char &)S_; //Primero aparece el simbolo del axioma
    V_.insert(S_);
    for (int i = 1; i < all_symbols; i++) {
      char aux;
      fich >> (char &)aux;
      V_.insert(aux);
    }

    //Prototipos
    P_.resize(all_symbols);
    fich.ignore(1);
    for (int i = 0; i < all_symbols; i++) {
      string s;
      getline(fich,s);
      get<0>(P_[i]) = s[0];
      get<1>(P_[i]) = split_prototype(s);
    }
}

void GR::DFAtoGR (const char* nombreFichero, bool& errorApertura) {
  DFA dfa;
  errorApertura = 0;
  dfa.create_dfa(nombreFichero,errorApertura);
  if (errorApertura) {
    cerr << "Error. Ha ocurrido un problema al abrir el archivo. No se puede continuar.\n";
    return;
  }
  //dfa.minDFA(); //Nos aseguramos de que el DFA sea minimo.

  //EMPEZAMOS A OBTENER LOS VALORES
  alphabet_ = dfa.alphabet(); //Alfabeto de la gramatica

  //Establecemos los valores de los simbolos no terminales
  vector<char> V(dfa.size());
  S_ = V[dfa.init()] = 'S'; //Valor del axioma de la gramatica
  V_.insert(S_);
  char ascii = 65;
  for (int i = 0; i < V.size(); i++) {
    if (i != dfa.init()) {
      if (ascii >= 65 && ascii <= 90) {
        V[i] = ascii; //Le asignamos una letra
        V_.insert(ascii); //Añadimos ese simbolo al conjunto
        ascii++;
      }
    }
  }
  //Prototipos
  /* Como habra tantos prototipos como estados del DFA */
  vector<pair<char, set<string> > > P;
  P.resize(dfa.size());
  for (state_t state : dfa.getStates()) { //Para cada estado...
    set<string> aux; //Generamos un conjunto de prototipos
    char iaux = V[state.id()];
    for (pair<char,unsigned> pair : state.getNext()) {
      std::string saux;
      saux = get<0>(pair);
      saux += V[get<1>(pair)];
      aux.insert(saux);
    }
    if (state.is_accept()) {
      string saux;
      saux = '~';
      aux.insert(saux);
    }
    get<0>(P[state.id()]) = iaux; //Estado del prototipo
    get<1>(P[state.id()]) = aux; //Prototipos del estado
  }
  P_ = P;
}

void GR::export_to (const char* nombreFichero, bool& errorApertura) {
  ofstream fich;
  fich.open(nombreFichero);
  if (!fich.is_open()) {
    cerr << "El fichero no se ha podido abrir con exito.\n";
    return;
  }

  /* Guardamos la informacion */
  //Numero total de simbolos terminales
  fich << alphabet_.size();
  fich << "\n";

  //Todos los simbolos terminales de la gramatica
  for (char c : alphabet_) {
    fich << c << " ";
  }
  fich << "\n";

  //Numero de simbolos no terminales de la gramatica
  fich << V_.size() << "\n";

  //Todos los simbolos no terminales de la gramatica
  fich << S_ << " "; //Primero el simbolo de arranque
  for (char c : V_) {
    if (c == S_) continue;
    fich << c << " ";
  }
  fich << "\n";

  //Imprimimos las reglas de produccion
  //Primero el axioma
  fich << S_ << " -> ";
/*  for (int i = 0; i < P_.size(); i++) {
    if (get<0>(P_[i]) != S_) continue; //Buscamos el axioma
    int cont = 0;
    for (pair<char,char> pair : get<1>(P_[i])) {
      fich << get<0>(pair);
      if (get<0>(pair) != '~') fich << get<1>(pair);
      fich << (++cont < get<1>(P_[i]).size() ? " | " : "");
    }
  }*/
  fich << "\n";

  //Las demas reglas
  for (int i = 0; i < P_.size(); i++) {
    if (get<0>(P_[i]) == S_) continue; //Buscamos el axioma
    fich << get<0>(P_[i]) << " -> ";
    int cont = 0;
  /*  for (pair<char,char> pair : get<1>(P_[i])) {
      fich << get<0>(pair);
      if (get<0>(pair) != '~') fich << get<1>(pair);
      fich << (++cont < get<1>(P_[i]).size() ? " | " : "");
    }*/
    fich << "\n";
  }
}

void GR::delete_unit_productions (void) {
  set<pair<char,char> > H;
  for (int i = 0; i < P_.size(); i++) {
    pair<char,char> pair;
    for (std::string str : get<1>(P_[i])) {
      if (str.size() == 1 && V_.find(str[0]) != V_.end()) {
        get<0>(pair) = get<0>(P_[i]);
        get<1>(pair) = str[0];
        H.insert(pair);
      }
    }
  }
  set<pair<char,char> > last_H;
  while (last_H != H) {
    last_H = H;
    for (pair<char,char> pair1 : H) {
      for (pair<char,char> pair2 : H) {
        if (get<1>(pair1) == get<0>(pair2)) {
          pair<char,char> paux(get<0>(pair1),get<1>(pair2));
          if (H.find(paux) == H.end()) {
            H.insert(paux);
          }
        }
      }
    }
  }

  for (int i = 0; i < P_.size(); i++) {
    set<std::string> set_str;
    for (std::string str : get<1>(P_[i])) {
      if (str.length() > 1) {
        set_str.insert(str);
        continue;
      }
      if (H.find(str[0]) != H.end()) continue;
      set_str.insert(str);
    }
  }

  for (int i = 0; i < P_.size(); i++) {
    set<std::string> set_str = get<1>(P_[i]);
    for (pair<char,char> pair : H) {
      if (get<1>(pair) != get<0>(P_[i])) continue;
      for (int j = 0; j < get<1>(P_[i]); j++) {
        if (get<0>(P_[j]) == get<1>(pair)) {
          
        }
      }
    }
  }

  //Mostramos los pares elegidos
  for (pair<char,char> pair : H) {
    std::cout << "(" << get<0>(pair) << "," << get<1>(pair) << ")\n";
  }
}

void GR::delete_empty_productions (void) {
  set<char> H;
  for (int i = 0; i < P_.size(); i++) {
    for (std::string str : get<1>(P_[i])) {
      if (str == "~") {
        H.insert(get<0>(P_[i]));
        break;
      }
    }
  }
  set<char> last_H;
  while (last_H != H) {
    last_H = H;
    for (int i = 0; i < P_.size(); i++) {
      for (std::string str : get<1>(P_[i])) {
        bool found = true;
        for (int k = 0; k < str.length(); k++) {
          if (H.find(str[k]) == H.end()) found = false;
        }
        if (found) H.insert(get<0>(P_[i]));
      }
    }
  }

  //Creamos las nuevas producciones
  set<std::string> set_str, last_set_str;
  for (int i = 0; i < P_.size(); i++) {
    set_str = get<1>(P_[i]);
    //Eliminamos las producciones de tipo A -> ~
    if (set_str.find("~") != set_str.end()) set_str.erase("~");
    while (last_set_str != set_str) {
      last_set_str = set_str;
      for (std::string str : set_str) {
        for (int k = 0; k < str.length(); k++) {
          std::string str1(str);
          if (H.find(str[k]) != H.end()) {
            str1.erase(k, 1);
          }
          if (!str1.empty()) set_str.insert(str1);
        }
      }
    }
    get<1>(P_[i]) = set_str;
  }
  //Insertamos la cadena vacia si '~' pertenece a L(G)
  if (H.find('S') != H.end()) get<1>(P_[0]).insert(std::string("~"));

  //Mostramos el conjunto resultante
  write(std::cout);
}

const set<char> GR::alphabet (void) {
  return alphabet_;
}

ostream& GR::write (ostream& os) {
  for (int i = 0; i < P_.size(); i++) {
    os << get<0>(P_[i]) << " -> ";
    int cont = 0;
    for (std::string str : get<1>(P_[i])) {
      os << str << (++cont < get<1>(P_[i]).size() ? "|" : "");
    }
    os << '\n';
  }
  return os;
}

set<string> GR::split_prototype (const string& s) {
  set<string> result;
  string aux = "";
  for (int i = 5; i < s.length(); i++) {
    aux += s[i];
    if (s[i+1] == '|' || i+1 == s.length()) {
      result.insert(aux);
      aux.clear();
      i++;
    }
  }
  return result;
}
