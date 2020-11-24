#include <stdio.h>

void Importar(TreeMap* data_base, char* nombre);
int transformar_fecha(int dia, int mes, int anio);

int menu(TreeMap* data_base, HashMap* finalizadas)
{
    int dia, mes, anio;
    printf("Bienvenido a tu ToDo List\n Porfavor ingresa la fecha de hoy en formato (dia mes anio)\n");
    scanf("%i %i %i", dia, mes, anio);
    int fecha = transformar_fecha(dia, mes, anio);
    char opcion;
    int flag = -1;

    while(flag != 0)
    {
        printf("\n ¿Que deseas hacer?\n a) Importar un archivo con tus tareas\n b) Exportar un archivo con tus tareas\n c) Eliminar una tarea o evaluacion\n d) Ordenar tus tareas en base a una proxima fecha\n e) Ver tus tareas finalizadas f) Seleccionar una tarea\n Si no quieres hacer nada presiona ENTER\n");
        fflush(stdin);
        scanf("%c", &opcion);
        switch (opcion)
        {
        case 'a':
            char nombre[40];
            printf("Ingresa el nombre del archivo que deseas importar (sin formato) \n");
            fflush(stdin);
            scanf("%s", nombre);
            Importar(data_base, nombre);
            break;
        case '\n':
        {
            flag = 0;
            break;
        }
        default:
        {
            printf("Entrada no valida\n \n");
            break;
        }
    }
    return 0;
}