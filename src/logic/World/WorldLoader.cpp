//
// Created by tibov on 22/12/23.
//

#include "WorldLoader.h"

namespace Logic::WorldLoading {
    WorldLoader::WorldLoader(const std::shared_ptr<AbstractFactory> &factory, const std::shared_ptr<Score>& score): factory{factory}, score{score}{

    }
} // WorldLoading