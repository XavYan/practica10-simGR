#include "../include/GR.hpp"

int main (int argc, char* argv[]) {
  if (argc != 3) {
    cout << "Error en el numero de argumentos.\n";
    cout << "USO: ./GR [file.dfa] [file.grammar]\n";
    return 1;
  }

  GR gr; //Crear gramatica regular

  bool errorApertura = false;

  //Generamos la gramatica
  cout << "Generamos la gramatica...\n";
  gr.gen_GR(argv[1],errorApertura);
  if (errorApertura) return 1;

  //Exportamos a fichero
  cout << "Exportando a fichero " << argv[2] << " los resultados...\n";
  gr.export_to(argv[2],errorApertura);
  if (errorApertura) return 2;

  cout << "Finalizado con exito.\n";
}
