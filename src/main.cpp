#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class $modify(AutoCheckpointPlayLayer, PlayLayer) {
    struct Fields {
        int m_lastIntervalPercent = -1;
    };

    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) {
            return false;
        }

        m_fields->m_lastIntervalPercent = -1;
        return true;
    }

    void resetLevel() {
        PlayLayer::resetLevel();
        m_fields->m_lastIntervalPercent = -1;
    }

    void postUpdate(float dt) {
        PlayLayer::postUpdate(dt);

        auto self = static_cast<PlayLayer*>(this);

        bool enabled = Mod::get()->getSettingValue<bool>("enabled");
        if (!enabled || !self->m_isPracticeMode) {
            return;
        }

        int interval = static_cast<int>(Mod::get()->getSettingValue<int64_t>("interval-percent"));
        if (interval <= 0) {
            return;
        }

        // Fixed: using getCurrentPercent() instead of getCurrentPercentInt()
        int currentPercent = static_cast<int>(self->getCurrentPercent());
        int currentIntervalStep = (currentPercent / interval) * interval;

        if (currentIntervalStep > 0 && currentIntervalStep > m_fields->m_lastIntervalPercent) {
            m_fields->m_lastIntervalPercent = currentIntervalStep;
            self->markCheckpoint();
        }
    }
};