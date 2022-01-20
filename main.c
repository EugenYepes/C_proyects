#include "funciones.h"

int main(int argc, char **argv)
{
    FILE *pFileBMP = NULL;
    union Cabecera cabecera, cabeceraCopia;
    pixel **matPixel = NULL, **copiaPixel = NULL;
    unsigned char *imagen = NULL;
    int i;
    float cambioBase[2][2];
    // Ingreso del header del archivo
    if (argc > 1)
    {
        printf("%s", argv[1]);
        pFileBMP = fopen(argv[1], "rb");
        if (pFileBMP == NULL)
        {
            printf("Error al abrir el archivo\n");
            exit(-1);
        }
        fread(cabecera.header, sizeof(char), TAM_CABECERA, pFileBMP);
        // Ingreso de la matriz cambio de base
        menu(cambioBase);
        // cargar la imagen en un vector
        imagen = malloc(sizeof(char) * cabecera.fHeader.tamImagen);
        if (imagen == NULL)
        {
            printf("Imagen: error al pedir memoria\n");
            exit(-1);
        }
        // fseek(pFileBMP, datosCabecera.formatHeader.inicioImagen, SEEK_SET);
        fread(imagen, sizeof(unsigned char), cabecera.fHeader.tamImagen, pFileBMP);
        // pedir memoria para la matriz de la imagen
        matPixel = malloc(sizeof(pixel *) * cabecera.fHeader.alto); // filas
        if (matPixel == NULL)
        {
            printf("malloc matPixel: error al pedir memoria\n");
            exit(-1);
        }
        for (i = 0; i < cabecera.fHeader.alto; i++)
        {
            matPixel[i] = malloc(sizeof(pixel) * cabecera.fHeader.ancho); // para cada columna
            if (matPixel[i] == NULL)
            {
                printf("realloc matPixel[%d]: error al pedir memoria\n", i);
                exit(-1);
            }
        }
        cargarImagenMatriz(imagen, cabecera, matPixel);
        copyHeader(cabeceraCopia.header, cabecera.header);

        modificarHeader(cabecera, &cabeceraCopia, cambioBase);

        // printf("Original:\n");
        // imprimirCabecera(cabecera);

        // printf("Copia:\n");
        // imprimirCabecera(cabeceraCopia);

        modificarImagen(cambioBase, cabecera, cabeceraCopia, matPixel, &copiaPixel);
        guardarImagen(cabecera, cabeceraCopia, copiaPixel);
        // cerrar/liberar
        fclose(pFileBMP);
        free(imagen);
        for (i = cabecera.fHeader.alto; i > 0; i--)
        {
            free(matPixel[i]);
        }
        free(matPixel);
        for (i = cabeceraCopia.fHeader.alto; i > 0; i--)
        {
            free(copiaPixel[i]);
        }
        free(copiaPixel);
    }
    else
        printf("Ingrese el nombre del archivo de la imagen con la extension\n");
    return 0;
}