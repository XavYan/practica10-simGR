#include "DFA.hpp"

/**
 * @file GR.hpp
 * @Author Himar Manuel Barquin Carrasco
 * @date 13/12/2018
 * @brief Clase GR
 */

class GR {
private:
  set<char> alphabet_; //Alfabeto de la gramatica
  set<char> V_; //Conjunto de simbolos no terminales
  char S_; //Axioma de la gramatica
  vector<pair<char, set<string> > > P_; //Conjunto de reglas (produciones) de la gramatica. El primer char es la letra, el segundo el nodo

public:

/**
 * @brief Constructor de la gramatica regular por defecto
 */

  GR (void);

/**
 * @brief Constructor para crear el GR a partir de un fichero. Solo funciona en fichero con extension 'grammar' o 'dfa'.
 * @param nombreFichero Nombre y direccion del fichero que se desea abrir.
 * @param errorApertura Booleano que indica si ha habido un error durante la creacion del objeto o si todo ha funcionado correctamente.
 */

  GR (const string& nombreFichero, bool& errorApertura);

/**
 * @brief Destructor de la gramatica regular.
 */

  ~GR (void);

/**
 * @brief Genera un GR a partir de un fichero con extension 'grammar'.
 * nombreFichero Fichero con extension 'grammar' que contiene la informacion de la gramatica.
 * errorApertura Booleano que indica si ha habido un fallo durante la creacion del mismo.
 */

  void gen_GR (const char* nombreFichero, bool& errorApertura);

/**
 * @brief Genera una gramatica regular a partir de un fichero .dfa, que describe un DFA.
 * @param nombreFichero Fichero con extension .dfa a partir del cual generaremos la gramatica regular.
 * @param errorApertura Variable booleana el cual se usa para indicar si ha habido error.
 */

 void DFAtoGR (const char* nombreFichero, bool& errorApertura);

/**
 * @brief Exporta a un fichero con extension '.grammar' la gramatica regular definida por la clase.
 * @param nombreFichero Fichero con extension '.grammar' donde guardar la informacion.
 * @param errorApertura Indicamos si ha habido algun error.
 */

 void export_to (const char* nombreFichero, bool& errorApertura);

/**
 * @brief Detecta y elimina las producciones vacias que existan en la gramatica.
 */

 void delete_empty_productions (void);

/**
 * @brief Detecta y elimina las producciones unitarias que existan en la gramatica
 */

 void delete_unit_productions (void);

 /**
  * @brief Devuelve el alfabeto de la gramatica
  * @return Alfabeto de la gramatica
  */

  const set<char> alphabet (void);

  /**
   * @brief Escribe la gramatica la salida indicada por parametro
   * @param os Salida donde escribir. Puede ser std::cout como cualquier fichero.
   * @return elemento solicitado por parametro
   */

  ostream& write (ostream& os);

private:
  set<string> split_prototype (const string& s);

};
