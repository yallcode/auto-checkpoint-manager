#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class $modify(AutoCheckpointPlayLayer, PlayLayer) {
    struct Fields {
        float m_lastTriggeredValue = 0.0f;
    };

    void resetLevel() {
        PlayLayer::resetLevel();
        // Reset our interval tracker when the player dies or restarts
        m_fields->m_lastTriggeredValue = 0.0f;
    }

    void update(float dt) {
        PlayLayer::update(dt);

        // Check overall mod toggle
        if (!Mod::get()->getSettingValue<bool>("enabled")) {
            return;
        }

        // Feature is strictly for Practice Mode
        if (!this->m_isPracticeMode) {
            return;
        }

        bool useTime = Mod::get()->getSettingValue<bool>("use-time");

        if (useTime) {
            // Time-based interval check
            float timeInterval = static_cast<float>(Mod::get()->getSettingValue<double>("time-interval"));
            if (timeInterval <= 0.0f) return;

            float currentTime = this->m_gameState.m_levelTime;

            // Trigger if we crossed the next interval threshold
            if (currentTime - m_fields->m_lastTriggeredValue >= timeInterval) {
                this->markCheckpoint();
                // Snap tracking to the exact threshold step to avoid drift
                m_fields->m_lastTriggeredValue = std::floor(currentTime / timeInterval) * timeInterval;
            }
        } else {
            // Percentage-based interval check
            int percentIntervalInt = Mod::get()->getSettingValue<int64_t>("percentage-interval");
            float percentInterval = static_cast<float>(percentIntervalInt);
            if (percentInterval <= 0.0f) return;

            float currentPercent = this->getCurrentPercent();

            // Safety check against false positive progress resets or backwards jumps
            if (currentPercent < m_fields->m_lastTriggeredValue) {
                m_fields->m_lastTriggeredValue = 0.0f;
            }

            // Trigger if current progress crossed the next step interval
            if (currentPercent - m_fields->m_lastTriggeredValue >= percentInterval) {
                this->markCheckpoint();
                m_fields->m_lastTriggeredValue = std::floor(currentPercent / percentInterval) * percentInterval;
            }
        }
    }
};
