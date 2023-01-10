#pragma once

#include <box2d/b2_body.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_contact.h>

#include <glm/glm.hpp>

namespace Escape {

	struct EntityCreateInfo
	{
		glm::vec2 Position = glm::vec2(0.0f);
		glm::vec2 Scale = glm::vec2(1.0f);
		float Angle = 0.0f;
		float FixedRotation = false;

		bool IsDynamic = false;
		float AllowSleep = true;
		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;

		glm::vec4 Color = glm::vec4(1.0f);

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
		void SetScale(const glm::vec2& scale) { m_Scale = scale; m_Shape.SetAsBox(m_Scale.x, m_Scale.y); }

		float GetAngle() const { return m_Body->GetAngle(); }
		void SetAngle(float angle) { m_Body->SetTransform(m_Body->GetPosition(), m_Body->GetAngle()); }

		glm::vec4 GetColor() const { return m_Color; }
		void SetColor(const glm::vec4& color) { m_Color = color; }
	private:
		b2Body* m_Body = nullptr;
		b2Fixture* m_Fixture = nullptr;

		b2PolygonShape m_Shape;

		const char* m_DebugName;
		glm::vec2 m_Scale = glm::vec2(1.0f);
		glm::vec4 m_Color = glm::vec4(1.0f);
	};

}