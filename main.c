#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "hashmap.h"
#include "hashmap.c"
#include "treemap.h"
#include "treemap.c"
#include "lectura_csv.c"
#include "menu.c"

int ID = 1;

typedef struct //Estructura de cada tarea.
{
    char* nombre;
    int progreso;
    bool flag; //Indica si la tarea posee progreso o no.
    int dia;
    int mes;
    int anio;
    bool finalizada; //Indica si la tarea ya fue finalizada.
    long* fecha_finalizacion;
} Tarea;

Tarea* create_tarea(char * nombre) //Funcion que inicializa las variables de un tipo de dato tarea.
{
    Tarea* input = (Tarea *) calloc (1, sizeof(Tarea));
    input->nombre = nombre;
    input->flag = false;
    input->finalizada = false;
    input->fecha_finalizacion = (long *) malloc (sizeof(long));
    return input;
}

int transformar_fecha(int dia, int mes, int anio) //Funcion que transforma una fecha a un numero entero.
{
    int fecha = 0;
    fecha = anio % 100;
    fecha = fecha * 12 + mes - 1;
    fecha = fecha * 31 + dia - 1;
    fecha = fecha * 1000 + ID;
    ID++;
    return fecha;
}

int lower_than_int(void* key1, void* key2) //Funcion que retorna 1 si la key 2 es mayor a la key 1.
{
    int k1 = *((int*) (key1));
    int k2 = *((int*) (key2));
    return k1<k2;
}

int main()
{
    TreeMap* data_base = createTreeMap(lower_than_int); //Se crea la base de datos de tipo TreeMap, la cual contiene las tareas.
    HashMap* finalizadas = createMap(100); //Se crea una tabla hash en donde estan las tareas finalizadas.
    FILE* tareas = fopen("tareas.csv", "r"); //Se abre el archivo del usuario que contiene sus tareas.
    char line[100];
    char* nombre;
    char* progre;
    long* fecha_finalizacion;
    while(fgets(line, 99, tareas) != NULL) //Se obtienen los datos del archivo para posteriormente ingresarlos a la base de datos.
    {
        nombre = (char *) get_csv_field(line, 0);
        Tarea* datos = create_tarea(nombre);
        fecha_finalizacion = (long *) malloc (sizeof(long));
        datos->dia = atoi(get_csv_field(line, 2));
        datos->mes = atoi(get_csv_field(line, 3));
        datos->anio = atoi(get_csv_field(line, 4));
        *fecha_finalizacion = transformar_fecha(datos->dia, datos->mes, datos->anio);
        datos->fecha_finalizacion = fecha_finalizacion;
        progre = (char *) get_csv_field(line,1);
        if(strcmp(progre, "ConProgreso")) datos->flag = true;
        insertTreeMap(data_base, fecha_finalizacion, datos);
    }
    fclose(tareas);
    free(tareas);
    menu(data_base, finalizadas);
    return 0;
}


/********  FUNCIONES ********/

bool filtro(Tarea* datos) //Funcion que se encarga de que no se ingresen tareas repetidas.
{
    bool flag = false;
    Tarea* aux;
    return flag;
}

void Importar(TreeMap* data_base, char* nombre) //Funcion que importa un archivo con tareas.
{
    FILE* input;
    char nombre_archivo[20];
    snprintf(nombre_archivo, sizeof(nombre_archivo), "%s%s", nombre, ".csv");
    input = fopen(nombre_archivo, "r");
    if(input == NULL)   printf("No existe el archivo\n"); return;

    char line[100];
    char* name;
    char* progre;
    long* fecha_finalizacion;
    while(fgets(line, 99, input) != NULL)
    {
        name = (char *) get_csv_field(line, 0);
        Tarea* datos = create_tarea(nombre);
        datos->dia = atoi(get_csv_field(line, 2));
        datos->mes = atoi(get_csv_field(line, 3));
        datos->anio = atoi(get_csv_field(line, 4));
        *fecha_finalizacion = transformar_fecha(datos->dia, datos->mes, datos->anio);
        datos->fecha_finalizacion = fecha_finalizacion;
        progre = (char *) get_csv_field(line,1);
        if(strcmp(progre, "ConProgreso")) datos->flag = true;
        insertTreeMap(data_base, fecha_finalizacion, datos);
    }
}

void Mostrar_proximas(TreeMap* data_base) //Funcion que muestra al usuario las proximas 10 tareas
{
    printf("ATENTO!\n Tus proximas tareas son:\n Nombre \t\t\t Progreso \t Fecha Finalizacion\n");
    Tarea* aux = firstTreeMap(data_base);
    int cont = 0;
    while(aux != NULL && cont < 5)
    {
        if(aux->flag == true)  printf("%s \t\t\t Sin Progreso \t %i %i %i\n", aux->nombre, aux->dia, aux->mes, aux->anio);
        else printf("%s \t\t\t %i \t %i %i %i\n", aux->nombre, aux->progreso, aux->dia, aux->mes, aux->anio);
        aux = nextTreeMap(data_base);
        cont++;
    }
}

void Mostrar_todo(TreeMap* data_base, HashMap* fecha) //Funcion que muestra al usuario las tareas que tiene
{
    printf("Todas tus tareas son:\n");
    Tarea* archivo = firstTreeMap(data_base);
    if(archivo == NULL){
        printf("No existen tareas para mostrar");
        return;
    }
    while(archivo != NULL){
        printf("%s %i %i %i\n", archivo->nombre, archivo->dia, archivo->mes, archivo->anio);
        archivo = nextTreeMap(data_base);
    }
    return;
}

void Mostrar_no_finalizadas(TreeMap* data_base) //Función que muestra las tareas no finalizadas
{
    printf("Tus tareas sin finalizar:\n");
    Tarea* archivo = firstTreeMap(data_base);
    int cont = 0;

    if(archivo == NULL){
        printf("No existen tareas para mostrar");
        return;
    }
    while(archivo != NULL){
        if(archivo->finalizada == false){
            printf("%s %i %i %i\n", archivo->nombre, archivo->dia, archivo->mes, archivo->anio);
            cont = 1;
        }
        archivo = nextTreeMap(data_base);
    }
    if(cont == 0){
        printf("No existen tareas no finalizadas");
    }
    return;
}

void Mostrar_finalizadas(TreeMap* data_base) //Función que muestra las tareas marcadas como finalizadas
{
    printf("Tus tareas finalizadas:\n");
    int cont = 0;
    Tarea* archivo = firstTreeMap(data_base);
    if(archivo == NULL){
        printf("No existen tareas para mostrar");
        return;
    }
    while(archivo != NULL){
        if(archivo->finalizada == true){
            printf("%s %i %i %i\n", archivo->nombre, archivo->dia, archivo->mes, archivo->anio);
            cont = 1;
        }
        archivo = nextTreeMap(data_base);
    }
    if(cont == 0){
        printf("No existen tareas finalizadas");
    }
    return;
}

void Eliminar (TreeMap* data_base)
{
    char* nombre;
    int dia, mes, year;
    Tarea* archivo = firstTreeMap(data_base);

    Mostrar_no_finalizadas(data_base);
    printf("Ingrese nombre y fecha de finalización de la tarea a eliminar(en formato de DD MM AA)\n");
    scanf("%s %d %d %d",&nombre,&dia,&mes,&year);
    while(archivo != NULL){
        if(strcmp(archivo->nombre,nombre) == 0 && (archivo->dia == dia) && (archivo->mes == mes) && (archivo->anio == year)){
            eraseTreeMap(data_base,archivo->fecha_finalizacion);
            printf("La tarea se ha eliminado con exito\n");
            return;
        }
        else{
            archivo = nextTreeMap(data_base);
        }
    }    
    printf("No se ha encontrado la tarea a eliminar\n");
    return;
}

void Exportar_finalizadas(HashMap* finalizadas, char* nombre)
{
    FILE* output;
    char nombreArchivo[20];
    snprintf(nombreArchivo ,sizeof(nombreArchivo),"%s%s", nombre,".csv");
    output = fopen(nombreArchivo, "TareasFinalizadas");

    Tarea* p = (Tarea *) calloc (1,sizeof(Tarea));
    p = firstMap(finalizadas);
    char line[100];

    while (p != NULL)
    {
        if(p->flag == 1){
            char cprogreso[] = "ConProgreso";
            snprintf(line, sizeof(line), "%c, %s, %d, %d ,%d\n", p->nombre, cprogreso, p->dia, p->mes, p->anio);
        }
        if(p->flag == 0){
            char sprogreso[] = "SinProgreso";
            snprintf(line, sizeof(line), "%c, %s, %d, %d ,%d\n", p->nombre, sprogreso, p->dia, p->mes, p->anio);
        }
    fclose(output);
    free(output);
    free(p);
    }
}

/*
void Exportar_no_finalizadas(finalizadas, nombre){
    return 0;
}

void Exportar_todas(HashMap* finalizadas, TreeMap* data_base){
    FILE* output;
    char* nombreArchivo[20];
    snprintf(nombreArchivo ,sizeof(nombreArchivo),"%s%s", nombre,".csv");
    output = fopen(nombreArchivo, "w");

    TreeMap* data_base

    return 0;
}
*/


/*  char* nombre;
    int progreso;
    bool flag; //Indica si la tarea posee progreso o no.
    int dia;
    int mes;
    int anio;
    bool finalizada; //Indica si la tarea ya fue finalizada.
    long* fecha_finalizacion;
*/
