//
// Created by avalibit on 18/04/2021.
//

#ifndef REMANIA_GAMEBOX_PS3_H
#define REMANIA_GAMEBOX_PS3_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <malloc.h>
#include <ppu-types.h>

#include <sysutil/video.h>
#include <rsx/rsx.h>

#include <Graphics/graphics.hpp>

namespace RMGB::Graphics::PS3 {
    class Backend : private RMGB::Graphics::Backend {
        bool CheckAPISpecObj(APISpec Obj);

        void Init(int what = 0xFF) override;

        void Update() override;

        void Schedule(RenderCommand command) override;

        void toggleImGui() override;

        void changeSettings() override;

        bool isMousePressed() override;

        SDL_KeyCode getLastKey() override;

        bool isKeyPressed(SDL_KeyCode key) override;

        glm::vec2 getMousePos() override;

        void Destroy() override;

    private:
        bool imGui = true;
        Settings settings{};
        Settings settingsbak{};

    };
}

#endif //REMANIA_GAMEBOX_PS3_H
