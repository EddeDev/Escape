#include "ClientPCH.h"
#include "Entity.h"

#include "ResourceManager.h"

#include <glm/gtx/vector_angle.hpp>

namespace esc {

	Entity::Entity(const EntityCreateInfo& createInfo)
		: m_Scale(createInfo.Scale), 
		m_Color(createInfo.Color), 
		m_Texture(createInfo.Texture), 
		m_DebugName(createInfo.DebugName),
		m_IsChunk(createInfo.ChunkSpec)
	{
		if (!createInfo.World)
			__debugbreak();

		if (m_IsChunk)
		{
			for (auto& [type, textureName] : createInfo.ChunkSpec->Mapping)
				m_ChunkTextures[type] = ResourceManager::GetTexture(textureName);
		}
		
		b2BodyDef bodyDef;
		if (createInfo.IsKinematic)
			bodyDef.type = b2_kinematicBody;
		else
			bodyDef.type = createInfo.IsDynamic ? b2_dynamicBody : b2_staticBody;
		bodyDef.position.Set(createInfo.Position.x, createInfo.Position.y);
		bodyDef.angle = createInfo.Angle;
		bodyDef.userData.pointer = (uintptr_t)m_DebugName;
		bodyDef.fixedRotation = createInfo.FixedRotation;
		bodyDef.allowSleep = createInfo.AllowSleep;
		m_Body = createInfo.World->CreateBody(&bodyDef);

		b2PolygonShape shape;
		shape.SetAsBox(createInfo.Scale.x * 0.5f, createInfo.Scale.y * 0.5f);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &shape;
		fixtureDef.density = createInfo.Density;
		fixtureDef.friction = createInfo.Friction;
		fixtureDef.restitution = createInfo.Restitution;
		fixtureDef.restitutionThreshold = createInfo.RestitutionThreshold;
		m_Fixture = m_Body->CreateFixture(&fixtureDef);
	}

	bool Entity::IsTouching(Ref<Entity> other)
	{
		for (b2ContactEdge* contactEdge = m_Body->GetContactList(); contactEdge; contactEdge = contactEdge->next)
		{
			b2Contact* contact = contactEdge->contact;

			if (contact->IsTouching())
			{
				const b2Body* bodyA = contact->GetFixtureA()->GetBody();
				const b2Body* bodyB = contact->GetFixtureB()->GetBody();

				if (bodyA == m_Body)
				{
					if (bodyB == other->m_Body)
						return true;
				}

				if (bodyB == m_Body)
				{
					if (bodyA == other->m_Body)
						return true;
				}
			}
		}

		return false;
	}

	bool Entity::IsGrounded()
	{
		for (b2ContactEdge* contactEdge = m_Body->GetContactList(); contactEdge; contactEdge = contactEdge->next)
		{
			b2Contact* contact = contactEdge->contact;

			if (contact->IsTouching())
			{
				const b2Body* bodyA = contact->GetFixtureA()->GetBody();
				const b2Body* bodyB = contact->GetFixtureB()->GetBody();

				auto normal = contact->GetManifold()->localNormal;
				if (normal.y == 1.0f || normal.y == -1.0f) // TODO
					return true;
			}
		}

		return false;
	}

}