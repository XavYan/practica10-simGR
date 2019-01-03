
/**
* @file DFA.hpp
* @Author Himar Manuel Barquin Carrasco
* @date 11/12/2018
* @brief Clase DFA
*/

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <algorithm>
#include <vector>

#include "state_t.hpp"

class DFA {
private:
  set<state_t> states_; //conjunto de estados
  int all_states_; //cantidad de estados totales
  unsigned init_; //id del estado inicial
  set<char> alphabet_; //alfabeto sobre el que el automata trabaja
public:

  /**
  * @brief Constructor del DFA por defecto. Crea un DFA vacío, sin estados, y el ID del estado inicial se pone a -1
  */

  DFA (void);

  /**
  * @brief Constructor de un DFA al que se le pasa un conjunto de estados ya definidos.
  * @param states Conjunto de estados del automata
  */

  DFA (const set<state_t>& states);

  /**
  * @brief Destructor del DFA.
  */

  ~DFA (void);

  /**
  * @brief Devuelve el ID del estado inicial
  * @return Valor ID del estado inicial
  */

  const unsigned init (void);

/**
 * @brief Devuelve el numero de estados del automata
 * @return Numero de estados del automata
 */

  const unsigned size (void);

/**
 * @brief Comprueba si el automata no tiene estados
 * @return '1' si el automata no tiene estados, '0' en caso contrario
 */

 const bool empty (void);

/**
 * @brief Devuelve una copia del conjunto de estados del DFA
 * @return Conjunto de estados del DFA
 */

 set<state_t> getStates (void);

  /**
  * @brief Crea el DFA
  * @param nombreFichero Nombre del fichero de lectura
  * @param errorApertura Valor que devuelve 1 si ocurre algun error al abrir el archivo, 0 en caso contrario
  */

  void create_dfa (const char* nombreFichero, bool& errorApertura);

  /**
  * @brief Crea el DFA. Version adaptada para crear el DFA tras la minimizacion del automata
  * @param OM Conjunto que almacena los conjuntos comunes entre si
  */

  void create_dfa (const set<set<state_t> >& OM);

  /**
  * @brief Muestra el resultado de la cadena analizada
  */

  void show_chain_result (void);

  /**
  * @brief Muestra los estados de muerte del automata
  */

  void show_dead_states (void);

  /**
  * @brief Muestra el alfabeto que se ha utilizado para la deficion del automata
  */

  void show_alphabet (void);

/**
 * @brief Devuelve el alfabeto que usa el automata
 * @return Retorna el alfabeto
 */

  const set<char> alphabet (void);

  /**
  * @brief Minimiza el DFA.
  */

  void minDFA (void);

  /**
  * @brief Guarda la informacion del automata en un fichero.
  * @param nombreFichero Nombre del fichero donde se va a guardar el automata
  * @param error Indica si ha habido error en la escritura del automata. 1 si ha habido fallo, 0 en caso contrario
  */

  void save (const char* nombreFichero, bool& error);

  /**
  * @brief Imprime por consola el automata, en un formato mas legible por el ser humano
  * @return Salida donde se ha escrito
  */

  ostream& dbg_write (void) const;

  /**
  * @brief Imprime por consola en formato fichero
  * @return Salida donde se ha escrito
  */

  ostream& write (void) const;

  /**
  * @brief Imprime en formato fichero por la salida indicada como parametro
  * @param os Salida donde se va a escribir
  * @return Salida donde se ha escrito
  */

  ostream& write (ostream& os) const;

  /**
  * @brief "Formatea el automata"
  */

  void clear (void);

private:
  set<set<state_t> > new_partition (const set<set<state_t> >& COM);
  set<set<state_t> > descomp (const set<state_t>& G, const set<set<state_t> >& COM);
  set<set<state_t> > part (const set<state_t>& G, const char c, const set<set<state_t> >& COM);

  bool chain_test (const string& chain) const;
  set<state_t> dead_states (void);
  const state_t find_by_id (const unsigned& id) const;
  const bool equal (const set<set<state_t> >& set1, const set<set<state_t> >& set2);
  const bool equal (const set<state_t>& set1, const set<state_t>& set2);

  set<set<state_t> > join (const set<set<state_t> >& set1, const set<set<state_t> >& set2);
  set<state_t> join (const set<state_t>& set1, const set<state_t>& set2);

  void write_set_of_set (const set<set<state_t> >& set);
};
