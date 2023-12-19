//
// Created by tibov on 19/12/23.
//

#ifndef PROJECTPACMAN_IMAGEANIMATION_H
#define PROJECTPACMAN_IMAGEANIMATION_H
#include "Animation.h"
#include "Image.h"
namespace View {

    class ImageAnimation: public Animation{
    public:
        ImageAnimation(const Logic::Vector2D& position, const Logic::Vector2D& size, double delay, std::vector<std::unique_ptr<Image>> images);
        void render() const override;
    private:
        mutable int animation_index = 0;
        std::vector<std::unique_ptr<Image>> images;

    };

} // View

#endif //PROJECTPACMAN_IMAGEANIMATION_H
