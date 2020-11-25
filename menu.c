#include <stdio.h>

void Importar(TreeMap* data_base, char* nombre);
int transformar_fecha(int dia, int mes, int anio);
void actualizar(TreeMap* data_base, HashMap* finalizadas, int fecha);
void Mostrar_proximas(TreeMap* data_base, int fecha);

int menu(TreeMap* data_base, HashMap* finalizadas)
{
    int dia, mes, anio;
    printf("Bienvenido a tu ToDo List\n Porfavor ingresa la fecha de hoy en formato (dia mes anio)\n");
    scanf("%i %i %i", dia, mes, anio);
    int fecha = transformar_fecha(dia, mes, anio);
    actualizar(data_base, finalizadas, fecha);
    Mostrar_proximas(data_base, fecha);
    char opcion;
    int flag = -1;

    while(flag != 0)
    {
        printf("\n ¿Que deseas hacer?\n a) Importar un archivo con tus tareas\n b) Exportar un archivo con tus tareas\n c) Eliminar una tarea o evaluacion\n d) Ver todas tus tareas\n e) Ver tus tareas finalizadas f) Seleccionar una tarea\n Si no quieres hacer nada presiona ENTER\n");
        fflush(stdin);
        scanf("%c", &opcion);
        switch (opcion)
        {
        case 'a':
        {
            char nombre[40];
            printf("Ingrese el nombre del archivo que deseas importar (sin formato) \n");
            fflush(stdin);
            scanf("%s", nombre);
            Importar(data_base, nombre);
            break;
        }
        case 'b':
        {
            char nombre[40];
            printf("Ingrese el nombre del archivo en el cual se exportaran tus tareas\n");
            fflush(stdin);
            scanf("%s", nombre);
            char opc;
            printf("¿Que desea exportar?\n a) Solo sus tareas finalizadas\n b) Solo sus tareas que aun no han sido finalizadas\n c) Todas tus tarea\n");
            scanf("%c", &opc);
            if(opc == 'a') Exportar(finalizadas, nombre);
            else if(opc == 'b') Exportar(data_base, nombre);
            else if(opc == 'c') Exportar(data_base, finalizadas, nombre);
            else printf("Opcion no valida, intentelo denuevo\n");
            break;
            
        }
        case 'c':
        {
            Eliminar(data_base);
            break;
        }
        case 'd':
        {
            char opc;
            printf("¿Que tareas desea ver?\n a) Solo tareas finalizadas\n b) Solo tareas que aun no han sido finalizadas\n c) Todas tus tareas");
            fflush(stdin);
            scanf("%c", &opc);
            if(opc == 'a') Mostrar_finalizadas(finalizadas);
            else if(opc == 'b') Mostrar_no_finalizadas(data_base);
            else if(opc == 'c') Mostrar_todo(data_base, finalizadas);
            else printf("Opcion no valida, intentelo denuevo\n");
            break;
        }
        case 'e':
        {
            Seleccionar(data_base);
            break;
        }
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