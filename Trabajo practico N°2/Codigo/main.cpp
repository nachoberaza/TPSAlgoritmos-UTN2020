/*  Seleccion de librerias opcionales para cada sistema */
#ifdef _WIN32
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#define WIDTH 7
#endif

#ifdef linux
#include "stdio.h"
#include <cstdlib>
#endif
/* **************************************************** */

#include <iostream>
#include <cstdlib>
#include <string.h>
#include <stdio.h>
using namespace std;

//Struct Viejos (para los archivos)
struct Repartidor
{
    unsigned DNI;
    char Nombre[40];
    unsigned Zona;
};

struct Negocio
{
    char Nombre[40];
    unsigned Zona;
};

//Struct Nuevos
struct Pedido
{
    char Domicilio[40];
    char NombreCom[40];
    unsigned Rubro;
    float Importe;
};

struct NodoSubLista
{
    Pedido info;
    NodoSubLista*sig;
};

struct Repartidores
{
    char Nombre[40];
    NodoSubLista*listaPedidos;
};

struct NodoLista
{
    Repartidores info;
    NodoLista*sig;
};

struct nodoColaPedidos
{
    Pedido info;
    nodoColaPedidos *sig;
};

struct DatoArbol{
    char NombreCom[40];
    int cantPedidos;
};

struct NodoArbol
{
    DatoArbol info;
    NodoArbol*izq;
    NodoArbol*der;
};

/* ------------------ Prototipos de Funciones ------------------ */
//Limpia la pantalla
int clearScreen();
//"Ingrese una tecla para continuar"
int pauseScreen();
void recibirPedido(nodoColaPedidos *pri[], nodoColaPedidos *ult[]);
void inicializarCola(nodoColaPedidos *cola[], int num);
void encolarPedido(nodoColaPedidos *pri[], nodoColaPedidos *ult[], Pedido ped, int zona);
bool verificarComercio(unsigned Rubro, char NombreCom[40], int zona);
void asignarPedidos(nodoColaPedidos *pri[], nodoColaPedidos *ult[], NodoLista*&lista);
int verificarRepartidor(char nombre[40]);
void desencolarPedidos(nodoColaPedidos *pri[],nodoColaPedidos *ult[],Pedido &ped,int zona);
void insertar(NodoSubLista*&sublista,Pedido ped);
void mostrarRepYPed(NodoLista *listaRepartidores);
NodoLista* buscarInsertar(NodoLista*&lista,Repartidores rep);
void crearArbol(NodoLista *listaRepartidores);
void buscarInsertarArbol(NodoArbol*&raiz,char NombreCom[40]);
void listarArbol(NodoArbol*raiz);
/* ------------------ *********************** ------------------ */

int main()
{
    bool exit = false;
    int option = 0;
    nodoColaPedidos *priCola[6], *ultCola[6];
    NodoLista *listaRepartidores=NULL;
    //Inicializamos los vectores de colas en NULL
    inicializarCola(priCola, 6);
    inicializarCola(ultCola, 6);
    while (!exit)
    {
        clearScreen();
        cout << "      === Menu===      "<<endl; 
        cout << "      1) Recibir Pedido" << endl;
        cout << "      2) Asignar Pedido a un Repartidor" << endl;
        cout << "      3) Mostrar" << endl;
        cout << "      4) Salir" << endl;
        cin >> option;
        clearScreen();
        switch (option)
        {
        case 1:
            //Ingresar los pedidos
            recibirPedido(priCola, ultCola);
            break;
        case 2:
            //Asignar pedidos a los repartidores
            asignarPedidos(priCola, ultCola, listaRepartidores);
            break;
        case 3:
            //Mostrar lista de repartidores y sus sublistas de pedidos
            mostrarRepYPed(listaRepartidores);
            break;
        case 4:
            //Crear arbol de negocios que recibieron pedidos y la cantidad de pedidos
            crearArbol(listaRepartidores);
            exit = true;
            break;
        default:
            cout << "Ingrese una opcion valida" << endl;
            break;
        }
        pauseScreen();
    }
    return 0;
}

void recibirPedido(nodoColaPedidos *pri[], nodoColaPedidos *ult[])
{
    int zona;
    Pedido ped;
    cout << "Ingrese la zona de entrega(1 a 6, 0 para salir): ";
    cin >> zona;
    while (zona != 0)
    {
        cin.ignore();
        cout << "Ingrese el nombre del comercio: ";
        cin.getline(ped.NombreCom, 39);
        cout << "Ingrese el rubro del comercio (1: Heladeria, 2: Pizzeria, 3: Bebidas, 4:Parrilla): ";
        cin >> ped.Rubro;
        //El if verifica que exista ese comercio en esa zona y en ese rubro
        if (verificarComercio(ped.Rubro, ped.NombreCom, zona))
        {
            cin.ignore();
            cout << "Ingrese el domicilio para la entrega: ";
            cin.getline(ped.Domicilio, 39);
            cout << "Ingrese el importe de la compra: ";
            cin >> ped.Importe;
            //Agrega el pedido a la cola que le corresponde por su zona
            encolarPedido(pri, ult, ped, zona);
        }
        else
        {
            cout << "Ese comercio no existe dentro de la zona seleccionada"<<endl;
        }
        cout << "Ingrese la zona de entrega(1 a 6, 0 para salir): ";
        cin >> zona;
    }
}

void inicializarCola(nodoColaPedidos *cola[], int num)
{
    for (int i = 0; i < num; i++)
        cola[i] = NULL;
}

void encolarPedido(nodoColaPedidos *pri[], nodoColaPedidos *ult[], Pedido ped, int zona)
{
    nodoColaPedidos *p = new nodoColaPedidos;
    p->info = ped;
    p->sig = NULL;
    if (pri[zona] == NULL)
        pri[zona] = p;
    else
        ult[zona]->sig = p;
    ult[zona] = p;
}

bool verificarComercio(unsigned Rubro, char NombreCom[40], int zona)
{
    Negocio neg;
    FILE *f;
    switch (Rubro)
    {
    case 1:
        f = fopen("Heladerias.dat", "rb");
        break;

    case 2:
        f = fopen("Pizzerias.dat", "rb");
        break;

    case 3:
        f = fopen("Bebidas.dat", "rb");
        break;
    case 4:
        f = fopen("Parrillas.dat", "rb");
        break;
    default:
        return false;
        break;
    }
    fread(&neg, sizeof(Negocio), 1, f);
    while (!feof(f)){
        if (strcmpi(neg.Nombre, NombreCom) == 0 && neg.Zona==zona){
            fclose(f);
            return true;
        }
        fread(&neg, sizeof(Negocio), 1, f);
    }
    fclose(f);
    return false;
}

void asignarPedidos(nodoColaPedidos *pri[], nodoColaPedidos *ult[], NodoLista*&lista)
{   
    Repartidores rep;
    Pedido ped;
    int cant,zona,i=0;
    cin.ignore();
    cout<<"Ingrese el nombre del repartidor: ";
    cin.getline(rep.Nombre, 39);
    //Verifica que el repartidor exista en el archivo de repartidores
    zona=verificarRepartidor(rep.Nombre);
    if(zona>0){
        cout<<"Ingrese la cantidad de pedidos a entregar: ";
        cin>>cant;
        while(i<cant && pri[zona]!=NULL)
        {
            //elimina nodos de la respectiva cola de pedidos
            desencolarPedidos(pri,ult,ped,zona);
            rep.listaPedidos=NULL;            
            //busca al repartidor en la lista de repartidores, si es su primer pedido, lo agrega.
            NodoLista*p=buscarInsertar(lista,rep);
            //inserta el pedido en la sublista de su repartidor
            insertar(p->info.listaPedidos,ped);
            i++;
        }
        //en el caso de que quiera cargar mas pedidos de los que existen, 
        //informa la solo los que fueron posible entregar
        if(i!=cant)
            cout<<"Solo se pudieron cargar "<<i<<" pedidos de los "<<cant<< " solicitados."<<endl;
    }
    else
        cout<<"No contamos con ese repartidor en nuestro equipo."<<endl;   
}

int verificarRepartidor(char nombre[40])
{
    Repartidor repArchivo;
    FILE*f=fopen("Repartidores.dat","rb");
    fread(&repArchivo,sizeof(Repartidor),1,f);
    while(!feof(f)){
        if(strcmpi(repArchivo.Nombre,nombre)==0){
            fclose(f);
            return repArchivo.Zona;
        }
        fread(&repArchivo,sizeof(Repartidor),1,f);
    }
    fclose(f);
    return -1;
}

void desencolarPedidos(nodoColaPedidos *pri[],nodoColaPedidos *ult[],Pedido &ped,int zona)
{
   nodoColaPedidos*p=pri[zona];
   ped=p->info;
   pri[zona]=p->sig;
   delete p;
   if(pri[zona]==NULL)
        ult[zona]=NULL;
}

NodoLista* buscarInsertar(NodoLista*&lista,Repartidores rep)
{
    NodoLista*ant,*p=lista;
    while(p!=NULL && strcmpi(p->info.Nombre,rep.Nombre)<0)
    {
        ant=p;
        p=p->sig;
    }
    if(p!=NULL && strcmpi(p->info.Nombre,rep.Nombre)==0)
        return p;
    else
    {
        NodoLista*n=new NodoLista;
        n->info=rep;
        n->sig=p;
        if(p!=lista)
            ant->sig=n;
        else
            lista=n;
        return n;
    }
}

void insertar(NodoSubLista*&sublista,Pedido ped)
{
    NodoSubLista*n=new NodoSubLista,*p=sublista,*ant;
    n->info=ped;
    while(p!=NULL && p->info.Importe > ped.Importe)
    {
        ant=p;
        p=p->sig;
    }
    n->sig=p;
    if(p!=sublista)
        ant->sig=n;
    else
        sublista=n;
}

//muestra la lista de repartidores, con sus respectivas sublistas de pedidos entregados
void mostrarRepYPed(NodoLista *listaRepartidores)
{
    NodoSubLista *r;
    while(listaRepartidores!=NULL){
        cout<<"Repartidor: " << listaRepartidores->info.Nombre<<endl;
        r=listaRepartidores->info.listaPedidos;
        while(r!=NULL){
            cout<<"Importe: "<<r->info.Importe<<" |Domicilio: "<<r->info.Domicilio<<" |Negocio: "<<r->info.NombreCom<<endl;
            r=r->sig;
        }  
        cout<<"------------------"<<endl;
        listaRepartidores=listaRepartidores->sig; 
    }
}

void crearArbol(NodoLista *listaRepartidores)
{
    NodoArbol*raiz=NULL;
    NodoSubLista *r;
    while(listaRepartidores!=NULL){
        r=listaRepartidores->info.listaPedidos;
        while(r!=NULL){
            //busca si existe el local en el árbol, si existe le suma 1 a la cantidad
            //de pedidos del local. Si no existe, lo agrega al árbol.
            buscarInsertarArbol(raiz,r->info.NombreCom);
            r=r->sig;
        }
        listaRepartidores=listaRepartidores->sig; 
    }
    //Muestra los negocios del arbol y la cantidad de pedidos de cada uno.
    listarArbol(raiz);
}

void buscarInsertarArbol(NodoArbol*&raiz,char NombreCom[40])
{
    NodoArbol*p=raiz;
    while(p!=NULL && strcmpi(p->info.NombreCom,NombreCom)!=0)
    {
        if(strcmpi(NombreCom,p->info.NombreCom)<0)
            p=p->izq;
        else
            p=p->der;
    }
    if(p!=NULL && strcmpi(p->info.NombreCom,NombreCom)==0)
        p->info.cantPedidos++;
    else
    {
        NodoArbol*p,*ant,*n=new NodoArbol;
        n->info.cantPedidos=1;
        strcpy(n->info.NombreCom,NombreCom);
        n->izq=n->der=NULL;
        p=raiz;
        while(p!=NULL)
        {
            ant=p;
            if(strcmpi(NombreCom,ant->info.NombreCom)<0)
                p=p->izq;
            else
                p=p->der;
        }
        if(raiz==NULL)
            raiz=n;
        else
        {
            if(strcmpi(NombreCom,ant->info.NombreCom)<0)
                ant->izq=n;
            else
                ant->der=n;
        }
    }    
}

void listarArbol(NodoArbol*raiz)
{
    if(raiz!=NULL)
    {
        listarArbol(raiz->izq);
        cout<<"Negocio: "<<raiz->info.NombreCom<<" |Cant. de pedidos: "<<raiz->info.cantPedidos<<endl;
        listarArbol(raiz->der);
    }
}


/* Clear and pause */
int clearScreen(){
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
    return 0;
}

int pauseScreen(){
    #ifdef _WIN32
        system("pause");
    #else
        cout<<"Presione enter para continuar..."<<endl;
        system("read _");
    #endif
    return 0;
}
/* *************** */
