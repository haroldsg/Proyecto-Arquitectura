#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <arpa/inet.h>
#include <cstring>
#include <cstdio>

#include "LIB/convertir.h"
#include "LIB/funciones.h"
#include "LIB/cabeceraBinaria.h"
#include "LIB/trazaCabecera.h"

using namespace std;
const int DEFAULT_TRACE = 0;
const int DEFAULT_SAMPLE = 0;
// 3200 Encabezado + 400 Encabezado binario
const int DEFAULT_SKIP = 3600;  

int main() {
    // Abrir el archivo SEG-Y en modo binario
    ifstream archivo,trazaInfoSGY;
    ofstream headerInfo("Encabezado.txt");
    ofstream binarioInfo("InformacionBinaria.txt");
    ofstream headerInfoExt("InfoEncabezadoExt.txt");
    ofstream trazaInfoSgy("TrazaInfo.csv");
    ofstream datosTraza;

    char *cabecera = new char[3200];
    char *datoBinario = new char[400];
    short int numHeaderExt;
    archivo.open("SGY/archivo.sgy", ios::binary);
    if (!archivo) {
        cerr << "No se pudo abrir el archivo." << endl;
        return 1;
    }

    // Leer los primeros 3200 bytes (la cabecera completa)
    archivo.read(cabecera, 3200);

    // Convertir la cabecera a texto ASCII
    convertASCII(cabecera,3200);
    write_text_header(headerInfo,cabecera);

    //liberar memoria
    delete cabecera;
/*____________________________________________________________*/

    // Leer los 400 bytes cabecera binaria
	archivo.read(datoBinario, 400);

	//crear un objeto tipo Binaryheader
	BinaryHeader cabeceraBinaria;
	cabeceraBinaria.store(datoBinario);
	//escribir al archivo
	cabeceraBinaria.write(binarioInfo);
	//liberar memoria
	delete datoBinario;

/*____________________________________________________________*/

    //leer informacion extendida del header
    numHeaderExt = cabeceraBinaria.get_exthead();
    if (numHeaderExt  == -1)
    {
    	cout << "\nnúmero variable de registros de encabezado de archivo de texto extendido";
    	headerInfoExt.close();
    	exit(-7);
    }
    else if (numHeaderExt  == 0)
    {
        cout <<"\nEste SGY esta sin encabezado de archivo de texto extendido\n";
        headerInfoExt.close();
        remove("InfoEncabezadoExt.txt");
    }
    else
    {
        //creo el archivo extendido txt
        
    	while(numHeaderExt--)
    	{
	        char* extHeader = new char[3200];
	        

	        //leer el encabezado de archivo extendido
	        archivo.read(extHeader,3200);
            //convertir 
	        convertASCII(extHeader,3200);
            //pasar info al archivo txt
	        write_text_header(headerInfoExt,cabecera);
	        // liberar memoria
	        delete extHeader;    	    	
    	}
    }
    //calculamos el numero de trazas
    long long pos = archivo.tellg();
    archivo.seekg(0, ios::end);
    long long end = archivo.tellg();
    archivo.seekg(pos, ios::beg);
    unsigned short int numSamples = cabeceraBinaria.get_num_of_samples();
    long long nTraza = (end - pos)/(numSamples*4 + 240);

    //tipo archivo traza
    TraceHeader traza;
    char *trazaInfo;
    //unsigned short int numSamples;

    for(int i = 0; i < nTraza; i++)
    {
        //guardar memoria para la informacion de la traza
	    trazaInfo = new char[240];
	    archivo.read(trazaInfo, 240);

        
	    traza.store(trazaInfo);
	    //escribir en el archivo
	    traza.write(trazaInfoSgy);
        //liberar memoria
        delete trazaInfo;

        numSamples = traza.get_numsampl();
        archivo.seekg(numSamples*4, ios::cur);
    }
    trazaInfoSgy.close();
////////////////////////////trazas///////////////////////////////////////
    int trazasNum = DEFAULT_TRACE;
	int muestrasNum = DEFAULT_SAMPLE;
    int skip = DEFAULT_SKIP;

	//se abre archivo que contiene info de las trazas csv
    trazaInfoSGY.open("TrazaInfo.csv");
    datosTraza.open("Trazas.txt");

    if (!trazaInfoSGY || !datosTraza ) {
        cerr << "No se pudo abrir un archivo." << endl;
        return 1;
    }

	archivo.seekg(skip, ios::beg);

	// Mueva el puntero del archivo al encabezado de rastreo requerido
	for(int i = 0; i < trazasNum; i++)
	{
		unsigned int numMuestras = csv_read(trazaInfoSGY, i);
		//colocará el puntero justo antes del siguiente encabezado de seguimiento
		int pos = 240 + numMuestras*4;
		archivo.seekg(pos, ios::cur);
	}
    // cuenta el número de muestra
	unsigned int numMuestras = csv_read(trazaInfoSGY, trazasNum);
	int muestrasF = numMuestras - muestrasNum;

	float ieeeMuestra;
	uint32_t ibmMuestra;

	// omitirá el encabezado de seguimiento y colocará el puntero en la muestra para leer
	archivo.seekg(240 + muestrasNum*4, ios::cur);

	//escribir las siguientes muestras restantes
    for(int i = 0; i < muestrasF; i++)
	{
		//reservar memoria para las muestras
		char* valorTraza;
		valorTraza = new char[4];
		archivo.read(valorTraza, 4);

		//convertir de ibm a ieee
		ibmMuestra = *reinterpret_cast<uint32_t*>(valorTraza);
		ieeeMuestra = toIeee(ibmMuestra);
		//pasar al archivo
		if (i == 2499)
			datosTraza << ieeeMuestra << "\n";
		else
		datosTraza << ieeeMuestra << "\n";
		//liberar
		delete valorTraza;
	}

    // Cerrar el archivos
    archivo.close();
    headerInfo.close();
    binarioInfo.close();
    trazaInfoSGY.close();
    datosTraza.close();
    remove("TrazaInfo.csv");
    return 0;
}