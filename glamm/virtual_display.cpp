

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

  if (!eglBindAPI(EGL_OPENGL_ES_API)) {
    throw std::runtime_error("Unable to bind to EGL_OPENGL_ES_API.");
  }

  //

  EGLint egl_config_attribs[] = {
    EGL_BUFFER_SIZE,
    32,
    EGL_DEPTH_SIZE,
    EGL_DONT_CARE,
    EGL_STENCIL_SIZE,
    EGL_DONT_CARE,
    EGL_RENDERABLE_TYPE,
    EGL_OPENGL_ES3_BIT,
    EGL_SURFACE_TYPE,
    EGL_WINDOW_BIT,
    EGL_NONE,
  };

  EGLint num_configs;
  if (!eglGetConfigs(this->egl_display_, NULL, 0, &num_configs)) {
    throw std::runtime_error("Unable to find any configurations.");
  }

  EGLConfig* configs = (EGLConfig*)malloc(num_configs * sizeof(EGLConfig));
  if (!eglChooseConfig(this->egl_display_,
                       egl_config_attribs,
                       configs,
                       num_configs,
                       &num_configs)) {
    throw std::runtime_error("Unable to fetch the configurations.");
  }
  if (num_configs == 0) {
    throw std::runtime_error("No configurations were returned.");
  }

  bool has_match = false;
  std::cout << num_configs << " egl configs found." << std::endl;
  for (int i = 0; i < num_configs; ++i) {
    EGLint gbm_format;

    if (!eglGetConfigAttrib(
          this->egl_display_, configs[i], EGL_NATIVE_VISUAL_ID, &gbm_format)) {
      throw std::runtime_error("Unable to access configuration attributes.");
    }

    // std::cout << "gbm_format: ";
    // switch (gbm_format) {
    //   case GBM_FORMAT_XRGB8888:
    //     std::cout << "GBM_FORMAT_XRGB8888";
    //     break; //('X', 'R', '2', '4') /* [31:0] x:R:G:B 8:8:8:8 little endian
    //     */
    //   case GBM_FORMAT_XBGR8888:
    //     std::cout << "GBM_FORMAT_XBGR8888";
    //     break; //('X', 'B', '2', '4') /* [31:0] x:B:G:R 8:8:8:8 little endian
    //     */
    //   case GBM_FORMAT_RGBX8888:
    //     std::cout << "GBM_FORMAT_RGBX8888";
    //     break; //('R', 'X', '2', '4') /* [31:0] R:G:B:x 8:8:8:8 little endian
    //     */
    //   case GBM_FORMAT_BGRX8888:
    //     std::cout << "GBM_FORMAT_BGRX8888";
    //     break; //('B', 'X', '2', '4') /* [31:0] B:G:R:x 8:8:8:8 little endian
    //     */

    //   case GBM_FORMAT_ARGB8888:
    //     std::cout << "GBM_FORMAT_ARGB8888";
    //     break; //('A', 'R', '2', '4') /* [31:0] A:R:G:B 8:8:8:8 little endian
    //     */
    //   case GBM_FORMAT_ABGR8888:
    //     std::cout << "GBM_FORMAT_ABGR8888";
    //     break; //('A', 'B', '2', '4') /* [31:0] A:B:G:R 8:8:8:8 little endian
    //     */
    //   case GBM_FORMAT_RGBA8888:
    //     std::cout << "GBM_FORMAT_RGBA8888";
    //     break; //('R', 'A', '2', '4') /* [31:0] R:G:B:A 8:8:8:8 little endian
    //     */
    //   case GBM_FORMAT_BGRA8888:
    //     std::cout << "GBM_FORMAT_BGRA8888";
    //     break; //('B', 'A', '2', '4') /* [31:0] B:G:R:A 8:8:8:8 little endian
    //     */

    //   case GBM_FORMAT_XRGB2101010:
    //     std::cout << "GBM_FORMAT_XRGB2101010";
    //     break; //('X', 'R', '3', '0') /* [31:0] x:R:G:B 2:10:10:10 little
    //     endian
    //            //*/
    //   case GBM_FORMAT_XBGR2101010:
    //     std::cout << "GBM_FORMAT_XBGR2101010";
    //     break; //('X', 'B', '3', '0') /* [31:0] x:B:G:R 2:10:10:10 little
    //     endian
    //            //*/
    //   case GBM_FORMAT_RGBX1010102:
    //     std::cout << "GBM_FORMAT_RGBX1010102";
    //     break; //('R', 'X', '3', '0') /* [31:0] R:G:B:x 10:10:10:2 little
    //     endian
    //            //*/
    //   case GBM_FORMAT_BGRX1010102:
    //     std::cout << "GBM_FORMAT_BGRX1010102";
    //     break; //('B', 'X', '3', '0') /* [31:0] B:G:R:x 10:10:10:2 little
    //     endian
    //            //*/

    //   case GBM_FORMAT_ARGB2101010:
    //     std::cout << "GBM_FORMAT_ARGB2101010";
    //     break; //('A', 'R', '3', '0') /* [31:0] A:R:G:B 2:10:10:10 little
    //     endian
    //            //*/
    //   case GBM_FORMAT_ABGR2101010:
    //     std::cout << "GBM_FORMAT_ABGR2101010";
    //     break; //('A', 'B', '3', '0') /* [31:0] A:B:G:R 2:10:10:10 little
    //     endian
    //            //*/
    //   case GBM_FORMAT_RGBA1010102:
    //     std::cout << "GBM_FORMAT_RGBA1010102";
    //     break; //('R', 'A', '3', '0') /* [31:0] R:G:B:A 10:10:10:2 little
    //     endian
    //            //*/
    //   case GBM_FORMAT_BGRA1010102:
    //     std::cout << "GBM_FORMAT_BGRA1010102";
    //     break;
    //   default:
    //     std::cout << "unknown";
    // }
    // std::cout << std::endl;

    if (gbm_format == GBM_FORMAT_ARGB8888) {
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

  //

  // TODO: promote hard coded window size to inputs
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
                                            2,
                                            EGL_NONE };

  this->egl_context_ = eglCreateContext(
    this->egl_display_, this->egl_config_, NULL, context_attribs);
  if (!this->egl_context_)
    throw std::runtime_error("Unable to create egl context.");

  eglMakeCurrent(this->egl_display_,
                 this->egl_surface_,
                 this->egl_surface_,
                 this->egl_context_);
}

}