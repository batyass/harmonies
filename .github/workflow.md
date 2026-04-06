Workflow de travail 
Objectif
Travailler à plusieurs sans se gêner, éviter les conflits et garder un projet qui compile.
Organisation des branches
main → version stable du projet
branches de travail → une par personne
Exemple :
clement
neo
yassir
peilin

Comment travailler (étapes)
1. Récupérer le projet
git pull origin main

2. Créer une branche
git checkout -b votrenom

3. Travailler normalement
git add .
git commit -m "feat: ajout de Player"

4. Envoyer sur GitHub
git push origin votrebranche

5. Créer une Pull Request
Aller sur GitHub
Cliquer sur "Compare & pull request"
Décrire rapidement ce que vous avez fait

6. Merge
Un autre membre regarde rapidement
Puis on merge dans main

Règles importantes
Toujours compiler avant de push
Le projet doit toujours compiler.
Faire des petits commits
Ne pas casser le travail des autres
Éviter de modifier les fichiers des autres sans prévenir
Si besoin → en discuter

Communication
Si vous êtes bloqués :
demander rapidement
ne pas rester bloqué 2 jours
En cas de problème Git
Si vous avez un doute :
ne forcez pas 
demandez à quelqu’un
Règle simple pour avancer
Tant que ça compile et que ça marche un minimum → c’est OK
On améliore ensuite.
