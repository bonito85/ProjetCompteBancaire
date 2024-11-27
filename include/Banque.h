#ifndef BANQUE_H
#define BANQUE_H

#include "CompteBancaire.h" // Insertion de la classe CompteBancaire
#include <vector>

using namespace std;

//Classe Banque
class Banque
{
    private:
        vector<CompteBancaire> comptes;
    public:
        void ajouterCompte(const CompteBancaire& compte);
        CompteBancaire* rechercherCompte(const string& numero);
        void afficherTousLesComptes();
};

#endif // BANQUE_H
