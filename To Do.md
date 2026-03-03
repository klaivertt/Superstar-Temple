# ENGINE PROJECT

## Fonctions CallBack
#### Collisions
    Ajouter un callback quand une collision trigger pour pouvoir utiliser une autre fonction, faire un std::vector de pointeur de fonction pour les appeler à ce moment
### Ray Cast
    ### Ray Cast
    But de la fonctionnalité
    - Permettre d'interroger le monde avec un rayon pour récupérer le premier objet touché ou tous les objets le long du rayon.
## User Interfaces
#### Widget (Full Parent)
    Va être la base de l'UI il aura toutes les fonctions de bases pour pouvoir accueillir l'interface
#### Canvas Panel
    Si ok, ajouter ceci comme dans Unreal qui aura comment enfant plusieurs Widgets avec des Anchors pour pouvoir mieux les placer avec toutes les tailles de fenêtres
#### Text
    Déjà créée mais doit être modifié pour le widget :)
#### Progress Bar
    Une progress bar, assez basique
#### Horizontal/Vertical box ?
    Des containers qui permetteraient de faire des boxs qui contiendraient des widgets et de bien les placer
#### Bouton
    Pas plus à dire, avoir plusieurs sprites (Hovered, Unhovered, Clicked), qui pourra avoir un Text en enfant

## Animations
    Implémenter le module d'animation déjà existant dans le moteur, le rendre enfant d'un Sprite (SEULEMENT)
    Aussi rajouter des fonctions pour des animations simple (Bounce, Rotate, Slide, MoveTo)
    