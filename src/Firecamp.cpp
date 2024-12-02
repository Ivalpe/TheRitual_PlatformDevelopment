#include "Firecamp.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"
#include "Map.h"

Firecamp::Firecamp() : Entity(EntityType::FIRECAMP) {
}

Firecamp::~Firecamp() {
}

bool Firecamp::Awake() {
	return true;
}

bool Firecamp::Start() {

	//initilize textures
	texture = Engine::GetInstance().textures.get()->Load(parameters.attribute("texture").as_string());
	texW = parameters.attribute("w").as_int();
	texH = parameters.attribute("h").as_int();
	//position.setX(420);
	//position.setY(104);

	//Load animations
	idle.LoadAnimations(parameters.child("animations").child("idle"));
	on.LoadAnimations(parameters.child("animations").child("on"));
	sf = StateFirecamp::IDLE;
	currentAnimation = &idle;

	return true;
}

void Firecamp::ActiveFirecamp() {
	sf = StateFirecamp::ON;
	currentAnimation = &on;
}

bool Firecamp::Update(float dt)
{

	//LOG("FIRECAMP: %d %d", position.getX(), position.getY());
	Engine::GetInstance().render.get()->DrawTexture(texture, SDL_FLIP_NONE, (int)position.getX(), (int)position.getY(), &currentAnimation->GetCurrentFrame());
	currentAnimation->Update();

	return true;
}

bool Firecamp::CleanUp()
{
	return true;
}

void Firecamp::SetPosition(Vector2D pos) {
	//LOG("FIRECAMP SET POSITION: %f %f", pos.getX(), pos.getY());
	position.setX(pos.getX() + texW / 2);
	position.setY(pos.getY() + texH / 2);
}

Vector2D Firecamp::GetPosition() {
	return position;
}