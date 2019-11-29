#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

struct tipos {
    int activo;
    int pasos[5];
    int cantidadDoc;
    float tiempoPromedioPorTipoPorArea[5];
    float desviacionTipicaPorArea[5];
};

struct documento{
    int procesado;
    int pendiente;
    int noEntro;
    int tipo;
    int id;
    int recorrido[2][5];
    float tiempoPromedioPorTipoPorArea[5];
    float desviacionTipicaPorArea[5];
    int pasosTotal;
    time_t tiempoInicio, tiempoFinal;
    double duracion;
};

struct documentoEnArea{
    int tipo;
    int id;
    int recorrido[2][5];
    float tiempoPromedioPorTipoPorArea[5];
    float desviacionTipicaPorArea[5];
    int pasosTotal;
    time_t tiempoInicio, tiempoFinal;
    time_t tiempoInicioProceso, tiempoMovimientoProceso;
    double duracion;
    struct documentoEnArea *ant;
    struct documentoEnArea *sig;
};

typedef struct tipos tipos;
typedef struct documento documento;
typedef struct documentoEnArea documentoEnArea;

int cantidadTipos;
int cantidadDocumentos;
tipos vecTipos[1];

void addDocumento(documentoEnArea**, documentoEnArea**, documento);
void agregarTiposNuevos(tipos*);
void agregarCantidadDeDocumentos(tipos*);
void cerearVecTipos(tipos*);
void run(tipos*, documentoEnArea* area[2][5]);
void ImprimirReporte(documentoEnArea*);
void procesarArea(documentoEnArea* area[2][5], int vecVerificadorInicio[5], documento vecTotalDoc[cantidadDocumentos]);

int main(){

    srand(time(NULL));

    int comenzarPrograma = 1;
    int simulacion;
    int yaSimulada = 0;
    int modificacion;
    int procesoModificacion;

    documentoEnArea* area[2][5] = {
        {NULL, NULL, NULL, NULL, NULL},
        {NULL, NULL, NULL, NULL, NULL}
    };


    while(comenzarPrograma != 0){
        printf("\nDESEA COMENZAR EL PROGRAMA?\n1) COMENZAR\n0) SALIR\nRespuesta: ");
        scanf("%d", &comenzarPrograma);

        if(comenzarPrograma){
            printf("\n1. SIMULACION NUEVA\n2. SIMULACION ANTERIOR\nRespuesta: ");
            scanf("%d", &simulacion);

            if(simulacion == 1){

                yaSimulada = 1;

                printf("\nCANTIDAD DE TIPOS: ");
                scanf("%d", &cantidadTipos);
                tipos vecTipos[cantidadTipos];

                agregarTiposNuevos(vecTipos);

                agregarCantidadDeDocumentos(vecTipos);

                run(vecTipos, area);
                
            }else if (simulacion == 2){

                if(yaSimulada == 1){

                    printf("\nDESEA HACER MODIFICACIONES?\n 1) MODIFICAR\n 0) NO MODIFICAR\nRespuesta: ");
                    scanf("%d", &modificacion);

                    if(modificacion){

                        printf("\nDESEA ELIMINAR? \n(Observacion: 0-NO, 1-SI)\nRespuesta: ");
                        scanf("%d", &procesoModificacion);
                        if(procesoModificacion){
                            //eliminar();
                        }
                        printf("\nDESEA AGREGAR? \n(Observacion: 0-NO, 1-SI)\nRespuesta: ");
                        scanf("%d", &procesoModificacion);
                        if(procesoModificacion){
                            //agregar();
                        }

                        //run();
                    }else{
                        //run();

                    }

                }else{
                    printf("NO SE HIZO NI UNA SIMULACION ANTES :(\n");
                }

            }else{
                printf("ELEGIR UNA OPCION VALIDA..\n");
            }

        }else{
            printf("HASTA LA PROXIMA\n");
        }

    }

    return 0;
}






void agregarTiposNuevos(tipos vecTipos[]){
    int verificadorDePasoDeArea[5];
    int i, j;
    int auxPaso;
    int tiempoTipoPorAreaAux;
    float desviacionTipicaPorAreaAux;

    // TIPO
    for(i = 0; i < cantidadTipos; i++){
        verificadorDePasoDeArea[0] = 0;
        verificadorDePasoDeArea[1] = 0;
        verificadorDePasoDeArea[2] = 0;
        verificadorDePasoDeArea[3] = 0;
        verificadorDePasoDeArea[4] = 0;
        vecTipos[i].activo = 1;

        // AREAS
        printf("INGRESE LAS AREAS POR LA QUE PASARA EL TIPO %d: \n(Observacion: Colocar 0 para finalizar)\n", i + 1);
        for(j = 0; j < 5; j++){
            do{
                printf("SU PASO %d SERA: ", j + 1);
                scanf("%d", &auxPaso);
                if(auxPaso == 0){
                    break;
                }
            }while (auxPaso < 0 || auxPaso >= 6 || verificadorDePasoDeArea[auxPaso - 1] == 1);

            if(auxPaso == 0){
                break;
            }

            printf("\nINGRESE EL TIEMPO POR CADA AREA: \n");
            do{
                printf("\nSU TIEMPO EN EL AREA %d PARA EL TIPO %d ES: ", auxPaso, i+1);
                scanf("%d", &tiempoTipoPorAreaAux);
            }while(tiempoTipoPorAreaAux <= 0 || tiempoTipoPorAreaAux >= 36000);

            printf("\nINGRESE UNA DESVIACION TIPICA POR CADA AREA: \n");
            do{
                printf("\nSU DESVIACION TIPICA EN EL AREA %d POR EL TIPO %d ES: ", auxPaso, i+1);
                scanf("%f", &desviacionTipicaPorAreaAux);
            }while(desviacionTipicaPorAreaAux < 0.00 || desviacionTipicaPorAreaAux > 1.00);

            if(auxPaso > 0 && auxPaso < 6){
                vecTipos[i].pasos[j] = auxPaso;
                vecTipos[i].tiempoPromedioPorTipoPorArea[j] = tiempoTipoPorAreaAux;
                vecTipos[i].desviacionTipicaPorArea[j] = desviacionTipicaPorAreaAux;
                verificadorDePasoDeArea[auxPaso - 1] = 1;
            }

        }

    }

    return ;
}







void agregarCantidadDeDocumentos(tipos vecTipos[]){
    int i, cantidadDefinida, agregarManualmente, auxCantidadDocumentoTipo;
    printf("\nLA CANTIDAD DE DOCUMENTOS YA ESTA DEFINIDA? (Observacion: 1-SI, 0-NO-Random)\nRespuesta: ");
    scanf("%d", &cantidadDefinida);
    if(cantidadDefinida){
        printf("LA CANTIDAD DE DOCUMENTOS ES: ");
        scanf("%d", &cantidadDocumentos);
    }else{
        cantidadDocumentos = 100 + rand() % 100;
    }

    int auxCantidadDocumentos = cantidadDocumentos;
    printf("DESEA AGREGAR MANUALMENTE LAS CANTIDADES POR TIPO? (Observacion: 1-SI, 0-NO-Random)\nRespuesta: ");
    scanf("%d", &agregarManualmente);
    printf("\n");
    if(agregarManualmente){
        while(auxCantidadDocumentos){
            cerearVecTipos(vecTipos);
            auxCantidadDocumentos = cantidadDocumentos;
            for(i = 0; i < cantidadTipos; i++){
                printf("LE SOBRA %d\n", auxCantidadDocumentos);

                printf("CUANTOS DOCUMENTOS TIENE EL TIPO %d? ", i + 1);
                scanf("%d", &auxCantidadDocumentoTipo);
                while(auxCantidadDocumentoTipo > auxCantidadDocumentos || auxCantidadDocumentoTipo < 0){
                    printf("AGREGUE CORRECTAMENTE POR FAVOR\n");
                    printf("CUANTOS DOCUMENTOS TIENE EL TIPO %d? ", i + 1);
                    scanf("%d", &auxCantidadDocumentoTipo);
                }
                vecTipos[i].cantidadDoc = auxCantidadDocumentoTipo;

                auxCantidadDocumentos -= vecTipos[i].cantidadDoc;
                if(auxCantidadDocumentos == 0){
                    printf("YA SELECCIONO TODO! NO SOBRA MAS :)\n");
                    break;
                }
            }
            if(auxCantidadDocumentos){
                printf("VUELVA A INGRESAR TODO, NO SE USO LA TOTALIDAD DE DOCUMENTOS DISPONIBLES :(\n");
            }
        }
    }else{
        auxCantidadDocumentos = cantidadDocumentos;
        cerearVecTipos(vecTipos);
        for(i = 0; i < cantidadTipos - 1; i++){
            printf("LE SOBRA %d\n", auxCantidadDocumentos);
            auxCantidadDocumentoTipo = rand() % auxCantidadDocumentos;
            printf("SE ELIGIO PARA EL TIPO %d UNA CANTIDAD DE %d\n", i + 1, auxCantidadDocumentoTipo);
            auxCantidadDocumentos -= auxCantidadDocumentoTipo;
            vecTipos[i].cantidadDoc = auxCantidadDocumentoTipo;
            if(auxCantidadDocumentos == 0){
                printf("LE SOBRA %d\n", auxCantidadDocumentos);
                break;
            }
        }
        printf("LE SOBRA %d\n", auxCantidadDocumentos);
        vecTipos[i].cantidadDoc = auxCantidadDocumentos;
        printf("SE ELIGIO PARA EL TIPO %d UNA CANTIDAD DE %d\n", i + 1, auxCantidadDocumentos);
        auxCantidadDocumentos -= auxCantidadDocumentos;
        printf("LE SOBRA %d\n", auxCantidadDocumentos);
        printf("YA SELECCIONO TODO! NO SOBRA MAS :)\n");
    }

    return ;
}







void run(tipos vecTipos[], documentoEnArea* area[2][5]){
    system("cls");

    int areaInicioProceso;
    documento vecTotalDoc[200];
    int i, j;
    int auxTipo;
    int vecVerificadorInicio[5];
    
    // AGREGA TODO EN EL DOCUMENTO
    for(i = 0; i < cantidadDocumentos; i++){
        auxTipo = rand() % cantidadTipos;

        while(vecTipos[auxTipo].cantidadDoc == 0){
            auxTipo = rand() % cantidadTipos;
        }
        vecTotalDoc[i].procesado = 0;
        vecTotalDoc[i].pendiente = 0;
        vecTotalDoc[i].noEntro = 1;
        vecTotalDoc[i].tipo = auxTipo + 1;
        vecTotalDoc[i].id = i+1;
        vecTotalDoc[i].pasosTotal = 0;
        for(j = 0; j < 5; j++){
            vecTotalDoc[i].recorrido[0][j] = vecTipos[vecTotalDoc[i].tipo-1].pasos[j];
            vecTotalDoc[i].recorrido[1][j] = 0;   
            vecTotalDoc[i].tiempoPromedioPorTipoPorArea[j] = vecTipos[vecTotalDoc[i].tipo-1].tiempoPromedioPorTipoPorArea[j];
            vecTotalDoc[i].desviacionTipicaPorArea[j] = vecTipos[vecTotalDoc[i].tipo-1].desviacionTipicaPorArea[j];          
        }
        
        vecTipos[auxTipo].cantidadDoc -= 1;
    }

    for(i = 0; i < 5; i++){
        vecVerificadorInicio[i] = 0;
    }

    ////////////////////////EMPIEZA SIMULACION///////////////////////////////////////////////////////////
    
    const time_t HoraInicioPrograma = time(NULL);
    time_t HoraMovimientoPrograma = time(NULL);
    time_t ultimoAgregado = time(NULL);
    
    int tiempoEsperaAdd = 0;
    int numeroDeDocumento = 0;
    
    int tiempoEsperaImp = 0;

    while(difftime(HoraMovimientoPrograma, HoraInicioPrograma) < 60) {
        HoraMovimientoPrograma = time(NULL);

        // PROCESO: AGREGAR EN AREA 
        if(difftime(HoraMovimientoPrograma, ultimoAgregado) == tiempoEsperaAdd){
            
            if(numeroDeDocumento < cantidadDocumentos){
                printf("\n\nENTRO EL DOCUMENTO NRO: |%d|\n", numeroDeDocumento+1);
                areaInicioProceso = vecTotalDoc[numeroDeDocumento].recorrido[0][0];
                vecTotalDoc[numeroDeDocumento].tiempoInicio = HoraMovimientoPrograma;
                addDocumento(&area[0][areaInicioProceso - 1], &area[1][areaInicioProceso - 1], vecTotalDoc[numeroDeDocumento]);
                numeroDeDocumento++;

            }else{
                printf("\nYA SE PROCESARON TODOS LOS DOCUMENTOS :D");
                
            }
            
            ultimoAgregado = vecTotalDoc[numeroDeDocumento - 1].tiempoInicio;
            
            //tiempoEsperaAdd = rand()%5;
            tiempoEsperaAdd = 3;
        }
        
        // PROCESO: PROCESAR LAS AREAS
        procesarArea(area, vecVerificadorInicio, vecTotalDoc);


        if(difftime(HoraMovimientoPrograma, HoraInicioPrograma) == tiempoEsperaImp) {
            for(i = 0; i < 5; i++) {
                if(area[0][i] != NULL){
                    ImprimirReporte(area[0][i]);
                }else{
                    printf("\tEl area |%d| esta vacia.. \n", i+1);
                }
            }
            tiempoEsperaImp = tiempoEsperaImp + 5;
        }

    }

    return ;
}




void cerearVecTipos(tipos* vecTipos){
    for(int i = 0; i < cantidadTipos; i++){
        vecTipos[i].cantidadDoc = 0;
    }
}




void addDocumento(documentoEnArea* *inicio, documentoEnArea* *ultimo, documento miDocumento){

    miDocumento.pendiente = 1;
    miDocumento.noEntro = 0;

    int i;
    documentoEnArea* nuevoDocumentoEnArea;

    nuevoDocumentoEnArea = (documentoEnArea*) malloc(sizeof(documentoEnArea));

    //////////////////////AGREGAR DATOS ///////////////////////////////
    nuevoDocumentoEnArea->tipo = miDocumento.tipo;
    nuevoDocumentoEnArea->id = miDocumento.id;
    nuevoDocumentoEnArea->pasosTotal = 0;
    for(i = 0; i < 5; i++){
        nuevoDocumentoEnArea->recorrido[0][i] = miDocumento.recorrido[0][i];
        nuevoDocumentoEnArea->tiempoPromedioPorTipoPorArea[i] = miDocumento.tiempoPromedioPorTipoPorArea[i];
        nuevoDocumentoEnArea->desviacionTipicaPorArea[i] = miDocumento.desviacionTipicaPorArea[i];
    }
    nuevoDocumentoEnArea->tiempoInicio = miDocumento.tiempoInicio;
    nuevoDocumentoEnArea->tiempoFinal = miDocumento.tiempoInicio;
    ///////////////////////////////////////////////////////////////////

    if(*inicio == NULL){
        nuevoDocumentoEnArea->ant = NULL;
        nuevoDocumentoEnArea->sig = NULL;
        *inicio = nuevoDocumentoEnArea;
        *ultimo = nuevoDocumentoEnArea;
    }else{
        nuevoDocumentoEnArea->ant = *ultimo;
        nuevoDocumentoEnArea->sig = NULL;
        (*ultimo)->sig = nuevoDocumentoEnArea;
        *ultimo = nuevoDocumentoEnArea;
    }

    return ;
}



void ImprimirReporte(documentoEnArea* area){
    printf("\n");
    printf("\n");
    printf("\n\tCOMIENZA LA IMPRESION DE LA AREA |%d|:\n\n", area->recorrido[0][area->pasosTotal]);

    while(area != NULL){
        printf("\t\tSU TIPO ES: |%d|\n", area->tipo);
        printf("\t\tLA ID DEL DOCUMENTO ES: |%d|\n", area->id);
        printf("\t\tESTA EN LA AREA |%d| EN SU PASO NUMERO |%d|\n", area->recorrido[0][area->pasosTotal], area->pasosTotal + 1);
        printf("\t\tSU TIEMPO DE INICIO ES: ");
        printf("%s", ctime(&(area->tiempoInicio)));
        printf("\n\n\t\t----------------------------------------------\n\n");
        area = area->sig;

    }
    printf("\n");

    return;
}

void procesarArea(documentoEnArea* area[2][5], int vecVerificadorInicio[5], documento vecTotalDoc[cantidadDocumentos]){

    int i;
    float duracionConDesviacionTipica;
    documentoEnArea* documentoTransladar;

    // PARA CADA AREA
    for(i = 0; i < 5; i++){

        if(area[0][i] != NULL){

            if(vecVerificadorInicio[i] == 0){
                area[0][i]->tiempoInicioProceso = time(NULL);
                vecVerificadorInicio[i] = 1;

                if(rand()%2){
                    area[0][i]->duracion = area[0][i]->tiempoPromedioPorTipoPorArea[i] + (area[0][i]->tiempoPromedioPorTipoPorArea[i] * area[0][i]->desviacionTipicaPorArea[i]);
                }else{
                    area[0][i]->duracion = area[0][i]->tiempoPromedioPorTipoPorArea[i] - (area[0][i]->tiempoPromedioPorTipoPorArea[i] * area[0][i]->desviacionTipicaPorArea[i]);
                }

            }
            
            area[0][i]->tiempoMovimientoProceso = time(NULL);

            if(difftime(area[0][i]->tiempoMovimientoProceso, area[0][i]->tiempoInicioProceso) >= area[0][i]->duracion){

                // PROCESO DE CAMBIO
                area[0][i]->pasosTotal++;

                if(area[0][i]->recorrido[0][area[0][i]->pasosTotal] != 0){
                    // PROCESO DE CAMBIO DE DOCUMENTO DE UNA AREA A OTRA
                    area[0][i]->recorrido[1][(area[0][i]->pasosTotal) - 1] = 1;

                    documentoTransladar = area[0][i];

                    if((area[0][(documentoTransladar->recorrido[0][documentoTransladar->pasosTotal]) - 1]) != NULL){
                        (area[1][(documentoTransladar->recorrido[0][documentoTransladar->pasosTotal]) - 1])->sig = documentoTransladar;
                        (area[1][(documentoTransladar->recorrido[0][documentoTransladar->pasosTotal]) - 1]) = documentoTransladar;
                    }else{
                        (area[0][(documentoTransladar->recorrido[0][documentoTransladar->pasosTotal]) - 1]) = documentoTransladar;
                        (area[1][(documentoTransladar->recorrido[0][documentoTransladar->pasosTotal]) - 1]) = documentoTransladar;
                    }

                    area[0][i] = (area[0][i])->sig;

                    documentoTransladar->sig = NULL;

                }else{
                    // AGREGAR AL ARCHIVO DE LOS FINALIZADOS
                    vecTotalDoc[(area[0][i]->id)-1].procesado = 1;
                    vecTotalDoc[(area[0][i]->id)-1].pendiente = 0;
                    vecTotalDoc[(area[0][i]->id)-1].tiempoFinal = time(NULL);

                    documentoEnArea* eliminar;
                    eliminar = area[0][i];
                    area[0][i] = area[0][i]->sig;
                    free(eliminar);

                }

                if(area[0][i] != NULL){
                    area[0][i]->tiempoInicioProceso = time(NULL);
                    vecVerificadorInicio[i] = 1;

                    if(rand()%2){
                        area[0][i]->duracion = area[0][i]->tiempoPromedioPorTipoPorArea[i] + (area[0][i]->tiempoPromedioPorTipoPorArea[i] * area[0][i]->desviacionTipicaPorArea[i]);
                    }else{
                        area[0][i]->duracion = area[0][i]->tiempoPromedioPorTipoPorArea[i] - (area[0][i]->tiempoPromedioPorTipoPorArea[i] * area[0][i]->desviacionTipicaPorArea[i]);
                    }

                }else{
                    vecVerificadorInicio[i] = 0;

                }

            }
        }
    }

    return ;
}
