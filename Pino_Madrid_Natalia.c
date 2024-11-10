#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct nodoArbol
{
    char rut[12];
    char nombre[30];
    int entradas;
    struct nodoArbol *izq;
    struct nodoArbol *der;
};
typedef struct nodoArbol nodoA;
typedef nodoA *AB;

/*
Entrada: un rut, un nombre y entradas.
Salida: un nodo tipo AB del árbol.
Proceso: recibe los nombrados datos, los cuales transforma en la estructura AB, quedando como un nodo.
*/
AB creaNodoArbol(char rut[12], char nombre[30], int entradas)
{
    AB aux;

    aux= malloc(sizeof(nodoA));

    if(aux== NULL)
    {
        printf("No hay espacio en la memoria. Se cerrara el programa.\n");
        exit(-1);
    }
    strcpy(aux->rut,rut);
    strcpy(aux->nombre,nombre);
    aux->entradas= entradas;
    aux->izq= NULL;
    aux->der= NULL;

    return aux;
}

/*
Entrada: el árbol, rut, nombre, entradas (sin evaluar).
Salida: retorna un árbol tipo AB ya armado.
Proceso: recibe un nodo, el cual evalúa su rut y ordena de menor (izq) a mayor (der).
*/
AB creaArbol(AB arbol, char rut[12], char nombre[30], int entradas)
{

    if(arbol== NULL)
    {
        arbol= creaNodoArbol(rut,nombre,entradas);
        return arbol;
    }
    else
    {
        if(strcmp(rut, arbol->rut)== 0)
        {
            arbol->entradas= arbol->entradas + entradas;
            return arbol;
        }
        else
        {
            if(strcmp(rut, arbol->rut)== -1)
            {
                arbol->izq= creaArbol(arbol->izq,rut,nombre,entradas);
            }
            else
            {
                arbol->der= creaArbol(arbol->der,rut,nombre,entradas);
            }
        }
    }
    return arbol;
}

/*
Entrada: recibe un árbol tipo AB.
Salida: la cantidad de personas que tienen más de 2 entradas.
Proceso: revisa las entradas de cada nodo del árbol, contando el total de personas que superaron la compra de 2 entradas.
*/
int cuentaSobrecupoEntradas(AB arbol)
{
    if(arbol== NULL)
    {
        return 0;
    }
    else
    {
        if(arbol->entradas> 2)
        {
            return 1 + cuentaSobrecupoEntradas(arbol->izq) + cuentaSobrecupoEntradas(arbol->der);
        }
        else
        {
            return cuentaSobrecupoEntradas(arbol->izq) + cuentaSobrecupoEntradas(arbol->der);
        }
    }
}

/*
Entrada: un arbol tipo AB, el archivo de salida tipo FILE.
Salida: no retorna nada.
Proceso: del árbol busca las personas con más de 2 entradas y las imprime dentro del archivo recibido.
*/
void inOrdenArchivoSobrecupo(AB arbol, FILE *archivoSalida)
{
    if(arbol!= NULL)
    {
        inOrdenArchivoSobrecupo(arbol->izq, archivoSalida);
        if(arbol->entradas>2)
        {
           fprintf(archivoSalida, "%s, %s: %i \n" ,arbol->rut,arbol->nombre,arbol->entradas);
        }
        inOrdenArchivoSobrecupo(arbol->der, archivoSalida);
    }
}

/*
Entrada: el árbol, nombre del archivo que leyó al principio.
Salida: no retorna nada.
Proceso: crea un archivo .sde con el nombre del que leyó previamente, para almacenar las personas que compraron más de 2 entradas.
*/
void escribeEnArchivo(AB arbol, char nombreArchivo[50])
{
    FILE *archivoSalida;

    char nombreSalida[50];

    strcpy(nombreSalida, nombreArchivo);
    strtok(nombreSalida, ".");
    strcat(nombreSalida, ".sde");

    archivoSalida= fopen(nombreSalida, "w");

    if(archivoSalida== NULL)
    {
        printf("No se puede abrir el archivo.");
        printf("\nEste programa se cerrara.");
        exit(-1);
    }
    inOrdenArchivoSobrecupo(arbol, archivoSalida);

    fprintf(archivoSalida,"\nTotal: %i" ,cuentaSobrecupoEntradas(arbol));
    fclose(archivoSalida);

    printf("\nEl archivo %s fue generado.\n" ,nombreSalida);
}

/*
Entrada: el nombre del archivo ingresado por el usuario.
Salida: no retorna nada.
Proceso: lee el archivo basado en el nombre ingresado, y almacena sus datos en variables para crear un árbol con ellas.
*/
void leerArbolArchivo(char nombreArchivo[50])
{
    FILE *archivo;
    char linea[150], *rut, *nombre, *textoEntradas;
    int entradas;
    AB arbol;

    arbol= NULL;

    archivo= fopen(nombreArchivo,"r");

    if(archivo==NULL)
    {
        printf("\nNo existe este archivo. Se cerrara el programa.\n");
        exit(-1);
    }

    while(fgets(linea,150,archivo)!= 0)
    {
        strtok(linea,"\n");
        rut= strtok(linea,",");
        nombre= strtok(NULL,",");
        textoEntradas= strtok(NULL,",");
        entradas= atoi(textoEntradas);

        arbol= creaArbol(arbol,rut,nombre,entradas);
    }
    fclose(archivo);

    if(cuentaSobrecupoEntradas(arbol)!=0)
    {
        escribeEnArchivo(arbol,nombreArchivo);
    }
    else
    {
        printf("\nTodos solicitaron la cantidad correcta de entradas.\n");
    }
}

/*
Entrada: no recibe nada.
Salida: no retorna nada.
Proceso: le pregunta al usuario sobre el archivo que debe leer. El nombre es guardado en una variable, y comienza el trabajo del programa.
*/
void iniciaProceso()
{
    char nombreArchivo[50];

    printf("Ingrese el nombre del archivo: ");

    fgets(nombreArchivo,50,stdin);
    strtok(nombreArchivo,"\n");

    leerArbolArchivo(nombreArchivo);
}

int main()
{
    iniciaProceso();
    return 0;
}
