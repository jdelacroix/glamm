

/*
 * Copyright 2023 Jean-Pierre de la Croix
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "virtual_display.hpp"

#include <cstring>
#include <fcntl.h>
#include <stdexcept>

#include <iostream>

namespace glamm {

VirtualDisplay::VirtualDisplay(const std::string& dri_device)
{

  // https://registry.khronos.org/EGL/extensions/MESA/EGL_MESA_platform_gbm.txt

  int fd = open(dri_device.c_str(), O_RDWR);
  if (fd < 0) {
    throw std::runtime_error("Unable to access /dev/dri/card0.");
  }

  this->gbm_device_ = gbm_create_device(fd);
  if (!this->gbm_device_) {
    throw std::runtime_error("Unable to create gbm device.");
  }

  // this->egl_display_ =
  //   eglGetPlatformDisplayEXT(EGL_PLATFORM_GBM_MESA, this->gbm_device_, NULL);
  this->egl_display_ = eglGetDisplay((EGLNativeDisplayType)this->gbm_device_);
  // this->egl_dispay_ = eglGetDisplay(EGL_DEFAULT_DISPLAY);

  if (this->egl_display_ == EGL_NO_DISPLAY) {
    throw std::runtime_error("Unable to create egl display.");
  }

  EGLint major, minor;
  if (!eglInitialize(this->egl_display_, &major, &minor)) {
    throw std::runtime_error("Unable to initialize egl display.");
  } else {
    std::cout << "egl version: " << major << "." << minor << std::endl;
  }

  EGLint egl_config_attribs[] = { EGL_SURFACE_TYPE,
                                  EGL_WINDOW_BIT,
                                  EGL_BLUE_SIZE,
                                  8,
                                  EGL_GREEN_SIZE,
                                  8,
                                  EGL_RED_SIZE,
                                  8,
                                  EGL_ALPHA_SIZE,
                                  8,
                                  EGL_DEPTH_SIZE,
                                  EGL_DONT_CARE,
                                  EGL_RENDERABLE_TYPE,
                                  EGL_OPENGL_ES3_BIT,
                                  EGL_NONE };

  EGLint num_configs;
  eglChooseConfig(this->egl_display_,
                  egl_config_attribs,
                  &this->egl_config_,
                  1,
                  &num_configs);

  if (num_configs == 0) {
    throw std::runtime_error("No usable configurations were found.");
  }

  this->gbm_surface_ = gbm_surface_create(
    this->gbm_device_, 1000, 1000, GBM_FORMAT_ARGB8888, GBM_BO_USE_RENDERING);
  if (!this->gbm_surface_) {
    throw std::runtime_error("Unable to create gbm surface.");
  }

  this->egl_surface_ =
    eglCreateWindowSurface(this->egl_display_,
                           this->egl_config_,
                           (EGLNativeWindowType)this->gbm_surface_,
                           NULL);

  if (this->egl_surface_ == EGL_NO_SURFACE) {
    throw std::runtime_error("Unable to create egl surface.");
  }

  static const EGLint context_attribs[] = { EGL_CONTEXT_CLIENT_VERSION,
                                            3,
                                            EGL_NONE };

  if (!eglBindAPI(EGL_OPENGL_API)) {
    throw std::runtime_error("Unable to bind to EGL_OPENGL_ES_API.");
  }

  this->egl_context_ = eglCreateContext(
    this->egl_display_, this->egl_config_, EGL_NO_CONTEXT, context_attribs);
  if (!this->egl_context_) {
    throw std::runtime_error("Unable to create egl context.");
  }

  eglMakeCurrent(
    this->egl_display_, EGL_NO_SURFACE, EGL_NO_SURFACE, this->egl_context_);
}

}