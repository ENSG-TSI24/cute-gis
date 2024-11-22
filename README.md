# GIS Application Project - Branch Dev

### Créer une branche

Pour créer une nouvelle branche et y basculer, utilisez l'une des commandes suivantes :

- Pour le front-end :
```bash
    git checkout -b <devfront/nom_de_fonction>
```

- Pour le back-end
```bash
    git checkout -b <devback/nom_de_fonction>
```

## Méthode : Pousser une branche

### Ajoutez un fichier spécifique ou tous les fichiers à la branche locale :

- Pour un fichier spécifique :
```bash
    git add <file>
```

- Pour tous les fichiers :
```bash
    git add .
```

### Enregistrez les modifications avec un message personnalisé :

```bash
    git commit -m "mon message personnalisé"
```


### Envoyez vos modifications vers la branche distante :

```bash
    git push origin <nom_de_la_branche_actuelle>
```