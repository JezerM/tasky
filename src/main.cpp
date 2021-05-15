/*** includes ***/
#include "winConf.h"
#include <cstring>
#include <ctype.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <locale.h>

#ifdef _WIN32
#include <windows.h>
#endif

#define MVER "1.0.0"

using namespace std;

void enableRawMode();
void disableRawMode();

void die(const char *c);

void processKey();
void refreshScreen();
void init();
void clear();
void exitAll();
void menu_Task();
void menu_Timer();
void menu_Help();

void help() {
  printf("Tasky - versión %s\n\n", MVER);
  printf("Este es un programa para ver y crear tareas, y poder monitorizar el tiempo, usando Sistemas de control del tiempo, como el Pomodoro.\n"); 
}

/*** init ***/

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING  0x0004
#endif

int main(int argc, char *argv[]) {

  if (argc == 2) {
    std::string arg1 = argv[1];
    if (arg1 == "--help") {
      help();
      return 0;
    }
  }

  #ifdef _WIN32
  HANDLE hStdin = GetStdHandle(STD_OUTPUT_HANDLE); 
  DWORD mode = 0;
  GetConsoleMode(hStdin, &mode);
  mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
  SetConsoleMode(hStdin, mode);
  #endif
  /* Activa el modo alternativo, para no afectar el output anterior a correr el programa */
  write(STDOUT_FILENO, "\e[?1049h", 8);
  write(STDOUT_FILENO, "\e[?1000;1006;1007;1015l", 23);
  clear();

  while (1) {
    printf("\e[H");
    string mag = "\e[1;95m";
    string clLine = "\e[K";
    string naimu = clLine + mag + "Tasky" + "\e[0m\n";
    string desci = clLine + "¡Esto es Tasky! Escribe la opción que desees" + "\n";
    printf("%s", naimu.c_str());
    printf("%s", desci.c_str());
    printf("\e[K\n");
    printf("\e[K\e[1m[1]\e[0m - Salir\n");
    printf("\e[K\e[1m[2]\e[0m - Lista de tareas\n");
    printf("\e[K\e[1m[3]\e[0m - Temporizador\n");
    printf("\e[K\e[1m[4]\e[0m - Ayuda\n");
    int opt;
    printf("\e[K\e[92;1mSelecciona:\e[0m \e[J");

    printf("\n%s\n", conf.statusMessage);

    printf("\e[2A\e[%dC", 12);
    int check = scanf("%d", &opt);
    char c;
    while((c = getchar()) != '\n' && c != EOF) {}
    if (check != 1) {
      strcpy(conf.statusMessage, "\e[91;1mError:\e[0m Ingresa un número");
      continue;
    }

    switch (opt) {
      case 1:
        strcpy(conf.statusMessage, "");
        exitAll();
        break;
      case 2:
        strcpy(conf.statusMessage, "");
        menu_Task();
        break;
      case 3:
        strcpy(conf.statusMessage, "");
        menu_Timer();
        break;
      case 4:
        strcpy(conf.statusMessage, "");
        menu_Help();
        break;
      default:
        strcpy(conf.statusMessage, "\e[93;1mError:\e[0m Número no válido");
        break;
    }
  }

  //disableRawMode();
  /* Desactiva el modo alternativo, regresando el output anterior al programa */
  write(STDOUT_FILENO, "\e[?1049l", 8);
  printf("\e[?25h");
  return 0;
}
