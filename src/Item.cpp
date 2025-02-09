#include "Item.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"
#include "Map.h"
#include "Module.h"

Item::Item() : Entity(EntityType::ITEM)
{

}

Item::~Item() {}

bool Item::Awake() {
	return true;
}

bool Item::Start() {

	//initilize textures
	texture = Engine::GetInstance().textures.get()->Load(parameters.attribute("texture").as_string());
	levelItem = parameters.attribute("level").as_int();
	position.setX(parameters.attribute("x").as_int() * 8);
	position.setY(parameters.attribute("y").as_int() * 8);
	texW = parameters.attribute("w").as_int();
	texH = parameters.attribute("h").as_int();

	//load animations
	idle.LoadAnimations(parameters.child("animations").child("idle"));
	collect.LoadAnimations(parameters.child("animations").child("collect"));
	currentAnimation = &idle;

	pugi::xml_document audioFile;
	pugi::xml_parse_result result = audioFile.load_file("config.xml");
	item_pickupSFX = Engine::GetInstance().audio.get()->LoadFx(audioFile.child("config").child("scene").child("audio").child("fx").child("item_pickupSFX").attribute("path").as_string());

	//Engine::GetInstance().textures.get()->GetSize(texture, texW, texH);
	pbody = Engine::GetInstance().physics.get()->CreateCircle((int)position.getX() + texH, (int)position.getY() + texH, texH / 2, bodyType::DYNAMIC);

	if (ittype == ItemType::COIN)
		pbody->ctype = ColliderType::COIN;
	else if (ittype == ItemType::FIREUP)
		pbody->ctype = ColliderType::FIREUP;
	else if (ittype == ItemType::HEALTH)
		pbody->ctype = ColliderType::HEALTH;
	pbody->listener = this;

	pbody->body->SetGravityScale(0);

	return true;
}

void Item::ShowItem(bool show) {
	if (!showing && show) {
		showing = show;

		//Engine::GetInstance().textures.get()->GetSize(texture, texW, texH);
		pbody = Engine::GetInstance().physics.get()->CreateCircle((int)position.getX() + texH, (int)position.getY() + texH, texH / 2, bodyType::DYNAMIC);

		if (ittype == ItemType::COIN)
			pbody->ctype = ColliderType::COIN;
		else if (ittype == ItemType::FIREUP)
			pbody->ctype = ColliderType::FIREUP;
		else if (ittype == ItemType::HEALTH)
			pbody->ctype = ColliderType::HEALTH;
		pbody->listener = this;

		pbody->body->SetGravityScale(0);
	}
	else if (showing && !show) {
		showing = show;
		Engine::GetInstance().physics->DeleteBody(pbody->body);
	}
}

void Item::SetItemType(ItemType it) {
	ittype = it;
}

bool Item::Update(float dt)
{
	if (showing) {
		if (stItem == StateItem::DIE) {
			if (currentAnimation->HasFinished()) {

				collected = true;
				return false;
			}
			pbody->body->SetLinearVelocity({ 0, 0 });
		}

		if (stItem == StateItem::IDLE) {
			currentAnimation = &idle;
		}

		velocity = b2Vec2(0, 0);
		pbody->body->SetLinearVelocity(velocity);

		b2Transform pbodyPos = pbody->body->GetTransform();
		position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - texH);
		position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - texW - texW / 2);

		Engine::GetInstance().render.get()->DrawTexture(texture, SDL_FLIP_NONE, (int)position.getX(), (int)position.getY(), &currentAnimation->GetCurrentFrame());
		currentAnimation->Update();
	}
	return true;
}

bool Item::CleanUp()
{
	Engine::GetInstance().textures.get()->UnLoad(texture);
	return true;
}

void Item::SetPosition(Vector2D pos) {

	pos.setX(pos.getX() + texW / 2);
	pos.setY(pos.getY() + texH / 2);

	b2Vec2 bodyPos = b2Vec2(PIXEL_TO_METERS(pos.getX()), PIXEL_TO_METERS(pos.getY()));
	pbody->body->SetTransform(bodyPos, 0);
}

void Item::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::GROUND:
		LOG("Collision PLATFORM");
		break;

	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	case ColliderType::DIE:
		collected = true;
		LOG("Collision DIE");
		break;
	case ColliderType::FIREBALLPLAYER:
		LOG("Collision FIREBALL");
		break;
	case ColliderType::PLAYER:
		if (stItem != StateItem::DIE && physB->ctype != ColliderType::SENSOR) {
			stItem = StateItem::DIE;
			Engine::GetInstance().audio.get()->PlayFx(item_pickupSFX);
			currentAnimation = &idle;
			collected = true;

			//currentAnimation->Reset();
			pbody->body->SetLinearVelocity({ 0, 0 });
		}
		break;

	default:
		break;
	}
}

void Item::OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		break;
	default:
		break;
	}
}

Vector2D Item::GetPosition() {
	b2Vec2 bodyPos = pbody->body->GetTransform().p;
	Vector2D pos = Vector2D(METERS_TO_PIXELS(bodyPos.x), METERS_TO_PIXELS(bodyPos.y));
	return pos;
}