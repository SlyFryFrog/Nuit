# Nuit

## What is Nuit?

Nuit is a simplistic OpenGL/GLFW wrapper. It features an input manager, windowing system, and OpenGL renderer context.

## Example CMakeUserPresets.json

```json
{
  "version": 8,
  "configurePresets": [
    {
      "name": "config",
      "inherits": "abstract",
      "hidden": true,
      "environment": {
        "VCPKG_ROOT": "~/.vcpkg-clion/vcpkg"
      },
      "cacheVariables": {
        "BUILD_SANDBOX": "ON"
      }
    },
    {
      "name": "DebugLinux",
      "inherits": [
        "config",
        "Debug"
      ]
    },
    {
      "name": "ReleaseLinux",
      "inherits": [
        "config",
        "Release"
      ]
    }
  ]
}
```