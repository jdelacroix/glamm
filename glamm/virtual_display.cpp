

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

VirtualDisplay::VirtualDisplay()
{

  // https://registry.khronos.org/EGL/extensions/MESA/EGL_MESA_platform_gbm.txt

  // 0. check client extensions for compatibility
  // const char* client_extensions =
  //   eglQueryString(EGL_NO_DISPLAY, EGL_EXTENSIONS);

  // if (!client_extensions) {
  //   throw std::runtime_error("EGL_EXT_client_extensions is unsupported.");
  // }
  // if (!std::strstr(client_extensions, "EGL_MESA_platform_gbm")) {
  //   throw std::runtime_error("EGL_MESA_platform_gbm is unsupported.");
  // }

  //

  int fd = open("/dev/dri/card0", O_RDWR);
  if (fd < 0) {
    throw std::runtime_error("Unable to access /dev/dri/card0.");
  }

  this->gbm_device_ = gbm_create_device(fd);
  if (!this->gbm_device_) {
    throw std::runtime_error("Unable to create gbm device.");
  }

  // this->egl_display_ =
  //   eglGetPlatformDisplayEXT(EGL_PLATFORM_GBM_MESA, this->gbm_device_, NULL);
  this->egl_display_ = eglGetDisplay(this->gbm_device_);

  if (this->egl_display_ == EGL_NO_DISPLAY) {
    throw std::runtime_error("Unable to create egl display.");
  }

  EGLint major, minor;
  if (!eglInitialize(this->egl_display_, &major, &minor)) {
    throw std::runtime_error("Unable to initialize egl display.");
  } else {
    std::cout << "egl version: " << major << "." << minor << std::endl;
  }

  //

  EGLint egl_config_attribs[] = {
    EGL_BUFFER_SIZE,
    24,
    EGL_DEPTH_SIZE,
    EGL_DONT_CARE,
    EGL_STENCIL_SIZE,
    EGL_DONT_CARE,
    EGL_RENDERABLE_TYPE,
    EGL_OPENGL_ES2_BIT,
    EGL_SURFACE_TYPE,
    EGL_WINDOW_BIT,
    EGL_NONE,
  };

  EGLint num_configs;
  if (!eglGetConfigs(this->egl_display_, NULL, 0, &num_configs)) {
    throw std::runtime_error("Unable to fin any configurations.");
  }

  EGLConfig* configs = (EGLConfig*)malloc(num_configs * sizeof(EGLConfig));
  if (!eglChooseConfig(this->egl_display_,
                       egl_config_attribs,
                       configs,
                       num_configs,
                       &num_configs)) {
    abort();
  }
  if (num_configs == 0) {
    abort();
  }

  bool has_match = false;
  for (int i = 0; i < num_configs; ++i) {
    EGLint gbm_format;

    if (!eglGetConfigAttrib(
          this->egl_display_, configs[i], EGL_NATIVE_VISUAL_ID, &gbm_format)) {
      throw std::runtime_error("Unable to access configuration attributes.");
    }

    if (gbm_format == GBM_FORMAT_XRGB8888) {
      this->egl_config_ = configs[i];
      free(configs);
      has_match = true;
      break;
    }
  }

  if (!has_match) {
    throw std::runtime_error(
      "Unable to find a configuration with matching gbm format.");
  }
}

}