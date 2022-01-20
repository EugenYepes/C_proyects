#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TAM_CABECERA 54
#pragma pack(1)
union Cabecera
{
    char header[TAM_CABECERA];
    struct formatHeader
    {
        char tipoArchivo[2];
        int tamArchivo;
        short int reserv1;
        short int reserv2;
        int inicioImagen;
        int tamTotalCabecera;
        int ancho;
        int alto;
        short int numPlanos;
        short int tamPunto;
        int compresion;
        int tamImagen;
        int resoHorizontal;
        int resoVertical;
        int tamTablaColores;
        int contColores;
    } fHeader;
};

#pragma pack(4)

typedef struct pixel
{
    unsigned char R;
    unsigned char G;
    unsigned char B;
    int x;
    int y;
} pixel;

void cargarMatrizCambioBase(float cambioBase[2][2]);
void cargarImagenMatriz(unsigned char *imagen, union Cabecera cabecera, pixel **matPixel);
int puntoMedio(int num);
int invPuntoMedio(int num);
void modificarImagen(float cambioBase[2][2], union Cabecera cabecera, union Cabecera cabeceraCopia, pixel **matPixel, pixel ***copiaPixel);
void ordenarMatriz(pixel **matPixel, int fila, int columna);
void guardarImagen(union Cabecera cabecera, union Cabecera cabeceraCopia, pixel **copiaPixel);
void multiplicarMatriz(pixel **matPixel, float cambioBase[2][2], int fila, int columna);
void cargarCoordenadas(union Cabecera cabecera, pixel **matPixel);
void imprimirCabecera(union Cabecera cabecera);
void swapColumna(pixel **matPixel, int fila, int columna);
void swapFila(pixel **matPixel, int fila, int columna);
void copyHeader(char *, char *);
void modificarHeader(union Cabecera cabecera, union Cabecera *cabeceraCopia, float cambioBase[2][2]);
int redondeo(float num);
float modulo(float num);
void continuidadCopia(pixel **copiaPixel, pixel **matPixel, union Cabecera cabeceraCopia, union Cabecera cabecera);
void imprimirCoordenadas(pixel **matPixel, union Cabecera cabecera);
int verificarMatrizCambioBase(float cambioBase[2][2]);
void menu(float cambioBase[2][2]);
