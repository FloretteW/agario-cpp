#include <SDL.h>
#include <iostream>
#include "Jeu.hpp"
#include "ReglageJeu.hpp"
#include <cmath>
#include <cstdlib>

Jeu::Jeu(): joueurPrincipal(ReglageJeu::LARGEUR_FENETRE/2.0, ReglageJeu::HAUTEUR_FENETRE/2.0, 30.0)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    win = SDL_CreateWindow("AgarIO CPP",
                           SDL_WINDOWPOS_CENTERED,
                           SDL_WINDOWPOS_CENTERED,
                           ReglageJeu::LARGEUR_FENETRE, ReglageJeu::HAUTEUR_FENETRE, SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_PRESENTVSYNC);
    joueurPrincipal.r = 255; joueurPrincipal.g = 255;  joueurPrincipal.b = 255;
    joueurPrincipal.corps.radius = 20.0;
    joueurPrincipal.vboost = 2;

    joueurPrincipal.updateVitesse();
    plateau.generationJoueurs(ReglageJeu::NB_PLAYER);
}

Jeu::~Jeu(){
    SDL_DestroyWindow(win);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

bool Jeu::tick()
{
    SDL_SetRenderDrawColor(renderer, 230, 230, 230, 255);
    SDL_RenderClear(renderer);
    SDL_Event e;
    plateau.fenetreXorigin = joueurPrincipal.corps.x - (ReglageJeu::LARGEUR_FENETRE/2);
    plateau.fenetreYorigin = joueurPrincipal.corps.y - (ReglageJeu::HAUTEUR_FENETRE/2);
    for (int i = 0 ; i <=ReglageJeu::HAUTEUR_MONDE_JEU; i++)
    {
      if (i >= plateau.fenetreYorigin &&
          i <= plateau.fenetreYorigin+ReglageJeu::HAUTEUR_FENETRE && i%50==0){
          SDL_SetRenderDrawColor(renderer, 90, 90, 90, 255);
          
          SDL_RenderDrawLine(renderer, 0-plateau.fenetreXorigin, i-plateau.fenetreYorigin,
                             ReglageJeu::LARGEUR_MONDE_JEU-plateau.fenetreXorigin, i-plateau.fenetreYorigin);
      }
    }
    for (int i = 0 ; i <= ReglageJeu::LARGEUR_MONDE_JEU; i++)
    {
      if (i >= plateau.fenetreXorigin &&
          i <= plateau.fenetreXorigin+ReglageJeu::LARGEUR_FENETRE && i%50==0){
          SDL_SetRenderDrawColor(renderer, 90, 90, 90, 255);
          SDL_RenderDrawLine(renderer, i-plateau.fenetreXorigin,-plateau.fenetreYorigin,
                             i-plateau.fenetreXorigin, ReglageJeu::HAUTEUR_MONDE_JEU-plateau.fenetreYorigin);
      }
    }
    while ( SDL_PollEvent(&e))
    {
        switch (e.type)
        {
            case SDL_MOUSEMOTION:
            {
                double px = joueurPrincipal.corps.x, py = joueurPrincipal.corps.y;
                plateau.convertToFenetreCoords(px, py, joueurPrincipal);
                double dx = e.motion.x - px;
                double dy = e.motion.y - py;
                joueurPrincipal.changeDirectionVitesse(dx, dy);
            }
                break;
            case SDL_QUIT:
                return false;
        }
        
    }
    joueurPrincipal.deplace();
    plateau.afficheJoueur(joueurPrincipal,renderer);
    
    for (auto &i: plateau.joueurs){
        if (i.estVisible){
            i.deplace();
        }

        /*double bpx = i.corps.x, bpy = i.corps.y;
        plateau.convertToFenetreCoords(bpx, bpy, i);
        double bdx = 0;
        double bdy = randomize - bpy + 30;
        i.changeDirectionVitesse(bdx, bdy);*/
        
        if (i.collision(joueurPrincipal) && i.estVisible && joueurPrincipal.estVisible){
            if (i.peutManger(joueurPrincipal)){
                i.mange(joueurPrincipal);
            }else{
                joueurPrincipal.mange(i);
            }
        }
        // Fuite des plus petit bot
        if (i.collision(joueurPrincipal, joueurPrincipal.dangerzone) && i.estVisible && joueurPrincipal.estVisible && i.peutManger(joueurPrincipal) == false) {
            //1
            if (i.corps.x <= joueurPrincipal.corps.x && i.corps.y <= joueurPrincipal.corps.y)
            {
                if (i.vx >= 0)
                {
                    i.vx *= -1;
                }
                if (i.vy >= 0)
                {
                    i.vy *= -1;
                }
                
            }
            //2
            if (i.corps.x > joueurPrincipal.corps.x && i.corps.y <= joueurPrincipal.corps.y)
            {
                if (i.vx < 0)
                {
                    i.vx *= -1;
                }
                if (i.vy >= 0)
                {
                    i.vy *= -1;
                }
            }
            //3
            if (i.corps.x > joueurPrincipal.corps.x && i.corps.y > joueurPrincipal.corps.y)
            {
                if (i.vx < 0)
                {
                    i.vx *= -1;
                }
                if (i.vy < 0)
                {
                    i.vy *= -1;
                }

            }
            //4
            if (i.corps.x <= joueurPrincipal.corps.x && i.corps.y > joueurPrincipal.corps.y)
            {
                if (i.vx >= 0)
                {
                    i.vx *= -1;
                }
                if (i.vy < 0)
                {
                    i.vy *= -1;
                }
            }            
        }
        //Attaque des plus gros bot
        if (i.collision(joueurPrincipal, joueurPrincipal.dangerzone) && i.estVisible && joueurPrincipal.estVisible && i.peutManger(joueurPrincipal) == true) {          
            //1
            if (i.corps.x <= joueurPrincipal.corps.x && i.corps.y <= joueurPrincipal.corps.y)
            {
                if (i.vx < 0)
                {
                    i.vx *= -1;
                }
                if (i.vy < 0)
                {
                    i.vy *= -1;
                }

            }
            //2
            if (i.corps.x > joueurPrincipal.corps.x && i.corps.y <= joueurPrincipal.corps.y)
            {
                if (i.vx >= 0)
                {
                    i.vx *= -1;
                }
                if (i.vy < 0)
                {
                    i.vy *= -1;
                }
            }
            //3
            if (i.corps.x > joueurPrincipal.corps.x && i.corps.y > joueurPrincipal.corps.y)
            {
                if (i.vx >= 0)
                {
                    i.vx *= -1;
                }
                if (i.vy >= 0)
                {
                    i.vy *= -1;
                }

            }
            //4
            if (i.corps.x <= joueurPrincipal.corps.x && i.corps.y > joueurPrincipal.corps.y)
            {
                if (i.vx < 0)
                {
                    i.vx *= -1;
                }
                if (i.vy >= 0)
                {
                    i.vy *= -1;
                }
            }
        }
        if(plateau.estJoueurDansFenetre(i)){
            plateau.afficheJoueur(i,renderer);
        }
        if ((rand()%10000) == 0) {
            plateau.ajoutStaticElem(1);
        }
        
    }
    
    SDL_RenderPresent(renderer);
    return joueurPrincipal.estVisible;
}
