#include "mb_tcp.h"

#define LOOPBACK_IP "127.0.0.1"

#define MB_DI_1_IP LOOPBACK_IP
#define MB_DI_2_IP LOOPBACK_IP
#define MB_DO_1_IP LOOPBACK_IP
#define MB_DO_2_IP LOOPBACK_IP
#define MB_DO_3_IP LOOPBACK_IP
#define MB_DO_4_IP LOOPBACK_IP
#define MB_DO_5_IP LOOPBACK_IP
#define MB_DI_1_PORT 1500
#define MB_DI_2_PORT 1501
#define MB_DO_1_PORT 1502
#define MB_DO_2_PORT 1503
#define MB_DO_3_PORT 1504
#define MB_DO_4_PORT 1505
#define MB_DO_5_PORT 1506

modbus_mapping_t *Modbus_mapping_new_start_address(
	guint32 start_bits, guint32 nb_bits,
	guint32 start_input_bits, guint32 nb_input_bits,
	guint32 start_registers, guint32 nb_registers,
	guint32 start_input_registers, guint32 nb_input_registers,
	gchar *mb_tcp_ip, gint mb_tcp_port)
{
	modbus_mapping_t *mb_mapping = modbus_mapping_new_start_address(start_bits, nb_bits,
																	start_input_bits, nb_input_bits,
																	start_registers, nb_registers,
																	start_input_registers, nb_input_registers);
	if (mb_mapping == NULL)
	{
		g_mutex_lock(&terminal_mutex);
		g_print("Неудалось выделить Modbus mapping %s:%d\n", mb_tcp_ip, mb_tcp_port);
		g_mutex_unlock(&terminal_mutex);
	}
	else
	{
		return mb_mapping;
	}
}
modbus_t *Modbus_new_tcp(const char *ip_address, int port)
{
	modbus_t *ctx = modbus_new_tcp(ip_address, port);
	if (ctx == NULL)
	{
		g_mutex_lock(&terminal_mutex);
		g_print("Невозможно создать контекст Modbus %s:%d\n", ip_address, port);
		g_mutex_unlock(&terminal_mutex);
	}
}
gint Modbus_tcp_listen(modbus_t *ctx, int nb_connection, gchar *mb_tcp_ip, gint mb_tcp_port)
{

	gint mb_listen_socket = modbus_tcp_listen(ctx, 1);
	if (mb_listen_socket == -1)
	{
		g_mutex_lock(&terminal_mutex);
		g_print("Невозможно слушать Modbus %s:%d\n", mb_tcp_ip, mb_tcp_port);
		g_mutex_unlock(&terminal_mutex);
	}
	return mb_listen_socket;
}
gint Modbus_tcp_accept(modbus_t *ctx, gint s, gchar *mb_tcp_ip, gint mb_tcp_port)
{
	gint mb_assept_socket = modbus_tcp_accept(ctx, &s);
	if (mb_assept_socket == -1)
	{

		g_mutex_lock(&terminal_mutex);
		g_print("Ошибка Modbus(%s:%d) %s\n", mb_tcp_ip, mb_tcp_port, modbus_strerror(errno));
		g_mutex_unlock(&terminal_mutex);
	}
	return mb_assept_socket;
}
//-----------------------------------------------------------------------
// mb_Server: DI_1
gpointer mb_DI_1_func(gpointer data)
{
	gchar *mb_tcp_ip = MB_DI_1_IP;
	gint mb_tcp_port = MB_DI_1_PORT;
	g_mutex_lock(&terminal_mutex);
	g_print("Запущен поток Modbus TCP server %s:%d\n", mb_tcp_ip, mb_tcp_port);
	g_mutex_unlock(&terminal_mutex);

	gint mb_ok = 1;
	gint mb_listen_socket, mb_assept_socket;
	modbus_t *mb_tcp_context;
	modbus_mapping_t *mb_mapping;
	gint query_length;
	guint8 query[MODBUS_TCP_MAX_ADU_LENGTH];
	// Создание Modbus маппинга
	mb_mapping = Modbus_mapping_new_start_address(0, 100, 0, 100, 0, 100, 0, 100, mb_tcp_ip, mb_tcp_port);

	// Инициализация Modbus контекста
	mb_tcp_context = Modbus_new_tcp(mb_tcp_ip, mb_tcp_port);

	// Запуск слушающего сокета Modbus сервера
	mb_listen_socket = Modbus_tcp_listen(mb_tcp_context, 1, mb_tcp_ip, mb_tcp_port);

	g_mutex_lock(&terminal_mutex);
	g_print("Запущен Modbus TCP server %s:%d\n", mb_tcp_ip, mb_tcp_port);
	g_mutex_unlock(&terminal_mutex);

	while (1)
	{
		mb_assept_socket = Modbus_tcp_accept(mb_tcp_context, mb_listen_socket, mb_tcp_ip, mb_tcp_port);
		while (1)
		{
			query_length = modbus_receive(mb_tcp_context, query);
			mb_ok = query_length;
			if (query_length > 0)
			{
				// Обработка запроса и заполнение регистров
				mb_ok = modbus_reply(mb_tcp_context, query, query_length, mb_mapping);
			}
			if (mb_ok == -1)
			{
				g_mutex_lock(&terminal_mutex);
				g_print("Ошибка Modbus(%s:%d) %s\n", mb_tcp_ip, mb_tcp_port, modbus_strerror(errno));
				g_mutex_unlock(&terminal_mutex);
				break; // Выходим из цикла в случае ошибки
			}
		}
		close(mb_assept_socket);
	}
	// Очистка ресурсов

	close(mb_assept_socket);
	close(mb_listen_socket);
	if (mb_mapping)
	{
		modbus_mapping_free(mb_mapping);
	}
	g_mutex_lock(&terminal_mutex);
	g_print("Остановлен поток Modbus TCP server %s:%d\n", mb_tcp_ip, mb_tcp_port);
	g_mutex_unlock(&terminal_mutex);
	g_thread_exit(NULL);
}
//-----------------------------------------------------------------------
// mb_Server: DI_2
gpointer mb_DI_2_func(gpointer data)
{
	gchar *mb_tcp_ip = MB_DI_2_IP;
	gint mb_tcp_port = MB_DI_2_PORT;
	g_mutex_lock(&terminal_mutex);
	g_print("Запущен поток Modbus TCP server %s:%d\n", mb_tcp_ip, mb_tcp_port);
	g_mutex_unlock(&terminal_mutex);

	gint mb_ok = 1;
	gint mb_listen_socket, mb_assept_socket;
	modbus_t *mb_tcp_context;
	modbus_mapping_t *mb_mapping;
	gint query_length;
	guint8 query[MODBUS_TCP_MAX_ADU_LENGTH];
	// Создание Modbus маппинга
	mb_mapping = Modbus_mapping_new_start_address(0, 100, 0, 100, 0, 100, 0, 100, mb_tcp_ip, mb_tcp_port);

	// Инициализация Modbus контекста
	mb_tcp_context = Modbus_new_tcp(mb_tcp_ip, mb_tcp_port);

	// Запуск слушающего сокета Modbus сервера
	mb_listen_socket = Modbus_tcp_listen(mb_tcp_context, 1, mb_tcp_ip, mb_tcp_port);

	g_mutex_lock(&terminal_mutex);
	g_print("Запущен Modbus TCP server %s:%d\n", mb_tcp_ip, mb_tcp_port);
	g_mutex_unlock(&terminal_mutex);

	while (1)
	{
		mb_assept_socket = Modbus_tcp_accept(mb_tcp_context, mb_listen_socket, mb_tcp_ip, mb_tcp_port);
		while (1)
		{
			query_length = modbus_receive(mb_tcp_context, query);
			mb_ok = query_length;
			if (query_length > 0)
			{
				// Обработка запроса и заполнение регистров
				mb_ok = modbus_reply(mb_tcp_context, query, query_length, mb_mapping);
			}
			if (mb_ok == -1)
			{
				g_mutex_lock(&terminal_mutex);
				g_print("Ошибка Modbus(%s:%d) %s\n", mb_tcp_ip, mb_tcp_port, modbus_strerror(errno));
				g_mutex_unlock(&terminal_mutex);
				break; // Выходим из цикла в случае ошибки
			}
		}
		close(mb_assept_socket);
	}
	// Очистка ресурсов

	close(mb_assept_socket);
	close(mb_listen_socket);
	if (mb_mapping)
	{
		modbus_mapping_free(mb_mapping);
	}
	g_mutex_lock(&terminal_mutex);
	g_print("Остановлен поток Modbus TCP server %s:%d\n", mb_tcp_ip, mb_tcp_port);
	g_mutex_unlock(&terminal_mutex);
	g_thread_exit(NULL);
}
//-----------------------------------------------------------------------
// mb_Server: DO_1
gpointer mb_DO_1_func(gpointer data)
{
	gchar *mb_tcp_ip = MB_DO_1_IP;
	gint mb_tcp_port = MB_DO_1_PORT;
	g_mutex_lock(&terminal_mutex);
	g_print("Запущен поток Modbus TCP server %s:%d\n", mb_tcp_ip, mb_tcp_port);
	g_mutex_unlock(&terminal_mutex);

	gint mb_ok = 1;
	gint mb_listen_socket, mb_assept_socket;
	modbus_t *mb_tcp_context;
	modbus_mapping_t *mb_mapping;
	gint query_length;
	guint8 query[MODBUS_TCP_MAX_ADU_LENGTH];
	// Создание Modbus маппинга
	mb_mapping = Modbus_mapping_new_start_address(0, 100, 0, 100, 0, 100, 0, 100, mb_tcp_ip, mb_tcp_port);

	// Инициализация Modbus контекста
	mb_tcp_context = Modbus_new_tcp(mb_tcp_ip, mb_tcp_port);

	// Запуск слушающего сокета Modbus сервера
	mb_listen_socket = Modbus_tcp_listen(mb_tcp_context, 1, mb_tcp_ip, mb_tcp_port);

	g_mutex_lock(&terminal_mutex);
	g_print("Запущен Modbus TCP server %s:%d\n", mb_tcp_ip, mb_tcp_port);
	g_mutex_unlock(&terminal_mutex);

	while (1)
	{
		mb_assept_socket = Modbus_tcp_accept(mb_tcp_context, mb_listen_socket, mb_tcp_ip, mb_tcp_port);
		while (1)
		{
			query_length = modbus_receive(mb_tcp_context, query);
			mb_ok = query_length;
			if (query_length > 0)
			{
				// Обработка запроса и заполнение регистров
				mb_ok = modbus_reply(mb_tcp_context, query, query_length, mb_mapping);
			}
			if (mb_ok == -1)
			{
				g_mutex_lock(&terminal_mutex);
				g_print("Ошибка Modbus(%s:%d) %s\n", mb_tcp_ip, mb_tcp_port, modbus_strerror(errno));
				g_mutex_unlock(&terminal_mutex);
				break; // Выходим из цикла в случае ошибки
			}
		}
		close(mb_assept_socket);
	}
	// Очистка ресурсов

	close(mb_assept_socket);
	close(mb_listen_socket);
	if (mb_mapping)
	{
		modbus_mapping_free(mb_mapping);
	}
	g_mutex_lock(&terminal_mutex);
	g_print("Остановлен поток Modbus TCP server %s:%d\n", mb_tcp_ip, mb_tcp_port);
	g_mutex_unlock(&terminal_mutex);
	g_thread_exit(NULL);
}
//-----------------------------------------------------------------------
// mb_Server: DO_2
gpointer mb_DO_2_func(gpointer data)
{
	gchar *mb_tcp_ip = MB_DO_2_IP;
	gint mb_tcp_port = MB_DO_2_PORT;
	g_mutex_lock(&terminal_mutex);
	g_print("Запущен поток Modbus TCP server %s:%d\n", mb_tcp_ip, mb_tcp_port);
	g_mutex_unlock(&terminal_mutex);

	gint mb_ok = 1;
	gint mb_listen_socket, mb_assept_socket;
	modbus_t *mb_tcp_context;
	modbus_mapping_t *mb_mapping;
	gint query_length;
	guint8 query[MODBUS_TCP_MAX_ADU_LENGTH];
	// Создание Modbus маппинга
	mb_mapping = Modbus_mapping_new_start_address(0, 100, 0, 100, 0, 100, 0, 100, mb_tcp_ip, mb_tcp_port);

	// Инициализация Modbus контекста
	mb_tcp_context = Modbus_new_tcp(mb_tcp_ip, mb_tcp_port);

	// Запуск слушающего сокета Modbus сервера
	mb_listen_socket = Modbus_tcp_listen(mb_tcp_context, 1, mb_tcp_ip, mb_tcp_port);

	g_mutex_lock(&terminal_mutex);
	g_print("Запущен Modbus TCP server %s:%d\n", mb_tcp_ip, mb_tcp_port);
	g_mutex_unlock(&terminal_mutex);

	while (1)
	{
		mb_assept_socket = Modbus_tcp_accept(mb_tcp_context, mb_listen_socket, mb_tcp_ip, mb_tcp_port);
		while (1)
		{
			query_length = modbus_receive(mb_tcp_context, query);
			mb_ok = query_length;
			if (query_length > 0)
			{
				// Обработка запроса и заполнение регистров
				mb_ok = modbus_reply(mb_tcp_context, query, query_length, mb_mapping);
			}
			if (mb_ok == -1)
			{
				g_mutex_lock(&terminal_mutex);
				g_print("Ошибка Modbus(%s:%d) %s\n", mb_tcp_ip, mb_tcp_port, modbus_strerror(errno));
				g_mutex_unlock(&terminal_mutex);
				break; // Выходим из цикла в случае ошибки
			}
		}
		close(mb_assept_socket);
	}
	// Очистка ресурсов

	close(mb_assept_socket);
	close(mb_listen_socket);
	if (mb_mapping)
	{
		modbus_mapping_free(mb_mapping);
	}
	g_mutex_lock(&terminal_mutex);
	g_print("Остановлен поток Modbus TCP server %s:%d\n", mb_tcp_ip, mb_tcp_port);
	g_mutex_unlock(&terminal_mutex);
	g_thread_exit(NULL);
}
//-----------------------------------------------------------------------
// mb_Server: DO_3
gpointer mb_DO_3_func(gpointer data)
{
	gchar *mb_tcp_ip = MB_DO_3_IP;
	gint mb_tcp_port = MB_DO_3_PORT;
	g_mutex_lock(&terminal_mutex);
	g_print("Запущен поток Modbus TCP server %s:%d\n", mb_tcp_ip, mb_tcp_port);
	g_mutex_unlock(&terminal_mutex);

	gint mb_ok = 1;
	gint mb_listen_socket, mb_assept_socket;
	modbus_t *mb_tcp_context;
	modbus_mapping_t *mb_mapping;
	gint query_length;
	guint8 query[MODBUS_TCP_MAX_ADU_LENGTH];
	// Создание Modbus маппинга
	mb_mapping = Modbus_mapping_new_start_address(0, 100, 0, 100, 0, 100, 0, 100, mb_tcp_ip, mb_tcp_port);

	// Инициализация Modbus контекста
	mb_tcp_context = Modbus_new_tcp(mb_tcp_ip, mb_tcp_port);

	// Запуск слушающего сокета Modbus сервера
	mb_listen_socket = Modbus_tcp_listen(mb_tcp_context, 1, mb_tcp_ip, mb_tcp_port);

	g_mutex_lock(&terminal_mutex);
	g_print("Запущен Modbus TCP server %s:%d\n", mb_tcp_ip, mb_tcp_port);
	g_mutex_unlock(&terminal_mutex);

	while (1)
	{
		mb_assept_socket = Modbus_tcp_accept(mb_tcp_context, mb_listen_socket, mb_tcp_ip, mb_tcp_port);
		while (1)
		{
			query_length = modbus_receive(mb_tcp_context, query);
			mb_ok = query_length;
			if (query_length > 0)
			{
				// Обработка запроса и заполнение регистров
				mb_ok = modbus_reply(mb_tcp_context, query, query_length, mb_mapping);
			}
			if (mb_ok == -1)
			{
				g_mutex_lock(&terminal_mutex);
				g_print("Ошибка Modbus(%s:%d) %s\n", mb_tcp_ip, mb_tcp_port, modbus_strerror(errno));
				g_mutex_unlock(&terminal_mutex);
				break; // Выходим из цикла в случае ошибки
			}
		}
		close(mb_assept_socket);
	}
	// Очистка ресурсов

	close(mb_assept_socket);
	close(mb_listen_socket);
	if (mb_mapping)
	{
		modbus_mapping_free(mb_mapping);
	}
	g_mutex_lock(&terminal_mutex);
	g_print("Остановлен поток Modbus TCP server %s:%d\n", mb_tcp_ip, mb_tcp_port);
	g_mutex_unlock(&terminal_mutex);
	g_thread_exit(NULL);
}
//-----------------------------------------------------------------------
// mb_Server: DO_4
gpointer mb_DO_4_func(gpointer data)
{
	gchar *mb_tcp_ip = MB_DO_4_IP;
	gint mb_tcp_port = MB_DO_4_PORT;
	g_mutex_lock(&terminal_mutex);
	g_print("Запущен поток Modbus TCP server %s:%d\n", mb_tcp_ip, mb_tcp_port);
	g_mutex_unlock(&terminal_mutex);

	gint mb_ok = 1;
	gint mb_listen_socket, mb_assept_socket;
	modbus_t *mb_tcp_context;
	modbus_mapping_t *mb_mapping;
	gint query_length;
	guint8 query[MODBUS_TCP_MAX_ADU_LENGTH];
	// Создание Modbus маппинга
	mb_mapping = Modbus_mapping_new_start_address(0, 100, 0, 100, 0, 100, 0, 100, mb_tcp_ip, mb_tcp_port);

	// Инициализация Modbus контекста
	mb_tcp_context = Modbus_new_tcp(mb_tcp_ip, mb_tcp_port);

	// Запуск слушающего сокета Modbus сервера
	mb_listen_socket = Modbus_tcp_listen(mb_tcp_context, 1, mb_tcp_ip, mb_tcp_port);

	g_mutex_lock(&terminal_mutex);
	g_print("Запущен Modbus TCP server %s:%d\n", mb_tcp_ip, mb_tcp_port);
	g_mutex_unlock(&terminal_mutex);

	while (1)
	{
		mb_assept_socket = Modbus_tcp_accept(mb_tcp_context, mb_listen_socket, mb_tcp_ip, mb_tcp_port);
		while (1)
		{
			query_length = modbus_receive(mb_tcp_context, query);
			mb_ok = query_length;
			if (query_length > 0)
			{
				// Обработка запроса и заполнение регистров
				mb_ok = modbus_reply(mb_tcp_context, query, query_length, mb_mapping);
			}
			if (mb_ok == -1)
			{
				g_mutex_lock(&terminal_mutex);
				g_print("Ошибка Modbus(%s:%d) %s\n", mb_tcp_ip, mb_tcp_port, modbus_strerror(errno));
				g_mutex_unlock(&terminal_mutex);
				break; // Выходим из цикла в случае ошибки
			}
		}
		close(mb_assept_socket);
	}
	// Очистка ресурсов

	close(mb_assept_socket);
	close(mb_listen_socket);
	if (mb_mapping)
	{
		modbus_mapping_free(mb_mapping);
	}
	g_mutex_lock(&terminal_mutex);
	g_print("Остановлен поток Modbus TCP server %s:%d\n", mb_tcp_ip, mb_tcp_port);
	g_mutex_unlock(&terminal_mutex);
	g_thread_exit(NULL);
}
//-----------------------------------------------------------------------
// mb_Server: DO_5
gpointer mb_DO_5_func(gpointer data)
{
	gchar *mb_tcp_ip = MB_DO_5_IP;
	gint mb_tcp_port = MB_DO_5_PORT;
	g_mutex_lock(&terminal_mutex);
	g_print("Запущен поток Modbus TCP server %s:%d\n", mb_tcp_ip, mb_tcp_port);
	g_mutex_unlock(&terminal_mutex);

	gint mb_ok = 1;
	gint mb_listen_socket, mb_assept_socket;
	modbus_t *mb_tcp_context;
	modbus_mapping_t *mb_mapping;
	gint query_length;
	guint8 query[MODBUS_TCP_MAX_ADU_LENGTH];
	// Создание Modbus маппинга
	mb_mapping = Modbus_mapping_new_start_address(0, 100, 0, 100, 0, 100, 0, 100, mb_tcp_ip, mb_tcp_port);

	// Инициализация Modbus контекста
	mb_tcp_context = Modbus_new_tcp(mb_tcp_ip, mb_tcp_port);

	// Запуск слушающего сокета Modbus сервера
	mb_listen_socket = Modbus_tcp_listen(mb_tcp_context, 1, mb_tcp_ip, mb_tcp_port);

	g_mutex_lock(&terminal_mutex);
	g_print("Запущен Modbus TCP server %s:%d\n", mb_tcp_ip, mb_tcp_port);
	g_mutex_unlock(&terminal_mutex);

	while (1)
	{
		mb_assept_socket = Modbus_tcp_accept(mb_tcp_context, mb_listen_socket, mb_tcp_ip, mb_tcp_port);
		while (1)
		{
			query_length = modbus_receive(mb_tcp_context, query);
			mb_ok = query_length;
			if (query_length > 0)
			{
				// Обработка запроса и заполнение регистров
				mb_ok = modbus_reply(mb_tcp_context, query, query_length, mb_mapping);
			}
			if (mb_ok == -1)
			{
				g_mutex_lock(&terminal_mutex);
				g_print("Ошибка Modbus(%s:%d) %s\n", mb_tcp_ip, mb_tcp_port, modbus_strerror(errno));
				g_mutex_unlock(&terminal_mutex);
				break; // Выходим из цикла в случае ошибки
			}
		}
		close(mb_assept_socket);
	}
	// Очистка ресурсов

	close(mb_assept_socket);
	close(mb_listen_socket);
	if (mb_mapping)
	{
		modbus_mapping_free(mb_mapping);
	}
	g_mutex_lock(&terminal_mutex);
	g_print("Остановлен поток Modbus TCP server %s:%d\n", mb_tcp_ip, mb_tcp_port);
	g_mutex_unlock(&terminal_mutex);
	g_thread_exit(NULL);
}