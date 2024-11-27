#include "Banque.h"
#include <iostream>
using namespace std;

//Methode pour aouter un nouveau compte
void Banque::ajouterCompte(const CompteBancaire& compte) {
    comptes.push_back(compte);
}
 // Methode pour rechercher tous les comptes enregistrés
CompteBancaire* Banque::rechercherCompte(const string& numero) {
    for(CompteBancaire& compte : comptes) {
        if(compte.getNumeroCompte() == numero) return &compte;
    }
    return nullptr;
}

// Méthode pour aficher tous les comptes

void Banque::afficherTousLesComptes() {
    for(const CompteBancaire& compte : comptes) {
        cout << compte.getNumeroCompte() << " - " << compte.getNomClient() << "\n" << "Solde : " << compte.getSolde() << endl;

    }
}
