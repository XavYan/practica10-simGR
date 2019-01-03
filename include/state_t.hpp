
/**
* @file state_t.hpp
* @Author Himar Manuel Barquin Carrasco
* @date 11/12/2018
* @brief Clase state_t (Estado)
*/

#pragma once

#include <iostream>
#include <cassert>
#include <set>
#include <utility>

using namespace std;

class state_t {
private:
  unsigned id_; //identificador del estado
  bool accept_; //indica si es un nodo de aceptacion
  set<pair<char, unsigned> > next_; //Conjunto de estados siguientes a la arista

public:

  /**
  * @brief Constructor que crea el estado
  * @param id Identificador del estado, por defecto -1
  * @param accept Indica si el estado es de aceptacion o no. Por defecto false
  */

  state_t (const unsigned id = -1, const bool accept = false);

  /**
  * @brief Constructor copia
  * @param state estado donde se copian los atributos
  */

  state_t (const state_t& state);

  /**
  * @brief Destructor de la clase estado (state_t)
  */

  ~state_t (void);

  /**
  * @brief Devuelve el ID del estado
  * @return ID del estado
  */

  const unsigned id (void) const;

  /**
  * @brief Devuelve el conjunto de aristas del estado
  * @return Conjunto de aristas del estado
  */

  set<pair<char, unsigned> > getNext (void) const;

  /**
  * @brief Devuelve un booleano inidicando si el estado es de aceptacion o no
  * @return Booleano indicando si el estado es de aceptacion
  */

  const bool is_accept (void) const;

  /**
  * @brief Devuelve el puntero que contiene dicha letra. Usado para transitar entre los estados
  * @param c Caracter que se usa para encontrar la arista por la que transita
  * @return ID del estado al que se apunta a traves de la arista
  */

  //Devuelve el puntero par que contiene dicha letra. Usado para transitar entre los estados
  unsigned find_by_letter (const char c);

  /**
  * @brief Inserta una nueva arista
  * @param symbol Simbolo de transicion
  * @param id ID del estado al que transita
  */

  void insert_pair (const char symbol, const unsigned id);

  /**
  * @brief Modifica el ID actual del estado
  * @param id ID nuevo del estado
  */

  void new_id (const unsigned& id);

  /**
  * @brief Imprime por pantalla el estado y sus conexiones en un formato legible
  * @return Salida donde se escribio
  */

  ostream& dbg_write (void) const;

  /**
  * @brief Sobrecarga del operador '<'
  * @param state Estado que se compara con el actual
  * @return Booleano que indica el resultado de la comparacion
  */

  const bool operator< (const state_t state) const;
};
