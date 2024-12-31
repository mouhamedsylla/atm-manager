# ANTISÈCHE BASES DE DONNÉES 📚

## 1. STRUCTURE DE BASE
### Clés
- **Clé Primaire (PK)**: Identifiant unique d'un enregistrement
  ```sql
  CREATE TABLE users (
    id INTEGER PRIMARY KEY,
    username TEXT UNIQUE
  );
  ```
- **Clé Étrangère (FK)**: Référence une PK d'une autre table
  ```sql
  CREATE TABLE accounts (
    id INTEGER PRIMARY KEY,
    user_id INTEGER,
    FOREIGN KEY (user_id) REFERENCES users(id)
  );
  ```

### Contraintes
- **NOT NULL**: Valeur obligatoire
- **UNIQUE**: Valeur unique dans la table
- **CHECK**: Vérifie une condition
- **DEFAULT**: Valeur par défaut
```sql
CREATE TABLE accounts (
    balance DECIMAL(10,2) NOT NULL DEFAULT 0.00,
    email TEXT UNIQUE,
    age INTEGER CHECK (age >= 18)
);
```

## 2. TRANSACTIONS ACID

### Définition
Transaction = Groupe d'opérations formant une unité logique de travail

### Propriétés ACID
- **Atomicité**: Tout ou rien
- **Cohérence**: Base de données toujours dans un état valide
- **Isolation**: Transactions indépendantes les unes des autres
- **Durabilité**: Modifications permanentes une fois validées

### Structure Transaction
```sql
BEGIN TRANSACTION;
    -- Opérations
    UPDATE accounts SET balance = balance - 100 WHERE id = 1;
    UPDATE accounts SET balance = balance + 100 WHERE id = 2;
    -- Si erreur: ROLLBACK;
COMMIT;
```

## 3. RELATIONS ENTRE TABLES

### Types de Relations
- **One-to-One (1:1)**
  ```sql
  user ─── profile
  ```
- **One-to-Many (1:N)**
  ```sql
  user ─┬─ post
        ├─ post
        └─ post
  ```
- **Many-to-Many (N:M)**
  ```sql
  student ─┬─ course
           ├─ course
  student ─┼─ course
           └─ course
  ```

### Table de Liaison (N:M)
```sql
CREATE TABLE student_courses (
    student_id INTEGER,
    course_id INTEGER,
    PRIMARY KEY (student_id, course_id),
    FOREIGN KEY (student_id) REFERENCES students(id),
    FOREIGN KEY (course_id) REFERENCES courses(id)
);
```

## 4. NORMALISATION

### Formes Normales Principales
- **1NF**: Valeurs atomiques
- **2NF**: 1NF + Dépendances totales
- **3NF**: 2NF + Pas de dépendances transitives

### Exemple
Non normalisé:
```
client (id, nom, commandes[produit, prix, date])
```

Normalisé (3NF):
```sql
clients (id, nom)
commandes (id, client_id, date)
commande_produits (commande_id, produit_id, prix)
produits (id, nom, prix_base)
```

## 5. INDEX ET PERFORMANCE

### Types d'Index
- **B-tree**: Index standard
- **Hash**: Égalité exacte
- **Bitmap**: Faible cardinalité

### Utilisation
```sql
-- Création
CREATE INDEX idx_username ON users(username);

-- Index composé
CREATE INDEX idx_name_email ON users(name, email);
```

## 6. BONNES PRATIQUES

### Sécurité
- Utiliser des requêtes préparées
- Gérer les permissions
- Valider les entrées utilisateur

### Performance
- Indexer les colonnes recherchées
- Optimiser les requêtes
- Utiliser EXPLAIN pour l'analyse

### Maintenance
- Sauvegarder régulièrement
- Mettre à jour les statistiques
- Monitorer les performances

## 7. GESTION DES ERREURS

### Types d'Erreurs
- Violations de contraintes
- Deadlocks
- Timeouts
- Erreurs de connexion

### Gestion
```c
if (sqlite3_exec(db, sql, 0, 0, &err_msg) != SQLITE_OK) {
    fprintf(stderr, "Error: %s\n", err_msg);
    sqlite3_free(err_msg);
    return ERROR_CODE;
}
```
