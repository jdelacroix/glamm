# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Fixed
- EGL versions prior to 1.5 require casting to `EGLNativeDisplayType`

## [0.1.0] - 2023-07-25

### Added

- `BlitMapsShader` supports merging while drawing `OccupancyGridTextureMap`s
- Maps can be loaded from and saved to PGM image format
- Offscreen rendering without X11 using EGL and GBM
- Basic example using a chessboard

[Unreleased]: https://github.com/jdelacroix/glamm/compare/v0.1.0...HEAD
[0.1.0]: https://github.com/jdelacroix/glamm/releases/tag/v0.1.0