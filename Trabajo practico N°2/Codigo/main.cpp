#include "./Funciones/source.h"
#include <iostream>
#include <cstdlib>
#include <string.h>

//Struct Viejos
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

//Funciones
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

int main()
{
    bool exit = false;
    int option = 0;
    nodoColaPedidos *priCola[6], *ultCola[6];
    NodoLista *listaRepartidores=NULL;
    inicializarCola(priCola, 6);
    inicializarCola(ultCola, 6);
    while (!exit)
    {
        clearScreen();
        cout << "Menu: 1) Recibir Pedido" << endl;
        cout << "      2) Asignar Pedido a un Repartidor" << endl;
        cout << "      3) Mostrar" << endl;
        cout << "      4) Salir" << endl;
        cin >> option;
        clearScreen();
        switch (option)
        {
        case 1:
            recibirPedido(priCola, ultCola);
            break;
        case 2:
            asignarPedidos(priCola, ultCola, listaRepartidores);
            break;
        case 3:
            mostrarRepYPed(listaRepartidores);
            break;
        case 4:
            exit = true;
            cout << "Programa terminado" << endl;
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
        if (verificarComercio(ped.Rubro, ped.NombreCom, zona))
        {
            cin.ignore();
            cout << "Ingrese el domicilio para la entrega: ";
            cin.getline(ped.Domicilio, 39);
            cout << "Ingrese el importe de la compra: ";
            cin >> ped.Importe;
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
        if (strcmp(neg.Nombre, NombreCom) == 0){
            fclose(f);
            return true;
        }
        fread(&neg, sizeof(Negocio), 1, f);
    }
    fclose(f);
    return false;
/* 
    unsigned desde,hasta,medio,cantRegistros;
    int pos=-1;
    desde=0;
    fseek(f,0,SEEK_END);
    cantRegistros=ftell(arch)/sizeof(Parcial);
    hasta=cantRegistros-1;
    while(pos==-1 && desde<=hasta)
    {
        medio=(desde+hasta)/2;
        fseek(f,medio*sizeof(Negocio),SEEK_SET);
        fread(&neg, sizeof(Negocio), 1, f);
        if(strcmp(neg.Nombre, NombreCom) == 0)
            pos=medio;
        else
        {
            if(strcmp(neg.Nombre, NombreCom) > 0)
                hasta=medio-1;
            else
                desde=medio+1;
        }
    }
    return pos;
*/
}

void asignarPedidos(nodoColaPedidos *pri[], nodoColaPedidos *ult[], NodoLista*&lista)
{   
    Repartidores rep;
    Pedido ped;
    int cant,zona,i=0;
    cin.ignore();
    cout<<"Ingrese el nombre del repartidor: ";
    cin.getline(rep.Nombre, 39);
    zona=verificarRepartidor(rep.Nombre);
    if(zona>0){
        cout<<"Ingrese la cantidad de pedidos a entregar: ";
        cin>>cant;
        while(i<cant&&pri[zona]!=NULL)
        {
            desencolarPedidos(pri,ult,ped,zona);
            rep.listaPedidos=NULL;
            NodoLista*p=buscarInsertar(lista,rep);
            insertar(p->info.listaPedidos,ped);
            i++;
        }
        if(i!=cant-1)
            cout<<"Solo se pudieron cargar "<<i+1<<" pedidos de los "<<cant<< "solicitados."<<endl;
    }    
}

int verificarRepartidor(char nombre[40]){
    Repartidor repArchivo;
    FILE*f=fopen("Repartidores.dat","rb");
    fread(&repArchivo,sizeof(Repartidor),1,f);
    while(!feof(f)){
        if(strcmp(repArchivo.Nombre,nombre)==0){
            fclose(f);
            return repArchivo.Zona;
        }
        fread(&repArchivo,sizeof(Repartidor),1,f);
    }
    fclose(f);
    return -1;
}

void desencolarPedidos(nodoColaPedidos *pri[],nodoColaPedidos *ult[],Pedido &ped,int zona){
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
    while(p!=NULL && strcmp(p->info.Nombre,rep.Nombre)<0)
    {
        ant=p;
        p=p->sig;
    }
    if(p!=NULL && strcmp(p->info.Nombre,rep.Nombre)==0)
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

void mostrarRepYPed(NodoLista *listaRepartidores){
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

/*

*/