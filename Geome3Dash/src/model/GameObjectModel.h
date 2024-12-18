#pragma once

#include "engine/sus3d/Model.h"
#include "engine/sus3d/Mesh.h"
#include "engine/sus3d/Scene.h"

#include "manager/ModelManager.h"

#include "transformer/AGomtBase.h"

class GameObject;

namespace g3d
{
	class G3DPlayLayer;

	class GameObjectModel : public sus3d::ModelProtocol
	{
	protected:
		float opacity = 1.0f;
		sus3d::Model* model;
		GameObject* gameObject;
		std::vector<AGomtBase*> transformers;
	public:
		GameObject* getGameObject() { return this->gameObject; }
		sus3d::Model* getModel() { return this->model; }
		void setOpacity(const float op) { this->opacity = op; }
		float getOpacity() const { return this->opacity; }

		virtual void update();
		virtual void applyTransformation();
		virtual void render(
			sus3d::ShaderProgram* shaderProgram,
			const sus3d::Camera& camera,
			const sus3d::Light& light);
		
		GameObjectModel() {}
		
		GameObjectModel(
			GameObject* obj, 
			const std::vector<AGomtBase*>& transformers) 
			: gameObject(obj)
			, transformers(transformers)
		{ 
			sus3d::ModelProtocol();
			this->model = ModelManager::get()->getBlockModel(gameObject->m_objectID);
		}

		// not the owner, cleanup is done by the caller
		virtual ~GameObjectModel() {}
	};
}