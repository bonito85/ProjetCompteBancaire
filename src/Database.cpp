#include "Database.h"
#include <iostream>
#include <fstream>
#include <sqlite3.h>  // inclusion de sqlite3

using namespace std;

// Constructeur - Ouvre la base de données et vérifie qu'il n'y a pas d'erreurs
Database::Database(const string& nomFichier) {
    if (sqlite3_open(nomFichier.c_str(), &db) != SQLITE_OK) {
        cerr << "Erreur d'ouverture de la base de données :)" << endl;
        exit(1);
    }
}

// Destructeur - Ferme la base de données
Database::~Database() {
    sqlite3_close(db);
}

// Méthode pour créer une table si elle n'existe pas déjà
void Database::creerTable() {
    string sql = "CREATE TABLE IF NOT EXISTS Comptes ("
                 " Numero TEXT PRIMARY KEY, "
                 " Nom TEXT, "
                 " Solde REAL, "
                 " Verrouille INTEGER );";
    sqlite3_exec(db, sql.c_str(), 0, 0, 0);
}

// Méthode pour ajouter un compte
void Database::ajouterCompte(const CompteBancaire& compte) {
    string sql = "INSERT INTO Comptes (Numero, Nom, Solde, Verrouille) VALUES ('" +
                 compte.getNumeroCompte() + "', '" + compte.getNomClient() + "', " +
                 to_string(compte.getSolde()) + ", " + to_string(compte.isverrouille()) + ");";
    sqlite3_exec(db, sql.c_str(), 0, 0, 0);
}

// Méthode pour afficher tous les comptes
void Database::afficherTousLesComptes() {
    string sql = "SELECT Numero, Nom, Solde FROM Comptes;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        cout << "Numero : " << sqlite3_column_text(stmt, 0)
             << "\nNom : " << sqlite3_column_text(stmt, 1)
             << "\nSolde : " << sqlite3_column_text(stmt, 2) << endl;
    }
    sqlite3_finalize(stmt);
}

// Méthode pour rechercher un compte par son numéro
void Database::rechercherCompteParId(const string& numeroCompte) {
    string sql = "SELECT Numero, Nom, Solde, Verrouille FROM Comptes WHERE Numero = '" + numeroCompte + "';";
    sqlite3_stmt* stmt;

    // Préparer la requête SQL
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) != SQLITE_OK) {
        cerr << "Erreur lors de la préparation de la requête : " << sqlite3_errmsg(db) << endl;
        return;
    }

    // Exécuter la requête et récupérer les résultats
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        // Afficher les résultats
        cout << "Numero : " << sqlite3_column_text(stmt, 0) << endl;
        cout << "Nom : " << sqlite3_column_text(stmt, 1) << endl;
        cout << "Solde : " << sqlite3_column_text(stmt, 2) << endl;
        cout << "Verrouille : " << (sqlite3_column_int(stmt, 3) == 1 ? "Oui" : "Non") << endl;
    } else {
        // Si aucun compte n'a été trouvé
        cout << "Aucun compte trouvé pour le numéro : " << numeroCompte << endl;
    }

    // Finaliser la requête pour libérer les ressources
    sqlite3_finalize(stmt);
}

void Database::exporterCompte(const string& numeroCompte) {
    // Ouvrir un fichier pour l'exportation
    ofstream fichier("compte_" + numeroCompte + ".txt");

    if (!fichier) {
        cout << "Erreur d'ouverture du fichier pour l'exportation." << endl;
        return;
    }

    // Requête SQL pour rechercher un compte par son numéro
    string sql = "SELECT Numero, Nom, Solde, Verrouille FROM Comptes WHERE Numero = ?";
    sqlite3_stmt* stmt;

    // Préparer la requête
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) != SQLITE_OK) {
        cerr << "Erreur de préparation de la requête SQL." << endl;
        return;
    }

    // Lier le numéro de compte à la requête préparée
    sqlite3_bind_text(stmt, 1, numeroCompte.c_str(), -1, SQLITE_STATIC);

    // Exécuter la requête
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        // Extraire les données de la base
        string numero = (char*)sqlite3_column_text(stmt, 0);
        string nom = (char*)sqlite3_column_text(stmt, 1);
        double solde = sqlite3_column_double(stmt, 2);
        bool verrouille = sqlite3_column_int(stmt, 3) == 1;

        // Écrire les données dans le fichier texte
        fichier << "Numero : " << numero << endl;
        fichier << "Nom : " << nom << endl;
        fichier << "Solde : " << solde << " FRCFA" << endl;
        fichier << "Verrouille : " << (verrouille ? "Oui" : "Non") << endl;
    } else {
        cout << "Aucun compte trouvé avec ce numéro." << endl;
    }

    // Finaliser et fermer la requête
    sqlite3_finalize(stmt);

    // Fermer le fichier
    fichier.close();
    cout << "Les informations du compte ont été exportées dans 'compte_" << numeroCompte << ".txt'." << endl;
}
