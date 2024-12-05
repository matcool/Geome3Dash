#pragma once

#include "delegate/CustomKeyboard.h"
#include "delegate/CustomMouse.h"
#include "delegate/CustomTouch.h"

#include "helper/spline/Spline.h"

namespace sus3d
{
	class Model;
	class ShaderProgram;
}

namespace g3d
{
	class CameraKeyframeEditorLoader;

	class CameraKeyframeEditorPopup 
		: public geode::Popup<CameraKeyframeEditorLoader*>
		, public CustomKeyboardDelegate
		, public CustomMouseDelegate
		, public CustomTouchDelegate 
	{
	protected:
		CameraKeyframeEditorLoader* ckel;

		bool isEditing = false;

		bool isPressingControl = false;
		bool isRightClicking = false;
		bool isRightClickingGetPos = false;
		float lastMouseX = 0.0;
		float lastMouseY = 0.0;

		virtual void onGLFWMouseCallBack(GLFWwindow* window, int button, int action, int mods);
		virtual void onGLFWMouseMoveCallBack(GLFWwindow* window, double x, double y);
		virtual void scrollWheel(float y, float x);
		virtual void onKey(enumKeyCodes key, bool pressed, bool holding);

		bool setup(CameraKeyframeEditorLoader* cel) override;

		void renderPlayer();
		void renderGround();

		void draw() override;

		void onAdd(CCObject*);
		void onRemoveLast(CCObject*);

		glm::vec3 getPlayerOrientedCameraFront();
		glm::vec3 getPlayerOrientedCameraPosition();
	public:
		void onClose(CCObject* obj);

		static CameraKeyframeEditorPopup* create(CameraKeyframeEditorLoader* ckel);
	};
}