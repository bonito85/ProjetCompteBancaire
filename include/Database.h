#ifndef DATABASE_H
#define DATABASE_H
#include "CompteBancaire.h"
#include "sqlite3.h"
#include <string>

using namespace std;

class Database
{

    private:
        sqlite3* db;

    public:
        Database(const string& nomFichier);
        ~Database();

        //Les methodes
        void creerTable();
        void ajouterCompte(const CompteBancaire& compte);
        bool compteExiste(const string& numeroCompte) const;
        CompteBancaire chargerCompte();
        void afficherTousLesComptes();
        void rechercherCompteParId(const string& numeroCompte);

        // Nouvelle méthode pour exporter les informations d'un compte dans un fichier texte
        void exporterCompte(const string& numeroCompte);

         // Méthode pour obtenir l'objet sqlite3* de la base de données
        sqlite3* getDb() const {
        return db;
    }

};

#endif // DATABASE_H
