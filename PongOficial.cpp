#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

#define HEIGHT 30
#define WIDTH 80

void gotoxy(int x, int y) {
    HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD dwPos;
    dwPos.X = x;
    dwPos.Y = y;
    SetConsoleCursorPosition(hcon, dwPos);
}

struct coord {
    int x;
    int y;
};

void limites() {
    for (int i = 0; i < WIDTH; i++) {
        gotoxy(i, 0); printf("%c", 205);
        gotoxy(i, HEIGHT); printf("%c", 205);
    }
    for (int i = 0; i < HEIGHT; i++) {
        gotoxy(0, i); printf("%c", 186);
        gotoxy(WIDTH, i); printf("%c", 186);
    }
    gotoxy(0, 0); printf("%c", 201);
    gotoxy(WIDTH, 0); printf("%c", 187);
    gotoxy(0, HEIGHT); printf("%c", 200);
    gotoxy(WIDTH, HEIGHT); printf("%c", 188);
}

void player(coord pos, int c) {
    switch (c) {
    case 0:
        gotoxy(pos.x, pos.y - 1); printf("%c", 219);
        gotoxy(pos.x, pos.y); printf("%c", 219);
        gotoxy(pos.x, pos.y + 1); printf("%c", 219);
        break;
    case 1:
        gotoxy(pos.x, pos.y - 1); printf(" ");
        gotoxy(pos.x, pos.y); printf(" ");
        gotoxy(pos.x, pos.y + 1); printf(" ");
        break;
    }
}

void mover(coord &pos, char up, char down, bool isPlayer) {
    if (kbhit()) {
        char tecla = getch();
        if (isPlayer) {
            if (tecla == up && pos.y > 2) {
                player(pos, 1);
                pos.y -= 1;
                player(pos, 0);
            }
            if (tecla == down && pos.y < HEIGHT - 2) {
                player(pos, 1);
                pos.y += 1;
                player(pos, 0);
            }
        } else {
            if (tecla == 0 || tecla == 224) {
                tecla = getch();
                if (tecla == up && pos.y > 2) {
                    player(pos, 1);
                    pos.y -= 1;
                    player(pos, 0);
                }
                if (tecla == down && pos.y < HEIGHT - 2) {
                    player(pos, 1);
                    pos.y += 1;
                    player(pos, 0);
                }
            }
        }
    }
}

void red() {
    for (int i = 1; i < HEIGHT; i++) {
        gotoxy(WIDTH / 2, i); printf("%c", 176);
    }
}

void pelota(coord pel, int c) {
    switch (c) {
    case 0:
        gotoxy(pel.x, pel.y); printf("%c", 184);
        break;
    case 1:
        gotoxy(pel.x, pel.y); printf(" ");
        break;
    }
}

bool colision(coord pos, coord pel) {
    if (pos.x == pel.x - 1 || pos.x == pel.x + 1) {
        if ((pos.y - 1) == pel.y || (pos.y + 1) == pel.y || pos.y == pel.y) {
            return true;
        }
    }
    return false;
}

void update(coord &pel, coord &aum, coord pos, coord ene, int &scorePlayer1, int &scorePlayer2) {
    if (pel.y < 2 || pel.y > HEIGHT - 2) {
        aum.y = (-aum.y);
    }
    if (pel.x < 2 || pel.x > WIDTH - 2) {
        pelota(pel, 1);
        if (pel.x < 2) {
            scorePlayer2++;
        } else {
            scorePlayer1++;
        }
        aum.x = (-aum.x);
        pel.x = WIDTH / 2;
        pel.y = HEIGHT / 2;
    }
    if (colision(pos, pel)) aum.x = (-aum.x);
    if (colision(ene, pel)) aum.x = (-aum.x);
}

coord mover_pel(coord &pel, coord aum) {
    pelota(pel, 1);
    pel.x = pel.x + aum.x;
    pel.y = pel.y + aum.y;
    pelota(pel, 0);
    return pel;
}

int main() {
    coord pos = { 10, HEIGHT / 2 };
    coord pel = { WIDTH / 2, HEIGHT / 2 };
    coord aum = { -1, -1 };
    coord ene = { WIDTH - 10, HEIGHT / 2 };

    limites();
    player(pos, 0);
    player(ene, 0);
    int x = 0;
    bool game = true;
    clock_t start = clock();
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    int scorePlayer1 = 0;
    int scorePlayer2 = 0;

    while (game) {
        clock_t end = clock();
        double tiempo_transcurrido = double(end - start) * 1000.0 / CLOCKS_PER_SEC;
        if (tiempo_transcurrido > frameDelay) {
            if (x == 5) {
                red();
                mover_pel(pel, aum);
                x = 0;
            }
            mover(pos, 'w', 's', true);
            mover(ene, 72, 80, true); // 72 y 80 son los códigos de las flechas arriba y abajo
            update(pel, aum, pos, ene, scorePlayer1, scorePlayer2);
            x++;
            start = clock();

            // Mostrar puntajes
            gotoxy(WIDTH / 2 - 5, 1);
            printf("Puntuacion:");
            gotoxy(WIDTH / 2 - 5, 2);
            printf("Jugador 1: %d", scorePlayer1);
            gotoxy(WIDTH / 2 - 5, 3);
            printf("Jugador 2: %d", scorePlayer2);

            // Verificar si un jugador ha ganado (alcanzar 10 puntos)
            if (scorePlayer1 >= 10 || scorePlayer2 >= 10) {
                game = false;
            }
        }
    }

    // Mostrar resultado final
    gotoxy(WIDTH / 2 - 10, HEIGHT / 2);
    if (scorePlayer1 > scorePlayer2) {
        printf("¡Jugador 1 gana!");
    } else if (scorePlayer2 > scorePlayer1) {
        printf("¡Jugador 2 gana!");
    } else {
        printf("¡Empate!");
    }

    gotoxy(WIDTH / 2 - 15, HEIGHT / 2 + 2);
    printf("Presiona cualquier tecla para salir");

    getch();
    return 0;
}

