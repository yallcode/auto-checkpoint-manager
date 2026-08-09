#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class $modify(AutoCheckpointPlayLayer, PlayLayer) {
    struct Fields {
        float m_lastTriggeredValue = 0.0f;
    };

    void resetLevel() {
        PlayLayer::resetLevel();
        m_fields->m_lastTriggeredValue = 0.0f;
    }

    void update(float dt) {
        PlayLayer::update(dt);

        if (!Mod::get()->getSettingValue<bool>("enabled")) {
            return;
        }

        if (!this->m_isPracticeMode) {
            return;
        }

        bool useTime = Mod::get()->getSettingValue<bool>("use-time");

        if (useTime) {
            float timeInterval = static_cast<float>(Mod::get()->getSettingValue<double>("time-interval"));
            if (timeInterval <= 0.0f) return;

            float currentTime = this->m_gameState.m_levelTime;

            if (currentTime - m_fields->m_lastTriggeredValue >= timeInterval) {
                this->markCheckpoint();
                m_fields->m_lastTriggeredValue = std::floor(currentTime / timeInterval) * timeInterval;
            }
        } else {
            int64_t percentIntervalInt = Mod::get()->getSettingValue<int64_t>("percentage-interval");
            float percentInterval = static_cast<float>(percentIntervalInt);
            if (percentInterval <= 0.0f) return;

            float currentPercent = this->getCurrentPercent();

            if (currentPercent < m_fields->m_lastTriggeredValue) {
                m_fields->m_lastTriggeredValue = 0.0f;
            }

            if (currentPercent - m_fields->m_lastTriggeredValue >= percentInterval) {
                this->markCheckpoint();
                m_fields->m_lastTriggeredValue = std::floor(currentPercent / percentInterval) * percentInterval;
            }
        }
    }
};