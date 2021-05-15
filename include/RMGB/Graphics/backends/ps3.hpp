//
// Created by avalibit on 18/04/2021.
//

#ifndef REMANIA_GAMEBOX_PS3_H
#define REMANIA_GAMEBOX_PS3_H

#warning "This rendering backend is very very incomplete, use at your own peril"

#include <cstdio>
#include <cstdlib>
#include <cstring>
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

        void Destroy() override;

    private:
        bool imGui = true;
        Settings settings{};
        Settings settingsbak{};
    };
}

#endif //REMANIA_GAMEBOX_PS3_H
