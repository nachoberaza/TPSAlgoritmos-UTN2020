#include <iostream>
#include <stdio.h>
#include <cstring>

using namespace std;

struct Repartidor
{
	unsigned DNI;
	char Nombre[40];
	unsigned Zona;
};

void cargarRepartidores(int repPorZona[]);
void mostrarZonas(int repPorZona[], int cant);
void vaciarVector(int repPorZona[], int cant);
void zonasSinRep(int repPorZona[], int cant);

int main(){
	//Se declara una variable que determinara el numero de zonas, y un vector que lleva la cuenta
	//de cuantos repartidores hay en cada zona.
	int cant=6;
	int repPorZona[cant];
	vaciarVector(repPorZona,cant);
	//Funcion para cargar a todos los repartidores, la carga finaliza con numero de zona=0.
	cargarRepartidores(repPorZona);
	//Muestra la cantidad de repartidores por zona.
	mostrarZonas(repPorZona,cant);
	//Muestra las zonas donde no hay repartidores.
	zonasSinRep(repPorZona,cant);
	return 0;
}

void cargarRepartidores(int repPorZona[]){
	FILE *f = fopen("Repartidores.dat","wb");
	int cont=1;
	if(f==NULL){
		cout<<"No se ha encontrado el archivo."<<'\n';
	} else{
		Repartidor rep;
		cout<<"Repartidor "<<cont<<endl;
		do{
			cout<<"Ingrese la zona de reparto (de 1 a 6, 0 para salir): ";
			cin>>rep.Zona;
		}while(rep.Zona<0||rep.Zona>6);
		while(rep.Zona!=0){
			repPorZona[rep.Zona-1]+=1;
			cin.ignore();
			cout<<"Ingrese nombre y apellido del repartidor: ";
			cin.getline(rep.Nombre,40);
			cout<<"Ingrese el DNI del repartidor: ";
			cin>>rep.DNI;
			fwrite(&rep,sizeof(Repartidor),1,f);
			cont++;
			cout<<"Repartidor "<<cont<<endl;
			do{
				cout<<"Ingrese la zona de reparto (de 1 a 6, 0 para salir): ";
				cin>>rep.Zona;
			}while(rep.Zona<0||rep.Zona>6);
		}
	}
	fclose(f);
}

void zonasSinRep(int repPorZona[], int cant){
	cout<<'\n'<<"Zonas sin repartidores:"<<endl;
	for(int i=0;i<cant;i++){
		if (repPorZona[i]==0){
			cout<<"Zona "<<i+1<<endl;
		}
	}
}

void vaciarVector(int repPorZona[],int cant){
	for (int i=0;i<cant;i++){
		repPorZona[i]=0;
	}
}

void mostrarZonas(int repPorZona[], int cant){
	cout<<endl<<"Cantidad de repartidores por zona:"<<endl;
	for(int i=0;i<cant;i++){
		cout<<"Zona "<<i+1<<": "<< repPorZona[i]<<" repartidores."<<endl;
	}
}

