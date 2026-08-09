#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class $modify(AutoCheckpointPlayLayer, PlayLayer) {
    // Member variable to track the last percentage threshold where a checkpoint was placed
    int m_lastIntervalPercent = -1;

    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) {
            return false;
        }

        // Reset tracking on level start
        m_fields->m_lastIntervalPercent = -1;
        return true;
    }

    void resetLevel() {
        PlayLayer::resetLevel();

        // Reset tracking when the player dies or resets the level
        m_fields->m_lastIntervalPercent = -1;
    }

    void postUpdate(float dt) {
        PlayLayer::postUpdate(dt);

        // Verify that the mod setting is enabled and we are in practice mode
        bool enabled = Mod::get()->getSettingValue<bool>("enabled");
        if (!enabled || !this->m_isPracticeMode) {
            return;
        }

        int interval = static_cast<int>(Mod::get()->getSettingValue<int64_t>("interval-percent"));
        if (interval <= 0) {
            return;
        }

        // Calculate current completion percentage
        int currentPercent = this->getCurrentPercentInt();

        // Determine current interval bucket (e.g., 23% with interval 10 -> step 20)
        int currentIntervalStep = (currentPercent / interval) * interval;

        // Ensure we don't trigger at 0% and only trigger once when entering a new step
        if (currentIntervalStep > 0 && currentIntervalStep > m_fields->m_lastIntervalPercent) {
            m_fields->m_lastIntervalPercent = currentIntervalStep;
            this->markCheckpoint();
        }
    }
};