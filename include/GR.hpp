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
  set<string> P_; //Conjunto de reglas (produciones) de la gramatica

public:

/**
 * @brief Constructor de la gramatica regular
 */

  GR (void);

/**
 * @brief Destructor de la gramatica regular
 */

  ~GR (void);

/**
 * @brief Genera una gramatica regular a partir de un fichero .dfa, que describe un DFA.
 * @param nombreFichero Fichero con extension .dfa a partir del cual generaremos la gramatica regular.
 * @param errorApertura Variable booleana el cual se usa para indicar si ha habido error
 */

 void gen_GR (const char* nombreFichero, bool& errorApertura);

/**
 * @brief Exporta a un fichero con extension '.grammar' la gramatica regular definida por la clase.
 * @param nombreFichero Fichero con extension '.grammar' donde guardar la informacion.
 * @param errorApertura Indicamos si ha habido algun error.
 */

 void export_to (const char* nombreFichero, bool& errorApertura);

 /**
  * @brief Devuelve el alfabeto de la gramatica
  * @return Alfabeto de la gramatica
  */

  const set<char> alphabet (void);

};
