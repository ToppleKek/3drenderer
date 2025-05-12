/*
    Ichigo! A simple, from scratch, minimal dependency game engine for 2D side scrolling games.

    Entity functions.

    Author:      Braeden Hong
    Last edited: 2025/03/12
*/

#include "entity.hpp"
#include "ichigo.hpp"

// TODO: @heap
Bana::Array<Ichigo::Entity> Ichigo::Internal::entities;
Bana::Array<Ichigo::EntityID> Ichigo::Internal::entity_ids_in_draw_order;

// TODO: If the entity id's index is 0, that means that entity has been killed and that spot can hold a new entity.
//       Should it really be like this? Maybe there should be an 'is_alive' flag on the entity?

Ichigo::Entity *Ichigo::spawn_entity() {
    // Search for an open entity slot
    for (u32 i = 1; i < Internal::entities.size; ++i) {
        if (Internal::entities[i].id.index == 0) {
            Ichigo::EntityID new_id = {Internal::entities[i].id.generation + 1, i};
            std::memset(&Internal::entities[i], 0, sizeof(Ichigo::Entity));
            Internal::entities[i].id = new_id;
            Ichigo::Internal::entity_ids_in_draw_order.insert(new_id, 0);

            return &Internal::entities[i];
        }
    }

    // If no slots are available, append the entity to the end of the entity list
    Ichigo::Entity ret{};
    ret.id = {0, (u32) Internal::entities.size};
    Internal::entities.append(ret);
    Ichigo::Internal::entity_ids_in_draw_order.insert(ret.id, 0);
    return &Internal::entities[Internal::entities.size - 1];
}

Ichigo::Entity *Ichigo::get_entity(Ichigo::EntityID id) {
    if (id.index < 1 || id.index >= Internal::entities.size)
        return nullptr;

    if (Internal::entities[id.index].id.index < 1 || Internal::entities[id.index].id.generation != id.generation)
        return nullptr;

    return &Internal::entities[id.index];
}

void Ichigo::change_entity_draw_layer(Ichigo::EntityID id, i8 new_layer) {
    Entity *e = get_entity(id);
    if (e) change_entity_draw_layer(e, new_layer);
}

void Ichigo::change_entity_draw_layer(Ichigo::Entity *entity, i8 new_layer) {
    Ichigo::Internal::entity_ids_in_draw_order.remove(Ichigo::Internal::entity_ids_in_draw_order.index_of(entity->id));

    entity->draw_layer = new_layer;
    for (isize i = 0; i < Ichigo::Internal::entity_ids_in_draw_order.size; ++i) {
        Entity *other = get_entity(Ichigo::Internal::entity_ids_in_draw_order[i]);
        if (other->draw_layer > entity->draw_layer) {
            Ichigo::Internal::entity_ids_in_draw_order.insert(entity->id, i == 0 ? 0 : i - 1);
            return;
        }
    }

    Ichigo::Internal::entity_ids_in_draw_order.append(entity->id);
}

// You can mark an entity to be killed by calling kill_entity_deferred to defer the death of that entity to the end of the frame.
// This function should be run at the end of each frame. It kills all entities that are marked to be killed.
void Ichigo::conduct_end_of_frame_executions() {
    for (u32 i = 1; i < Internal::entities.size; ++i) {
        Entity &entity = Internal::entities[i];
        if (entity.id.index != 0 && FLAG_IS_SET(entity.flags, EF_MARKED_FOR_DEATH)) {
            CLEAR_FLAG(entity.flags, EF_MARKED_FOR_DEATH);
            kill_entity(&entity);
        }
    }
}

// Kill an entity immediately.
void Ichigo::kill_entity(EntityID id) {
    Ichigo::Entity *entity = Ichigo::get_entity(id);
    if (!entity) {
        ICHIGO_ERROR("Tried to kill a non-existant entity!");
        return;
    }

    kill_entity(entity);
}

void Ichigo::kill_entity(Entity *entity) {
    if (entity->kill_proc) entity->kill_proc(entity);
    Ichigo::Internal::entity_ids_in_draw_order.remove(Ichigo::Internal::entity_ids_in_draw_order.index_of(entity->id));
    entity->id.index = 0;
}

// Kill an entity at the end of the frame. This is useful for if you know you still need to access the entity later on
// in the frame before it is killed.
void Ichigo::kill_entity_deferred(EntityID id) {
    Ichigo::Entity *entity = Ichigo::get_entity(id);
    if (!entity) {
        ICHIGO_ERROR("Tried to kill a non-existant entity!");
        return;
    }

    SET_FLAG(entity->flags, EF_MARKED_FOR_DEATH);
}

void Ichigo::kill_entity_deferred(Entity *entity) {
    SET_FLAG(entity->flags, EF_MARKED_FOR_DEATH);
}

void Ichigo::kill_all_entities() {
    for (u32 i = 1; i < Internal::entities.size; ++i) {
        if (!entity_is_dead(Internal::entities[i].id)) {
            kill_entity(&Internal::entities[i]);
        }
    }
}


Vec2<f32> Ichigo::calculate_projected_next_position(Ichigo::Entity *entity) {
    return {};
}

// Move the entity in the world, considering all external forces (gravity, friction) and performing all collision detection (other entities and tilemap).
Ichigo::EntityMoveResult Ichigo::move_entity_in_world(Ichigo::Entity *entity) {
    EntityMoveResult result = NOTHING_SPECIAL;
    return result;
}

void Ichigo::teleport_entity_considering_colliders(Entity *entity, Vec2<f32> pos) {
    entity->col.pos = pos;
    for (u32 i = 1; i < Ichigo::Internal::entities.size; ++i) {
        Ichigo::Entity &other_entity = Ichigo::Internal::entities[i];

        // Do not check against ourselves or dead entites.
        if (other_entity.id == entity->id || other_entity.id.index == 0) {
            continue;
        }

        if (rectangles_intersect(entity->col, other_entity.col)) {
            if (entity->collide_proc)      entity->     collide_proc(entity, &other_entity, {0.0f, 0.0f}, {0.0f, 0.0f}, entity->col.pos);
            if (other_entity.collide_proc) other_entity.collide_proc(&other_entity, entity, {0.0f, 0.0f}, {0.0f, 0.0f}, other_entity.col.pos);
        }
    }
}

// Simple way to get a string representation of an entity ID for debug purposes.
char *Ichigo::Internal::entity_id_as_string(EntityID entity_id) {
    static char str[22];
    snprintf(str, sizeof(str), "%u:%u", entity_id.generation, entity_id.index);
    return str;
}
