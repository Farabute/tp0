#include "client.h"
#include <string.h>
#include "utils.h"



int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/
	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;
	t_paquete* paquete;

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"
	log_info(logger, "Soy un log");

	config = iniciar_config();

	// Usando el config creado previamente
	// Lee las variables de IP, Puerto y Valor
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	valor = config_get_string_value(config, "CLAVE");

	//Loggear valor de config
	log_info(logger, "Leyendo %s", valor);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo porque lo necesitaremos para lo que sigue.

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	handshake(conexion);

	//enviar CLAVE al servirdor
	enviar_mensaje(valor, conexion);

	paquete = crear_paquete();
	//paquete = crear_super_paquete();

	leer_consola(logger, paquete);

	enviar_paquete(paquete, conexion);

	eliminar_paquete(paquete);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;

	if ((nuevo_logger = log_create("tp0.log", "TP0", 1, LOG_LEVEL_INFO)) == NULL){
		printf("No pude crear el logger\n");
		exit(1);
	}

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;

	if((nuevo_config = config_create("tp0.config")) == NULL){
		printf("No pude leer la config\n");
		exit(2);
	}

	return nuevo_config;
}

void leer_consola(t_log* logger, t_paquete* paquete)
{
	char* leido;

	leido = readline(">");

	while(strcmp(leido, "")){
		log_info(logger, leido);
		agregar_a_paquete(paquete, leido, strlen(leido) + 1);
		free(leido);
		leido = readline(">");
	}
}

void paquete(int conexion)
{
	//Ahora toca lo divertido!

	char* leido;
	t_paquete* paquete = crear_super_paquete();

	leido = readline(">");

	while(strcmp(leido, "\0") != 0){
		agregar_a_paquete(paquete, leido, strlen(leido) + 1);
		free(leido);
		leido=readline(">");
	}
	enviar_paquete(paquete, conexion);
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	if(logger != NULL){
		log_destroy(logger);
	}
	if (config != NULL){
		config_destroy(config);
	}

	liberar_conexion(conexion);
	//Y por ultimo, para cerrar, hay que liberar lo que utilizamos (conexion, log y config) con las funciones de las commons y del TP mencionadas en el enunciado
}
