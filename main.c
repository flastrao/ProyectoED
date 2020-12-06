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

int ID = 1; //Asignador de IDs

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
        if(strcmp(progre, " ConProgreso") == 0) datos->flag = true;
        insertTreeMap(data_base, fecha_finalizacion, datos);
    }
    fclose(tareas);
    free(tareas);
    menu(data_base, finalizadas);
    return 0;
}


/********  FUNCIONES ********/
bool filtro(TreeMap* data_base, Tarea* datos) //Funcion que se encarga de que no se ingresen tareas repetidas
{
    bool flag = false;
    Tarea* aux = firstTreeMap(data_base);

    while(aux != NULL)
    {
        if((strcmp(aux->nombre, datos->nombre) == 0) && (aux->dia == datos->dia) && (aux->mes == datos->mes) && (aux->anio == datos->anio))
        {
            flag = true;
            return flag;
        }
        aux = nextTreeMap(data_base);
    }
    return flag;
}

void Importar(TreeMap* data_base, char* nombre) //Funcion que importa un archivo con tareas.
{
    FILE* input;
    char nombre_archivo[20];
    snprintf(nombre_archivo, sizeof(nombre_archivo), "%s%s", nombre, ".csv");
    input = fopen(nombre_archivo, "r");
    if(input == NULL)
    {
        printf("No existe el archivo\n"); 
        return;
    }   

    char line[100];
    char* name;
    char* progre;
    long* fecha_finalizacion;
    int cont = 0;
    while(fgets(line, 99, input) != NULL)
    {
        name = (char *) get_csv_field(line, 0);
        Tarea* datos = create_tarea(name);
        fecha_finalizacion = (long *) malloc (sizeof(long));
        datos->dia = atoi(get_csv_field(line, 2));
        datos->mes = atoi(get_csv_field(line, 3));
        datos->anio = atoi(get_csv_field(line, 4));
        *fecha_finalizacion = transformar_fecha(datos->dia, datos->mes, datos->anio);
        datos->fecha_finalizacion = fecha_finalizacion;
        progre = (char *) get_csv_field(line,1);
        if(strcmp(progre, " ConProgreso") == 0) datos->flag = true;
        if(filtro(data_base, datos) == false)
        {
            insertTreeMap(data_base, fecha_finalizacion, datos);
            cont++;
        }
    }

    fclose(input);
    free(input);
    printf("Se importaron exitosamente %i tareas\n", cont);
}

void Mostrar_proximas(TreeMap* data_base) //Funcion que muestra al usuario las proximas 10 tareas
{
    printf("ATENTO!\n Tus proximas tareas son:\n Nombre \t\t\t Progreso \t Fecha Finalizacion\n");
    Tarea* aux = firstTreeMap(data_base);
    int cont = 0;
    while(aux != NULL && cont < 5)
    {
        if(aux->flag == true)  printf("%-32s|%10i\t|%i %i %i\n", aux->nombre, aux->progreso, aux->dia, aux->mes, aux->anio);
        else printf("%-32s|SinProgreso\t|%i %i %i\n", aux->nombre, aux->dia, aux->mes, aux->anio);
        aux = nextTreeMap(data_base);
        cont++;
    }
}

void Mostrar_todo(TreeMap* data_base, HashMap* fecha) //Funcion que muestra al usuario las tareas que tiene
{
    
    Tarea* archivo = firstTreeMap(data_base);
    if(archivo == NULL){
        printf("No existen tareas para mostrar");
        return;
    }
    printf("\n Nombre \t\t\t Progreso \t Fecha Finalizacion\n");
    while(archivo != NULL)
    {
        if(archivo->flag == true)  printf("%-32s|%10i\t|%i %i %i\n", archivo->nombre, archivo->progreso, archivo->dia, archivo->mes, archivo->anio);
        else printf("%-32s|SinProgreso\t|%i %i %i\n", archivo->nombre, archivo->dia, archivo->mes, archivo->anio);
        archivo = nextTreeMap(data_base);
    }
    return;
}

void Mostrar_no_finalizadas(TreeMap* data_base) //Función que muestra las tareas no finalizadas
{
    
    Tarea* archivo = firstTreeMap(data_base);
    int cont = 0;

    if(archivo == NULL){
        printf("No existen tareas para mostrar");
        return;
    }
    printf("\n Nombre \t\t\t Progreso \t Fecha Finalizacion\n");
    while(archivo != NULL)
    {
        if(archivo->flag == true)  printf("%-32s|%10i\t|%i %i %i\n", archivo->nombre, archivo->progreso, archivo->dia, archivo->mes, archivo->anio);
        else printf("%-32s|SinProgreso\t|%i %i %i\n", archivo->nombre, archivo->dia, archivo->mes, archivo->anio);
        archivo = nextTreeMap(data_base);
        cont = 1;
    }
    if(cont == 0){
        printf("No existen tareas no finalizadas");
    }
    return;
}

void Mostrar_finalizadas(HashMap* data_base) //Función que muestra las tareas marcadas como finalizadas
{
    

    Tarea* archivo = firstMap(data_base);
    if(archivo == NULL){
        printf("No existen tareas finalizadas\n");
        return;
    }
    printf("\n Nombre \t\t\t Progreso \t Fecha Finalizacion\n");
    while(archivo != NULL)
    {
        if(archivo->flag == true)  printf("%-32s|%10i\t|%i %i %i\n", archivo->nombre, archivo->progreso, archivo->dia, archivo->mes, archivo->anio);
        else printf("%-32s|SinProgreso\t|%i %i %i\n", archivo->nombre, archivo->dia, archivo->mes, archivo->anio);
        archivo = nextMap(data_base);
    }
    return;
}

void Eliminar (TreeMap* data_base){
    char nombre[50];
    int dia, mes, year;
    
    Mostrar_no_finalizadas(data_base);
    printf("\nIngrese nombre de la tarea a eliminar\n");
    fflush(stdin);
    scanf("%[^\n]s",nombre);
    printf("Ingrese dia de finalizacion de la tarea\n");
    fflush(stdin);
    scanf("%d",&dia);
    printf("Ingrese mes de finalizacion de la tarea\n");
    fflush(stdin);
    scanf("%d",&mes);
    printf("Ingrese año de finalizacion de la tarea\n");
    fflush(stdin);
    scanf("%d",&year);

    Tarea* archivo = firstTreeMap(data_base);
    while(archivo != NULL){
        if((strcmp(archivo->nombre,nombre) == 0) && (archivo->dia == dia) && (archivo->mes == mes) && (archivo->anio == year)){
            eraseTreeMap(data_base,archivo->fecha_finalizacion);
            printf("La tarea se ha eliminado con exito\n");
            return;
        }
        archivo = nextTreeMap(data_base);
    }    
    printf("No se ha encontrado la tarea a eliminar\n");
    return;
}

void Seleccionar (TreeMap* data_base, HashMap* finalizadas)
{
    char desicion;
    char nombre[20];
    int dia, mes, year;

    char nuevo_Nombre[20];
    int dia2, mes2, year2;
    int progreso2 = 0;

    int flag = 1;
    char fin[2];
    
    Mostrar_no_finalizadas(data_base);
    Tarea* archivo = firstTreeMap(data_base);
    Tarea* aux = archivo;
    while(flag != 0){
        printf("\nIngrese nombre de la tarea a seleccionar\n");
        fflush(stdin);
        scanf("%[^\n]s",nombre);
        printf("Ingrese fecha de finalizacion de la tarea a modificar(en formato de DD MM AA)\n");
        fflush(stdin);
        scanf("%d %d %d",&dia,&mes,&year);

        while(archivo != NULL){
            if((strcmp(archivo->nombre,nombre) == 0) && (archivo->dia == dia) && (archivo->mes == mes) && (archivo->anio == year)){
                printf("¿Que desea modificar?\n a) Nombre de la tarea\n b) Fecha de la tarea\n c) Progreso de la tarea\n Si no desea continuar presione ENTER\n");
                fflush(stdin);
                scanf("%c", &desicion);
                switch (desicion)
                {
                    case 'a':
                    {
                        printf("Ingrese nuevo nombre\n");
                        fflush(stdin);
                        scanf("%[^\n]s",nuevo_Nombre);
                        strcpy(archivo->nombre,nuevo_Nombre);
                        printf("Nombre modificado exitosamente\n");
                        break;
                    }
                    case 'b':
                    {
                        printf("Ingrese nueva fecha(en formato de DD MM AA)\n");
                        fflush(stdin);
                        scanf("%d %d %d",&dia2, &mes2, &year2);
                        archivo->dia = dia2;
                        archivo->mes = mes2;
                        archivo->anio = year2;
                        printf("Fecha modificada exitosamente\n");
                        break;
                    }
                    case 'c':
                    {
                        printf("Ingrese porcentaje progreso (solo numero)\n");
                        fflush(stdin);
                        scanf("%d",&progreso2);
                        archivo->progreso += progreso2;
                        if(archivo->progreso < 100) printf("Progreso modificado exitosamente\n");
                        else
                        {
                            insertMap(finalizadas, archivo->fecha_finalizacion, archivo);
                            eraseTreeMap(data_base, archivo->fecha_finalizacion);
                            printf("Su tarea ha sido finalizada\n");
                        }
                        
                        break;
                    }
                    default:
                    {
                        flag = 0;
                        return;
                    }
                }
            }
            else{
                archivo = nextTreeMap(data_base);
            }
        }
        printf("¿Desea finalizar la funcion? (si/no)\n");
        fflush(stdin);
        scanf("%s",fin);
        if(strcmp(fin,"si") == 0){
            return;
        }
    }
}

void Exportar_finalizadas(HashMap* finalizadas, char* nombre)
{
    FILE* output;
    char nombreArchivo[20];
    snprintf(nombreArchivo ,sizeof(nombreArchivo),"%s%s", nombre,".csv");
    output = fopen(nombreArchivo, "w");

    Tarea* p = (Tarea *) calloc (1,sizeof(Tarea));
    p = firstMap(finalizadas);
    char line[100];

    while (p != NULL)
    {
        if(p->flag == true){
            char cprogreso[] = "ConProgreso";
            snprintf(line, sizeof(line), "%c, %s, %d, %d ,%d\n", p->nombre, cprogreso, p->dia, p->mes, p->anio);
        }
        if(p->flag == false){
            char sprogreso[] = "SinProgreso";
            snprintf(line, sizeof(line), "%c, %s, %d, %d ,%d\n", p->nombre, sprogreso, p->dia, p->mes, p->anio);
        }
        fputs(line, output);
        p = nextMap(finalizadas);
    }

    fclose(output);
    free(output);
    free(p);
}


void Exportar_no_finalizadas(TreeMap* data_base, char* nombre){
    FILE* output;
    char nombreArchivo[20];
    snprintf(nombreArchivo ,sizeof(nombreArchivo),"%s%s", nombre,".csv");
    output = fopen(nombreArchivo, "w");

    Tarea* p = (Tarea *) calloc (1,sizeof(Tarea));
    p = firstTreeMap(data_base);
    char line[100];

    while (p != NULL)
    {
        if(p->flag == true){
            char cprogreso[] = "ConProgreso";
            snprintf(line, sizeof(line), "%s, %s, %d, %d ,%d\n", p->nombre, cprogreso, p->dia, p->mes, p->anio);
        }
        if(p->flag == false){
            char sprogreso[] = "SinProgreso";
            snprintf(line, sizeof(line), "%s, %s, %d, %d ,%d\n", p->nombre, sprogreso, p->dia, p->mes, p->anio);
        }
        fputs(line, output);
        p = nextTreeMap(data_base);
    }

    fclose(output);
    free(output);
    free(p);
}

void Exportar_todas(HashMap* finalizadas, TreeMap* data_base, char* nombre){
    FILE* output;
    char nombreArchivo[20];
    snprintf(nombreArchivo ,sizeof(nombreArchivo),"%s%s", nombre,".csv");
    output = fopen(nombreArchivo, "w");

    char cprogreso[40] = "ConProgreso";
    char sprogreso[40] = "SinProgreso";
    Tarea* p = (Tarea *) calloc (1,sizeof(Tarea));
    p = firstMap(finalizadas);
    char line[100];
    while (p != NULL)
    {
        if(p->flag == true){
            snprintf(line, sizeof(line), "%s, %s, %d, %d ,%d\n", p->nombre, cprogreso, p->dia, p->mes, p->anio);
        }
        if(p->flag == false){
            snprintf(line, sizeof(line), "%s, %s, %d, %d ,%d\n", p->nombre, sprogreso, p->dia, p->mes, p->anio);
        }
        fputs(line, output);
        p = nextMap(finalizadas);
    }

    p = firstTreeMap(data_base);

    while (p != NULL)
    {
        if(p->flag == true){
            char cprogreso[] = "ConProgreso";
            snprintf(line, sizeof(line), "%s, %s, %d, %d ,%d\n", p->nombre, cprogreso, p->dia, p->mes, p->anio);
        }
        if(p->flag == false){
            char sprogreso[] = "SinProgreso";
            snprintf(line, sizeof(line), "%s, %s, %d, %d ,%d\n", p->nombre, sprogreso, p->dia, p->mes, p->anio);
        }
        fputs(line, output);
        p = nextTreeMap(data_base);
    }

    fclose(output);
    free(output);
    free(p);
}

void Agregar_tarea(TreeMap* data_base, char* nombre, int dia, int mes, int anio, int indicadorProgreso)
{
    Tarea* nuevo = (Tarea *) calloc (1, sizeof(Tarea));
    nuevo = create_tarea(nombre);
    nuevo->progreso = 0;
    if(indicadorProgreso == 1){
        nuevo->flag = true;
    }
    if(indicadorProgreso == 0){
        nuevo->flag = false;
    }
    nuevo->dia = dia;
    nuevo->mes = mes;
    nuevo->anio = anio;
    nuevo->finalizada = false;
    long* fecha_finalizacion;
    fecha_finalizacion = (long *) malloc (sizeof(long));
    *fecha_finalizacion = transformar_fecha(nuevo->dia, nuevo->mes, nuevo->anio);
    nuevo->fecha_finalizacion = fecha_finalizacion;
    if(filtro(data_base, nuevo) == false)
    {
        insertTreeMap(data_base, fecha_finalizacion, nuevo);
        printf("Tarea agregada exitosamente!\n");
        return;
    }   
    printf("La tarea ingresada ya se encuentra almacenada\n");
}