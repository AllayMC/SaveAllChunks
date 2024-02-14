#include "SaveAllChunks.h"

#include <functional>
#include <ll/api/plugin/NativePlugin.h>
#include <ll/api/plugin/PluginManagerRegistry.h>
#include <ll/api/memory/Hook.h>
#include <memory>
#include <stdexcept>

#include <mc/world/level/chunk/LevelChunk.h>

namespace save_all_chunks {

namespace {

LL_AUTO_TYPE_INSTANCE_HOOK(
    NonActorDataNeedsSavingHook,
    ll::memory::HookPriority::Normal,
    LevelChunk,
    "nonActorDataNeedsSaving@LevelChunk@@QEBA_NHH@Z",
    bool,
    int,
    int
) {
    return true;
}

std::unique_ptr<std::reference_wrapper<ll::plugin::NativePlugin>> selfPluginInstance; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

auto disable(ll::plugin::NativePlugin& /*self*/) -> bool {return true;}

auto enable(ll::plugin::NativePlugin& /*self*/) -> bool {return true;}

auto load(ll::plugin::NativePlugin& self) -> bool {
    selfPluginInstance = std::make_unique<std::reference_wrapper<ll::plugin::NativePlugin>>(self);
    return true;
}

auto unload(ll::plugin::NativePlugin& self) -> bool {
    selfPluginInstance.reset();
    return true;
}

} // namespace

auto getSelfPluginInstance() -> ll::plugin::NativePlugin& {
    if (!selfPluginInstance) {
        throw std::runtime_error("selfPluginInstance is null");
    }
    return *selfPluginInstance;
}

} // namespace save_all_chunks

extern "C" {
_declspec(dllexport) auto ll_plugin_disable(ll::plugin::NativePlugin& self) -> bool { return save_all_chunks::disable(self); }
_declspec(dllexport) auto ll_plugin_enable(ll::plugin::NativePlugin& self) -> bool { return save_all_chunks::enable(self); }
_declspec(dllexport) auto ll_plugin_load(ll::plugin::NativePlugin& self) -> bool { return save_all_chunks::load(self); }
_declspec(dllexport) auto ll_plugin_unload(ll::plugin::NativePlugin& self) -> bool { return save_all_chunks::unload(self); }
}
