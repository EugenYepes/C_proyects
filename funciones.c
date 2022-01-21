#include "funciones.h"

void menu(float cambioBase[2][2])
{
    int opcion;
    float zoom;
    do
    {
        printf("Ingrese una opcion:\n");
        printf("\t1-para no espejar ni rotar\n");
        printf("\t2-para espejar respecto a las ordenadas\n");
        printf("\t3-para espejar respecto a las coordenadas\n");
        printf("\t4-para ingresar tu propia matriz\n");
        scanf(" %d", &opcion);
        printf("Ingrese el zoom deseado \n");
        scanf(" %f", &zoom);
        if (zoom != 0)
        {
            switch (opcion)
            {
            case 1:
                cambioBase[0][0] = 1 * zoom;
                cambioBase[0][1] = 0 * zoom;
                cambioBase[1][0] = 0 * zoom;
                cambioBase[1][1] = 1 * zoom;
                break;
            case 2:
                cambioBase[0][0] = -1 * zoom;
                cambioBase[0][1] = 0 * zoom;
                cambioBase[1][0] = 0 * zoom;
                cambioBase[1][1] = 1 * zoom;
                break;
            case 3:
                cambioBase[0][0] = 1 * zoom;
                cambioBase[0][1] = 0 * zoom;
                cambioBase[1][0] = 0 * zoom;
                cambioBase[1][1] = -1 * zoom;
                break;
            case 4:
                cargarMatrizCambioBase(cambioBase);
                break;
            default:
                printf("Opcion incorrecta\n");
                break;
            }
        }
        else
        {
            printf("El zoom ingresado es menor que 0, la imagen no se vera\n");
            opcion = -1;
        }
    } while (opcion > 4 || opcion < 1);
}

void cargarMatrizCambioBase(float cambioBase[2][2])
{
    do
    {
        printf("Ingrese la matriz cambio de base:\n");
        printf("a\tb\n");
        printf("c\td\n");

        printf("a:\t");
        scanf(" %f", &cambioBase[0][0]);

        printf("b:\t");
        scanf(" %f", &cambioBase[0][1]);

        printf("c:\t");
        scanf(" %f", &cambioBase[1][0]);

        printf("d:\t");
        scanf(" %f", &cambioBase[1][1]);
    } while (verificarMatrizCambioBase(cambioBase) < 0);
    printf("Ingreso:\n");
    printf("%.3f\t%.3f\n", cambioBase[0][0], cambioBase[0][1]);
    printf("%.3f\t%.3f\n", cambioBase[1][0], cambioBase[1][1]);
}

int verificarMatrizCambioBase(float cambioBase[2][2])
{
    if (cambioBase[0][1] == 0 && cambioBase[1][0] == 0)
    {
        return 1;
    }
    // casos no aceptados
    if (cambioBase[0][0] == 0 && cambioBase[1][0] == 0)
    {
        printf("ERROR, con esta matriz la imagen no tendra alto\n");
        return -1;
    }
    if (cambioBase[0][1] == 0 && cambioBase[1][1] == 0)
    {
        printf("ERROR, con esta matriz la imagen no tendra ancho\n");
        return -1;
    }
    if (cambioBase[0][0] == 0 && cambioBase[0][1] == 0)
    {
        printf("ERROR, con esta matriz la imagen no tendra ancho\n");
        return -1;
    }
    if (cambioBase[1][0] == 0 && cambioBase[1][1] == 0)
    {
        printf("ERROR, con esta matriz la imagen no tendra alto\n");
        return -1;
    }
    if (cambioBase[0][1] == 1 && cambioBase[1][0] == 1)
    {
        printf("ERROR, con esta matriz el programa no funcionara\n");
        return -1;
    }
    return 0;
}

void cargarImagenMatriz(unsigned char *imagen, union Cabecera cabecera, pixel **matPixel)
{
    int fila, columna, i = 0;
    for (fila = 0; fila < cabecera.fHeader.alto; fila++)
    {
        for (columna = 0; columna < cabecera.fHeader.ancho; columna++)
        {
            // BGR porque en el archivo comienza desde el final
            matPixel[fila][columna].B = imagen[i];
            i++;
            matPixel[fila][columna].G = imagen[i];
            i++;
            matPixel[fila][columna].R = imagen[i];
            i++;
        }
    }
    cargarCoordenadas(cabecera, matPixel);
    imprimirCoordenadas(matPixel, cabecera);
}

void cargarCoordenadas(union Cabecera cabecera, pixel **matPixel)
{
    int fila, columna, x, y;
    //(-1) porque se empieza a cargar desde el tercer cuadrante, abajo a la izquierda
    y = (-1) * puntoMedio(cabecera.fHeader.alto);
    for (fila = 0; fila < cabecera.fHeader.alto; fila++)
    {
        x = (-1) * puntoMedio(cabecera.fHeader.ancho);
        for (columna = 0; columna < cabecera.fHeader.ancho; columna++, x++)
        {
            matPixel[fila][columna].x = x;
            matPixel[fila][columna].y = y;
        }
        y++;
    }
}

int puntoMedio(int num)
{
    if (num % 2 == 0)
        num = num / 2;
    else
        num = (num + 1) / 2;
    return num;
}

void modificarImagen(float cambioBase[2][2], union Cabecera cabecera, union Cabecera cabeceraCopia, pixel **matPixel, pixel ***copiaPixel)
{
    int i;
    // pedir memoria para para la matriz copia con el nuevo tamaño
    *copiaPixel = realloc(*copiaPixel, sizeof(pixel *) * cabeceraCopia.fHeader.alto);
    if (*copiaPixel == NULL)
    {
        printf("realloc copiaPixel: error al pedir memoria\n");
        exit(-1);
    }
    for (i = 0; i < cabeceraCopia.fHeader.alto; i++)
    {
        (*copiaPixel)[i] = realloc((*copiaPixel)[i], sizeof(pixel) * cabeceraCopia.fHeader.ancho);
        if ((*copiaPixel)[i] == NULL)
        {
            printf("realloc copiaPixel[%d]: error al pedir memoria\n", i);
            exit(-1);
        }
    }
    multiplicarMatriz(matPixel, cambioBase, cabecera.fHeader.alto, cabecera.fHeader.ancho);

    imprimirCoordenadas(matPixel, cabecera);

    ordenarMatriz(matPixel, cabecera.fHeader.alto, cabecera.fHeader.ancho);

    imprimirCoordenadas(matPixel, cabecera);
    continuidadCopia(*copiaPixel, matPixel, cabeceraCopia, cabecera);
}

void multiplicarMatriz(pixel **matPixel, float cambioBase[2][2], int fila, int columna)
{
    int i, j, aux;
    for (i = 0; i < fila; i++)
    {
        for (j = 0; j < columna; j++)
        {
            // con redondeo
            aux = redondeo(cambioBase[0][0] * matPixel[i][j].x + cambioBase[0][1] * matPixel[i][j].y);
            matPixel[i][j].y = redondeo(cambioBase[1][0] * matPixel[i][j].x + cambioBase[1][1] * matPixel[i][j].y);
            matPixel[i][j].x = aux;
        }
    }
}

int redondeo(float num)
{
    int redondeado;
    float dif;
    dif = num - (int)num;
    redondeado = (int)num;
    if (dif > 0.5)
    {
        redondeado++;
    }
    else if (dif < -0.5)
    {
        redondeado--;
    }
    return redondeado;
}

float modulo(float num)
{
    if (num > 0)
    {
    }
    else
    {
        num = num * -1;
    }
    return num;
}

void ordenarMatriz(pixel **matPixel, int filaMax, int columnaMax)
{
    int i, j, k;
    for (i = 0; i < filaMax; i++)
    {
        for (j = 0; j < columnaMax - 1; j++)
        {
            for (k = 0; k < columnaMax - 1; k++)
            {
                if (matPixel[i][k].x > matPixel[i][k + 1].x)
                {
                    swapColumna(matPixel, i, k);
                }
            }
        }
    }
    // ordenar las filas
    for (i = 0; i < filaMax - 1; i++)
    {
        for (j = 0; j < filaMax - 1; j++)
        {
            if (matPixel[j][0].y > matPixel[j + 1][0].y)
            {
                for (k = 0; k < columnaMax; k++)
                {
                    swapFila(matPixel, j, k);
                }
            }
        }
    }
}

void swapColumna(pixel **matPixel, int fila, int columna)
{
    unsigned char aux;
    int aux1;
    aux1 = matPixel[fila][columna].x;
    matPixel[fila][columna].x = matPixel[fila][columna + 1].x;
    matPixel[fila][columna + 1].x = aux1;

    aux = matPixel[fila][columna].B;
    matPixel[fila][columna].B = matPixel[fila][columna + 1].B;
    matPixel[fila][columna + 1].B = aux;

    aux = matPixel[fila][columna].G;
    matPixel[fila][columna].G = matPixel[fila][columna + 1].G;
    matPixel[fila][columna + 1].G = aux;

    aux = matPixel[fila][columna].R;
    matPixel[fila][columna].R = matPixel[fila][columna + 1].R;
    matPixel[fila][columna + 1].R = aux;
}

void swapFila(pixel **matPixel, int fila, int columna)
{
    unsigned char aux;
    int aux1;
    aux1 = matPixel[fila][columna].y;
    matPixel[fila][columna].y = matPixel[fila + 1][columna].y;
    matPixel[fila + 1][columna].y = aux1;

    aux = matPixel[fila][columna].B;
    matPixel[fila][columna].B = matPixel[fila + 1][columna].B;
    matPixel[fila + 1][columna].B = aux;

    aux = matPixel[fila][columna].G;
    matPixel[fila][columna].G = matPixel[fila + 1][columna].G;
    matPixel[fila + 1][columna].G = aux;

    aux = matPixel[fila][columna].R;
    matPixel[fila][columna].R = matPixel[fila + 1][columna].R;
    matPixel[fila + 1][columna].R = aux;
}

void guardarImagen(union Cabecera cabecera, union Cabecera cabeceraCopia, pixel **copiaPixel)
{
    int fila, columna, i = 0;
    unsigned char *imagenCopia = NULL;
    FILE *pFileCopiaBMP = NULL;
    pFileCopiaBMP = fopen("copiaImagen.bmp", "wb");
    if (pFileCopiaBMP == NULL)
    {
        printf("Fallo al abrir el archivo donde se guardara la imagen\n");
        exit(-1);
    }
    // cargar en un array todos los campos de todas las estructuras
    imagenCopia = malloc(sizeof(unsigned char) * cabeceraCopia.fHeader.tamImagen);
    if (imagenCopia == NULL)
    {
        printf("malloc imagenCopia: error al pedir memoria\n");
        exit(-1);
    }
    // distancia maxima en x del centro de la imagen respecto a las coordenadas cartesianas
    for (fila = 0; fila < cabeceraCopia.fHeader.alto; fila++)
    {
        for (columna = 0; columna < cabeceraCopia.fHeader.ancho; columna++)
        {
            imagenCopia[i] = copiaPixel[fila][columna].B;
            i++;
            imagenCopia[i] = copiaPixel[fila][columna].G;
            i++;
            imagenCopia[i] = copiaPixel[fila][columna].R;
            i++;
        }
    }
    // guardar header
    fwrite(cabeceraCopia.header, sizeof(unsigned char), TAM_CABECERA, pFileCopiaBMP);
    // posicionar el puntero en donde comenzar a guardar la imagen.
    fseek(pFileCopiaBMP, cabeceraCopia.fHeader.inicioImagen, SEEK_SET);
    // guardar imagen
    fwrite(imagenCopia, sizeof(unsigned char), cabeceraCopia.fHeader.tamImagen, pFileCopiaBMP);
    free(imagenCopia);
    fclose(pFileCopiaBMP);
}

void copyHeader(char *dest, char *src)
{
    int i;
    for (i = 0; i < TAM_CABECERA; i++)
    {
        dest[i] = src[i];
    }
}

void modificarHeader(union Cabecera cabecera, union Cabecera *cabeceraCopia, float cambioBase[2][2])
{
    // calcular ancho multiplicando la ultima posicion con la matriz cambio de base
    (*cabeceraCopia).fHeader.ancho = redondeo(modulo(cabecera.fHeader.ancho * cambioBase[0][0])) + redondeo(modulo(cabecera.fHeader.ancho * cambioBase[0][1]));
    // calcular alto multiplicando la ultima posicion con la matriz cambio de base
    (*cabeceraCopia).fHeader.alto = redondeo(modulo(cabecera.fHeader.alto * cambioBase[1][0])) + redondeo(modulo(cabecera.fHeader.alto * cambioBase[1][1]));
    // tamaño en bytes de la imagen
    (*cabeceraCopia).fHeader.tamImagen = (*cabeceraCopia).fHeader.ancho * (*cabeceraCopia).fHeader.alto * (cabecera.fHeader.tamPunto / 8);
    // tamaño del fichero
    (*cabeceraCopia).fHeader.tamArchivo = (*cabeceraCopia).fHeader.tamImagen + TAM_CABECERA;
}

void continuidadCopia(pixel **copiaPixel, pixel **matPixel, union Cabecera cabeceraCopia, union Cabecera cabecera)
{
    int fila, columna; // contadores para matPixel
    int i = 0, j = 0;  // contadores para copiaPixel
    int constante, pos = 1;
    //!!!!!CUIDADO CON LOS LIMITES DE LA MATRIZ, NO SOBREPASARLOS con [columna +1] y [fila +1]
    // introducir un espacio para cada pixel discontinuo o elimiar los que tienen las mismas coordenadas
    for (fila = 0; fila < cabecera.fHeader.alto; fila++) // recorre todas las filas de matPixel
    {
        for (columna = 0; columna < cabecera.fHeader.ancho; columna++) // recorre todas las columnas de matPixel
        {
            if ((columna <= cabecera.fHeader.ancho - 2) && (j <= (cabeceraCopia.fHeader.ancho - 2)))
            {
                if (matPixel[fila][columna + 1].x == matPixel[fila][columna].x + 1) // el zoom se mantiene
                {
                    copiaPixel[i][j].x = matPixel[fila][columna].x;
                    copiaPixel[i][j].y = matPixel[fila][columna].y;
                    copiaPixel[i][j].B = matPixel[fila][columna].B;
                    copiaPixel[i][j].G = matPixel[fila][columna].G;
                    copiaPixel[i][j].R = matPixel[fila][columna].R;
                    j++;
                }
                else if (matPixel[fila][columna + 1].x == matPixel[fila][columna].x) // reduccion en el zoom
                {
                    constante = columna;
                    for (; matPixel[fila][columna + 1].x == matPixel[fila][constante].x;)
                    {
                        // se copia el siguiente pixel sobreescribiendo el actual hasta que la coordenada del siguiente pixel sea distinta del actual
                        copiaPixel[i][j].x = matPixel[fila][columna].x;
                        copiaPixel[i][j].y = matPixel[fila][columna].y;
                        copiaPixel[i][j].B = matPixel[fila][columna].B;
                        copiaPixel[i][j].G = matPixel[fila][columna].G;
                        copiaPixel[i][j].R = matPixel[fila][columna].R;
                        columna++;
                    }
                    j++;
                }
                else if (matPixel[fila][columna + 1].x > (matPixel[fila][columna].x + pos)) // hay zoom
                {
                    copiaPixel[i][j].x = matPixel[fila][columna].x;
                    copiaPixel[i][j].y = matPixel[fila][columna].y;
                    copiaPixel[i][j].B = matPixel[fila][columna].B;
                    copiaPixel[i][j].G = matPixel[fila][columna].G;
                    copiaPixel[i][j].R = matPixel[fila][columna].R;
                    j++;
                    for (; matPixel[fila][columna + 1].x > (matPixel[fila][columna].x + pos); pos++)
                    {
                        copiaPixel[i][j].x = matPixel[fila][columna].x + pos;
                        copiaPixel[i][j].y = matPixel[fila][columna].y;
                        copiaPixel[i][j].B = (matPixel[fila][columna].B + matPixel[fila][columna + 1].B) / 2;
                        copiaPixel[i][j].G = (matPixel[fila][columna].G + matPixel[fila][columna + 1].G) / 2;
                        copiaPixel[i][j].R = (matPixel[fila][columna].R + matPixel[fila][columna + 1].R) / 2;
                        j++;
                    }
                    pos = 1;
                }
            }
            else
            {
                // copiar borde derecho
            }
        }
        columna = 0;
        j = 0; // se termino de copiar la fila
               // para filas
        if ((fila <= (cabecera.fHeader.alto - 2)) && (i <= (cabeceraCopia.fHeader.alto - 2)))
        {
            if (matPixel[fila + 1][columna].y == (matPixel[fila][columna].y + 1)) // el zoom se mantiene
            {
                i++; // pasa a la fila inmediatamente posterior
            }
            else if (matPixel[fila + 1][columna].y == matPixel[fila][columna].y) // reduccion en el zoom
            {
                constante = fila;
                for (; matPixel[fila + 1][columna].y == matPixel[constante][columna].y; fila++)
                { // hasta la fila en la que aumente la coordenada
                }
                i++;
            }
            else if (matPixel[fila + 1][columna].y > (matPixel[fila][columna].y + pos)) // hay zoom
            {
                i++; // se llena la siguiente columna
                for (pos = 1; matPixel[fila + 1][columna].y > (matPixel[fila][columna].y + pos); pos++)
                {
                    for (j = 0; j < cabeceraCopia.fHeader.ancho; j++)
                    {
                        copiaPixel[i][j].x = j;
                        copiaPixel[i][j].y = i;
                        copiaPixel[i][j].B = copiaPixel[i - 1][j].B;
                        copiaPixel[i][j].G = copiaPixel[i - 1][j].G;
                        copiaPixel[i][j].R = copiaPixel[i - 1][j].R;
                    }
                    i++;
                }
                pos = 1;
            }
            j = 0; // se terminaron de copiar la filas
        }
        else
        {
            // copiar borde superior
        }
    }
}

void imprimirCabecera(union Cabecera cabecera)
{
    printf("Tipo de archivo: %c%c\n", cabecera.fHeader.tipoArchivo[0], cabecera.fHeader.tipoArchivo[1]);
    printf("Tamaño del archivo: %d\n", cabecera.fHeader.tamArchivo);
    printf("Reservado: %d\n", cabecera.fHeader.reserv1);
    printf("Reservado: %d\n", cabecera.fHeader.reserv2);
    printf("Inicio de los datos de la imagen: %d\n", cabecera.fHeader.inicioImagen);
    printf("Tamaño de la cabecera del bitmap: %d\n", cabecera.fHeader.tamTotalCabecera);
    printf("Ancho (pixels): %d\n", cabecera.fHeader.ancho);
    printf("Alto (pixels): %d\n", cabecera.fHeader.alto);
    printf("Numero de planos: %d\n", cabecera.fHeader.numPlanos);
    printf("Tamaño del punto: %d\n", cabecera.fHeader.tamPunto);
    printf("Compresion (0 = no comprimido): %d\n", cabecera.fHeader.compresion);
    printf("Tamaño de la imagen: %d\n", cabecera.fHeader.tamImagen);
    printf("Resolucion horizontal: %d\n", cabecera.fHeader.resoHorizontal);
    printf("Resolucion Vertical: %d\n", cabecera.fHeader.resoVertical);
    printf("Tamaño de la tabla de color: %d\n", cabecera.fHeader.tamTablaColores);
    printf("Contador de colores importantes: %d\n", cabecera.fHeader.contColores);
}

void imprimirCoordenadas(pixel **matPixel, union Cabecera cabecera)
{
    int i, j;
    for (i = 0; i < cabecera.fHeader.alto; i++)
    {
        for (j = 0; j < cabecera.fHeader.ancho; j++)
        {
            // printf("%.3d %.3d    ", matPixel[i][j].x, matPixel[i][j].y);
        }
        // printf("\n");
    }
}