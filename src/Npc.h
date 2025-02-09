#pragma once

#include "Entity.h"
#include "SDL2/SDL.h"
#include "Animation.h"
#include "Pathfinding.h"

struct SDL_Texture;

class Npc : public Entity
{
public:

	Npc();
	virtual ~Npc();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void SetParameters(pugi::xml_node parameters, int id) {
		this->parameters = parameters;
		this->id = id;
	}

	void SetPosition(Vector2D pos);
	void OnCollision(PhysBody* physA, PhysBody* physB);
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB);

public:

	b2Body* getBody() {
		return pbody->body;
	}

private:

	SDL_Texture* texture;
	int texW, texH;

	pugi::xml_node parameters;
	Animation* currentAnimation = nullptr;
	Animation idle;

	PhysBody* pbody;
	b2Vec2 velocity;
	float speed;
	int id;
	bool isJumping = false;
	float jumpForce = 0.37f;
	int coolDown = 100;
	SDL_RendererFlip flip;
};
