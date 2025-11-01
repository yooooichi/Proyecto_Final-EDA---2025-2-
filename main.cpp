#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <string>
#include <random>
#include <ctime>
using namespace std;

class Persona {
  private:
  string DNI;
  string Nombres;
  string Apellidos;
  string Nacionalidad;
  string Lugar_Nacimiento;
  string Direccion;
  string Telefono;
  string Correo;
  string Estado_C;

  public:
  string getDNI() { return DNI; }
  string getNombres() { return Nombres; }
  string getApellidos() { return Apellidos; }
  string getNacionalidad() { return Nacionalidad; }
  string getLugar_Nacimiento() { return Lugar_Nacimiento; }
  string getDireccion() { return Direccion; }
  string getTelefono() { return Telefono; }
  string getCorreo() { return Correo; }
  string getEstado() { return Estado_C; }

  void setDNI(string x) { DNI = x; }
  void setNombres(string x) { Nombres = x; }
  void setApellidos(string x) { Apellidos = x; }
  void setNacionalidad(string x) { Nacionalidad = x; }
  void setLugar_Nacimiento(string x) { Lugar_Nacimiento = x; }
  void setDireccion(string x) { Direccion = x; }
  void setTelefono(string x) { Telefono = x; }
  void setCorreo(string x) { Correo = x; }
  void setEstado(string x) { Estado_C = x; }
  
  void CrearPersona(string a, string b, string c, string d, string e, string f, string g, string h, string i) {
    setDNI(a);
    setNombres(b);
    setApellidos(c);
    setNacionalidad(d);
    setLugar_Nacimiento(e);
    setDireccion(f);
    setTelefono(g);
    setCorreo(h);
    setEstado(i);
  }

  void MostrarPersona() {
    cout << "DNI: " << getDNI() << endl;
    cout << "Nombres: " << getNombres() << endl;
    cout << "Apellidos: " << getApellidos() << endl;
    cout << "Nacionalidad: " << getNacionalidad() << endl;
    cout << "Lugar de nacimiento: " << getLugar_Nacimiento() << endl;
    cout << "Dirección: " << getDireccion() << endl;
    cout << "Telefono: " << getTelefono() << endl;
    cout << "Correo: " << getCorreo() << endl;
    cout << "Estado civil: " << getEstado() << endl;
  }
};

ofstream archivoDatos("datos.csv");


int main() {


  return 0;
}
