#include "projectile.h"
#include <iostream>

Projectile::Projectile(int id, Vector2 pos, Vector2 velocity, int damage, float lifetime, int sourceTowerId, int targetEntityId)
    : Entity(id, pos), velocity(velocity), damage(damage), lifetime(lifetime),
    sourceTowerId(sourceTowerId), targetEntityId(targetEntityId) {
}

void Projectile::update(float dt) {
    // Met à jour la position du projectile en fonction de sa vitesse
    // TODO : ajouter un operateur pour vector2
    position += velocity * dt;

    // Décrémente la durée de vie
    lifetime -= dt;

    // Si la durée de vie est écoulée, marque le projectile pour destruction
    if (lifetime <= 0.0f) {
        onDestroy();
    }
    // Sinon, vérifie si le projectile a atteint sa cible (à implémenter selon ta logique de collision)
    // Exemple basique : vérifier la distance avec la cible (si tu as accès à sa position)
    // Cette partie dépend de ton système de gestion des entités et des collisions.
}

void Projectile::onHit(int targetId) {
    // Logique à exécuter quand le projectile touche une cible
    // Par exemple, infliger des dégâts à la cible (si targetId == targetEntityId)
    if (targetId == targetEntityId) {
        std::cout << "Projectile " << id << " a touché la cible " << targetId << " et inflige " << damage << " dégâts !" << std::endl;
        // Ici, tu pourrais appeler une méthode sur la cible pour lui infliger des dégâts
        // Exemple : target.takeDamage(damage);
        onDestroy(); // Détruit le projectile après impact
    }
}

void Projectile::onDestroy() {
    // Logique de destruction du projectile
    std::cout << "Projectile " << id << " détruit (fin de vie ou collision)." << std::endl;
    // Marquer le projectile comme inactif ou le supprimer de la liste des projectiles actifs
    // (cette partie est généralement gérée par le ProjectileSystem)
}
