#include "glbase/glbase.h"

int main(int argc, char **argv)
{
    Scene scene;

    ObjRender lightSource("resource/model/cube.obj", "resource/shader/mvp.vert", "resource/shader/plain.frag");

    auto trans = AnimatorActor::getActor(AnimationType::Translate, "trans");
    trans->setOrigin({0, 0.2f, 0});
    auto scale = AnimatorActor::getActor(AnimationType::Scale, "scale");
    scale->setOrigin({ 0.1f, 0.1f, 0.1f });
    auto rotate = AnimatorActor::getActor(AnimationType::Rotate, "rotate");
    rotate->setOrigin({0.5f, 0.7f, 0.9f});
    rotate->setSpeed(0.9f);
    auto trans2 = AnimatorActor::getActor(AnimationType::Translate, "trans2");
    trans2->setOrigin({0.5f, 0, 0});
    lightSource.animator.addDynamicActor(trans);
    lightSource.animator.addDynamicActor(rotate);
    lightSource.animator.addDynamicActor(trans2);
    lightSource.animator.addDynamicActor(scale);

    ObjRender cylinder("resource/model/cube.obj", "resource/shader/mvp.vert", "resource/shader/light.frag");
    auto cyScale = AnimatorActor::getActor(AnimationType::Scale, "cylinder-scale");
    cyScale->setOrigin({0.3f, 0.3f, 0.3f});
    cylinder.addAnimationActor(cyScale);

    scene.addModel(cylinder);
    scene.addModel(lightSource);

    scene.draw();

    return 0;
}

