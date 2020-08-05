#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>

using namespace std;

struct Negocio{
	char Nombre[40];
    unsigned Zona;
};

void cargarNegocios(int negPorZona[6][4],Negocio hel[60],Negocio piz[60],Negocio beb[60],Negocio par[60],int cont[4]);
void vaciar(Negocio neg[], int cant);
void ordenarNegocios(Negocio x[],int t);
void guardarNegocios(Negocio x[],int cant,string nom,string arhivo);
void cargarMas(int &a, Negocio x[60], int rubro,int negPorZona[6][4]);
void tabla(int negPorZona[6][4]);
void cantidadEspacios(int num, int car_disponble);

int main(){
    //Declara la matriz de cantidad de negocios por cada rubro por cada zona y la inicializa en 0
    int negPorZona[6][4];
    for(int i=0;i<6;i++){ 
        for(int j=0;j<4;j++){         
            negPorZona[i][j]=0;
        }
    }
    //Declara e incializa un vector de contadores junto con un vector de struct 
    //para cada rubro y uno para todos los negocios.
    int cont[4]={0,0,0,0};
    Negocio hel[60],piz[60],beb[60],par[60];
    //Vacian los vectores de struct.
    vaciar(hel,60);
    vaciar(piz,60);
    vaciar(beb,60);
    vaciar(par,60);
    //Carga todos los negocios en un vector de struct unico para todos los rubros y
    //cuenta cuantos negocios hay por cada zona por  cada rubro.
    cargarNegocios(negPorZona,hel,piz,beb,par,cont);
    //Ordenan los vectores de struct de negocios alfabeticamente (por burbujeo).
    ordenarNegocios(hel,cont[0]);
    ordenarNegocios(piz,cont[1]);  
    ordenarNegocios(beb,cont[2]);
    ordenarNegocios(par,cont[3]);
    //Muestran los negocios por rubro y los guarda en sus respectivos archivos.
    cout<<endl<<"Nombre y zona de los negocios segun su rubro:"<<endl;
    guardarNegocios(hel,60,"Heladerias:","Heladerias.dat");
    guardarNegocios(piz,60,"Pizzerias:","Pizzerias.dat");
    guardarNegocios(beb,60,"Bebidas:","Bebidas.dat");
    guardarNegocios(par,60,"Parrillas:","Parrillas.dat");
    //Se muestra la cantidad de negocios por rubro por zona
    tabla(negPorZona);
    return 0;
}

void guardarNegocios(Negocio x[],int cant,string nom,string archivo){
    char arch[15];
    strcpy(arch,archivo.c_str());
    cout<<nom<<endl;
    int i=0;
    FILE *f=fopen(arch,"wb");
    while(i<cant){
        if(x[i].Zona!=0){
            cout<<"-Nombre: "<<x[i].Nombre<<" -Zona: "<<x[i].Zona<<endl;
            fwrite(&x[i],sizeof(Negocio),1,f);
        }
        i++;
    }
    fclose(f);
}

void ordenarNegocios(Negocio x[],int t){
    int i=0,j;
    Negocio aux;
    bool cambio;
    do{
        cambio=false;
        for(j=0;j<t-1-i;j++){
            if(strcmpi(x[j].Nombre,x[j+1].Nombre)>0){  
                strcpy(aux.Nombre,x[j].Nombre);
                aux.Zona=x[j].Zona;
                strcpy(x[j].Nombre,x[j+1].Nombre);
                x[j].Zona=x[j+1].Zona;
                strcpy(x[j+1].Nombre,aux.Nombre);
                x[j+1].Zona=aux.Zona;
                cambio=true;
            }
        }
        i++;
    }while(i<t && cambio);
}

void vaciar(Negocio neg[], int cant){
    for(int i=0;i<cant;i++){          
            neg[i].Zona=0;
            strcpy(neg[i].Nombre,"");
    }
}

void cargarNegocios(int negPorZona[6][4],Negocio hel[60],Negocio piz[60],Negocio beb[60],Negocio par[60],int cont[4]){
    int rubro;
    do{
        cout<<"Ingrese el rubro (1: Heladeria, 2: Pizzeria, 3: Bebidas, 4:Parrilla, 0: salir): ";
        cin>>rubro;
    }while(rubro<0||rubro>4);    
    while(rubro!=0){
        switch(rubro){
            case 1:
                cargarMas(cont[rubro-1],hel,rubro,negPorZona);
            break;
            case 2:
                cargarMas(cont[rubro-1],piz,rubro,negPorZona);
            break;
            case 3:
                cargarMas(cont[rubro-1],beb,rubro,negPorZona);
            break;
            case 4:
                cargarMas(cont[rubro-1],par,rubro,negPorZona);
            break;
        } 
        do{
            cout<<"Ingrese el rubro (1: Heladeria, 2: Pizzeria, 3: Bebidas, 4:Parrilla, 0: salir): ";
            cin>>rubro;
        }while(rubro<0||rubro>4); 
    }    
}

//Funcion para poder cargar los negocios en vectores de struct segun su rubro, pasado como parametro.
void cargarMas(int &a,Negocio x[],int rubro,int negPorZona[6][4]){
    cout<<"Ingrese la zona (de 1 a 6): ";
    cin>>x[a].Zona;
    if(negPorZona[x[a].Zona-1][rubro-1]<10){ 
        cin.ignore();
        cout<<"Ingrese el nombre del comercio: ";
        cin.getline(x[a].Nombre,40);     
        negPorZona[x[a].Zona-1][rubro-1]++;
        a++;
    }else{
        cout<<"Esa zona ya tiene 10 negocios de ese tipo."<<endl;
        x[a].Zona=0;
    }
}

void tabla(int cant[6][4])
{
    cout<<endl<<"Cantidad de negocios por rubro por zona:"<<endl;
    cout<<"                          | Zona 1 | Zona 2 | Zona 3 | Zona 4 | Zona 5 | Zona 6"<<endl;
    cout<<"--------------------------|--------|--------|--------|--------|--------|--------"<<endl;
    cout<<"        Heladerias        ";
    for(int i=0;i<6;i++)
        cantidadEspacios(cant[i][0],10);
    cout<<endl<<"--------------------------|--------|--------|--------|--------|--------|--------"<<endl;
    cout<<"         Pizzerias        ";
    for(int i=0;i<6;i++)
        cantidadEspacios(cant[i][1],10);
    cout<<endl<<"--------------------------|--------|--------|--------|--------|--------|--------"<<endl;
    cout<<"          Bebidas         ";
    for(int i=0;i<6;i++)
        cantidadEspacios(cant[i][2],10);
    cout<<endl<<"--------------------------|--------|--------|--------|--------|--------|--------"<<endl;
    cout<<"         Parrillas        ";
    for(int i=0;i<6;i++)
        cantidadEspacios(cant[i][3],10);
}

//Funcion para contar los espacios de la tabla y que quede asi mas prolija la exposicion de datos.
void cantidadEspacios(int num, int car_disponble)
{
    int cont=1,espacios,mitad,temp=num;
    while((temp/10)>0)
    {
        temp/=10;
        cont++;
    }
    for(int i=0;i<(car_disponble-cont);i++)
    {
        if(i==0)
            cout<<"|";
        else if(i==(car_disponble/2)-(cont/2))
            cout<<num;
        else
            cout<<" ";
    }
}