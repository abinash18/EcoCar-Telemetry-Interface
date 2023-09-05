#pragma once
#include "AEngine/Layer.h"
#include "AEngine/core/Core.h"
#include "AEngine/input/event/Event.h"
#include "AEngine/input/event/KeyEvent.h"
#include "AEngine/input/event/MouseEvent.h"
#include "AEngine/input/event/WindowEvent.h"
#include "imgui.h"
#include "IMPLOT/implot.h"

class InterfaceLayer : public AEngine::Layer {
    public:
        InterfaceLayer();
        ~InterfaceLayer() override;

        virtual void attach() override;
        virtual void detach() override;
        virtual void render() override;
        virtual void update(float delta) override;

        void begin();
        void end();

    private:
        float m_Time = 0;
};

