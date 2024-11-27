#include "CompteBancaire.h"
#include <iostream>
#include <fstream>

using namespace std;



CompteBancaire::CompteBancaire(string num, string nom, double soldeInitial)
    : numeroCompte(num), nomclient(nom), solde(soldeInitial), verrouille(false)
{
    transactions.push_back("Compte créé avec un solde initial de " + to_string(soldeInitial));
}

// Methode pour la verification de depôt sur le compte bancaire
void CompteBancaire::depot(double montant) {
    if(verrouille) { // Verifier si le compte est verreouiller
        cout << "Le compte est vérrouillé, opération impossible :)" << endl;
        return;
    }
    solde += montant;
    transactions.push_back("Dépôt de " + to_string(montant) + "\n" + "Votre nouveau solde est de : " + to_string(solde));
}

// Methode pour la veriicaion de retrait sur le compte bancaire
void CompteBancaire::retrait(double montant) {
    if(verrouille) {
        cout << "Le compte est vérrouillé, opération impossible :)" << endl;
        return;
    }
    //Verifier que le montant n'est pas superieur au solde du compte bancaire
    if(montant > solde) {
        cout << "Fonds inssuffisant pour efectuer le retrait (:" << endl;
        //return false;
    }
    solde -= montant;
    transactions.push_back("Retrait de " + to_string(montant) + "\n" + "Nouveau solde : " + to_string(solde));
    //return true;
}
//Methode de verrouillage du compte bancaire
void CompteBancaire::verrouiller() { verrouille = true; }
void CompteBancaire::deverrouiller() { verrouille = false; }

//Methode pour afficher le releve de compte
void CompteBancaire::afficherReleve(const string& cheminFichier) const {
    ofstream fichier(cheminFichier, ios::app);
    fichier <<"**********************************************************";
    fichier <<"*                                                        *";
    fichier <<"*                                                        *";
    fichier <<"*           RELEVE DE COMPTE DU CLIENT                   *";
    fichier <<"*                                                        *";
    fichier <<"*                                                        *";
    fichier <<"**********************************************************";
    fichier << "Rélévé du compte " << numeroCompte << "( " << nomclient << " )" <<endl;
    for(const string& t : transactions) {
        fichier << " - " << t << endl;
    }
    fichier.close();
}

string CompteBancaire::getNumeroCompte() const { return numeroCompte; }
string CompteBancaire::getNomClient() const { return nomclient; }
double CompteBancaire::getSolde() const { return solde; }
bool CompteBancaire::isverrouille() const { return verrouille; }
string CompteBancaire::toString() const { return numeroCompte + ";" + nomclient + ";" + to_string(solde); }





