#pragma once

#include "Texture.h"
#include "Chunk.h"

#include <box2d/b2_body.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_contact.h>
#include <box2d/b2_world.h>

#include <glm/glm.hpp>

namespace esc {

	struct EntityCreateInfo
	{
		b2World* World = nullptr;

		glm::vec2 Position = glm::vec2(0.0f);
		glm::vec2 Scale = glm::vec2(1.0f);
		float Angle = 0.0f;
		float FixedRotation = false;

		bool IsDynamic = false;
		bool IsKinematic = false;
		float AllowSleep = true;
		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;

		glm::vec4 Color = glm::vec4(1.0f);
		Ref<Texture> Texture;
		ChunkSpecification* ChunkSpec = nullptr;

		const char* DebugName = "Unnamed Entity";
	};

	class Entity : public ReferenceCounted
	{
	public:
		Entity(const EntityCreateInfo& createInfo);
		virtual ~Entity() {}

		bool IsTouching(Ref<Entity> other);
		bool IsGrounded();

		glm::vec2 GetLinearVelocity() const { return { m_Body->GetLinearVelocity().x, m_Body->GetLinearVelocity().y }; }
		void SetLinearVelocity(const glm::vec2& velocity) { m_Body->SetLinearVelocity({ velocity.x, velocity.y }); }

		glm::vec2 GetPosition() const { return { m_Body->GetPosition().x, m_Body->GetPosition().y }; }
		void SetPosition(const glm::vec2& position) { m_Body->SetTransform({ position.x, position.y }, m_Body->GetAngle()); }

		glm::vec2 GetScale() const { return m_Scale; }
		void SetScale(const glm::vec2& scale) { m_Scale = scale; ((b2PolygonShape*)m_Fixture->GetShape())->SetAsBox(m_Scale.x * 0.5f, m_Scale.y * 0.5f); }

		float GetAngle() const { return m_Body->GetAngle(); }
		void SetAngle(float angle) { m_Body->SetTransform(m_Body->GetPosition(), m_Body->GetAngle()); }

		glm::vec4 GetColor() const { return m_Color; }
		void SetColor(const glm::vec4& color) { m_Color = color; }

		Ref<Texture> GetTexture() const { return m_Texture; }
		void SetTexture(Ref<Texture> texture) { m_Texture = texture; }

		bool IsChunk() const { return m_IsChunk; }
		const std::unordered_map<TileType, Ref<Texture>>& GetChunkTextures() const { return m_ChunkTextures; }

		const char* GetDebugName() const { return m_DebugName; }
	private:
		b2Body* m_Body = nullptr;
		b2Fixture* m_Fixture = nullptr;

		const char* m_DebugName = "Unnamed Entity";
		glm::vec2 m_Scale = glm::vec2(1.0f);
		glm::vec4 m_Color = glm::vec4(1.0f);
		Ref<Texture> m_Texture;

		bool m_IsChunk = false;
		std::unordered_map<TileType, Ref<Texture>> m_ChunkTextures;
	};

}