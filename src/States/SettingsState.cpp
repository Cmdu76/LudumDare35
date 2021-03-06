#include "SettingsState.hpp"

SettingsState::SettingsState(ah::StateManager& manager)
: ah::State(manager)
{
    mKeySelected = 0;

    sf::Vector2u wSize = ah::Application::getWindow().getSize();
    sf::Vector2f scale = sf::Vector2f(wSize.x/800.f,wSize.y/600.f);

    sf::Vector2f bSize = sf::Vector2f(400 * scale.x, 75 * scale.y);

    mWindow = sfg::Window::Create(sfg::Window::BACKGROUND);
    auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.f);

    //Begin Audio
    mGlobalL = sfg::Label::Create(std::to_string(static_cast<int>(ah::Application::getAudio().getGlobalVolume())));
    mGlobalS = sfg::Scale::Create(0.f,100.f,1.f,sfg::Range::Orientation::HORIZONTAL);
    mGlobalS->SetValue(ah::Application::getAudio().getGlobalVolume());
    mGlobalS->GetAdjustment()->GetSignal(sfg::Adjustment::OnChange).Connect([&]()
    {
        float v = mGlobalS->GetValue();
        mGlobalL->SetText(std::to_string(static_cast<int>(v)));
        ah::Application::getAudio().setGlobalVolume(v);
    });
    auto h1 = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 5.f);
    h1->Pack(sfg::Label::Create("Global Volume"));
    h1->Pack(mGlobalS);
    h1->Pack(mGlobalL);

    mMusicL = sfg::Label::Create(std::to_string(static_cast<int>(ah::Application::getAudio().getMusicVolume())));
    mMusicS = sfg::Scale::Create(0.f,100.f,1.f,sfg::Range::Orientation::HORIZONTAL);
    mMusicS->SetValue(ah::Application::getAudio().getMusicVolume());
    mMusicS->GetAdjustment()->GetSignal(sfg::Adjustment::OnChange).Connect([&]()
    {
        float v = mMusicS->GetValue();
        mMusicL->SetText(std::to_string(static_cast<int>(v)));
        ah::Application::getAudio().setMusicVolume(v);
    });
    auto h2 = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 5.f);
    h2->Pack(sfg::Label::Create("Music Volume"));
    h2->Pack(mMusicS);
    h2->Pack(mMusicL);

    mSoundL = sfg::Label::Create(std::to_string(static_cast<int>(ah::Application::getAudio().getSoundVolume())));
    mSoundS = sfg::Scale::Create(0.f,100.f,1.f,sfg::Range::Orientation::HORIZONTAL);
    mSoundS->SetValue(ah::Application::getAudio().getSoundVolume());
    mSoundS->GetAdjustment()->GetSignal(sfg::Adjustment::OnChange).Connect([&]()
    {
        float v = mSoundS->GetValue();
        mSoundL->SetText(std::to_string(static_cast<int>(v)));
        ah::Application::getAudio().setSoundVolume(v);
    });
    auto h3 = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 5.f);
    h3->Pack(sfg::Label::Create("Sound Volume"));
    h3->Pack(mSoundS);
    h3->Pack(mSoundL);

    auto a = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.f);
    a->Pack(h1);
    a->Pack(h2);
    a->Pack(h3);

    auto frameA = sfg::Frame::Create("Audio");
    frameA->Add(a);
    frameA->SetAlignment(sf::Vector2f(0.1f,0.f));

    box->Pack(frameA);
    //End Audio

    // Begin Graphics
    mRes = sfg::ComboBox::Create();
    auto res = sf::VideoMode::getFullscreenModes();
    for (std::size_t i = 0; i < res.size(); i++)
    {
        sf::Vector2u s = sf::Vector2u(res[i].width,res[i].height);
        if (s.x != wSize.x && s.y != wSize.y)
        {
            mRes->AppendItem(std::to_string(s.x) + "x" + std::to_string(s.y));
        }
    }
    mRes->PrependItem(std::to_string(wSize.x) + "x" + std::to_string(wSize.y));
    mRes->SelectItem(0);
    mRes->GetSignal(sfg::ComboBox::OnSelect).Connect([&]()
    {
        std::string s = static_cast<std::string>(mRes->GetSelectedText());
        std::string w = s.substr(0,s.find("x"));
        std::string h = s.substr(s.find("x")+1);
        ah::Application::getWindow().setVideoMode(sf::VideoMode(std::stoi(w),std::stoi(h)));
        sf::Vector2u ws = ah::Application::getWindow().getSize();
        sf::Vector2f sc = sf::Vector2f(ws.x/800.f,ws.y/600.f);
        mWindow->Refresh();
        mWindow->SetPosition(sf::Vector2f(ws.x / 2.f - mWindow->GetAllocation().width / 2.f, ws.y / 2.f - mWindow->GetAllocation().height / 2.f));
        mBackground.setScale(sc);
    });

    auto h4 = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 5.f);
    h4->Pack(sfg::Label::Create("VideoMode"));
    h4->Pack(mRes);

    mVerticalSync = sfg::CheckButton::Create("VerticalSync");
    mVerticalSync->SetActive(ah::Application::getWindow().isVerticalSyncEnabled());
    mVerticalSync->GetSignal(sfg::Widget::OnLeftClick).Connect([&]()
    {
        ah::Application::getWindow().setVerticalSyncEnabled(mVerticalSync->IsActive());
        ah::Application::getWindow().setDebugInfo("VerticalSync",std::to_string(ah::Application::getWindow().isVerticalSyncEnabled()));
    });
    auto h5 = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 5.f);
    h5->Pack(mVerticalSync);

    mFullscreen = sfg::CheckButton::Create("Fullscreen");
    mFullscreen->SetActive(ah::Application::getWindow().isFullscreen());
    mFullscreen->GetSignal(sfg::Widget::OnLeftClick).Connect([&]()
    {
        ah::Application::getWindow().setFullscreen(mFullscreen->IsActive());
    });
    auto h6 = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 5.f);
    h6->Pack(mFullscreen);

    auto b = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.f);
    b->Pack(h4);
    b->Pack(h5);
    b->Pack(h6);

    auto frameG = sfg::Frame::Create("Graphics");
    frameG->Add(b);
    frameG->SetAlignment(sf::Vector2f(0.1f,0.f));

    box->Pack(frameG);
    // End Graphics

    // Begin Key Binding
    mKeyUp = sfg::Button::Create(Game::getKeyBinding().getKeyString("up"));
    mKeyUp->GetSignal(sfg::Widget::OnLeftClick).Connect([&]()
    {
        mKeySelected = 1;
    });
    auto h7 = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 5.f);
    h7->Pack(sfg::Label::Create("Up"));
    h7->Pack(mKeyUp);

    mKeyLeft = sfg::Button::Create(Game::getKeyBinding().getKeyString("left"));
    mKeyLeft->GetSignal(sfg::Widget::OnLeftClick).Connect([&]()
    {
        mKeySelected = 2;
    });
    auto h8 = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 5.f);
    h8->Pack(sfg::Label::Create("Left"));
    h8->Pack(mKeyLeft);

    mKeyDown = sfg::Button::Create(Game::getKeyBinding().getKeyString("down"));
    mKeyDown->GetSignal(sfg::Widget::OnLeftClick).Connect([&]()
    {
        mKeySelected = 3;
    });
    auto h9 = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 5.f);
    h9->Pack(sfg::Label::Create("Down"));
    h9->Pack(mKeyDown);

    mKeyRight = sfg::Button::Create(Game::getKeyBinding().getKeyString("right"));
    mKeyRight->GetSignal(sfg::Widget::OnLeftClick).Connect([&]()
    {
        mKeySelected = 4;
    });
    auto h10 = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 5.f);
    h10->Pack(sfg::Label::Create("Right"));
    h10->Pack(mKeyRight);

    mKeyAction = sfg::Button::Create(Game::getKeyBinding().getKeyString("action"));
    mKeyAction->GetSignal(sfg::Widget::OnLeftClick).Connect([&]()
    {
        mKeySelected = 5;
    });
    auto h11 = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 5.f);
    h11->Pack(sfg::Label::Create("Action"));
    h11->Pack(mKeyAction);

    mKey1 = sfg::Button::Create(Game::getKeyBinding().getKeyString("1"));
    mKey1->GetSignal(sfg::Widget::OnLeftClick).Connect([&]()
    {
        mKeySelected = 6;
    });
    auto h12 = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 5.f);
    h12->Pack(sfg::Label::Create("1"));
    h12->Pack(mKey1);

    mKey2 = sfg::Button::Create(Game::getKeyBinding().getKeyString("2"));
    mKey2->GetSignal(sfg::Widget::OnLeftClick).Connect([&]()
    {
        mKeySelected = 7;
    });
    auto h13 = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 5.f);
    h13->Pack(sfg::Label::Create("2"));
    h13->Pack(mKey2);

    mKey3 = sfg::Button::Create(Game::getKeyBinding().getKeyString("3"));
    mKey3->GetSignal(sfg::Widget::OnLeftClick).Connect([&]()
    {
        mKeySelected = 8;
    });
    auto h14 = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 5.f);
    h14->Pack(sfg::Label::Create("3"));
    h14->Pack(mKey3);

    auto c = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.f);
    c->Pack(h7);
    c->Pack(h8);
    c->Pack(h9);
    c->Pack(h10);
    c->Pack(h11);
    c->Pack(h12);
    c->Pack(h13);
    c->Pack(h14);

    auto frameB = sfg::Frame::Create("Key Binding");
    frameB->Add(c);
    frameB->SetAlignment(sf::Vector2f(0.1f,0.f));

    box->Pack(frameB);
    // End Key Binding

    // Begin Game Settings
    mScale = sfg::Scale::Create(0.0001f,0.002f,0.0002f);
    mScale->SetValue(ah::Application::getResources().getShader("pixel").getUniformFloat("pixel_threshold"));
    mPixel = sfg::Label::Create("Pixel");

    auto h15 = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 5.f);
    h15->Pack(mPixel);
    h15->Pack(mScale);

    auto d = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.f);
    d->Pack(h15);

    auto frameS = sfg::Frame::Create("Game Settings");
    frameS->Add(d);
    frameS->SetAlignment(sf::Vector2f(0.1f,0.f));

    box->Pack(frameS);
    // End Game Settings

    auto buttonQuit = sfg::Button::Create("Return");
	buttonQuit->SetClass("menu_button");
	buttonQuit->SetAllocation(sf::FloatRect(wSize.x / 2 - bSize.x * 0.5f, 450.f * scale.y, bSize.x, bSize.y));
	buttonQuit->GetSignal(sfg::Widget::OnLeftClick).Connect([&]()
    {
        requestClear();
        requestPush("MenuState");
    });

    box->Pack(buttonQuit);

    mWindow->Add(box);
    mWindow->SetPosition(sf::Vector2f(wSize.x / 2.f - mWindow->GetAllocation().width / 2.f, wSize.y / 2.f - mWindow->GetAllocation().height / 2.f));

	mDesktop.Add(mWindow);

	mBackground.setTexture(ah::Application::getResources().getTexture("back"));
	mBackground.setScale(scale);
}

SettingsState::~SettingsState()
{
    ah::Application::getResources().getShader("pixel").setUniform("pixel_threshold",mScale->GetValue());
}

bool SettingsState::handleEvent(sf::Event const& event)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        mKeySelected = 0;
    }
    if (event.type == sf::Event::KeyPressed && mKeySelected != 0)
    {
        switch (mKeySelected)
        {
            case 1:
            {
                Game::getKeyBinding().setKey("up",event.key.code);
                mKeyUp->SetLabel(lp::keyToString(event.key.code));
            } break;

            case 2:
            {
                Game::getKeyBinding().setKey("left",event.key.code);
                mKeyLeft->SetLabel(lp::keyToString(event.key.code));
            } break;

            case 3:
            {
                Game::getKeyBinding().setKey("down",event.key.code);
                mKeyDown->SetLabel(lp::keyToString(event.key.code));
            } break;

            case 4:
            {
                Game::getKeyBinding().setKey("right",event.key.code);
                mKeyRight->SetLabel(lp::keyToString(event.key.code));
            } break;

            case 5:
            {
                Game::getKeyBinding().setKey("action",event.key.code);
                mKeyAction->SetLabel(lp::keyToString(event.key.code));
            } break;

            case 6:
            {
                Game::getKeyBinding().setKey("1",event.key.code);
                mKey1->SetLabel(lp::keyToString(event.key.code));
            } break;

            case 7:
            {
                Game::getKeyBinding().setKey("2",event.key.code);
                mKey2->SetLabel(lp::keyToString(event.key.code));
            } break;

            case 8:
            {
                Game::getKeyBinding().setKey("3",event.key.code);
                mKey3->SetLabel(lp::keyToString(event.key.code));
            } break;

            default: break;
        }
    }

    mDesktop.HandleEvent(event);
    return true;
}

bool SettingsState::update(sf::Time dt)
{
    mDesktop.Update(dt.asSeconds());
    return true;
}

void SettingsState::render(sf::RenderTarget& target, sf::RenderStates states)
{
    states.transform *= getTransform();
    target.draw(mBackground,states);
    Game::drawGui(ah::Application::getWindow());
}

void SettingsState::onActivate()
{
}

void SettingsState::onDeactivate()
{
    ah::Application::getWindow().removeDebugInfo("VerticalSync");
    ah::Application::getResources().getShader("pixel").setUniform("pixel_threshold",mScale->GetValue());
}
