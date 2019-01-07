#include "../include/GR.hpp"

int main (int argc, char* argv[]) {
  if (argc != 3) {
    cout << "Error en el numero de argumentos.\n";
    cout << "USO: ./GR [file.dfa] [file.grammar]\n";
    return 1;
  }

  bool errorApertura = false;

  //Generamos la gramatica
  cout << "Generamos la gramatica...\n";
  GR gr(argv[1], errorApertura);
  if (errorApertura) return 1;

  //Simplificamos
  cout << "Eliminando producciones y variables inutiles...\n";
  gr.delete_useless_elements();
  
  cout << "Eliminando producciones vacias...\n";
  gr.delete_empty_productions();

  cout << "Eliminando producciones unitarias...\n";
  gr.delete_unit_productions();

  cout << "Finalizado con exito.\n";
}
