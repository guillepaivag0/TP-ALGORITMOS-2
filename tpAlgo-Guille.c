#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <math.h>
//ESTRUCTURAS

struct tipos { //PARA LOS TIPOS DE SOLICITUDES O DOCUMENTOS
    int idTipo;
    char nombreTipo[10]; // Act
    int pasos[5]; // Act
    int cantidadDoc; // Act (POISSON)
    float tiempoPromedioPorTipoPorArea[5]; // Act
    float desviacionTipicaPorArea[5]; // Act
};

struct UTA {
    int idTipo;
    char nombreTipo[10]; // Act
    int pasos[5]; // Act
    int cantidadDoc; // Act (POISSON)
    float tiempoPromedioPorTipoPorArea[5]; // Act
    float desviacionTipicaPorArea[5]; // Act
};

struct guardartipos {
    int cantidadTipos;
    int idTipo;
    char nombreTipo[10]; // Act
    int pasos[5]; // Act
    int cantidadDoc; // Act (POISSON)
    float tiempoPromedioPorTipoPorArea[5]; // Act
    float desviacionTipicaPorArea[5]; // Act
};

struct documento {
    int procesado;
    int pendiente;
    int noEntro;
    int tipo;
    int id;
    int recorrido[2][5];
    float tiempoPromedioPorTipoPorArea[5];
    float desviacionTipicaPorArea[5];
    double duracion[5];
    int pasoActual;
    time_t tiempoInicio, tiempoFinal;

};

struct documentoEnArea {
    int tipo;
    int id;
    int recorrido[2][5];
    float tiempoPromedioPorTipoPorArea[5];
    float desviacionTipicaPorArea[5];
    double duracion[5];
    int pasoActual;
    time_t tiempoInicio, tiempoFinal;
    time_t tiempoInicioProceso;

    struct documentoEnArea *ant;
    struct documentoEnArea *sig;
};

struct almacenamientoSimulaciones {
    int idSimulacion;
    int final;
};

struct tipoAlmacenado {
    int idSimulacion;
    int tipo;
    int cantidadXtipo;
    int pendientes;
};

typedef struct almacenamientoSimulaciones almacenamientoSimulaciones;
typedef struct tipoAlmacenado tipoAlmacenado;
typedef struct UTA UTA;
typedef struct tipos tipos;
typedef struct documento documento;
typedef struct documentoEnArea documentoEnArea;

int cantidadTipos;
int cantidadDocumentos;
tipos vecTipos[1];
int idContadorTipo = 0;
double duracionSimulacion = 0;
int auxCantidadDoc = 0;
int cantidadAgregar = 0;
int ultimoIndice = 0;

void addDocumento(documentoEnArea**, documentoEnArea**, documento);
void agregarTiposNuevos(tipos*);
void agregarCantidadDeDocumentos(tipos*);
void cerearVecTipos(tipos*);
void run(tipos*, documentoEnArea* area[2][5]);
void ImprimirReporte(documentoEnArea* area[][5]);
void procesarArea(documentoEnArea* area[2][5], int vecVerificadorInicio[5], documento vecTotalDoc[cantidadDocumentos]);
void imprimirInforme(documento vecTotalDoc[cantidadDocumentos], tipos vecTipos[cantidadTipos]);
void guardarDatosDeSimulacion(tipos vecTipos[cantidadTipos], int contadorPendientes[cantidadTipos]);
void actualizar(tipos vecTipos[cantidadTipos]);
void guardarTipos(tipos vecTipos[cantidadTipos]);
void liberarProcesoBasura(documentoEnArea* *inicio, documentoEnArea* *ultimo);
void historial();

void calcularPoisson(tipos vecTipos[]);
int poissonRandom(double expectedValue);
int Cantidad(int CanMin, int CanProm);

int main() {

    srand(time(NULL));

    int comenzarPrograma = 1;
    int simulacion;
    int modificacion;
    int procesoModificacion;
    int i, j;

    documentoEnArea * area[2][5] = {
        {NULL, NULL, NULL, NULL, NULL},
        {NULL, NULL, NULL, NULL, NULL}
    };
    printf("\nELIJA UNA OPCION:\n2) HISTORIAL\n1) COMENZAR\n0) SALIR\nRespuesta: ");
    scanf("%d", &comenzarPrograma);

    while (comenzarPrograma != 0) {

        if (comenzarPrograma == 1) {

            printf("\n1. SIMULACION NUEVA\n2. SIMULACION ANTERIOR\nRespuesta: ");
            scanf("%d", &simulacion);

            printf("\nINGRESE LA DURACION DE LA SIMULACION EN MINUTOS: ");
            scanf("%lf", &duracionSimulacion);
            duracionSimulacion *= 60;
            if (simulacion == 1) { //EN CASO DE HACER UNA SIMULACION NUEVA DESDE 0
                //vaciamos
                FILE *cerar;
                cerar = fopen("idSimulaciones.txt", "wb");
                fclose(cerar);
                cerar = fopen("tipoAlmacenado.txt", "wb");
                fclose(cerar);

                printf("\nCANTIDAD DE TIPOS: ");
                scanf("%d", &cantidadTipos);
                tipos vecTipos[cantidadTipos];

                agregarTiposNuevos(vecTipos);

                agregarCantidadDeDocumentos(vecTipos);

                run(vecTipos, area);

                guardarTipos(vecTipos);

            } else if (simulacion == 2) { //EN CASO DE HACERCE UNA SIMULACION CON DATOS ANTERIORES

                FILE* simulacionArchivo;

                simulacionArchivo = fopen("idSimulaciones.txt", "rb");
                //CONSULTAMOS SI EXISTEN DATOS ANTERIORES EN LOS ARCHIVOS
                if (simulacionArchivo != NULL) {
                    fclose(simulacionArchivo);
                    int j;
                    //CONSULAMOS LOS DATOS DE LA SIMULACION ANTERIOR, LOS TIPOS
                    UTA ultimoTipo;

                    FILE* cantidadTotalDeTipos;

                    cantidadTotalDeTipos = fopen("cantidadTotalDeTipos.txt", "rb");

                    fread(&cantidadTipos, sizeof (int), 1, cantidadTotalDeTipos);
                    printf("\nLa cantidad recuperada es: |%d|", cantidadTipos);
                    tipos vecTipos[cantidadTipos];
                    fclose(cantidadTotalDeTipos);
                    FILE* ultimosTipos;

                    ultimosTipos = fopen("ultimosTipos.txt", "rb");

                    for (i = 0; i < cantidadTipos; i++) {

                        fread(&ultimoTipo, sizeof (UTA), 1, ultimosTipos);

                        vecTipos[i].idTipo = ultimoTipo.idTipo;
                        for (j = 0; ultimoTipo.nombreTipo[j] != '\0'; j++) {
                            vecTipos[i].nombreTipo[j] = ultimoTipo.nombreTipo[j];
                        }
                        for (j = 0; j < 5; j++) {
                            vecTipos[i].pasos[j] = ultimoTipo.pasos[j];

                        }
                        vecTipos[i].cantidadDoc = ultimoTipo.cantidadDoc;
                        for (j = 0; j < 5; j++) {
                            vecTipos[i].tiempoPromedioPorTipoPorArea[j] = ultimoTipo.tiempoPromedioPorTipoPorArea[j];
                        }
                        for (j = 0; j < 5; j++) {
                            vecTipos[i].desviacionTipicaPorArea[j] = ultimoTipo.desviacionTipicaPorArea[j];
                        }

                    }
                    fclose(ultimosTipos);

                    printf("\nDESEA HACER MODIFICACIONES?\n 1) MODIFICAR\n 0) NO MODIFICAR\nRespuesta: ");
                    scanf("%d", &modificacion);
                    while (modificacion) {

                        actualizar(vecTipos);

                        printf("\n----------------------------------------------------------------\n");
                        printf("\nDESEA HACER MODIFICACIONES?\n 1) MODIFICAR\n 0) NO MODIFICAR\nRespuesta: ");
                        scanf("%d", &modificacion);
                    }

                    int cantidadDefinida;
                    printf("\nLA CANTIDAD DE DOCUMENTOS YA ESTA DEFINIDA? (Observacion: 1-SI, 0-NO-Random)\nRespuesta: ");
                    scanf("%d", &cantidadDefinida);
                    if (cantidadDefinida) {
                        printf("\nLA CANTIDAD DE DOCUMENTOS ES: ");
                        scanf("%d", &cantidadDocumentos);
                    } else {
                        cantidadDocumentos = 100 + rand() % 100;
                    }
                    cerearVecTipos(vecTipos);
                    auxCantidadDoc = cantidadDocumentos;
                    calcularPoisson(vecTipos);


                    run(vecTipos, area);

                    guardarTipos(vecTipos);

                } else {
                    printf("\nNO SE HIZO NI UNA SIMULACION ANTES :(");
                }

            } else {
                printf("\nELEGIR UNA OPCION VALIDA..");
            }

        } else if (comenzarPrograma == 2) {
            FILE *comprobar;
            if ((comprobar = fopen("idSimulaciones.txt", "rb")) != NULL) {
                historial();
            } else {
                printf("\nNO HAY HISTORIAL");
            }
        } else {
            printf("\nHASTA LA PROXIMA");
        }
        printf("\nELIJA UNA OPCION?\n2) HISTORIAL\n1) COMENZAR\n0) SALIR\nRespuesta: ");
        scanf("%d", &comenzarPrograma);

    }

    return 0;
}

void agregarTiposNuevos(tipos vecTipos[]) {
    int verificadorDePasoDeArea[5];
    int i, j, k;
    int auxPaso;
    int tiempoTipoPorAreaAux;
    float desviacionTipicaPorAreaAux;


    //TIPO
    for (i = 0; i < cantidadTipos; i++) {

        idContadorTipo++;

        vecTipos[i].idTipo = idContadorTipo;


        printf("\nLEER NOMBRE DEL TIPO: ");
        scanf("%s", vecTipos[i].nombreTipo);

        verificadorDePasoDeArea[0] = 0;
        verificadorDePasoDeArea[1] = 0;
        verificadorDePasoDeArea[2] = 0;
        verificadorDePasoDeArea[3] = 0;
        verificadorDePasoDeArea[4] = 0;

        // AREAS
        printf("\nINGRESE LAS AREAS POR LA QUE PASARA EL TIPO %d: \n(Observacion: Colocar 0 para finalizar)\n", i + 1);
        for (j = 0; j < 5; j++) {
            do {
                printf("\nSU PASO %d SERA: ", j + 1);
                scanf("%d", &auxPaso);
                if (auxPaso == 0) {
                    vecTipos[i].pasos[j] = auxPaso;
                    break;
                }
            } while (auxPaso < 0 || auxPaso >= 6 || verificadorDePasoDeArea[auxPaso - 1] == 1);

            if (auxPaso == 0) {
                for (k = j + 1; k < 5; k++) {
                    vecTipos[i].pasos[k] = 0;
                }
                break;
            }

            do {
                printf("\nSU TIEMPO EN SEGUNDOS EN EL AREA %d PARA EL TIPO %d ES: ", auxPaso, i + 1);
                scanf("%d", &tiempoTipoPorAreaAux);
            } while (tiempoTipoPorAreaAux <= 0 || tiempoTipoPorAreaAux >= 36000);

            do {
                printf("\nSU DESVIACION TIPICA EN EL AREA %d POR EL TIPO %d ES: ", auxPaso, i + 1);
                scanf("%f", &desviacionTipicaPorAreaAux);
            } while (desviacionTipicaPorAreaAux < 0.00 || desviacionTipicaPorAreaAux > 1.00);

            if (auxPaso > 0 && auxPaso < 6) {
                vecTipos[i].pasos[j] = auxPaso;
                vecTipos[i].tiempoPromedioPorTipoPorArea[j] = tiempoTipoPorAreaAux;
                vecTipos[i].desviacionTipicaPorArea[j] = desviacionTipicaPorAreaAux;
                verificadorDePasoDeArea[auxPaso - 1] = 1;
            }

        }

    }

    return;
}
//SOLICITAMOS EN CASO DE UNA NUEVA SIMULACION LA CANTIDAD DE DOCUMENTO Y LA CANTIDAD POR TIPOS

void agregarCantidadDeDocumentos(tipos vecTipos[]) {
    int i, cantidadDefinida, agregarManualmente, auxCantidadDocumentoTipo;
    printf("\nLA CANTIDAD DE DOCUMENTOS YA ESTA DEFINIDA? (Observacion: 1-SI, 0-NO-Random)\nRespuesta: ");
    scanf("%d", &cantidadDefinida);
    if (cantidadDefinida) {
        printf("\nLA CANTIDAD DE DOCUMENTOS ES: ");
        scanf("%d", &cantidadDocumentos);
    } else {
        cantidadDocumentos = 100 + rand() % 100;
    }

    int auxCantidadDocumentos = cantidadDocumentos;
    printf("\nDESEA AGREGAR MANUALMENTE LAS CANTIDADES POR TIPO? (Observacion: 1-SI, 0-NO-Random)\nRespuesta: ");
    scanf("%d", &agregarManualmente);
    printf("\n");
    if (agregarManualmente) {
        while (auxCantidadDocumentos) {
            cerearVecTipos(vecTipos);
            auxCantidadDocumentos = cantidadDocumentos;
            for (i = 0; i < cantidadTipos; i++) {
                printf("\nLE SOBRA %d", auxCantidadDocumentos);

                printf("\nCUANTOS DOCUMENTOS TIENE EL TIPO %d? ", i + 1);
                scanf("%d", &auxCantidadDocumentoTipo);
                while (auxCantidadDocumentoTipo > auxCantidadDocumentos || auxCantidadDocumentoTipo < 0) {
                    printf("\nAGREGUE CORRECTAMENTE POR FAVOR");
                    printf("\nCUANTOS DOCUMENTOS TIENE EL TIPO %d? ", i + 1);
                    scanf("%d", &auxCantidadDocumentoTipo);
                }
                vecTipos[i].cantidadDoc = auxCantidadDocumentoTipo;

                auxCantidadDocumentos -= vecTipos[i].cantidadDoc;
                if (auxCantidadDocumentos == 0) {
                    printf("\nYA SELECCIONO TODO! NO SOBRA MAS :)");
                    break;
                }
            }
            if (auxCantidadDocumentos) {
                printf("\nVUELVA A INGRESAR TODO, NO SE USO LA TOTALIDAD DE DOCUMENTOS DISPONIBLES :(");
            }
        }
    } else {
        auxCantidadDocumentos = cantidadDocumentos;
        cerearVecTipos(vecTipos);
        for (i = 0; i < cantidadTipos - 1; i++) {
            printf("\nLE SOBRA %d", auxCantidadDocumentos);
            auxCantidadDocumentoTipo = rand() % auxCantidadDocumentos;
            printf("\nSE ELIGIO PARA EL TIPO %d UNA CANTIDAD DE %d", i + 1, auxCantidadDocumentoTipo);
            auxCantidadDocumentos -= auxCantidadDocumentoTipo;
            vecTipos[i].cantidadDoc = auxCantidadDocumentoTipo;
            if (auxCantidadDocumentos == 0) {
                printf("\nLE SOBRA %d", auxCantidadDocumentos);
                break;
            }
        }
        printf("\nLE SOBRA %d", auxCantidadDocumentos);
        vecTipos[i].cantidadDoc = auxCantidadDocumentos;
        printf("\nSE ELIGIO PARA EL TIPO %d UNA CANTIDAD DE %d", i + 1, auxCantidadDocumentos);
        auxCantidadDocumentos -= auxCantidadDocumentos;
        printf("\nLE SOBRA %d", auxCantidadDocumentos);
        printf("\nYA SELECCIONO TODO! NO SOBRA MAS :)");
    }

    return;
}
/////////////////////////////////////////////////////////////////////////////////////////////////
//FUNCION QUE CONTIENE EL PROCESO DE SIMULACION

void run(tipos vecTipos[], documentoEnArea* area[2][5]) {

    if (area[0][0] != NULL) {
        liberarProcesoBasura(&area[0][0], &area[1][0]);
    }
    if (area[0][1] != NULL) {
        liberarProcesoBasura(&area[0][1], &area[1][1]);
    }
    if (area[0][2] != NULL) {
        liberarProcesoBasura(&area[0][2], &area[1][2]);
    }
    if (area[0][3] != NULL) {
        liberarProcesoBasura(&area[0][3], &area[1][3]);
    }
    if (area[0][4] != NULL) {
        liberarProcesoBasura(&area[0][4], &area[1][4]);
    }

    int i, j;

    int cantDoc[cantidadTipos];
    for (i = 0; i < cantidadTipos; i++) {
        cantDoc[i] = vecTipos[i].cantidadDoc;
    }
    int areaInicioProceso;
    documento vecTotalDoc[200];

    int auxTipo;
    int procesando[5];
    int intervaloActualizacion;


    printf("\nINGRESE EL INTERVALO DE ACTUALIZACION: ");
    scanf("%d", &intervaloActualizacion);

    // AGREGA TODO EN EL ARREGLO DE DOCUMENTOS PARA PORCESARSE EN LAS AREAS
    for (i = 0; i < cantidadDocumentos; i++) {
        auxTipo = rand() % cantidadTipos;

        while (cantDoc[auxTipo] == 0) {
            auxTipo = rand() % cantidadTipos;
        }
        vecTotalDoc[i].procesado = 0;
        vecTotalDoc[i].pendiente = 0;
        vecTotalDoc[i].noEntro = 1;
        vecTotalDoc[i].tipo = auxTipo + 1;

        vecTotalDoc[i].id = i + 1;

        vecTotalDoc[i].pasoActual = 0;
        for (j = 0; j < 5; j++) {
            vecTotalDoc[i].recorrido[0][j] = vecTipos[vecTotalDoc[i].tipo - 1].pasos[j];

            vecTotalDoc[i].recorrido[1][j] = 0;
            vecTotalDoc[i].tiempoPromedioPorTipoPorArea[j] = vecTipos[vecTotalDoc[i].tipo - 1].tiempoPromedioPorTipoPorArea[j];

            vecTotalDoc[i].desviacionTipicaPorArea[j] = vecTipos[vecTotalDoc[i].tipo - 1].desviacionTipicaPorArea[j];

        }

        cantDoc[auxTipo] -= 1;
    }


    for (i = 0; i < 5; i++) {
        procesando[i] = 0;
    }

    ////////////////////////EMPIEZA SIMULACION///////////////////////////////////////////////////////////

    const time_t HoraInicioPrograma = time(NULL);
    time_t HoraMovimientoPrograma = time(NULL);
    time_t ultimoAgregado = time(NULL);

    int tiempoEsperaAdd = 0;
    int numeroDeDocumento = 0;

    int tiempoEsperaImp = 0;


    //WHILE RELOJ DE LA SIMULACION
    while (difftime(HoraMovimientoPrograma, HoraInicioPrograma) < duracionSimulacion) {
        HoraMovimientoPrograma = time(NULL);

        // PROCESO: AGREGAR EN AREA 
        if (difftime(HoraMovimientoPrograma, ultimoAgregado) == tiempoEsperaAdd) {

            if (numeroDeDocumento < cantidadDocumentos) {
                printf("\n\nINGRESO EL DOCUMENTO NRO: |%d| DE TIPO: |%d|", numeroDeDocumento + 1, vecTotalDoc[numeroDeDocumento].tipo);
                areaInicioProceso = vecTotalDoc[numeroDeDocumento].recorrido[0][0];
                vecTotalDoc[numeroDeDocumento].tiempoInicio = time(NULL);
                vecTotalDoc[numeroDeDocumento].procesado = 0;
                vecTotalDoc[numeroDeDocumento].pendiente = 1;
                vecTotalDoc[numeroDeDocumento].noEntro = 0;

                addDocumento(&area[0][areaInicioProceso - 1], &area[1][areaInicioProceso - 1], vecTotalDoc[numeroDeDocumento]);
                numeroDeDocumento++;
                ultimoAgregado = vecTotalDoc[numeroDeDocumento - 1].tiempoInicio;
            }

            tiempoEsperaAdd = rand() % 5;
        }

        // PROCESO: PROCESAR LAS AREAS
        procesarArea(area, procesando, vecTotalDoc);

        //IMRPIMIR INFORME PERIODICO DURANTE SIMULACION
        if (difftime(HoraMovimientoPrograma, HoraInicioPrograma) == tiempoEsperaImp) {

            ImprimirReporte(area);

            tiempoEsperaImp = tiempoEsperaImp + intervaloActualizacion;
        }

    }
    //INFORME FINAL DE SIMULACION
    imprimirInforme(vecTotalDoc, vecTipos);

    return;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////

void cerearVecTipos(tipos* vecTipos) {
    for (int i = 0; i < cantidadTipos; i++) {
        vecTipos[i].cantidadDoc = 0;
    }

    return;
}
//FUNCION QUE INTRODUCE UN DOCUMENTO A LAS COLAS DE LAS AREAS

void addDocumento(documentoEnArea* *inicio, documentoEnArea* *ultimo, documento miDocumento) {

    miDocumento.procesado = 0;
    miDocumento.pendiente = 1;
    miDocumento.noEntro = 0;

    int i;
    documentoEnArea* nuevoDocumentoEnArea;

    nuevoDocumentoEnArea = (documentoEnArea*) malloc(sizeof (documentoEnArea));

    ////////////////////// AGREGAR DATOS //////////////////////////////q6
    nuevoDocumentoEnArea->tipo = miDocumento.tipo;
    nuevoDocumentoEnArea->id = miDocumento.id;
    nuevoDocumentoEnArea->pasoActual = 0;
    for (i = 0; i < 5; i++) {
        nuevoDocumentoEnArea->recorrido[0][i] = miDocumento.recorrido[0][i];

        nuevoDocumentoEnArea->tiempoPromedioPorTipoPorArea[i] = miDocumento.tiempoPromedioPorTipoPorArea[i];
        nuevoDocumentoEnArea->desviacionTipicaPorArea[i] = miDocumento.desviacionTipicaPorArea[i];

        if (rand() % 2) {
            nuevoDocumentoEnArea->duracion[i] = nuevoDocumentoEnArea->tiempoPromedioPorTipoPorArea[i] + (nuevoDocumentoEnArea->tiempoPromedioPorTipoPorArea[i] * nuevoDocumentoEnArea->desviacionTipicaPorArea[i]);
        } else {
            nuevoDocumentoEnArea->duracion[i] = nuevoDocumentoEnArea->tiempoPromedioPorTipoPorArea[i] - (nuevoDocumentoEnArea->tiempoPromedioPorTipoPorArea[i] * nuevoDocumentoEnArea->desviacionTipicaPorArea[i]);
        }

    }
    nuevoDocumentoEnArea->tiempoInicio = miDocumento.tiempoInicio;
    ///////////////////////////////////////////////////////////////////

    if (*inicio == NULL) {
        nuevoDocumentoEnArea->ant = NULL;
        nuevoDocumentoEnArea->sig = NULL;
        *inicio = nuevoDocumentoEnArea;
        *ultimo = nuevoDocumentoEnArea;
    } else {
        nuevoDocumentoEnArea->ant = *ultimo;
        nuevoDocumentoEnArea->sig = NULL;
        (*ultimo)->sig = nuevoDocumentoEnArea;
        *ultimo = nuevoDocumentoEnArea;
    }


    return;
}
//FUNCION DONDE SE PROCESA CADA AREA, SE VAN MOVIENDO LOS DOCUMENTOS A SUS SIGUIENTES AREAS O SE LIBERAN

void procesarArea(documentoEnArea* area[2][5], int procesando[5], documento vecTotalDoc[cantidadDocumentos]) {

    int i;
    documentoEnArea* documentoTransladar;

    // PARA CADA AREA
    for (i = 0; i < 5; i++) {

        if (area[0][i] != NULL) {

            if (procesando[i] == 0) {
                procesando[i] = 1;
                area[0][i]->tiempoInicioProceso = time(NULL);

            }

            if (difftime(time(NULL), area[0][i]->tiempoInicioProceso) >= area[0][i]->duracion[ area[0][i]->pasoActual ]) {

                area[0][i]->pasoActual++;

                area[0][i]->recorrido[1][ (area[0][i]->pasoActual) - 1 ] = 1;
                vecTotalDoc[(area[0][i]->id) - 1].recorrido[1][ (area[0][i]->pasoActual) - 1 ] = 1;

                // PROCESO DE CAMBIO
                if (area[0][i]->pasoActual < 5) {
                    // PREGUNTAMOS SI EXISTE MAS RECORRIDOS
                    if (area[0][i]->recorrido[0][area[0][i]->pasoActual] != 0) {

                        // PROCESO DE CAMBIO DE DOCUMENTO DE UNA AREA A OTRA
                        documentoTransladar = area[0][i];

                        if ((area[0][ (documentoTransladar->recorrido[0][documentoTransladar->pasoActual]) - 1 ]) != NULL) {
                            (area[1][ (documentoTransladar->recorrido[0][documentoTransladar->pasoActual]) - 1 ])->sig = documentoTransladar;
                            (area[1][ (documentoTransladar->recorrido[0][documentoTransladar->pasoActual]) - 1 ]) = documentoTransladar;

                        } else {
                            (area[0][ (documentoTransladar->recorrido[0][documentoTransladar->pasoActual]) - 1 ]) = documentoTransladar;
                            (area[1][ (documentoTransladar->recorrido[0][documentoTransladar->pasoActual]) - 1 ]) = documentoTransladar;

                        }

                        area[0][i] = (area[0][i])->sig;

                        if (area[0][i] != NULL) {
                            area[0][i]->tiempoInicioProceso = time(NULL);
                            procesando[i] = 1;

                        } else {
                            procesando[i] = 0;
                        }

                        documentoTransladar->sig = NULL;

                    } else {

                        // AGREGAR AL ARCHIVO DE LOS FINALIZADOS
                        vecTotalDoc[(area[0][i]->id) - 1].procesado = 1;
                        vecTotalDoc[(area[0][i]->id) - 1].pendiente = 0;
                        vecTotalDoc[(area[0][i]->id) - 1].noEntro = 0;
                        vecTotalDoc[(area[0][i]->id) - 1].tiempoFinal = time(NULL);

                        documentoEnArea* eliminar;
                        eliminar = area[0][i];
                        area[0][i] = (area[0][i])->sig;

                        if (area[0][i] != NULL) {
                            area[0][i]->tiempoInicioProceso = time(NULL);
                            procesando[i] = 1;

                        } else {
                            procesando[i] = 0;
                        }

                        free(eliminar);


                    }
                } else {
                    // AGREGAR AL ARCHIVO DE LOS FINALIZADOS
                    vecTotalDoc[(area[0][i]->id) - 1].procesado = 1;
                    vecTotalDoc[(area[0][i]->id) - 1].pendiente = 0;
                    vecTotalDoc[(area[0][i]->id) - 1].noEntro = 0;
                    vecTotalDoc[(area[0][i]->id) - 1].tiempoFinal = time(NULL);

                    documentoEnArea* eliminar;
                    eliminar = area[0][i];
                    area[0][i] = (area[0][i])->sig;

                    if (area[0][i] != NULL) {
                        area[0][i]->tiempoInicioProceso = time(NULL);
                        procesando[i] = 1;

                    } else {
                        procesando[i] = 0;
                    }

                    free(eliminar);
                }
            }

        }
    }

    return;
}
//CADA X MOMENTO DURANTE LA SIMULACION, IMPRIMIMOS POR AREA TODOS LOS DOCUMENTOS ENCOLADOS EN ESE INSTANTE

void ImprimirReporte(documentoEnArea* area[2][5]) {
    int i, j;
    printf("\n");
    printf("\n");
    documentoEnArea *aux1 = area[0][0];
    documentoEnArea *aux2 = area[0][1];
    documentoEnArea *aux3 = area[0][2];
    documentoEnArea *aux4 = area[0][3];
    documentoEnArea *aux5 = area[0][4];
    printf("\n--------------\t--------------\t--------------\t--------------\t--------------"
            "\n|            |\t|            |\t|            |\t|            |\t|            |"
            "\n|   AREA 1   |\t|   AREA 2   |\t|   AREA 3   |\t|   AREA 4   |\t|   AREA 5   |"
            "\n|            |\t|            |\t|            |\t|            |\t|            |"
            "\n--------------\t--------------\t--------------\t--------------\t--------------");
    while (aux1 != NULL || aux5 != NULL || aux2 != NULL || aux3 != NULL || aux4 != NULL) {

        printf("\n");
        for (j = 1; j <= 2; j++) {
            for (i = 1; i <= 5; i++) {

                if (i == 1) {
                    if (aux1 != NULL) {
                        printf("      ||      \t");
                    } else {
                        printf("              \t");
                    }
                }
                if (i == 2) {
                    if (aux2 != NULL) {
                        printf("      ||      \t");
                    } else {
                        printf("              \t");
                    }
                }
                if (i == 3) {
                    if (aux3 != NULL) {
                        printf("      ||      \t");
                    } else {
                        printf("              \t");
                    }
                }
                if (i == 4) {
                    if (aux4 != NULL) {
                        printf("      ||      \t");
                    } else {
                        printf("              \t");
                    }
                }
                if (i == 5) {
                    if (aux5 != NULL) {
                        printf("      ||      \t");
                    } else {
                        printf("              \t");
                    }
                }
            }
            printf("\n");
        }
        for (i = 1; i <= 5; i++) {
            if (i == 1) {
                if (aux1 != NULL) {
                    printf(" ------------ \t");
                } else {
                    printf("              \t");
                }
            }
            if (i == 2) {
                if (aux2 != NULL) {
                    printf(" ------------ \t");
                } else {
                    printf("              \t");
                }
            }
            if (i == 3) {
                if (aux3 != NULL) {
                    printf(" ------------ \t");
                } else {
                    printf("              \t");
                }
            }
            if (i == 4) {
                if (aux4 != NULL) {
                    printf(" ------------ \t");
                } else {
                    printf("              \t");
                }
            }
            if (i == 5) {
                if (aux5 != NULL) {
                    printf(" ------------ \t");
                } else {
                    printf("              \t");
                }
            }
        }
        printf("\n");
        for (i = 1; i <= 5; i++) {
            if (i == 1) {
                if (aux1 != NULL) {
                    printf(" |   ID %2d  | \t", aux1->id);
                } else {
                    printf("              \t");
                }
            }
            if (i == 2) {
                if (aux2 != NULL) {
                    printf(" |   ID %2d  | \t", aux2->id);
                } else {
                    printf("              \t");
                }
            }
            if (i == 3) {
                if (aux3 != NULL) {
                    printf(" |   ID %2d  | \t", aux3->id);
                } else {
                    printf("              \t");
                }
            }
            if (i == 4) {
                if (aux4 != NULL) {
                    printf(" |   ID %2d  | \t", aux4->id);
                } else {
                    printf("              \t");
                }
            }
            if (i == 5) {
                if (aux5 != NULL) {
                    printf(" |   ID %2d  | \t", aux5->id);
                } else {
                    printf("              \t");
                }
            }
        }
        printf("\n");
        for (i = 1; i <= 5; i++) {
            if (i == 1) {
                if (aux1 != NULL) {
                    printf(" |  TIPO %2d | \t", aux1->tipo);
                } else {
                    printf("              \t");
                }
            }
            if (i == 2) {
                if (aux2 != NULL) {
                    printf(" |  TIPO %2d | \t", aux2->tipo);
                } else {
                    printf("              \t");
                }
            }
            if (i == 3) {
                if (aux3 != NULL) {
                    printf(" |  TIPO %2d | \t", aux3->tipo);
                } else {
                    printf("              \t");
                }
            }
            if (i == 4) {
                if (aux4 != NULL) {
                    printf(" |  TIPO %2d | \t", aux4->tipo);
                } else {
                    printf("              \t");
                }
            }
            if (i == 5) {
                if (aux5 != NULL) {
                    printf(" |  TIPO %2d | \t", aux5->tipo);
                } else {
                    printf("              \t");
                }
            }
        }
        printf("\n");
        for (i = 1; i <= 5; i++) {
            if (i == 1) {
                if (aux1 != NULL) {
                    printf(" |  PASO %2d | \t", aux1->pasoActual + 1);
                } else {
                    printf("              \t");
                }
            }
            if (i == 2) {
                if (aux2 != NULL) {
                    printf(" |  PASO %2d | \t", aux2->pasoActual + 1);
                } else {
                    printf("              \t");
                }
            }
            if (i == 3) {
                if (aux3 != NULL) {
                    printf(" |  PASO %2d | \t", aux3->pasoActual + 1);
                } else {
                    printf("              \t");
                }
            }
            if (i == 4) {
                if (aux4 != NULL) {
                    printf(" |  PASO %2d | \t", aux4->pasoActual + 1);
                } else {
                    printf("              \t");
                }
            }
            if (i == 5) {
                if (aux5 != NULL) {
                    printf(" |  PASO %2d | \t", aux5->pasoActual + 1);
                } else {
                    printf("              \t");
                }
            }
        }
        printf("\n");
        for (i = 1; i <= 5; i++) {
            if (i == 1) {
                if (aux1 != NULL) {
                    printf(" ------------ \t");
                    aux1 = aux1->sig;
                } else {
                    printf("              \t");
                }
            }
            if (i == 2) {
                if (aux2 != NULL) {
                    printf(" ------------ \t");
                    aux2 = aux2->sig;
                } else {
                    printf("              \t");
                }
            }
            if (i == 3) {
                if (aux3 != NULL) {
                    printf(" ------------ \t");
                    aux3 = aux3->sig;
                } else {
                    printf("              \t");
                }
            }
            if (i == 4) {
                if (aux4 != NULL) {
                    printf(" ------------ \t");
                    aux4 = aux4->sig;
                } else {
                    printf("              \t");
                }
            }
            if (i == 5) {
                if (aux5 != NULL) {
                    printf(" ------------ \t");
                    aux5 = aux5->sig;
                } else {
                    printf("              \t");
                }
            }
        }
        

    }


    return;
}
//GENERMOS EL INFORME FINAL DE LA SIMULACION

void imprimirInforme(documento vecTotalDoc[cantidadDocumentos], tipos vecTipos[cantidadTipos]) {
    int i, j, k, pos;
    int areaImpCant1,areaImpCant2,areaImpCant3,areaImpCant4,areaImpCant5;
    int contadorPendientes[cantidadTipos];
    int contadorProcesados[cantidadTipos];
    int contadorNoEntro[cantidadTipos];
    
    char hora1Text[9], hora2Text[9];
    char textAux[25];

    for (i = 0; i < cantidadTipos; i++) {
        contadorPendientes[i] = 0;
        contadorProcesados[i] = 0;
        contadorNoEntro[i] = 0;
    }
    printf("\n------------------------------------------------------\n");
    printf("\nGENERANDO INFORME");
    printf("\n------------------------------------------------------\n");
    printf("SIMULACION NRO: |%d| (ARREGLAR)\n", 1);
    //IMPRIMIR PROCESADAS
    printf("\n------------------------------------------------------\n");
    printf("\nPROCESADAS COMPLETAMENTE: ");


    for (i = 0; i < cantidadDocumentos; i++) {
        if (vecTotalDoc[i].procesado) {

            strcpy(textAux, ctime(&(vecTotalDoc[i].tiempoInicio)));
            pos = 0;
            for(k = 11; k < 19; k++){
                hora1Text[pos] = textAux[k];
                pos++;
            }
            hora1Text[pos] = '\0';

            strcpy(textAux, ctime(&(vecTotalDoc[i].tiempoFinal)));
            pos = 0;
            for(k = 11; k < 19; k++){
                hora2Text[pos] = textAux[k];
                pos++;
            }
            hora2Text[pos] = '\0';


            printf("\n\n---------------------------------------------------");
            printf("\n|   Numero de solicitud: %d                         |", vecTotalDoc[i].id);
            printf("\n|   Tipo de solicitud: %d                           |", vecTotalDoc[i].tipo);
            printf("\n|   Hora de recepcion: %s                    |", hora1Text);
            printf("\n|   Hora de conclusion: %s                   |", hora2Text);
            printf("\n|   Duracion: |%.2lf| segundos                      |", difftime(vecTotalDoc[i].tiempoFinal, vecTotalDoc[i].tiempoInicio));
            printf("\n---------------------------------------------------");


            contadorProcesados[vecTotalDoc[i].tipo - 1]++;
        } else if (vecTotalDoc[i].pendiente) {
            contadorPendientes[vecTotalDoc[i].tipo - 1]++;
        } else {
            contadorNoEntro[vecTotalDoc[i].tipo - 1]++;
        }

    }
    printf("\n------------------------------------------------------\n");

    
/*
    //IMPRIMIR POR AREA
    printf("\n------------------------------------------------------\n");
    for (i = 0; i < 5; i++) {

        printf("\nPROCESADAS COMPLETAMENTE EN EL AREA %d : ", i + 1);
        for (j = 0; j < cantidadTipos; j++) {
            if (vecTipos[j].pasos[0] == i + 1 || vecTipos[j].pasos[1] == i + 1 || vecTipos[j].pasos[2] == i + 1 || vecTipos[j].pasos[3] == i + 1 || vecTipos[j].pasos[4] == i + 1) {
                printf("\nTIPO: %d", j + 1);
                printf("\nCantidad procesadas: |%d|", contadorProcesados[j]);
                printf("\nCantidad pendientes: |%d|", contadorPendientes[j]);
                printf("\nCantidad sin entrar en area: |%d|", contadorNoEntro[j]);
                printf("\n");
            }
        }
        printf("\n------------------------------------------------------\n");
    }
    printf("\n------------------------------------------------------\n");
*/


    printf("\n\n");

    for(i = -1; i < cantidadTipos; i++)
    {   
        if(i == -1){
            printf("\n--------------\t----------------\t----------------\t----------------\t----------------\t----------------");
            printf("\n|       AREAS|\t|              |\t|              |\t|              |\t|              |\t|              |");
            printf("\n|            |\t|   AREA  %3d  |\t|   AREA  %3d  |\t|   AREA  %3d  |\t|   AREA  %3d  |\t|   AREA  %3d  |",1,2,3,4,5);
            printf("\n|TIPOS       |\t|              |\t|              |\t|              |\t|              |\t|              |");
            printf("\n--------------\t----------------\t----------------\t----------------\t----------------\t----------------");
        }else if(i > -1){
            if(vecTipos[i+1].pasos[0]){
                //areaImpCant1 = ;
            }
            printf("(ARREGLAR)\n");

             printf("\n--------------\t----------------\t----------------\t----------------\t----------------\t----------------");
                printf("\n|            |\t|PROCESADAS %3d|\t|PROCESADAS %3d|\t|PROCESADAS %3d|\t|PROCESADAS %3d|\t|PROCESADAS %3d|", contadorProcesados[i],contadorProcesados[i],contadorProcesados[i],contadorProcesados[i],contadorProcesados[i]);
                printf("\n|  TIPO %2d   |\t|PENDIENTES %3d|\t|PENDIENTES %3d|\t|PENDIENTES %3d|\t|PENDIENTES %3d|\t|PENDIENTES %3d|", i+1, contadorPendientes[i],contadorPendientes[i],contadorPendientes[i],contadorPendientes[i],contadorPendientes[i]);
                printf("\n|            |\t|NO ENTRO   %3d|\t|NO ENTRO   %3d|\t|NO ENTRO   %3d|\t|NO ENTRO   %3d|\t|NO ENTRO   %3d|", contadorNoEntro[i],contadorNoEntro[i],contadorNoEntro[i],contadorNoEntro[i],contadorNoEntro[i]);
                printf("\n--------------\t----------------\t----------------\t----------------\t----------------\t----------------");
                printf("\n");
                printf("\n");
                printf("\n");
        }

    }

    guardarDatosDeSimulacion(vecTipos, contadorPendientes);

    return;
}
//GUARDAMOS EN DOS ARCHIVO LOS RESULTADOS DE LA SIMULACION
//UN ARCHIVO DE INDICE, QUE SON LAS ID DE LAS SIMULACIONES
//UN ARCHIVO DE LOS RESULTADOS DE LA SIMULACION

void guardarDatosDeSimulacion(tipos vecTipos[cantidadTipos], int contadorPendientes[cantidadTipos]) {
    int i;


    almacenamientoSimulaciones archivoID = {0, 0};
    tipoAlmacenado tipoGuardar;

    FILE* simulacionArchivo;

    simulacionArchivo = fopen("idSimulaciones.txt", "rb");

    if (simulacionArchivo != NULL) {
        fclose(simulacionArchivo);

        simulacionArchivo = fopen("idSimulaciones.txt", "rb");
        //Buscamos la ultima simulacion guardada
        while (!feof(simulacionArchivo)) {
            fread(&archivoID, sizeof ( almacenamientoSimulaciones), 1, simulacionArchivo);
        }
        printf("\nLa id anterior es: |%d| \nLa cantidad anterior: |%d|", archivoID.idSimulacion, archivoID.final);
        fclose(simulacionArchivo);
        simulacionArchivo = fopen("idSimulaciones.txt", "ab");
        //una vez hallada la ultima simulacion aniadida, aumentamos id
        archivoID.idSimulacion++;
        archivoID.final += cantidadTipos;

    } else {

        simulacionArchivo = fopen("idSimulaciones.txt", "wb");
        archivoID.idSimulacion = 1;
        archivoID.final = cantidadTipos;

    }

    fwrite(&archivoID, sizeof ( almacenamientoSimulaciones), 1, simulacionArchivo);
    fclose(simulacionArchivo);
    //segundo archivo, guardamos datos
    simulacionArchivo = fopen("tipoAlmacenado.txt", "ab");

    for (i = 0; i < cantidadTipos; i++) {
        tipoGuardar.idSimulacion = archivoID.idSimulacion;
        tipoGuardar.tipo = i + 1;
        tipoGuardar.cantidadXtipo = vecTipos[i].cantidadDoc;
        tipoGuardar.pendientes = contadorPendientes[i];
        fwrite(&tipoGuardar, sizeof ( tipoAlmacenado), 1, simulacionArchivo);
    }

    fclose(simulacionArchivo);



    return;
}
//GENERADOR POISSON PARA LOS TIPOS EXISTENTES YA ALMACENADOS

void calcularPoisson(tipos vecTipos[]) {
    srand(time(NULL));
    int CanP, horas;
    int numero, CanM;
    int i;
    int n;
    int contadorTipo = 0;
    int acumCantTipo = 0;
    int promedioTipo = 0;
    tipoAlmacenado archivo;

    FILE* simulacionArchivo;
    simulacionArchivo = fopen("tipoAlmacenado.txt", "rb");

    //buscar apariciones por tipo

    for (i = 0; i < cantidadTipos; i++) {
        fread(&archivo, sizeof (tipoAlmacenado), 1, simulacionArchivo);
        while (!feof(simulacionArchivo)) {
            if (archivo.tipo == i + 1) {
                contadorTipo++;
                acumCantTipo += archivo.cantidadXtipo;
                ultimoIndice = i;
            }
            fread(&archivo, sizeof ( tipoAlmacenado), 1, simulacionArchivo);
        }
        if (contadorTipo != 0) {
            promedioTipo = acumCantTipo / contadorTipo;
            n = Cantidad(duracionSimulacion / 60, promedioTipo);
            if (i + 1 == cantidadTipos) {
                n = n + auxCantidadDoc;
            }
            vecTipos[i].cantidadDoc = n;
            printf("\nLa cantidad para el tipo %d es: |%d|", i + 1, vecTipos[i].cantidadDoc);
            contadorTipo = 0;
            acumCantTipo = 0;
            promedioTipo = 0;
            fseek(simulacionArchivo, 0 * sizeof (tipoAlmacenado), SEEK_SET);
        }
    }
    fclose(simulacionArchivo);

    return;
}

int Cantidad(int CanMin, int CanProm) {
    double lambda = CanProm * CanMin / 2;
    int numazar = poissonRandom(lambda);
    return numazar;
}
//FUNCION GENERADORA DE LA CANTIDADES DE LOS TIPOS ANTERIORES

int poissonRandom(double expectedValue) {
    int auxanterior = auxCantidadDoc;
    int n = 0; //counter of iteration
    double limit;
    double x; //pseudo random number
    limit = exp(-expectedValue);
    do {
        n = 0;
        auxCantidadDoc = auxanterior;
        x = (rand() % 101);
        x /= 100;
        while (x > limit) {
            n++;
            x = (rand() % 101);
            x /= 100;
        }
        auxanterior = auxCantidadDoc;
        auxCantidadDoc -= n;
    } while (auxCantidadDoc < 0);

    return n;
}
//SI SE DESEA ACTUALIZAR YA UN TIPO QUE SE TIENE ALMACENADO

void actualizar(tipos vecTipos[cantidadTipos]) {

    int i;
    int opcion, modificacionIdTipo;

    int volver = 0;
    int verificadorDePasoDeArea[5];

    printf("\nELIJA SU OPCION DE MODIFICACION: \n");
    printf("\t1-NOMBRE\n\t2-PASOS\n\t3-TIEMPO PROMEDIO\n\t4-DESVIACION TIPICA");
    printf("\nRespuesta: ");
    scanf("%d", &opcion);

    printf("\nELIJA LA ID A MODIFICAR: ");
    scanf("%d", &modificacionIdTipo);

    if (opcion == 1) {
        printf("\nNOMBRE ACTUAL: %s", vecTipos[modificacionIdTipo - 1].nombreTipo);

        vecTipos[modificacionIdTipo - 1].nombreTipo[0] = '\0';

        printf("\nNOMBRE NUEVO: ");
        scanf(" %s", vecTipos[modificacionIdTipo - 1].nombreTipo);
    }

    if (opcion == 2) {
        verificadorDePasoDeArea[0] = 0;
        verificadorDePasoDeArea[1] = 0;
        verificadorDePasoDeArea[2] = 0;
        verificadorDePasoDeArea[3] = 0;
        verificadorDePasoDeArea[4] = 0;

        printf("\nINGRESE LOS PASOS NUEVOS: ");
        printf("\nINGRESE 0 PARA QUE NO EXISTA PASO QUE DAR: \n");
        for (i = 0; i < 5; i++) {

            if (vecTipos[modificacionIdTipo - 1].pasos[i] == 0) {
                printf("\nNO HAY PASOS..");
            } else {
                printf("\nSU PASO ACTUAL ES: %d", vecTipos[modificacionIdTipo - 1].pasos[i]);
            }
            do {
                volver = 0;
                printf("\nSU PASO A MODIFICAR ES: ");
                scanf("%d", &vecTipos[modificacionIdTipo - 1].pasos[i]);

                if (verificadorDePasoDeArea[vecTipos[modificacionIdTipo - 1].pasos[i] - 1] == 1) {
                    volver = 1;
                }
            } while ((vecTipos[modificacionIdTipo - 1].pasos[i] < 0 || vecTipos[modificacionIdTipo - 1].pasos[i] > 5 || volver == 1) && vecTipos[modificacionIdTipo - 1].pasos[i] != 0);

            verificadorDePasoDeArea[vecTipos[modificacionIdTipo - 1].pasos[i] - 1] = 1;

            if (!vecTipos[modificacionIdTipo - 1].pasos[i]) {
                break;
            }
        }
    }

    if (opcion == 3) {
        printf("\nINGRESE EL TIEMPO PROMEDIO NUEVO: \n");
        for (i = 0; i < 5; i++) {
            if (vecTipos[modificacionIdTipo - 1].pasos[i] == 0) {
                break;
            } else {
                printf("\nSU TIEMPO PROMEDIO ACTUAL ES: %f", vecTipos[modificacionIdTipo - 1].tiempoPromedioPorTipoPorArea[i]);
                do {
                    printf("\nNUEVO TIEMPO PROMEDIO: ");
                    scanf("%f", &vecTipos[modificacionIdTipo - 1].tiempoPromedioPorTipoPorArea[i]);
                } while (vecTipos[modificacionIdTipo - 1].tiempoPromedioPorTipoPorArea[i] <= 0 || vecTipos[modificacionIdTipo - 1].tiempoPromedioPorTipoPorArea[i] >= 36000);
            }
        }
    }

    if (opcion == 4) {
        printf("\nINGRESE LA DESVIACION TIPICA: \n");
        for (i = 0; i < 5; i++) {
            if (vecTipos[modificacionIdTipo - 1].pasos[i] == 0) {
                break;
            } else {
                printf("\nSU PASO ACTUAL ES: %f", vecTipos[modificacionIdTipo - 1].desviacionTipicaPorArea[i]);
                do {
                    printf("\nNUEVA DESVIACION TIPICA: ");
                    scanf("%f", &vecTipos[modificacionIdTipo - 1].desviacionTipicaPorArea[i]);
                } while (vecTipos[modificacionIdTipo - 1].desviacionTipicaPorArea[i] < 0.00 || vecTipos[modificacionIdTipo - 1].desviacionTipicaPorArea[i] >= 1.00);
            }
        }
    }
    return;
}
//GUARDA EN UN ARCHIVO EL ULTIMO VECTOR DE TIPOS PARA LUEGO UTILIZARLO EN OTRA SIMULACION

void guardarTipos(tipos vecTipos[cantidadTipos]) {

    UTA ultimoTipo;
    int i, j;

    FILE* cantidadTotalDeTipos;
    FILE* ultimosTipos;
    cantidadTotalDeTipos = fopen("cantidadTotalDeTipos.txt", "wb");


    fwrite(&cantidadTipos, sizeof (int), 1, cantidadTotalDeTipos);
    fclose(cantidadTotalDeTipos);

    ultimosTipos = fopen("ultimosTipos.txt", "wb");

    for (i = 0; i < cantidadTipos; i++) {
        ultimoTipo.idTipo = vecTipos[i].idTipo;
        for (j = 0; vecTipos[i].nombreTipo[j] != '\0'; j++) {
            ultimoTipo.nombreTipo[j] = vecTipos[i].nombreTipo[j];
        }
        for (j = 0; j < 5; j++) {
            if (vecTipos[i].pasos[j] == 1 || vecTipos[i].pasos[j] == 2 || vecTipos[i].pasos[j] == 3 || vecTipos[i].pasos[j] == 4 || vecTipos[i].pasos[j] == 5) {
                ultimoTipo.pasos[j] = vecTipos[i].pasos[j];
            } else {
                ultimoTipo.pasos[j] = 0;
            }
        }
        ultimoTipo.cantidadDoc = vecTipos[i].cantidadDoc;
        for (j = 0; j < 5; j++) {
            ultimoTipo.tiempoPromedioPorTipoPorArea[j] = vecTipos[i].tiempoPromedioPorTipoPorArea[j];
        }
        for (j = 0; j < 5; j++) {
            ultimoTipo.desviacionTipicaPorArea[j] = vecTipos[i].desviacionTipicaPorArea[j];
        }

        fwrite(&ultimoTipo, sizeof (UTA), 1, ultimosTipos);
    }
    fclose(ultimosTipos);

    return;
}
//FUNCION PARA LIMPIAR LA MATRIZ DE AREAS

void liberarProcesoBasura(documentoEnArea* *inicio, documentoEnArea* *ultimo) {

    documentoEnArea* eliminar;

    while (*inicio != *ultimo) {
        eliminar = *ultimo;
        *ultimo = (*ultimo)->ant;
        (*ultimo)->sig = NULL;
        free(eliminar);
    }
    eliminar = *inicio;
    *inicio = NULL;
    *ultimo = NULL;
    free(eliminar);

    return;
}

void historial() {
    int i = 1, j;
    almacenamientoSimulaciones archivoID;
    tipoAlmacenado tipoGuardar;

    FILE* simulacionArchivo, *simulacionArchivo2;

    simulacionArchivo = fopen("idSimulaciones.txt", "rb");

    fread(&archivoID, sizeof ( almacenamientoSimulaciones), 1, simulacionArchivo);
    while (!feof(simulacionArchivo)) {

        fread(&archivoID, sizeof ( almacenamientoSimulaciones), 1, simulacionArchivo);
    }
    fclose(simulacionArchivo);


    simulacionArchivo2 = fopen("tipoAlmacenado.txt", "rb");
    printf("\n\n\t\tSIMULACION | %d |", i);


    fread(&tipoGuardar, sizeof ( tipoAlmacenado), 1, simulacionArchivo2);
    while (!feof(simulacionArchivo2)) {
        if (tipoGuardar.idSimulacion == i) {
            printf("\nTipo:|%d| Cantidad: |%d| Pendientes: |%d|", tipoGuardar.tipo, tipoGuardar.cantidadXtipo, tipoGuardar.pendientes);
        } else {
            i++;
            printf("\n\n\t\tSIMULACION | %d |", i);
            printf("\nTipo:|%d| Cantidad: |%d| Pendientes: |%d|", tipoGuardar.tipo, tipoGuardar.cantidadXtipo, tipoGuardar.pendientes);
        }
        fread(&tipoGuardar, sizeof ( tipoAlmacenado), 1, simulacionArchivo2);
    }

    fclose(simulacionArchivo2);

    return;

}

