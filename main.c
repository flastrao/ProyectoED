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

typedef struct
{
    char* nombre;
    int progreso;
    bool flag; //Indica si la tarea posee progreso o no
    int dia;
    int mes;
    int anio;
    bool finalizada;
    long* fecha_finalizacion;
} Tarea;

Tarea* create_tarea(char * nombre) //Funcion que inicializa las variables de un tipo de dato tarea
{
    Tarea* input = (Tarea *) calloc (1, sizeof(Tarea));
    input->nombre = nombre;
    input->flag = false;
    input->finalizada = false;
    input->fecha_finalizacion = (long *) malloc (sizeof(long));
    return input;
}

int transformar_fecha(int dia, int mes, int anio) //Funcion que transforma una fecha a un numero entero
{
    int fecha = 0;
    fecha = anio % 100;
    fecha = fecha * 12 + mes - 1;
    fecha = fecha * 31 + dia - 1;
    fecha = fecha * 1000 + ID;
    ID++;
    return fecha;
}

// return 1 if key1<key2
int lower_than_int(void* key1, void* key2){
    int k1 = *((int*) (key1));
    int k2 = *((int*) (key2));
    return k1<k2;
}

int main()
{
    TreeMap* data_base = createTreeMap(lower_than_int);
    HashMap* finalizadas = createMap(100);
    FILE* tareas = fopen("tareas.csv", "r");
    char line[100];
    char* nombre;
    char* progre;
    long* fecha_finalizacion;
    while(fgets(line, 99, tareas) != NULL)
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
bool filtro(Tarea* datos) //Funcion que se encarga de que no se ingresen tareas repetidas
{
    bool flag = false;
    Tarea* aux;
    return flag;
}

void Importar(TreeMap* data_base, char* nombre) //Funcion que importa un archivo con tareas
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
    while(aux != NULL)
    {
        if(aux->flag == true)  printf("%s \t Sin Progreso \t\t %i %i %i\n", aux->nombre, aux->dia, aux->mes, aux->anio);
        else printf("%s \t %i \t\t %i %i %i\n", aux->nombre, aux->progreso, aux->dia, aux->mes, aux->anio);
        aux = nextTreeMap(data_base);
    }
}

void actualizar(TreeMap* data_base, HashMap* finalizadas, int fecha) //Funcion que actualiza los datos almacenados en base a la fecha ingresada (Elimina las tareas expirasdas)
{
    Tarea* archivo = firstTreeMap(data_base);
    while(archivo != NULL){
        if(archivo->fecha_finalizacion){
            Eliminar(archivo);
            archivo = nextTreeMap(data_base);
        }
        archivo = nextTreeMap(data_base);
    }
    return;
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

void Eliminar (TreeMap* data_base){
    char nombre;
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