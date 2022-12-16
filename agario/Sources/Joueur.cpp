
#include <SDL.h>
#include "Joueur.hpp"
#include <iostream>

Joueur::Joueur()
{
    
}

Joueur::Joueur(double x, double y, double taille): taille(taille)
{
    corps.x = x;
    corps.y = y;
    updateVitesse();
}


Joueur::~Joueur(){
}

void
Joueur::changeDirectionVitesse(double nx, double ny){
    double normeN = sqrt(nx*nx+ny*ny);
    double nxx = nx/normeN;
    double nyy = ny/normeN;
    double norme = sqrt(vx*vx+vy*vy);
    vx = nxx*norme;
    vy = nyy*norme;
}


// Regulation de la norme de la vitesse en fonction du la taille.
void
Joueur::updateVitesse()
{
    double norme = sqrt(vx*vx+vy*vy);
    double regulNorme = 25.0/corps.radius * vboost;
    vx = (vx/norme)*regulNorme;
    vy = (vy/norme)*regulNorme;
}

bool Joueur::collision(const Joueur& joueur, double dg)
{
    return isPointInCircle(
        corps.x, corps.y,
        joueur.corps.x, joueur.corps.y,
        corps.radius + dg + joueur.corps.radius);
}

bool Joueur::isPointInCircle(float xa, float ya, float xc, float yc, float r)
{
    return ((xa - xc) * (xa - xc) + (ya - yc) * (ya - yc)) < r * r;
}

void
Joueur::mange(Joueur &joueur)
{
    joueur.estVisible = false;
    corps.radius += (joueur.corps.radius/100.0)*30.0;
    vx -= vx/50.0;
    vy -= vy/50.0;

}

void
Joueur::deplace(){
    corps.x += vx;
    corps.y += vy; 
    if (corps.x + corps.radius > ReglageJeu::LARGEUR_MONDE_JEU)
    {
        corps.x = ReglageJeu::LARGEUR_MONDE_JEU - corps.radius;
        vx *= -1;
    }
    if (corps.y + corps.radius > ReglageJeu::HAUTEUR_MONDE_JEU)
    {
        corps.y = ReglageJeu::HAUTEUR_MONDE_JEU -  corps.radius;
        vy *= -1;
    }
    if (corps.x - corps.radius < 0.0)
    {
        corps.x = corps.radius;
        vx *= -1;
    }
    if (corps.y - corps.radius < 0.0)
    {
        corps.y = corps.radius;
        vy *= -1;
    }
}

void
Joueur::incrementeVitesse(double varX, double varY){
    vx += varX;
    vy += varY;
    if (abs(vx+varX) < vittMax)
    {
        vx += varX;
    }
    if (abs(vy+varY) < vittMax)
    {
        vy += varY;
    }

}

bool
Joueur::peutManger(const Joueur &joueur){
    return corps.radius > joueur.corps.radius;
}