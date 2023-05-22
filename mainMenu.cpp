#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <arpa/inet.h>
#include <cstring>
#include <cstdio>
#include <vector>


#include "LIB/convertir.h"
#include "LIB/funciones.h"
#include "LIB/cabeceraBinaria.h"
#include "LIB/trazaCabecera.h"

using namespace std;
// Definir la estructura que contiene las coordenadas
struct Coordenadas {
    float x;
    float y;
    float z;
};
int main() {
    int fd;
    struct stat sb;
    char *mem;
    int opc;
    //string Data = 'SGY/DataSet1';
    ofstream headerInfo("Encabezado.txt");
    ofstream binarioInfo("InformacionBinaria.txt");
    
       do{
        cout<<endl<<endl;
        cout<<"----------------------------MENU---------------------------"<<endl;
        cout<<"------------------Tenemos 3 DataSet de pruebas---------------"<<endl<<endl;
        cout<<"  DataSet 1: 46,2 MB (46.175.760 bytes)   Caso Basico"<<endl;
        cout<<"  DataSet 2: 380,7 MB (380.678.544 bytes) Caso Intermedio"<<endl;
        cout<<"  DataSet 3: 1,3 GB (1.321.186.320 bytes) Caso Alto"<<endl;
        cout<<"  Salir: 0"<<endl;
        cin>>opc;
        switch(opc){
            case 1: fd = open("SGY/DataSet1.sgy", O_RDONLY);break;
            case 2: fd = open("SGY/DataSet2.sgy", O_RDONLY);break;
            case 3: fd = open("SGY/DataSet3.sgy", O_RDONLY);break;
            case 0:cout<<"\nTrabajo hecho por:\n\t\tMoises Merza CI: 25547115\n\t\tHarold Franco CI: 28330946"<<endl<<endl;break;
            default: cout<<"Opcion no encontrada"<<endl;break;
        }
      
    
    // Abrir el archivo SEG-Y en modo de solo lectura
    //fd = open("SGY/DataSet3.sgy", O_RDONLY);
    if(opc !=0){
        if (fd == -1) {
            cerr << "No se pudo abrir el archivo." << endl;
            return 1;
        }
        
        // Obtener información sobre el archivo
        if (fstat(fd, &sb) == -1) {
            cerr << "No se pudo obtener información del archivo." << endl;
            close(fd);
            return 1;
        }
        
        // Mapear el archivo en memoria
        //cout<<sb.st_size<<endl;
        mem = (char*) mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
        if (mem == MAP_FAILED) {
            cerr << "No se pudo mapear el archivo en memoria." << endl;
            close(fd);
            return 1;
        }

        // Leer los primeros 3200 bytes (la cabecera completa)
        char cabecera[3200];
        memcpy(cabecera, mem, 3200);
        
        // Convertir la cabecera a texto ASCII
        convertASCII(cabecera,3200);
        write_text_header(headerInfo,cabecera);
        
        // Leer los siguientes 400 bytes (el encabezado binario)
        char encabezado_binario[400];
        memcpy(encabezado_binario, mem + 3200, 400);

        //crear un objeto tipo Binaryheader
        BinaryHeader cabeceraBinaria;
        cabeceraBinaria.store(encabezado_binario);
        //escribir al archivo
        cabeceraBinaria.write(binarioInfo);


        unsigned short int numSamples = cabeceraBinaria.get_num_of_samples();
        short int nTraza = cabeceraBinaria.get_num_of_trace();

        //cout<<"Nro Trazas: "<<nTraza<<endl;
        // Leer las trazas de datos sísmicos
        //tipo archivo traza
        TraceHeader traza;
        char trazaInfo[240];
        //unsigned short int numSamples;
        int spl = 3600;
        //ofstream trazaInfoSgy("TrazaInfo.csv");
        // Definir la estructura que contiene las coordenadas



    // Crear un vector para almacenar las coordenadas de todas las trazas
        vector<Coordenadas> coordenadasTodasTrazas;

    // Agregar las coordenadas de cada traza al vector
        for(int i = 0; i < nTraza; i++)
        {
            // Guardar memoria para la información de la traza
            char* trazaInfo = new char[240];
            memcpy(trazaInfo, mem+spl, 240);
            spl += 240;

            // Leer las coordenadas de la traza
            Coordenadas coordenadas;
            memcpy(&coordenadas.x, trazaInfo+72, sizeof(float));
            memcpy(&coordenadas.y, trazaInfo+76, sizeof(float));
            memcpy(&coordenadas.z, trazaInfo+80, sizeof(float));

            // Agregar las coordenadas al vector
            coordenadasTodasTrazas.push_back(coordenadas);

            // Liberar memoria
            delete[] trazaInfo;

            // Actualizar spl para la siguiente traza
            numSamples = traza.get_numsampl();
            spl += numSamples*4;
        }
        // Recorrer el vector y mostrar las coordenadas de cada elemento
        for (int i = 0; i < coordenadasTodasTrazas.size(); i++) {
            cout<<i+1<<endl;
            cout <<"Coordenada X: " << coordenadasTodasTrazas[i].x<<endl;
            cout << "Coordenada Y: " << coordenadasTodasTrazas[i].y <<endl;
            cout<< "Coordenada Z: " << coordenadasTodasTrazas[i].z <<endl<<endl;
        }
        
        // Liberar la memoria mapeada
        
        cout<<"tamaño del archivo en bytes: "<<sb.st_size<<endl;
        
        if (munmap(mem, sb.st_size) == -1) {
            cerr << "No se pudo liberar la memoria mapeada." << endl;
            close(fd);
            return 1;
        }
        // Cerrar el archivo
        close(fd);
    }
    }while(opc != 0);
    
    return 0;
}
