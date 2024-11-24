#pragma once
#include <stdexcept>
#include <Geode/platform/platform.hpp>
#include <Geode/c++stl/gdstdlib.hpp>
#include <cocos2d.h>
#include <cocos-ext.h>
#include <Geode/GeneratedPredeclare.hpp>
#include <Geode/Enums.hpp>
#include <Geode/utils/SeedValue.hpp>


class SecretGame01Layer : public cocos2d::CCLayer {
public:
    static constexpr auto CLASS_NAME = "SecretGame01Layer";
    GEODE_CUSTOM_CONSTRUCTOR_CUTOFF(SecretGame01Layer, cocos2d::CCLayer)
    
private:
    [[deprecated("SecretGame01Layer::create not implemented")]]
    /**
     * @note[short] Android
     */
    static SecretGame01Layer* create();
public:
    
private:
    [[deprecated("SecretGame01Layer::update not implemented")]]
    /**
     * @note[short] MacOS (ARM): 0x3dcfe8
     * @note[short] MacOS (Intel): 0x46dc90
     * @note[short] Android
     */
    virtual void update(float p0);
public:
    
private:
    [[deprecated("SecretGame01Layer::init not implemented")]]
    /**
     * @note[short] MacOS (ARM): 0x3dc234
     * @note[short] MacOS (Intel): 0x46ce50
     * @note[short] Android
     */
    virtual bool init();
public:
    
private:
    [[deprecated("SecretGame01Layer::didSelectCorrectObject not implemented")]]
    /**
     * @note[short] Android
     */
    TodoReturn didSelectCorrectObject(CCMenuItemSpriteExtra* p0);
public:
    
private:
    [[deprecated("SecretGame01Layer::gameStep01 not implemented")]]
    /**
     * @note[short] Android
     */
    TodoReturn gameStep01();
public:
    
private:
    [[deprecated("SecretGame01Layer::gameStep02 not implemented")]]
    /**
     * @note[short] Android
     */
    TodoReturn gameStep02();
public:
    
private:
    [[deprecated("SecretGame01Layer::getCountForDifficulty not implemented")]]
    /**
     * @note[short] Android
     */
    TodoReturn getCountForDifficulty(int p0);
public:
    
private:
    [[deprecated("SecretGame01Layer::getFrameForDifficulty not implemented")]]
    /**
     * @note[short] MacOS (ARM): 0x3dc440
     * @note[short] MacOS (Intel): 0x46d070
     * @note[short] Android
     */
    gd::string getFrameForDifficulty(int p0);
public:
    
private:
    [[deprecated("SecretGame01Layer::getRowsForDifficulty not implemented")]]
    /**
     * @note[short] Android
     */
    TodoReturn getRowsForDifficulty(int p0);
public:
    
private:
    [[deprecated("SecretGame01Layer::getTimeForDifficulty not implemented")]]
    /**
     * @note[short] Android
     */
    TodoReturn getTimeForDifficulty(int p0);
public:
    
private:
    [[deprecated("SecretGame01Layer::onSelectButton not implemented")]]
    /**
     * @note[short] Android
     */
    void onSelectButton(cocos2d::CCObject* sender);
public:
    
private:
    [[deprecated("SecretGame01Layer::resetGame not implemented")]]
    /**
     * @note[short] Android
     */
    TodoReturn resetGame();
public:
    
private:
    [[deprecated("SecretGame01Layer::resetGameTimer not implemented")]]
    /**
     * @note[short] Android
     */
    TodoReturn resetGameTimer();
public:
    
private:
    [[deprecated("SecretGame01Layer::scaleOutGame not implemented")]]
    /**
     * @note[short] Android
     */
    TodoReturn scaleOutGame(bool p0);
public:
    
private:
    [[deprecated("SecretGame01Layer::showGameOver not implemented")]]
    /**
     * @note[short] Android
     */
    TodoReturn showGameOver();
public:
    
private:
    [[deprecated("SecretGame01Layer::showGameWon not implemented")]]
    /**
     * @note[short] Android
     */
    TodoReturn showGameWon();
public:
};