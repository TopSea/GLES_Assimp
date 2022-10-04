//
// Created by Gao on 2022/8/5.
//
#include "EGL/egl.h"
#include "GLES3/gl32.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../learnopengl/shader_m.h"
#include "../learnopengl/camera.h"
#include "../learnopengl/animator.h"
#include "../learnopengl/animation.h"
#include "../learnopengl/model_animation.h"

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include "../learnopengl/my_log.h"
#include "../learnopengl/animation.h"
#include <jni.h>

using namespace cv;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 8.0f));
Shader* ourShader = nullptr;
Model* ourModel = nullptr;
Animator* animator = nullptr;
//update animation firstly
//    float deltaTime = 0.03f; // 播放动画的速度 30帧每秒
//    float deltaTime = 0.024f; // 24帧每秒
float deltaTime = 0.001f; //

void initGL() {
    glEnable(GL_DEPTH_TEST);
    string vshader_path = string(INTERNAL_PATH).append("shader/vShader.src");
    string fshader_path = string(INTERNAL_PATH).append("shader/fShader.src");
    ourShader = new Shader(vshader_path.c_str(),
                           fshader_path.c_str());

    // load models
    ourModel = new Model(std::string(INTERNAL_PATH).append("model/cowboy/cowboy.dae"));
    animator = new Animator(new Animation(std::string(INTERNAL_PATH).append("model/cowboy/cowboy.dae"), &*ourModel));

}


int widthViewPort, heightViewPort;
void setViewPortSize(int width, int height) {
    glViewport(0, 0, width, height);
    widthViewPort = width;
    heightViewPort = height;
}

void draw() {

    //根据时间戳，计算 动画矩阵
    animator->UpdateAnimation(deltaTime);
    // render
    // ------
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // don't forget to enable shader before setting uniforms
    ourShader->use();

    // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)widthViewPort / (float)heightViewPort, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    ourShader->setMat4("projection", projection);
    ourShader->setMat4("view", view);

    auto transforms = animator->GetFinalBoneMatrices();
    for (int i = 0; i < transforms.size(); ++i)
        ourShader->setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);


    // render the loaded model
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f)); // translate it down so it's at the center of the scene
    model = glm::scale(model, glm::vec3(.3f, .3f, .3f));	// it's a bit too big for our scene, so scale it down
    ourShader->setMat4("model", model);
    ourModel->Draw(*ourShader);
}

extern "C"
JNIEXPORT void JNICALL
Java_top_topsea_gles_1assimp_MainActivity_changeFrameRate(JNIEnv *env, jobject thiz, jfloat frame) {
    deltaTime = frame;
}