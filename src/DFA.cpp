#include "../include/DFA.hpp"

using namespace std;
DFA::DFA (void) : states_(), all_states_(0), init_(-1) {}

DFA::DFA (const set<state_t>& states) : states_ (states), all_states_(states.size()), init_(-1) {}

DFA::~DFA (void) { clear(); }

const unsigned DFA::init (void) {
  return init_;
}

const unsigned DFA::size (void) {
  return all_states_;
}

const bool DFA::empty (void) {
  return (all_states_ <= 0 ? true : false);
}

set<state_t> DFA::getStates (void) {
  return states_;
}

void DFA::create_dfa (const char* nombreFichero, bool& errorApertura) {
  states_.clear();
  ifstream fich;
  fich.open(nombreFichero);

  if (!fich.is_open()) {
    cerr << "Error. El fichero no se ha podido abrir con exito.\n";
    errorApertura = 1;
    return;
  }

  //Empezamos leyendo el valor de estados totales
  fich >> (int &)all_states_;

  //Leemos el estado de arranque del DFA
  fich >> (unsigned &)init_;

  //Leemos, para cada estado, sus diferentes valores
  unsigned id, accepted, transition;
  char symbol;
  for (int i = 0; i < all_states_; i++) {
    if (fich.eof()) { cout << "Se ha llegado al final del fichero.\n"; return;}
    //Primero el ID y si es de aceptacion
    fich >> (unsigned &)id >> (unsigned &)accepted;

    //Obtenemos el estado e indicamos si es de aceptacion
    state_t state(id,accepted);

    //Recogemos el numero de transiciones de cada estado
    fich >> (unsigned &)transition;

    //Procedemos a leer los estados sucesores para cada estado
    set<char> symbol_read;
    symbol_read.clear();
    for (int j = 0; j < transition; j++) {
      fich >> (char &)symbol;
      if (alphabet_.find(symbol) == alphabet_.end()) alphabet_.insert(symbol);
      if (symbol_read.find(symbol) != symbol_read.end()) {
        cerr << "Error. Letra repetida en la definicion de las aristas del estado " << state.id()+1 << ". DFA erroneo.\n";
        errorApertura = 1;
        return;
      }

      symbol_read.insert(symbol);
      fich >> (unsigned &)id;
      state.insert_pair(symbol, id);
    }
    states_.insert(state);
  }
  fich.close();
}

void DFA::create_dfa (const set<set<state_t> >& OM) {
  all_states_ = OM.size();

  //Almacenamos en states los estados que no se van a borrar
  set<state_t> states;
  for (set<state_t> set : OM) {
    states.insert(*(set.begin()));
  }

  //Procedemos a modificar las aristas para que apunten a los nodos correctos
  set<state_t> vegar; //Conjuntos que sobran
  for (set<state_t> set : OM) {
    vegar.clear();
    state_t primary(*(set.begin())); //Elemento primario
    //Los ID's de los estados que estan en vegar se sobrescribiran con el ID del estado primary
    if (set.size() != 1) {
      //Calculamos los estados que se van a sustituir, y los almacenamos en vegar
      for (state_t st : set) {
        if (primary.id() != st.id()) {
          vegar.insert(st);
        }
      }

      //Leemos las aristas de los estados de states y buscamos las conexiones erroneas
      for (state_t st : states) {
        state_t aux(st.id(), st.is_accept());
        for (pair<char,unsigned> pair : st.getNext()) {
          if (vegar.find(get<1>(pair)) != vegar.end()) {
            aux.insert_pair(get<0>(pair), primary.id());
          } else {
            aux.insert_pair(get<0>(pair), get<1>(pair));
          }
        }
        states.erase(aux);
        states.insert(aux);
      }
    }
  }

  //Antes de terminar, procedemos a actualizar los ID's de cada estado
  /* Para cada estado, habra que leer todas las aristas, y sustituir el valor de las mismas por el que corresponda */
  unsigned cont = 0;
  for (state_t state : states) { //Leemos los estados
    //Buscamos las aristas que apunten a este nodo
    for (state_t st : states) {
      state_t aux(st.id(),st.is_accept());
      for (pair<char,unsigned> pair : st.getNext()) {
        if (get<1>(pair) == state.id()) {
          aux.insert_pair(get<0>(pair),cont);
        } else {
          aux.insert_pair(get<0>(pair),get<1>(pair));
        }
      }
      states.erase(aux);
      states.insert(aux);
    }
    cont++;
  }
  cont = 0;
  set<state_t> new_states;
  for(state_t state : states) {
    state.new_id(cont++);
    new_states.insert(state);
  }
  states_ = new_states;
}

void DFA::show_chain_result (void) {
  string s;
  bool result;
  cout << "Inserta la cadena a analizar: ";
  cin >> s;
  cout << "\x1b[1J\x1b[H"; //Limpio pantalla
  result = chain_test(s);
  cout << "Cadena de entrada " << (result ? "" : "NO ") << "ACEPTADA\n";
}

void DFA::show_dead_states (void) {
  set<state_t> states = dead_states();

  cout << "\x1b[1J\x1b[H"; //Limpio pantalla
  if (states.empty()) cout << "\tNo hay estados de muerte."; else {
    cout << "\tHay " << states.size() << " estado" << (states.size() > 1 ? "s" : "") << " de muerte, " << (states.size() > 1 ? "los cuales son" : "el cual es") << ": ";
    cout << "{";
    int cont = 0;
    for (set<state_t>::iterator it = states.begin(); it != states.end(); it++) {
      cout << it->id() << (cont++ != states.size()-1 ? "," : "");
    }
    cout << "}";
  }
  cout << "\n";
}

void DFA::show_alphabet (void) {
  cout << "Alfabeto utilizado por el automata: ";
  cout << "{";
  int cont = 0;
  for (set<char>::iterator it = alphabet_.begin(); it != alphabet_.end(); it++) {
    cout << *it << (cont++ != alphabet_.size()-1 ? "," : "");
  }
  cout << "}\n";
}

const set<char> DFA::alphabet (void) {
  return alphabet_;
}

void DFA::minDFA (void) {
  set<set<state_t> > OM; //Conjunto de los estados que vamos a minimizar
  set<state_t> ACEPT; //Conjunto de estados de aceptacion
  set<state_t> NO_ACEPT; //Conjunto de estados de no aceptacion

  for (set<state_t>::iterator it = states_.begin(); it != states_.end(); it++) {
    if (it->is_accept()) {
      ACEPT.insert(*it);
    } else {
      NO_ACEPT.insert(*it);
    }
  }
  OM.insert(ACEPT);
  OM.insert(NO_ACEPT);

  /* La particion tiene que ocurrir hasta que no se pueda partir mas */
  set<set<state_t> > COM;
  unsigned cont = 0;
  do {
    COM = OM; //Antiguo conjunto

    OM = new_partition(COM); //Particiona los conjuntos
  } while (!equal(COM,OM));

  create_dfa(OM); //Crea el autómata. Uso del overload
}

void DFA::save (const char* nombreFichero, bool& error) {
    error = false;
    ofstream fich;
    fich.open(nombreFichero);
    if (!fich.is_open()) {
      cerr << "Error. El fichero no se ha podido abrir con exito.";
      error = true;
      return;
    }
    write(fich);
}

ostream& DFA::dbg_write (void) const {
  cout << "Estado totales: " << all_states_ << "\n";
  cout << "ID del estado inicial: " << init_ << "\n";
  for (set<state_t>::iterator it = states_.begin(); it != states_.end(); it++) {
    it->dbg_write();
  }
  return cout;
}

ostream& DFA::write (void) const {
  cout << all_states_ << "\n";
  cout << init_ << "\n";
  for (set<state_t>::iterator it = states_.begin(); it != states_.end(); it++) {
    cout << it->id() << " ";
    cout << (it->is_accept() ? 1 : 0) << " ";
    cout << (it->getNext().size());
    for (set<pair<char, unsigned> >::iterator k = it->getNext().begin(); k != it->getNext().end(); k++) {
      cout << " " << get<0>(*k) << " " << get<1>(*k);
    }
    cout << "\n";
  }
  return cout;
}

ostream& DFA::write (ostream& os) const {
  os << all_states_ << "\n";
  os << init_ << "\n";
  for (set<state_t>::iterator it = states_.begin(); it != states_.end(); it++) {
    os << it->id() << " ";
    os << (it->is_accept() ? 1 : 0) << " ";
    os << (it->getNext().size());
    for (set<pair<char, unsigned> >::iterator k = it->getNext().begin(); k != it->getNext().end(); k++) {
      os << " " << get<0>(*k) << " " << get<1>(*k);
    }
    os << "\n";
  }
  return os;
}

void DFA::clear (void) {
  states_.clear();
  alphabet_.clear();
}

//METODOS PRIVADOS////////////////////////////////////////


//Se encarga de realizar las divisiones para formar los nuevos conjuntos de estados, si procede
set<set<state_t> > DFA::new_partition (const set<set<state_t> >& COM) {
  set<set<state_t> > aux;

  //Para cada conjunto de estados de COM, vamos descomponiendolos segun se vaya viendo necesario
  for (set<state_t> it : COM) {
    aux = join(aux, descomp(it, COM));
  }
  //aux se volvera el resultado final, que se retorna
  return aux;
}

//Descompone un conjunto de estados si no cumple las reglas de minimizacion
set<set<state_t> > DFA::descomp (const set<state_t>& G, const set<set<state_t> >& COM) {
  set<set<state_t> > T;
  T.insert(G);
  for (char it : alphabet_) { //Para cada letra del alfabeto...
    set<set<state_t> > P;
    for (set<state_t> j : T) { //Lee todos los conjuntos de T
      set<set<state_t> > T2;
      T2 = part(j, it, COM); //T2 sera la particion de la letra it, y el conjunto j
      P = join(P,T2); //P se ira actualizando con las nuevas particiones de T2
    }
    T = P; //P representara la salida T, que se ira actualizando tras cada iteracion
  }
  return T; //Se devuelven todas los valores de T2 hallados
}

set<set<state_t> > DFA::part (const set<state_t>& G, const char c, const set<set<state_t> >& COM) {
  set<set<state_t> > T;
  set<state_t> part;
  for (std::set<state_t> it : COM) { //Leemos todos los conjuntos del conjunto COM
    part.clear();
    for (state_t i : G) { //Para cada elemento del conjunto que vamos a particionar...
      for (std::pair<char,unsigned> j : i.getNext()) { //Leemos las aristas que lo conectan con otros nodos
        if (get<0>(j) == c) { //Si coinciden con la letra buscada...
          state_t aux = find_by_id(get<1>(j)); //Encontramos el estado
          assert(aux.id() != -1);
          if (it.find(aux) != it.end()) { //Si ese estado se encuentra
            set<state_t> aux;
            aux.insert(i);
            part = join(part, aux); //Añadido un estado nuevo
          }
        }
      }
    }
    if (part.size() != 0) T.insert(part);
  }
  return T;
}

bool DFA::chain_test (const string& chain) const {
  cout << "Cadena de entrada: " << chain << "\n";
  cout << "ESTADO ACTUAL\tENTRADA\t\tSIGUIENTE ESTADO\n";
  state_t aux = find_by_id(init_);
  unsigned cid;
  for (int i = 0; i < chain.length(); i++) {
    cout << aux.id() << "\t\t" << chain[i] << "\t\t";
    cid = aux.find_by_letter(chain[i]);
    if (cid != -1) {
      aux = find_by_id(cid);
      cout << aux.id() << "\n";
    } else {
      cout << "NONE\n";
      return false;
    }
  }
  if (aux.is_accept()) return true; else return false;
}

set<state_t> DFA::dead_states (void) {
  set<state_t> deadStates;
  for (set<state_t>::iterator i = states_.begin(); i != states_.end(); i++) {
    bool is_dead = true;
    for (set<pair<char,unsigned> >::iterator j = i->getNext().begin(); j != i->getNext().end(); j++) {
      if (i->id() != get<1>(*j)) { is_dead = false; break; }
    }
    if (is_dead) deadStates.insert(*i);
  }
  return deadStates;
}

const state_t DFA::find_by_id (const unsigned& id) const {

  vector<state_t> aux;
  for (std::set<state_t>::iterator it = states_.begin(); it != states_.end(); it++) {
    if (it->id() == id) return *it;
  }
  state_t none(-1);
  return none;
}

const bool DFA::equal (const set<set<state_t> >& set1, const set<set<state_t> >& set2) {
  if (set1.size() != set2.size()) return false;
  for (set<state_t> it : set1) {
    if (set2.find(it) == set2.end()) return false;
  }
  return true;
}

const bool DFA::equal (const set<state_t>& set1, const set<state_t>& set2) {
  if (set1.size() != set2.size()) return false;
  for (state_t it : set1) {
    if (set2.find(it) == set2.end()) return false;
  }
  return true;
}

set<set<state_t> > DFA::join (const set<set<state_t> >& set1, const set<set<state_t> >& set2) {
  set<set<state_t> > result;
  for (set<state_t> it : set1) {
    result.insert(it);
  }
  for (set<state_t> it : set2) {
    result.insert(it);
  }
  return result;
}

set<state_t> DFA::join (const set<state_t>& set1, const set<state_t>& set2) {
  set<state_t> result;
  for (state_t it : set1) {
    result.insert(it);
  }
  for (state_t it : set2) {
    result.insert(it);
  }
  return result;
}

void DFA::write_set_of_set (const set<set<state_t> >& set) {
  assert(set.size() != 0);
  cout << "{";
  unsigned cont1 = 0;
  for (std::set<std::set<state_t> >::iterator i = set.begin(); i != set.end(); i++) {
    cout << "{";
    unsigned cont2 = 0;
    for (std::set<state_t>::iterator j = i->begin(); j != i->end(); j++) {
      cout << j->id() << (cont2 + 1 != i->size() ? "," : "");
      cont2++;
    }
    cout << "}";
    cout << (cont1 + 1 != set.size() ? "," : "");
    cont1++;
  }
  cout << "}";
}
