#include <stdio.h>

void Importar(TreeMap* data_base, char* nombre);
int transformar_fecha(int dia, int mes, int anio);
void Mostrar_proximas(TreeMap* data_base);
void actualizar(TreeMap* data_base, HashMap* finalizadas, int fecha);
void Eliminar (TreeMap* data_base);
void Mostrar_todo (TreeMap* data_base, HashMap* finalizadas);
void Mostrar_no_finalizadas(TreeMap* data_base);
void Mostrar_finalizadas(HashMap* finalizadas);
void Seleccionar (TreeMap* data_base, HashMap* finalizadas);
void Exportar_finalizadas(HashMap* finalizadas, char* nombre);
void Exportar_no_finalizadas(TreeMap* data_base, char* nombre);
void Exportar_todas(HashMap* finalizadas,TreeMap* data_base, char* nombre);
void Agregar_tarea(TreeMap* base_datos, char* nombre, int dia, int mes, int anio, int indicadorProgreso);

int menu(TreeMap* data_base, HashMap* finalizadas)
{
    int dia, mes, anio;
    printf("Bienvenido a tu ToDo List\n \n");
    Mostrar_proximas(data_base);
    char opcion;
    int flag = -1;

    while(flag != 0)
    {
        printf("\n ¿Que deseas hacer?\n a) Importar un archivo con tus tareas\n b) Exportar un archivo con tus tareas\n c) Eliminar una tarea o evaluacion\n d) Ver tus tareas\n e) Seleccionar una tarea\n f) Agregar una tarea\n Si no quieres hacer nada presiona ENTER\n");
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
                fflush(stdin);
                scanf("%c", &opc);

                if(opc == 'a') Exportar_finalizadas(finalizadas, nombre);
                else if(opc == 'b') Exportar_no_finalizadas(data_base, nombre);
                else if(opc == 'c') Exportar_todas(finalizadas, data_base, nombre);
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
                printf("¿Que tareas desea ver?\n a) Solo tareas finalizadas\n b) Solo tareas que aun no han sido finalizadas\n c) Todas tus tareas\n");
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
                Seleccionar(data_base, finalizadas);
                break;
            }
            case 'f':
            {
                char* nombre = (char *) calloc (40, sizeof(char));
                int dia;
                int mes;
                int anio;
                int indicadorProgreso;

                printf("Ingrese el nombre de la tarea que desea agregar:\n");
                fflush(stdin);
                scanf("%[^\n]s",nombre);
                printf("Indique si la tarea posee progreso o no (1 si tiene progreso, 0 si no tiene progreso):\n");
                fflush(stdin);
                scanf("%d", &indicadorProgreso);
                printf("Ingrese el dia de finalizacion de dicha tarea:\n");
                fflush(stdin);
                scanf("%d", &dia);
                printf("Ingrese el mes de finalizacion de dicha tarea:\n");
                fflush(stdin);
                scanf("%d", &mes);
                printf("Ingrese el anio de finalizacion de dicha tarea:\n");
                fflush(stdin);
                scanf("%d", &anio);
                Agregar_tarea(data_base, nombre, dia, mes, anio, indicadorProgreso);
                break;
            }
            case '\n':
            {
                printf("Hasta pronto!\n");
                flag = 0;
                break;
            }
            default:
            {
                printf("Entrada no valida\n \n");
                break;
            }
        }       
    }
    return 0;
}