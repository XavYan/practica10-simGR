#include "../include/GR.hpp"

int main (int argc, char* argv[]) {
  if (argc != 3) {
    cout << "Error en el numero de argumentos.\n";
    cout << "USO: ./GR [file.dfa] [file.grammar]\n";
    return 1;
  }

  bool errorApertura = false;

  //Generamos la gramatica
  cout << "Generando la gramatica...\n";
  GR gr(argv[1], errorApertura);
  if (errorApertura) return 1;
  GR new_gr(gr); //Creamos el objeto que se va a modificar

  //Simplificamos
  cout << "Eliminando producciones y variables inutiles...\n";
  new_gr.delete_useless_elements();

  cout << "Eliminando producciones vacias...\n";
  new_gr.delete_empty_productions();

  cout << "Eliminando producciones unitarias...\n";
  new_gr.delete_unit_productions();

  cout << "Eliminando producciones y variables inutiles por segunda vez...\n";
  new_gr.delete_useless_elements();

  //Exportamos
  cout << "Exportando a fichero " << argv[2] << "...\n";
  new_gr.export_to(argv[2], errorApertura);
  if (errorApertura) return 1;

  cout << "Finalizado con exito.\n";
}
