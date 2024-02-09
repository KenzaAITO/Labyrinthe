/*
Author : Dimitri Britan et Vivien Mahut
Comment : Pierrick Ledys
*/
#include "mbed.h"
#include "max7219.h"
#include "platform/mbed_thread.h"
#include <cstdlib>

const uint16_t START = 0x203;
const uint16_t VALIDE = 0x204;
const uint16_t ERREUR = 0x205;

// Déclaration des broches
CAN can1(p30, p29);
Max7219 max7219(p5, p6, p7, p8);
AnalogIn xAxis(p20);
AnalogIn yAxis(p19);
DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);
CANMessage msg;

// Fonction pour éteindre toutes les LEDs
void turn_off_all_leds(uint8_t device_number)
{
    for (int col = 0; col <= 8; col++) {
        max7219.write_digit(device_number, Max7219::MAX7219_DIGIT_0+col, 0x00);  // Utiliser 0x00 pour éteindre toutes les LEDs
    }
}

// Fonction pour allumer toutes les LEDs
void turn_on_all_leds(uint8_t device_number)
{
    for (int col = 0; col <= 8; col++) {
        max7219.write_digit(device_number, Max7219::MAX7219_DIGIT_0+col, 0xFF);  // Utiliser 0x00 pour éteindre toutes les LEDs
    }
}

// initialisation des LEDs
void demarrage(int nbr){
    for (int i = 0; i <= nbr; i++) {
    turn_on_all_leds(1);
    thread_sleep_for(100);
    turn_off_all_leds(1);
    thread_sleep_for(100); 
    }
}



// Fonction pour lire l'état du joystick et des LEDs
uint8_t* joystick() {
    float x = xAxis.read();
    float y = yAxis.read();
    static uint8_t ledStates[4] = {0,0,0,0};

    // Allumer les LEDs en fonction des positions du joystick
    led1 = (x > 0.8 && y > 0.4 && y < 0.8) ? 1 : 0;
    led2 = (x < 0.4 && y > 0.4 && y < 0.8) ? 1 : 0;
    led3 = (y > 0.8 && x > 0.4 && x < 0.8) ? 1 : 0;
    led4 = (y < 0.4 && x > 0.4 && x < 0.8) ? 1 : 0;
    
    // États des LEDs : Droite Gauche Bas Haut
    ledStates[0] = static_cast<uint8_t>(led1);
    ledStates[1] = static_cast<uint8_t>(led2);
    ledStates[2] = static_cast<uint8_t>(led3);
    ledStates[3] = static_cast<uint8_t>(led4);
    return ledStates;
}

// Fonction pour afficher la position du joueur
void display_user_pos(uint8_t device_number, uint16_t * joueur)
{
    max7219.write_digit(device_number, joueur[0], joueur[1]);
    ThisThread::sleep_for(250ms);
    turn_off_all_leds(device_number);
    ThisThread::sleep_for(250ms);
}

// Fonction pour déplacer le joueur
uint16_t* user_move(uint16_t *joueur, uint8_t *deplacement) {
    if (deplacement[0] == 1 && joueur[0] < 8) {
        joueur[0] = joueur[0] + 1; // gauche
    }
    if (deplacement[1] == 1 && joueur[0] > 1) {
        joueur[0] = joueur[0] - 1; // droite
    }
    if (deplacement[2] == 1 && joueur[1] > 1) {
        joueur[1] = joueur[1] >> 1; // bas
    }
    if (deplacement[3] == 1 && joueur[1] < 128) {
        joueur[1] = joueur[1] << 1; // haut
    }

    // Afficher la position du joueur
    uint16_t* tmp = new uint16_t[2];
    tmp[0] = joueur[0]; //colonne
    tmp[1] = joueur[1]; //ligne
    display_user_pos(1, tmp);
    return tmp;
}

// Fonction pour compter le nombre de décalage
int countShifts(int num) {
    int shiftCount = 0;

    while (num != 1) {
        num = (num >> 1);
        shiftCount++;
    }

    return shiftCount;
}


// Fonction principale
int main()
{
    // Initialisation de la matrice de LEDs
    max7219_configuration_t cfg = {
        .device_number = 1,
        .decode_mode = 0,
        .intensity = Max7219::MAX7219_INTENSITY_8,
        .scan_limit = Max7219::MAX7219_SCAN_8
    };
    std::srand(static_cast<unsigned>(time(nullptr)));
    max7219.init_device(cfg);
    max7219.enable_device(1);
    max7219.set_display_test();
    thread_sleep_for(10);
    max7219.clear_display_test();

    //Variables du jeu
    int game_status;
    uint16_t user_deplacement[3];
    uint16_t led_find[2];
    uint16_t user_pos[2];
    bool lab[64];
    int random = std::rand()%2; //génération aléatoire pour choisir le labyrinthe

    bool lab0[64] = {0,1,0,0,0,0,1,0, // A en 1,1 et B en 1,8
                    0,1,0,1,1,0,1,0,
                    0,1,0,1,0,0,1,0,
                    0,1,0,1,0,1,1,0,
                    0,1,0,1,0,1,0,0,
                    0,1,0,1,0,1,0,1,
                    0,1,0,1,0,1,0,1,
                    0,0,0,1,0,0,0,1};

     bool lab1[64] = {0,0,0,1,1,0,0,0, // A en 8,1 et B en 8,8
                    0,1,0,0,1,0,1,1,
                    0,1,1,0,1,0,0,1,
                    0,0,1,0,1,1,0,0,
                    1,0,1,0,0,1,1,0,
                    0,0,1,1,0,1,1,0,
                    0,1,1,1,0,1,1,0,
                    0,1,1,1,0,0,0,0};

    can1.frequency(250000);
    while (1) {
        // Envoi du message CAN
        if (can1.read(msg)) {
            switch(msg.id){
                case START:
                    led1 = *reinterpret_cast<int*>(msg.data);
                    break;
            }
        if(led1==1){
            break;
        } //quitter la lecture pour lancer le jeux
        }
    }

    // Initialisation du labyrinthe
    if(random==0){
    std::copy(std::begin(lab0), std::end(lab0), std::begin(lab));
    led_find[0] = 8; //colonne
    led_find[1] = (1 << 0); //ligne
    
    user_pos[0] = 1; //colonne | de 1 à 8
    user_pos[1] = (1 << 0); //ligne | décalage allant de 0 à 7
    }
    
    
    if(random==1){
        std::copy(std::begin(lab1), std::end(lab1), std::begin(lab));
        led_find[0] = 8; //colonne
        led_find[1] = (1 << 7); //ligne
        
        user_pos[0] = 1; //colonne | de 1 à 8
        user_pos[1] = (1 << 7); //ligne | décalage allant de 0 à 7
    }

    
    demarrage(3);
    while (1) {
            
            // Lire l'état du joystick
            uint8_t* states = joystick();
            display_user_pos(1, led_find);
            uint16_t* user_deplacement = user_move(user_pos, states);
            // Vérifier si le joueur n'a pas touché un mur
            if(lab[user_deplacement[0]-1+(countShifts(user_deplacement[1])*8)]==1){
                game_status = 1;
                can1.write(CANMessage(ERREUR, reinterpret_cast<char*>(game_status),sizeof(game_status)));
                demarrage(1);//faute détécté
                break;
            }
            // Vérifier si le joueur a atteint la position B pour gagner
            if(user_deplacement[0]==led_find[0] && user_deplacement[1]==led_find[1]){
                game_status = 1;
                can1.write(CANMessage(VALIDE, reinterpret_cast<char*>(game_status),sizeof(game_status)));
                demarrage(3);//jeu fini
                break;
            }

    }


    return 0;
}


