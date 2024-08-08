#include "globals.h"
#include "main.h"
#include "mb_tcp.h"

int main(int argc, char **argv)
{
	// Установка русской локали
	setlocale(LC_ALL, "ru_RU.UTF-8");

	g_mutex_init(&terminal_mutex);

	printf("Программа запущена. Нажмите Ctrl+C для завершения.\n");

	mb_DI_1 = g_thread_new(NULL, mb_DI_1_func, NULL);
	mb_DI_2 = g_thread_new(NULL, mb_DI_2_func, NULL);
	mb_DO_1 = g_thread_new(NULL, mb_DO_1_func, NULL);
	mb_DO_2 = g_thread_new(NULL, mb_DO_2_func, NULL);
	mb_DO_3 = g_thread_new(NULL, mb_DO_3_func, NULL);
	mb_DO_4 = g_thread_new(NULL, mb_DO_4_func, NULL);
	mb_DO_5 = g_thread_new(NULL, mb_DO_5_func, NULL);

	g_thread_join(mb_DI_1);
	g_thread_join(mb_DI_2);
	g_thread_join(mb_DO_1);
	g_thread_join(mb_DO_2);
	g_thread_join(mb_DO_3);
	g_thread_join(mb_DO_4);
	g_thread_join(mb_DO_5);

	g_thread_unref(mb_DI_1);
	g_thread_unref(mb_DI_2);
	g_thread_unref(mb_DO_1);
	g_thread_unref(mb_DO_2);
	g_thread_unref(mb_DO_3);
	g_thread_unref(mb_DO_4);
	g_thread_unref(mb_DO_5);

	g_mutex_clear(&terminal_mutex);

	printf("Программа завершается.\n");
	return 0;
}
