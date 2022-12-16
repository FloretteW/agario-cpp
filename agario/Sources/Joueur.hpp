#include <SDL.h>
#include "ReglageJeu.hpp"

#pragma once
class Joueur {
public:
    Joueur();
    Joueur(double x, double y, double taille);
    
    ~Joueur();

    struct Cercle {
        double x, y;
        double radius;
    };
    
    void deplace();
    void incrementeVitesse(double varX, double varY);
    void changeDirectionVitesse(double nx, double ny);
    bool collision(const Joueur &joueur, double dg = 0);
    bool isPointInCircle(float xa, float ya, float xc, float yc, float r);
    void mange(Joueur &joueur);
    bool peutManger(const Joueur &joueur);
    void updateVitesse();
    double taille = 20.0;
    double dangerzone = taille * 4;
    const double vittMax = 2.0;
    bool estVisible = true;
    Uint8 r=255, g=255, b = 255;
    Cercle corps = {ReglageJeu::LARGEUR_FENETRE/2.0,
                      ReglageJeu::HAUTEUR_FENETRE/2.0, taille};
    double vx = 1.111;
    double vy = 1.0;
    double vboost = 1.0;
    

};
