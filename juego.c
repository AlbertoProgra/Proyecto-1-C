//Bibliotecas a importar dentro del programa
#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/keyboard.h>
#include <allegro5/allegro_primitives.h>
#include <stdbool.h>

//Constantes del programa
#define FPS 30.0
#define CANTMOV 5
#define SCREEN_W 1024
#define SCREEN_H 640

//Enumera los nombres de variable segun un indice que empieza en 0   
enum KEYS{
    UP,   //0
    DOWN, //1
    LEFT, //2
    RIGHT, //3
    SPACE //4
};

//Arreglo creado para efectuar una correspondencia entre KEYS y su posicion correspondiente dentro de teclas[] 
int teclas[5] = {0, 0, 0, 0, 0};

//Struct para el fondo del juego
typedef struct fondo {
    ALLEGRO_BITMAP *fondog; //Imagen a renderizar
} fondo_f;

//Struct para el jugador
typedef struct jugador {
    int x; //Posicion x de la nave
    int y; //Posicion y de la nave
    ALLEGRO_BITMAP *nave; //Imagen a renderizar
} jugador_t;

//Struct para el enemigo
typedef struct bitty {
    int x;
    int y;
    int vida;
    int aux;
    ALLEGRO_BITMAP *bittys;
} enemigo_s;

//Struct para los disparos
typedef struct bullet{
    int x;
    int y;
    int vel_y;
    bool used;
} shoot_b;

//Funcion que ayuda a dibujar el juego
void dibujarJugador(jugador_t *jugador, enemigo_s *bitty, shoot_b *bullet[], fondo_f *fondo) {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_bitmap(fondo->fondog, 0, 0, 0);
    al_draw_bitmap(jugador->nave, jugador->x, jugador->y, 0);
    al_draw_bitmap(bitty->bittys, bitty->x, bitty->y, 0);
    int i;
    for(i=0; i<5; i++){
        if(bullet[i]->used){
            al_draw_filled_circle(bullet[i]->x, bullet[i]->y, 4, al_map_rgb(0, 0, 0));
        }
    }   
   al_flip_display();
}

//Funcion_1_enemigo: controla el movimiento del enemigo
void primeraEq(enemigo_s *bitty){
    if (!bitty->aux){
        if (bitty->x <= (SCREEN_W - 52 - 4.0)){
            bitty->x += 2.0;
        }
        else{
            bitty->aux = 1;
        }   
    }
    else{
        if (bitty->x >= (2.0)){
            bitty->x -= 2.0;
        }
        else{
            bitty->aux = 0;
        }  
    }
}

//Funcion_2_enemigo: controla el movimiento del enemigo_2
void segundaEq(enemigo_s *bitty){
    /*Escriba su codigo aqui*/
}

//Funcion_1_jugador: modifica el movimiento del avion en el eje -y
void moverArriba(jugador_t *jugador) {
    jugador->y -= 12.0;
}

//Funcion_2_jugador: modifica el movimiento del avion en el eje +y
void moverAbajo(jugador_t *jugador) {
    jugador->y += 12.0;
}

//Funcion_3_jugador: modifica el movimiento del avion en el eje +x
void moverDerecha(jugador_t *jugador) {
    jugador->x += 12.0;
    jugador->nave = al_load_bitmap("nave_d.png");
}

//Funcion_3_jugador: modifica el movimiento del avion en el eje -x
void moverIzquierda(jugador_t *jugador) {
    jugador->x -= 12.0;
    jugador->nave = al_load_bitmap("nave_i.png");
}

//Funcion_1_disparo: alinea el disparo con el avion en el eje x
void creaDisparo(shoot_b *bullet, jugador_t *jugador){
    bullet->y = jugador->y;
    bullet->x = jugador->x + 20.5;
    bullet->used = true;
}

//Funcion_2_disparo: modifica el movimiento del disparo en el eje -y
void moverDisparo(shoot_b *bullet){
    if(bullet->y < 0){
        bullet->y = 0;
        bullet->x = 0;
        bullet->used = false;
    }
    else {
        bullet->y -= bullet->vel_y;
    }
}

//Funcion main principal
int main(int argc, char **argv) {
    //Nuestra pantalla
    ALLEGRO_DISPLAY *display = NULL;
    //Con esto podemos manejar eventos
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    //Timer para actulizar eventos
    ALLEGRO_TIMER *timer = NULL;

    al_init_primitives_addon();

    //Tratamos de inicializar allegro
    if(!al_init()) {
        fprintf(stderr, "%s\n", "No se pudo inicializar allegro 5");
        return -1;
    }

    //Creamos un nuevo display de 1024x640 para empezar
    display = al_create_display(SCREEN_W, SCREEN_H);
    //Si no se pudo crear el display al_create_display devuelve false (0)
    if(!display) {
        fprintf(stderr, "%s\n", "No se pudo crear un display");
        return -1;
    }

    //Tratamos de agregar el addon de imagenes de allegro
    if(!al_init_image_addon()) {
        fprintf(stderr, "%s\n", "No se pudo inicializar el addon de imagenes");
        //Tenemos que destruir el display que creamos
        al_destroy_display(display);
        return -1;
    }

    //Tratamos de instalar el teclado en allegro
    if(!al_install_keyboard()) {
        fprintf(stderr, "%s\n", "No se pudo instalar el teclado");
        //Tenemos que destruir el display que creamos
        al_destroy_display(display);
        return -1;
    }

    //Evitamos que se suspenda la computadora mientras esta el juego abierto
    al_inhibit_screensaver(1);
    //Le ponemos un titulo a nuestro display
    al_set_window_title(display, "\t\t\t\t\t\t\t\t\t...........::::::::[ G     A     L     A     G     A ]::::::::...........");
    //Creamos el timer
    timer = al_create_timer(1.0 / FPS);

    //Creamos los eventos del juego
    event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    //Creamos un jugador e inicializamos su posicion en (532.5, 579)
    jugador_t *player = (jugador_t *)malloc(sizeof(jugador_t));
    player->nave = al_load_bitmap("nave.png");
    player->x = SCREEN_W/2 + 20.5;
    player->y = SCREEN_H - 61;

    //Creamos un arreglo de 5 disparos, inicializamos su posicion en (0,0) y llevamos control de cuantos (tenemos y restan) 
    shoot_b *bullet[5];
    int cont_b;
    for (cont_b = 0; cont_b < 5; cont_b ++){
        bullet[cont_b] = (shoot_b *)malloc(sizeof(shoot_b));
        bullet[cont_b]->vel_y = 10;
        bullet[cont_b]->y = 0;
        bullet[cont_b]->x = 0;
        bullet[cont_b]->used = 0;
    }
    cont_b = 0;

    //Creamos un fondo por default se inicializa su posicion en (0,0)
    fondo_f *bg = (fondo_f *)malloc(sizeof(fondo_f));
    bg->fondog = al_load_bitmap("fondo.jpg");

    //Si la imagen de la nave no se pudo cargar
    if(!player->nave) {
        fprintf(stderr, "%s\n", "No se pudo crear un display");
        al_destroy_display(display);
        al_destroy_event_queue(event_queue);
        al_destroy_timer(timer);
        return -1;
    }

    //Creamos un enemigo e inicializamos su posicion en (0,0)
    enemigo_s *malo = (enemigo_s *)malloc(sizeof(enemigo_s));
    malo->bittys = al_load_bitmap("bitty.png");
    malo->x = 0;
    malo->y = 0;
    malo->aux = 0;

    //Si la imagen de la nave no se pudo cargar
    if(!malo->bittys) {
        fprintf(stderr, "%s\n", "No se pudo crear un enemigo");
        al_destroy_display(display);
        al_destroy_event_queue(event_queue);
        al_destroy_timer(timer);
        return -1;
    }

    //Dibujemos el juego por primera vez
    dibujarJugador(player, malo, bullet, bg);

    //srand a un numero que tire el reloj
    srand(time(NULL));

    //Comenzamos el timer
    al_start_timer(timer);

    //Bandera para salir del juego se preciona escape
    int terminar = 0;

    //Una variable que recibe eventos (?)
    ALLEGRO_EVENT ev;

    
    //Loop del juego
    while(!terminar) {
        al_wait_for_event(event_queue, &ev);
        //Si el evento es key_up
        if(ev.type == ALLEGRO_EVENT_KEY_UP) {
            switch(ev.keyboard.keycode) {
                case ALLEGRO_KEY_ESCAPE:
                    terminar = 1;
                break;
                case ALLEGRO_KEY_UP:
                    teclas[UP] = 0;
                    break;
                case ALLEGRO_KEY_DOWN:
                    teclas[DOWN] = 0;
                    break;
                case ALLEGRO_KEY_LEFT:
                    teclas[LEFT] = 0;
                    player->nave = al_load_bitmap("nave.png");
                    break;
                case ALLEGRO_KEY_RIGHT:
                    teclas[RIGHT] = 0;
                    player->nave = al_load_bitmap("nave.png");
                    break;
                case ALLEGRO_KEY_SPACE:
                    teclas[SPACE] = 0;
                    break;
            }
            //Si el evento es key_down
        } else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch(ev.keyboard.keycode) {
                case ALLEGRO_KEY_UP:
                    teclas[UP] = 1;
                    break;
                case ALLEGRO_KEY_DOWN:
                    teclas[DOWN] = 1;
                    break;
                case ALLEGRO_KEY_LEFT:
                    teclas[LEFT] = 1;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    teclas[RIGHT] = 1;
                    break;
                case ALLEGRO_KEY_SPACE:
                    teclas[SPACE] = 1;
                    cont_b ++;
                    if (cont_b <= 5){
                        creaDisparo(bullet[cont_b-1], player);
                    }
                    break;
            }
            //Si el evento es Timer
        } else if(ev.type == ALLEGRO_EVENT_TIMER) {
            if(teclas[UP]){
                if (player->y >= (SCREEN_H/2)){
                    moverArriba(player);
                }
            }
            else if(teclas[DOWN]){
                if (player->y <= (SCREEN_H -61 -4.0)){
                    moverAbajo(player);
                }
            }
            else if(teclas[LEFT]){
                if (player->x >= 12.0){
                    moverIzquierda(player);
                }
            }
            else if(teclas[RIGHT]){
                if (player->x <= (SCREEN_W -51)){
                    moverDerecha(player);
                }
            }
            int i;
            int cont_i = 0;
            for (i = 0; i < 5; i ++){
                if(bullet[i]->used) {
                    moverDisparo(bullet[i]);
                }
                else {
                    cont_i++;
                }
            }
            if(cont_i == 5){
                cont_b = 0;
            }

            i = 0;
            cont_i = 0;
            primeraEq(malo);
        }
  
        //Recargamos el juego (del backBuffer al frontBuffer)
        dibujarJugador(player, malo, bullet, bg);
    }

    //Se Limpia memoria (solo cuando el ciclo while termina)
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_bitmap(player->nave);
    al_destroy_timer(timer);
    free(player);
    free(malo);
    free(bg);
    return 0; //Como costumbre de buen programador
}