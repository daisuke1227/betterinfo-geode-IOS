#include <Geode/Geode.hpp>
#include <Geode/modify/EditLevelLayer.hpp>

#include "../utils.hpp"

using namespace geode::prelude;

class BI_DLL $modify(BIEditLevelLayer, EditLevelLayer) {
    static void onModify(auto& self) {
        (void) self.setHookPriority("EditLevelLayer::onLevelInfo", Priority::VeryLatePre);
    }

    /*
     * Callbacks
     */

    void onEditLevelGoToLevel(CCObject* sender) {
        auto searchObject = GJSearchObject::create(SearchType::Search, std::to_string(this->m_level->m_levelID));
        auto browserLayer = LevelBrowserLayer::scene(searchObject);

        auto transitionFade = CCTransitionFade::create(0.5, browserLayer);

        CCDirector::sharedDirector()->pushScene(transitionFade);
    }

    /*
     * Hooks
     */

    bool init(GJGameLevel* level) {
        if(!EditLevelLayer::init(level)) return false;

        if(this->m_level->m_levelID > 0) {
            auto idLabel = typeinfo_cast<CCLabelBMFont*>(this->getChildByID("level-id-label"));
            if(!idLabel) return true;

            auto winSize = CCDirector::sharedDirector()->getWinSize();

            idLabel->setVisible(false);

            auto newBMFont = CCLabelBMFont::create(idLabel->getString(), "goldFont.fnt");
            newBMFont->setScale(.6f);

            auto newBMFontBtn = CCMenuItemSpriteExtra::create(
                newBMFont,
                this,
                menu_selector(BIEditLevelLayer::onEditLevelGoToLevel)
            );
            newBMFontBtn->setPosition(75, - winSize.height / 2 + 14);

            auto newMenu = CCMenu::create();
            newMenu->addChild(newBMFontBtn);
            this->addChild(newMenu);
            newMenu->setID("level-id-menu"_spr);
        }

        return true;
    }

    void onLevelInfo(CCObject* sender) {
        static_assert(&EditLevelLayer::onLevelInfo, "Hook not implemented");

        LevelProgressDialog::show(this->m_level);
    }
};