#include "reclaim/core/Paths.hpp"

namespace reclaim::paths {
namespace {

std::string join(std::string_view folder, std::string_view name, std::string_view extension) {
    std::string path;
    path.reserve(kAssetRoot.size() + folder.size() + name.size() + extension.size() + 2);
    path.append(kAssetRoot);
    path.append(folder);
    path.push_back('/');
    path.append(name);
    path.append(extension);
    return path;
}

}  // namespace

std::string image(std::string_view name) { return join("images", name, ".png"); }
std::string sound(std::string_view name) { return join("sounds", name, ".wav"); }
std::string font(std::string_view name) { return join("fonts", name, ".ttf"); }

}  // namespace reclaim::paths
