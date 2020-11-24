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

typedef struct
{
    char* nombre;
    int progreso;
    bool flag; //Indica si la tarea posee progreso o no
    int fecha_finalizacion;
    int dia;
    int mes;
    int anio;
    bool finalizada;
} Tarea;

Tarea* create_tarea(char * nombre) //Funcion que inicializa las variables de un tipo de dato tarea
{
    Tarea* input = (Tarea *) calloc (1, sizeof(Tarea));
    input->nombre = nombre;
    input->flag = false;
    input->finalizada = false;
    return input;
}

int transformar_fecha(int dia, int mes, int anio) //Funcion que transforma una fecha a un numero entero
{
    int fecha = 0;
    fecha = anio % 100;
    fecha = fecha * 12 + mes - 1;
    fecha = fecha * 31 + dia - 1;
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
    while(fgets(line, 99, tareas) != NULL)
    {
        nombre = (char *) get_csv_field(line, 0);
        Tarea* datos = create_tarea(nombre);
        datos->dia = get_csv_field(line, 2);
        datos->mes = get_csv_field(line, 3);
        datos->anio = get_csv_field(line, 4);
        datos->fecha_finalizacion = transformar_fecha(datos->dia, datos->mes, datos->anio);
        if(strcmp(get_csv_field(line, 1), "ConProgreso")) datos->flag = true;
        insertTreeMap(data_base, datos->fecha_finalizacion, datos);
    }

    fclose(tareas);
    free(tareas);
    menu(data_base, finalizadas);
    return 0;
}


/********  FUNCIONES ********/
bool filtro(Tarea* datos)
{
    bool flag = false;
    Tarea* aux;
    return flag;
}

void Importar(TreeMap* data_base, char* nombre)
{
    FILE* input;
    char nombre_archivo[20];
    snprintf(nombre_archivo, sizeof(nombre_archivo), "%s%s", nombre, ".csv");
    input = fopen(nombre_archivo, "r");
    if(input == NULL)   printf("No existe el archivo\n"); return;

    char line[100];
    char* nombre;
    while(fgets(line, 99, input) != NULL)
    {
        nombre = (char *) get_csv_field(line, 0);
        Tarea* datos = create_tarea(nombre);
        datos->dia = get_csv_field(line, 2);
        datos->mes = get_csv_field(line, 3);
        datos->anio = get_csv_field(line, 4);
        datos->fecha_finalizacion = transformar_fecha(datos->dia, datos->mes, datos->anio);
        if(strcmp(get_csv_field(line, 1), "ConProgreso")) datos->flag = true;
        if(filtro(datos) == false) insertTreeMap(data_base, datos->fecha_finalizacion, datos);
    }
}