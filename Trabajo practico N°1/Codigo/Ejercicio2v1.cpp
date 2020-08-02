/*
Primera version del ejercicio 2 con una pequeña diferencia del enunciado ya que no esta optimizado
y el rubro se encuentra en la struct de Negocio.
*/
#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

struct Negocio{
	char Nombre[40];
    unsigned Rubro;
    unsigned Zona;
};

struct 

void cargarNegocios(int negPorZona[6][4], Negocio neg[240]);
void vaciar(Negocio neg[], int cant);
void separarNegocios(Negocio neg[240],Negocio hel[60],Negocio piz[60],Negocio beb[60],Negocio par[60],int cont[4]);
void ordenarNegocios(Negocio x[],int t);
void guardarNegocios(Negocio x[],int cant,string nom,string arhivo);

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
    Negocio neg[240],hel[60],piz[60],beb[60],par[60];
    //Vacian los vectores de struct.
    vaciar(neg,240);
    vaciar(hel,60);
    vaciar(piz,60);
    vaciar(beb,60);
    vaciar(par,60);
    //Carga todos los negocios en un vector de struct unico para todos los rubros y
    //cuenta cuantos negocios hay por cada zona por  cada rubro.
    cargarNegocios(negPorZona,neg);
    //Separa todos los negocios en 4 vectores struct, uno por cada rubro.
    separarNegocios(neg,hel,piz,beb,par,cont);
    //Ordenan los vectores de struct de negocios alfabeticamente (por burbujeo).
    ordenarNegocios(hel,cont[0]);
    ordenarNegocios(piz,cont[1]);  
    ordenarNegocios(beb,cont[2]);
    ordenarNegocios(par,cont[3]);
    //Muestran los negocios por rubro y los guarda en sus respectivos archivos.
    guardarNegocios(hel,60,"Heladerias:","Heladerias.dat");
    guardarNegocios(piz,60,"Pizzerias:","Pizzerias.dat");
    guardarNegocios(beb,60,"Bebidas:","Bebidas.dat");
    guardarNegocios(par,60,"Parrillas:","Parrillas.dat"); 
    return 0;
}

void guardarNegocios(Negocio x[],int cant,string nom,string archivo){
    char arch[15];
    strcpy(arch,archivo.c_str());
    cout<<nom<<endl;
    int i=0;
    FILE *f=fopen(arch,"wb");
    while(i<cant){
        if(x[i].Rubro!=0){
            cout<<"Nombre: "<<x[i].Nombre<<" Rubro: "<<x[i].Rubro<< " Zona: "<<x[i].Zona<<endl;
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
            if(strcmp(x[j].Nombre,x[j+1].Nombre)>0){
                strcpy(aux.Nombre,x[j].Nombre);
                aux.Rubro=x[j].Rubro;
                aux.Zona=x[j].Zona;
                strcpy(x[j].Nombre,x[j+1].Nombre);
                x[j].Rubro=x[j+1].Rubro;
                x[j].Zona=x[j+1].Zona;
                strcpy(x[j+1].Nombre,aux.Nombre);
                x[j+1].Rubro=aux.Rubro;
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
            neg[i].Rubro=0;
            strcpy(neg[i].Nombre,"");
    }
}

void separarNegocios(Negocio neg[240],Negocio hel[60],Negocio piz[60],Negocio beb[60],Negocio par[60],int cont[4]){
    for(int i=0;i<240;i++){
        //Utilizamos el switch para separar 
        switch(neg[i].Rubro){
            case 1:
                hel[cont[0]].Rubro=neg[i].Rubro;
                strcpy(hel[cont[0]].Nombre,neg[i].Nombre);
                hel[cont[0]].Zona=neg[i].Zona;
                cont[0]++;
                break;
            case 2:
                piz[cont[1]].Rubro=neg[i].Rubro;
                strcpy(piz[cont[1]].Nombre,neg[i].Nombre);
                piz[cont[1]].Zona=neg[i].Zona;
                cont[1]++;
                break;
            case 3:
                beb[cont[2]].Rubro=neg[i].Rubro;
                strcpy(beb[cont[2]].Nombre,neg[i].Nombre);
                beb[cont[2]].Zona=neg[i].Zona;
                cont[2]++;
                break;
            case 4:
                par[cont[3]].Rubro=neg[i].Rubro;
                strcpy(par[cont[3]].Nombre,neg[i].Nombre);
                par[cont[3]].Zona=neg[i].Zona;
                cont[3]++;
                break;               
        }
    }
}

void cargarNegocios(int negPorZona[6][4], Negocio neg[240]){
    int rubro,cont=0;
    do{
        cout<<"Ingrese el rubro (1: Heladeria, 2: Pizzeria, 3: Bebidas, 4:Parrilla, 0: salir): ";
        cin>>rubro;
    }while(rubro<0||rubro>4);    
    while(rubro!=0&&cont<240){
        neg[cont].Rubro=rubro;        
        cout<<"Ingrese la zona (de 1 a 6): ";
        cin>>neg[cont].Zona;
        if(negPorZona[neg[cont].Zona-1][neg[cont].Rubro-1]<10){ 
            cin.ignore();
            cout<<"Ingrese el nombre del comercio: ";
            cin.getline(neg[cont].Nombre,40);     
            negPorZona[neg[cont].Zona-1][neg[cont].Rubro-1]++;
            cont++;
        }else{
            cout<<"Esa zona ya tiene 10 Negocios de ese rubro."<<endl;
            neg[cont].Zona=0;
            neg[cont].Rubro=0;
        }
        do{
            cout<<"Ingrese el rubro (1: Heladeria, 2: Pizzeria, 3: Bebidas, 4:Parrilla, 0: salir): ";
            cin>>rubro;
        }while(rubro<0||rubro>4); 
    }    
}
