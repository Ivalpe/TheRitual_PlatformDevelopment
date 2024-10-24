#include "Player.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name = "Player";
}

Player::~Player() {

}

bool Player::Awake() {

	//L03: TODO 2: Initialize Player parameters
	position = Vector2D(100, 100);
	dp = DirectionPlayer::RIGHT;
	stPlayer = StatePlayer::IDLE;
	return true;
}

bool Player::Start() {

	//L03: TODO 2: Initialize Player parameters
	texture = Engine::GetInstance().textures.get()->Load(parameters.attribute("texture").as_string());
	position.setX(parameters.attribute("x").as_int());
	position.setY(parameters.attribute("y").as_int());
	texW = parameters.attribute("w").as_int();
	texH = parameters.attribute("h").as_int();

	//Load animations
	idle.LoadAnimations(parameters.child("animations").child("idle"));
	currentAnimation = &idle;

	//Player
	pbody = Engine::GetInstance().physics.get()->CreateCircle((int)position.getX(), (int)position.getY(), texW / 2, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	//Sensor
	sensorPlayer = Engine::GetInstance().physics.get()->CreateRectangleSensor((int)position.getX(), (int)position.getY() + 10, 10, 10, bodyType::KINEMATIC);
	sensorPlayer->listener = this;
	sensorPlayer->ctype = ColliderType::SENSOR;

	return true;
}

bool Player::Update(float dt)
{
	// L08 TODO 5: Add physics to the player - updated player position using physics
	b2Vec2 velocity = b2Vec2(0, -GRAVITY_Y);
	stPlayer = StatePlayer::IDLE;

	// Move left
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		velocity.x = -speed * dt;
		dp = DirectionPlayer::LEFT;
		stPlayer = StatePlayer::RUN;
	}

	// Move right
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		velocity.x = speed * dt;
		dp = DirectionPlayer::RIGHT;
		stPlayer = StatePlayer::RUN;
	}

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) {
		speed = 0.16;
	}
	else {
		speed = 0.06;
	}

	//Jump
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && isJumping == false) {
		// Apply an initial upward force
		pbody->body->ApplyLinearImpulseToCenter(b2Vec2(0, -jumpForce), true);
		isJumping = true;
	}

	// If the player is jumpling, we don't want to apply gravity, we use the current velocity prduced by the jump
	if (isJumping == true)
	{
		velocity = pbody->body->GetLinearVelocity();
	}

	// Apply the velocity to the player
	pbody->body->SetLinearVelocity(velocity);


	b2Transform pbodyPos = pbody->body->GetTransform();
	if (isJumping == true) {
		if ((int)position.getY() > METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2)
			stPlayer = StatePlayer::JUMP;
		else if ((int)position.getY() < METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2)
			stPlayer = StatePlayer::FALL;
	}
	position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2);
	position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2);


	if (dp == DirectionPlayer::LEFT)
		flipType = SDL_FLIP_HORIZONTAL;
	else
		flipType = SDL_FLIP_NONE;


	Engine::GetInstance().render.get()->DrawTexture(texture, flipType, (int)position.getX(), (int)position.getY(), &currentAnimation->GetCurrentFrame());
	currentAnimation->Update();

	//Sensor
	b2Vec2 playerPosition = pbody->body->GetPosition();
	sensorPlayer->body->SetTransform({ playerPosition.x, playerPosition.y - 0.3f }, sensorPlayer->body->GetAngle());

	return true;
}

bool Player::CleanUp()
{
	LOG("Cleanup player");
	Engine::GetInstance().textures.get()->UnLoad(texture);
	return true;
}

// L08 TODO 6: Define OnCollision function for the player. 
void Player::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physA->ctype)
	{
	case ColliderType::GROUND:
		LOG("Collision PLATFORM");
		isJumping = false;
		break;
	case ColliderType::ITEM:
		LOG("Collision ITEM with");
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN  with");
		break;
	case ColliderType::SENSOR:
		LOG("Collision SENSOR with");
		break;
	case ColliderType::PLAYER:
		LOG("Collision PLAYER with");
		break;
	default:
		break;
	}

	switch (physB->ctype)
	{
	case ColliderType::GROUND:
		LOG("Collision PLATFORM");
		isJumping = false;
		break;
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	case ColliderType::SENSOR:
		LOG("Collision SENSOR");
		break;
	case ColliderType::PLAYER:
		LOG("Collision PLAYER");
		break;
	default:
		break;
	}

	LOG("-----------------------------------------");
}

void Player::OnCollisionEnd(PhysBody* physA, PhysBody* physB)
{
	switch (physB->ctype)
	{
	case ColliderType::GROUND:
		LOG("End Collision PLATFORM DOWN");
		break;
	case ColliderType::ITEM:
		LOG("End Collision ITEM");
		break;
	case ColliderType::UNKNOWN:
		LOG("End Collision UNKNOWN");
		break;
	case ColliderType::SENSOR:
		LOG("End Collision SENSOR");
		break;
	default:
		break;
	}
}