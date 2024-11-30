#include "pch.h"

#include "game/planet/G3DPlanetLayer.h"
#include "game/planet/G3DPlanetPopup.h"
#include "game/component/G3DBaseNode.h"
#include "game/component/G3DFragmentShaderLayer.h"

#include "CocosShaderProgram.h"
#include "PlanetStateManager.h"

#include "engine/sus3d/Shader.h"
#include "engine/sus3d/Shaders.h"
#include "engine/sus3d/Model.h"
#include "engine/sus3d/Mesh.h"

#include "helper/OpenGLStateHelper.h"

namespace g3d
{
    void G3DPlanetLayer::onGLFWMouseCallBack(GLFWwindow* window, int button, int action, int mods) {
        if (G3DPlanetPopup::checkIsOpened()) return;
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            if (action == GLFW_PRESS) {
                isRightClicking = true;
                isRightClickingGetPos = false;
            }
            else if (action == GLFW_RELEASE) {
                isRightClicking = false;

                auto selected = layer3d->getObjectIDByMousePosition();

                if (selected.first == 0 && selected.second >= 982 && selected.second <= 1061) {

                    int levelID = (selected.second - 982) / 4;

                    std::cout << levelID << std::endl;

                    for (size_t meshIndex = 0; meshIndex < layer3d->models[0]->meshes.size(); meshIndex++) {
                        if (meshIndex == levelID * 4 + 982 || meshIndex == levelID * 4 + 983 || meshIndex == levelID * 4 + 984 || meshIndex == levelID * 4 + 985)
                            layer3d->models[0]->meshes[meshIndex]->setCustomKa(glm::vec3(1, 0, 0));
                        else
                            layer3d->models[0]->meshes[meshIndex]->disableKa();
                    }

                    G3DPlanetPopup::tryOpen(levelID);
                }
            }
        }
    }

    void G3DPlanetLayer::detectBiomeMusic() {
        glm::quat currentRotation = glm::quat(glm::vec3(
            glm::radians(planetModel->getRotationX()),
            glm::radians(planetModel->getRotationY()),
            glm::radians(planetModel->getRotationZ())
        ));

        glm::vec3 iceVector(0.0f, 1.0f, 0.0f);
        glm::vec3 worldIceVector = currentRotation * iceVector;
        float iceDotProduct = glm::dot(glm::normalize(worldIceVector), glm::normalize(layer3d->camera.getFront()));
        float iceAngle = glm::degrees(glm::acos(iceDotProduct));

        glm::vec3 desertVector(0.4f, 0.0f, 1.0f);
        glm::vec3 worldDesertVector = currentRotation * desertVector;
        float desertDotProduct = glm::dot(glm::normalize(worldDesertVector), glm::normalize(layer3d->camera.getFront()));
        float desertAngle = glm::degrees(glm::acos(desertDotProduct));

        std::cout << desertAngle << std::endl;
        if (iceAngle < 45.0f || iceAngle > 135.0f) {
            setMusicType(Ice);
        }
        else if (desertAngle < 30.f) {
            setMusicType(Desert);
        }
        else {
            setMusicType(Plains);
        }
    }

    void G3DPlanetLayer::playNewSongType() {
        auto songPath = geode::Mod::get()->getResourcesDir() / "music";
        switch (musicType) {
        case MusicType::Plains:
            songPath = songPath / "A145 - A Newborn Spirit.mp3";
            break;
        case MusicType::Ice:
            songPath = songPath / "A145 - Each Other's Backs.mp3";
            break;
        case MusicType::Desert:
            songPath = songPath / "A145 - On Top Of The Desert.mp3";
            break;
        case MusicType::Default:
        default:
            songPath = songPath / "A145 - That One Talks.mp3";
            break;
        }
        static int counter = 0;
        FMODAudioEngine::get()->fadeOutMusic(3.f, (counter % 2) + 1);
        FMODAudioEngine::get()->playMusic(songPath.string(), true, 3.f, ((counter + 1) % 2) + 1);
    }


    void G3DPlanetLayer::onGLFWMouseMoveCallBack(GLFWwindow* window, double x, double y) {
        if (G3DPlanetPopup::checkIsOpened()) return;
        if (isRightClicking) {
            if (!isRightClickingGetPos) {
                lastMouseX = static_cast<float>(x);
                lastMouseY = static_cast<float>(y);
                isRightClickingGetPos = true;
            }
            else {
                float deltaX = static_cast<float>(x) - lastMouseX;
                float deltaY = static_cast<float>(y) - lastMouseY;
                if (isPressingControl) {

                }
                else {
                    float sensitivity = 0.005f;

                    glm::quat rotationX = glm::angleAxis(deltaY * sensitivity, glm::vec3(1.0f, 0.0f, 0.0f));
                    glm::quat rotationY = glm::angleAxis(deltaX * sensitivity, glm::vec3(0.0f, 1.0f, 0.0f));
                    glm::quat currentRotation = glm::quat(glm::vec3(
                        glm::radians(planetModel->getRotationX()),
                        glm::radians(planetModel->getRotationY()),
                        glm::radians(planetModel->getRotationZ())
                    ));

                    glm::quat newRotation = rotationY * rotationX * currentRotation;
                    glm::vec3 eulerAngles = glm::eulerAngles(newRotation);

                    planetModel->setRotationX(glm::degrees(eulerAngles.x));
                    planetModel->setRotationY(glm::degrees(eulerAngles.y));
                    planetModel->setRotationZ(glm::degrees(eulerAngles.z));
                    planetModelWater->setRotationX(glm::degrees(eulerAngles.x));
                    planetModelWater->setRotationY(glm::degrees(eulerAngles.y));
                    planetModelWater->setRotationZ(glm::degrees(eulerAngles.z));
                    cloud->setRotationX(glm::degrees(eulerAngles.x));
                    cloud->setRotationY(glm::degrees(eulerAngles.y));
                    cloud->setRotationZ(glm::degrees(eulerAngles.z));

                    if (layer3d->camera.getPosition().z < 30) {
                        detectBiomeMusic();
                    }
                    else {
                        setMusicType(Default);
                    }
                }

                lastMouseX = static_cast<float>(x);
                lastMouseY = static_cast<float>(y);
            }
        }
    }

    void G3DPlanetLayer::scrollWheel(float y, float x) {
        float zoomSensitivity = 0.128f;
        layer3d->camera.setPosition(layer3d->camera.getPosition() + glm::vec3(0, 0, y * zoomSensitivity));
        if (layer3d->camera.getPosition().z < 10) layer3d->camera.setPosition(glm::vec3(layer3d->camera.getPosition().x, layer3d->camera.getPosition().y, 10));
        if (layer3d->camera.getPosition().z > 50) layer3d->camera.setPosition(glm::vec3(layer3d->camera.getPosition().x, layer3d->camera.getPosition().y, 50));
    }

    void G3DPlanetLayer::onKey(enumKeyCodes key, bool pressed, bool holding) {
        switch (key) {
        case KEY_Control:
            isPressingControl = pressed;
            break;
        case KEY_Escape:
            onBack(this);
            break;
        }
    }

    bool G3DPlanetLayer::init() {
        FMODAudioEngine::get()->fadeOutMusic(3.f, 0);
        CCLayer::init();
        insideThePlanetLayerFlag = true;

        setKeyboardEnabled(true);
        OpenGLStateHelper::saveState();
        auto vertexShader = sus3d::Shader::createWithString(sus3d::shaders::vertexShaderSource, sus3d::ShaderType::kVertexShader);
        auto fragmentShader = sus3d::Shader::createWithString(sus3d::shaders::fragmentShaderSource, sus3d::ShaderType::kFragmentShader);
        shaderProgram = CocosShaderProgram::create(vertexShader, fragmentShader);
        delete vertexShader;
        delete fragmentShader;

        const auto planetPath = geode::Mod::get()->getResourcesDir() / "model3d" / "planet";
        const auto shaderPath = planetPath / "shader";
        const auto modelPath = planetPath / "model";

        auto vertexShader2 = sus3d::Shader::createWithString(sus3d::shaders::vertexShaderSource, sus3d::ShaderType::kVertexShader);
        auto fragmentShader2 = sus3d::Shader::createWithFile(shaderPath / "water2.fsh", sus3d::ShaderType::kFragmentShader);
        auto shaderProgram2 = CocosShaderProgram::create(vertexShader2, fragmentShader2);
        delete vertexShader2;
        delete fragmentShader2;

        auto vertexShader3 = sus3d::Shader::createWithFile(shaderPath / "cloud.vsh", sus3d::ShaderType::kVertexShader);
        auto fragmentShader3 = sus3d::Shader::createWithFile(shaderPath / "cloud.fsh", sus3d::ShaderType::kFragmentShader);
        auto shaderProgram3 = CocosShaderProgram::create(vertexShader3, fragmentShader3);
        delete vertexShader3;
        delete fragmentShader3;
        OpenGLStateHelper::pushState();

        layer3d = G3DBaseNode::create();
        layer3d->light.setPosition(glm::vec3(0, 50, 1000));
        layer3d->setZOrder(10);


        planetModel = layer3d->loadAndAddModel<PlanetModel>(modelPath / "new_planet_textured.obj", shaderProgram);
        planetModelWater = layer3d->loadAndAddModel<PlanetModel>(modelPath / "planet_water.obj", shaderProgram2);
        planetModelWater->setScale(glm::vec3(1.001, 1.001, 1.001));

        cloud = layer3d->loadAndAddModel<PlanetModel>(modelPath / "clouds.obj", shaderProgram3);
        cloud->setScale(glm::vec3(0.85));

        this->addChild(layer3d);
        layer3d->camera.setPosition(glm::vec3(0, 0, 25));

        auto size = CCDirector::sharedDirector()->getWinSize();

        auto testLayer = G3DFragmentShaderLayer::create(shaderPath / "space.fsh");
        //auto testLayer = FragmentShaderLayer::create("./water.fsh");
        this->addChild(testLayer);

        auto bg = CCSprite::create("GJ_gradientBG.png");
        bg->setScaleX(size.width / bg->getContentSize().width);
        bg->setScaleY(size.height / bg->getContentSize().height);
        bg->setColor({ 0, 75, 110 });
        bg->setZOrder(-5);

        bg->setPosition(size / 2);

        this->addChild(bg);

        auto backButtonSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
        auto backButton = CCMenuItemSpriteExtra::create(backButtonSprite, this, menu_selector(G3DPlanetLayer::onBack));
        auto backButtonMenu = CCMenu::create();
        backButtonMenu->setPosition(25, size.height - 25);
        backButtonMenu->addChild(backButton);
        this->addChild(backButtonMenu);

        playNewSongType();

        return true;
    }

    void G3DPlanetLayer::onEnter() {
        CCLayer::onEnter();
        for (int i = 0; i < cloud->meshes.size(); i++) {
            int realMeshId = cloud->meshes.size() - 1 - i;
            if (i == 0) {
                cloud->meshes[realMeshId]->setVisible(0);
                continue;
            }

            cloud->meshes[realMeshId]->setVisible(
                (PlanetStateManager::getInstance()->getProgressByLevelID(i - 1)->normal == 100)
                ? 0 : 1);
        }
    }

    void G3DPlanetLayer::onBack(CCObject*) {
        keyBackClicked();
    }

    void G3DPlanetLayer::keyBackClicked(void) {
        insideThePlanetLayerFlag = false;

        FMODAudioEngine::get()->fadeInMusic(3.f, 0);

        CCDirector::sharedDirector()->popSceneWithTransition(0.3, PopTransition::kPopTransitionFade);
    }

    void G3DPlanetLayer::draw() {
        CCLayer::draw();


    }

    G3DPlanetLayer* G3DPlanetLayer::create() {
        auto node = new G3DPlanetLayer;
        if (node->init()) {
            node->autorelease();
        }
        else {
            CC_SAFE_DELETE(node);
        }
        return node;
    }

    bool G3DPlanetLayer::insideThePlanetLayerFlag = false;

    PlanetModel* PlanetModel::create(const aiScene* scene, sus3d::ShaderProgram* shaderProgram) {
        PlanetModel* ret = new PlanetModel();
        ret->shaderProgram = shaderProgram;

        if (!ret || !ret->init(scene)) {
            delete ret;  // Cleanup if initialization fails
            return nullptr;
        }

        return ret;
    }

    glm::mat4 PlanetModel::prepareModelMatrix() {
        glm::mat4 model = glm::mat4(1.0f); // Start with an identity matrix

        // Apply translation
        model = glm::translate(model, position);

        // Apply rotations (Z, Y, X in this order)
        if (rotation.z != 0.0f)  // Only apply rotation if non-zero
            model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));  // Rotate around Z axis

        if (rotation.y != 0.0f)  // Only apply rotation if non-zero
            model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));  // Rotate around Y axis

        if (rotation.x != 0.0f)  // Only apply rotation if non-zero
            model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));  // Rotate around X axis

        // Apply scaling
        model = glm::scale(model, scale);

        return model;
    }
}