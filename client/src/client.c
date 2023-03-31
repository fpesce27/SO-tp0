#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	log_info(logger, "Soy un Log");


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	valor = config_get_string_value(config, "CLAVE");

	printf("Corriendo en la ip %s y en el puerto %s\n", ip, puerto);

	log_info(logger, "El valor obtenido es %s", valor);


	/* ---------------- LEER DE CONSOLA ---------------- */

	//leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor, conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;

	nuevo_logger = log_create("tp0.log", "tp0", 1, LOG_LEVEL_INFO);

	if (!nuevo_logger) {
		printf("error en el logger create\n");
		exit(1);
	}

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;

	nuevo_config = config_create("./cliente.config");

	if (!nuevo_config) {
		printf("Error al crear la config\n");
		exit(2);
	}

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	while (true) {
		leido = readline("> ");

		if (!logger || strcmp(leido, "")) {
			log_info(logger, leido);
		} else {
			printf("Fin del ingreso de info\n");
			free(leido);
			break;
		}

	}


}

void paquete(int conexion)
{

	// Creamos el paquete y agregamos el contenido del archivo de log
	char* leido = readline("> ");

    t_paquete* paquete = crear_paquete();

    agregar_a_paquete(paquete, leido, strlen(leido) + 1);

    // Enviamos el paquete al servidor
    enviar_paquete(paquete, conexion);

    // Liberamos la memoria asignada
    free(leido);
    eliminar_paquete(paquete);
}


void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */

	if (logger) {
		log_destroy(logger);
	}

	if (config){
		config_destroy(config);
	}

	if (conexion) {
		close(conexion);
	}
}
