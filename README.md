# PitchCorrectionPlugin — VS Code Developer Cheat Sheet (created by ChatGPT)

A simple reference for continuing development of **PitchCorrectionPlugin**, a JUCE C++ pitch-correction audio plugin on **Windows**.

**Your tools:** VS Code (editor), CMake (build system), Visual Studio 2022 C++ compiler (builds the code), JUCE (audio framework), FL Studio (VST3 testing) and GitHub Desktop (version control).

> **Important:** Always open the **PitchCorrectionPlugin main folder** in VS Code — **not** its `JUCE` subfolder. Keep your existing working `CMakeLists.txt`; you do not need to recreate the project each day.

---

## 1. Open your project

1. Open **VS Code** → **File → Open Folder** → select `PitchCorrectionPlugin`.
2. Open a terminal: **Terminal → New Terminal** (shortcut: **Ctrl + `**).
3. Check you are in the folder containing your project's `CMakeLists.txt` and `Source` folder.

Example, if you are using the same location as before:

```powershell
cd "C:\Users\alfie\Desktop\JUCE_PROJECTS\PitchCorrectionPlugin"
```

If you moved or renamed the folder, use its new location instead.

Your project should roughly contain:

```text
PitchCorrectionPlugin/
├── CMakeLists.txt          <-- YOUR plugin build settings
├── Source/
│   ├── PluginProcessor.cpp
│   ├── PluginProcessor.h
│   ├── PluginEditor.cpp
│   └── PluginEditor.h
├── JUCE/
│   ├── CMakeLists.txt      <-- JUCE's own file: leave this alone
│   └── modules/
├── .gitignore
└── build/                  <-- generated; not committed to Git
```

The JUCE folder may instead be located elsewhere if your existing `CMakeLists.txt` references an external JUCE installation.

---

## 2. Build and RUN the Standalone plugin (everyday workflow)

**Use this while writing/debugging your autocorrelation pitch detector.** It opens your plugin as a Windows application without FL Studio.

### First configure (or after changing CMake settings)

```powershell
cmake -S . -B build -A x64
```

> If the `build` directory is **already configured successfully**, you normally do **not** need to reconfigure or supply `-A x64` every time. If the directory was configured using a different generator, reuse that generator or clean the build directory first (see troubleshooting).

### Build Standalone (Debug)

```powershell
cmake --build build --config Debug --target PitchCorrectionPlugin_Standalone
```

### Run Standalone

The **usual JUCE CMake output path** is:

```powershell
.\build\PitchCorrectionPlugin_artefacts\Debug\Standalone\PitchCorrectionPlugin.exe
```

If you get a “file not found” error, **find the actual executable** instead of guessing:

```powershell
Get-ChildItem .\build -Recurse -Filter PitchCorrectionPlugin.exe | Select-Object -ExpandProperty FullName
```

Then run the returned path (put it inside `& "..."` if there are spaces), for example:

```powershell
& "C:\path\returned\by\the\command\PitchCorrectionPlugin.exe"
```

**Typical daily routine:** edit C++ → **Ctrl + S** → run the build command → launch Standalone → test. Close the running app before rebuilding if Windows reports a locked file.

### Alternative: VS Code buttons

With the **CMake Tools** extension installed:

1. **Ctrl + Shift + P** → `CMake: Configure` (initial setup/after edits to CMakeLists.txt).
2. `CMake: Set Build Target` → choose `PitchCorrectionPlugin_Standalone`.
3. `CMake: Build`.
4. `CMake: Set Launch/Debug Target` → choose the standalone executable (when offered).
5. `CMake: Launch` or `CMake: Debug`.

If launch-target detection is not available, use the terminal method above.

---

## 3. Build the VST3 for FL Studio

A **VST3 cannot run by itself**; FL Studio (or another plugin host) loads it.

### Build VST3 (Release)

```powershell
cmake --build build --config Release --target PitchCorrectionPlugin_VST3
```

### Find the generated `.vst3` file/folder

Typical JUCE CMake output:

```text
build\PitchCorrectionPlugin_artefacts\Release\VST3\PitchCorrectionPlugin.vst3
```

To locate the actual output on your PC:

```powershell
Get-ChildItem .\build -Recurse -Filter *.vst3 | Select-Object -ExpandProperty FullName
```

A `.vst3` on Windows can be a **bundle directory** even if File Explorer makes it look like a single file; **copy the entire `.vst3` bundle**, not just a DLL from inside it.

### Install it for FL Studio

The standard 64-bit VST3 folder on Windows is:

```text
C:\Program Files\Common Files\VST3\
```

1. Close FL Studio if it is using the plugin.
2. If your build **did not automatically copy the VST3**, copy `PitchCorrectionPlugin.vst3` into the folder above (administrator permission may be requested).
3. Open FL Studio → **Options → Manage plugins** → **Find installed plugins** (plugin scan).
4. Load **PitchCorrectionPlugin** on an audio/mixer track.

> If your `CMakeLists.txt` sets `COPY_PLUGIN_AFTER_BUILD TRUE`, the build may already install/copy your VST3. Check before copying manually.

**After each VST3 code change:** rebuild Release → copy/update bundle if necessary → rescan or reload in FL Studio. Some hosts keep plugins loaded, so closing/reopening FL Studio may be necessary.

---

## 4. Debug vs Release

| Configuration | Best for | Command target |
| --- | --- | --- |
| **Debug** | Testing and debugging your Standalone app in VS Code | `PitchCorrectionPlugin_Standalone` |
| **Release** | Normal VST3 testing in FL Studio | `PitchCorrectionPlugin_VST3` |

**Both formats must be listed in your project's existing `juce_add_plugin(...)` settings:**

```cmake
FORMATS VST3 Standalone
```

Do not replace your entire working `juce_add_plugin(...)` block just to add this line.

---

## 5. Where to write code

- **`Source/PluginProcessor.cpp` / `.h`** — audio processing and pitch-detection code. The `processBlock(...)` function is where audio buffers arrive.
- **`Source/PluginEditor.cpp` / `.h`** — interface, buttons, sliders and displaying detected pitch.
- **New `Source/` files** — good place for a separate autocorrelation detector class. If you add new `.cpp`/`.h` files, include them in your CMake source list and reconfigure.
- **Root `CMakeLists.txt`** — add source files, libraries, JUCE modules and plugin settings.
- **`JUCE/`** — framework source. Generally **do not edit** JUCE's files.
- **`build/`** — generated output. **Do not edit or commit** this folder.

For your autocorrelation prototype, a sensible progression is: detect a sine wave frequency → verify known test frequencies → display the result → handle silent/noisy input → measure performance and latency.

---

## 6. GitHub Desktop after development

1. Save your files in VS Code.
2. Open **GitHub Desktop** and select the PitchCorrectionPlugin repository.
3. Review **Changes** (make sure `build/` isn't included).
4. Enter a **Conventional Commit** summary and click **Commit to main**.
5. Click **Push origin**.

Examples:

```text
feat(detection): add autocorrelation pitch detection
fix(detection): handle silence in frequency estimation
test(detection): add known-frequency tests
feat(ui): display detected frequency
build: update JUCE CMake configuration
docs: document prototype test results
```

If you make an experiment that fails, you can still commit what you learned, using an accurate commit message. Keep screenshots, test measurements and notes as coursework evidence where appropriate.

**Check `.gitignore`:** include `/build/`, `/cmake-build-*/`, compiled plugin outputs, and `.vs/`. If `JUCE/` is a Git submodule, ensure its `.gitmodules` file is committed. If JUCE is an ordinary source copy, ensure it is not accidentally ignored. Do not delete JUCE from your working project just to clean Git status.

---

## 7. Fixes for errors you already encountered

### Error A — CMake source directory/cache mismatch

Typical message:

```text
The source .../PitchCorrectionPlugin/CMakeLists.txt does not match
.../PitchCorrectionPlugin/JUCE/CMakeLists.txt used to generate cache
```

**Cause:** CMake configured `JUCE/` as the main project instead of `PitchCorrectionPlugin/` and cached it.

**Fix:** In the **project root**, first close the app and check that `build/` is just a generated CMake build folder. Then run:

```powershell
Remove-Item -Recurse -Force .\build
cmake -S . -B build -A x64
```

This deletes **only generated build output**. Never delete `Source/`, `CMakeLists.txt`, or your `JUCE/` framework folder.

If you use **CMake Tools** in VS Code, ensure the workspace is the main `PitchCorrectionPlugin` folder, not `JUCE/`.

### Error B — `MSB1009: Project file does not exist`

Typical message:

```text
MSBUILD : error MSB1009: Project file does not exist.
Switch: PitchCorrectionPlugin_Standalone.vcxproj
```

**Cause:** The Standalone target was not generated in that build directory, or CMake was configured against the wrong project.

**Fix:**

1. Confirm your project `CMakeLists.txt` specifies `FORMATS VST3 Standalone`.
2. From the project root, run:

```powershell
cmake -S . -B build -A x64
cmake --build build --config Debug --target PitchCorrectionPlugin_Standalone
```

3. If the error persists, use **Error A's cleanup** and configure again.
4. You can check available targets:

```powershell
cmake --build build --config Debug --target help
```

If `PitchCorrectionPlugin_Standalone` is absent, check configuration output and your `juce_add_plugin` settings.

### Error C — `cmake` is not recognised

Install CMake from https://cmake.org/download/ and choose the installer option to add CMake to PATH, then **restart VS Code**. Check:

```powershell
cmake --version
```

### Error D — Compiler not found / CMake cannot configure

In **Visual Studio Installer**, confirm **Desktop development with C++** and the Windows SDK are installed. Restart VS Code, select a compatible Visual Studio 2022 kit/generator, and configure again.

### Error E — Build succeeded but no plugin appears in FL Studio

Check that you built `PitchCorrectionPlugin_VST3` (not just Standalone), locate the `.vst3` bundle with the search command, ensure it is in the recognised VST3 folder, and rescan in FL Studio.

---

## 8. Quick copy-and-paste command reference

Run these **from the project root** in the VS Code PowerShell terminal:

**Configure (new project, changed CMake files, or after clearing cache):**

```powershell
cmake -S . -B build -A x64
```

**Build Standalone for debugging:**

```powershell
cmake --build build --config Debug --target PitchCorrectionPlugin_Standalone
```

**Run Standalone (usual path):**

```powershell
.\build\PitchCorrectionPlugin_artefacts\Debug\Standalone\PitchCorrectionPlugin.exe
```

**Build VST3 for FL Studio:**

```powershell
cmake --build build --config Release --target PitchCorrectionPlugin_VST3
```

**Find VST3 output:**

```powershell
Get-ChildItem .\build -Recurse -Filter *.vst3 | Select-Object -ExpandProperty FullName
```

**Find Standalone output:**

```powershell
Get-ChildItem .\build -Recurse -Filter PitchCorrectionPlugin.exe | Select-Object -ExpandProperty FullName
```

---

## 9. Starting a *different* JUCE project later

1. Create a **new project root folder** with its own `CMakeLists.txt` and `Source/`.
2. Point it to JUCE (via `add_subdirectory(JUCE)` for a local copy, or your existing external dependency configuration).
3. Set its own plugin **target name**, `PRODUCT_NAME`, and valid unique 4-character plugin identifiers. Include `FORMATS VST3 Standalone` if you need both.
4. Open the **new project root**, not JUCE, in VS Code.
5. Configure/build using the same commands, **replacing `PitchCorrectionPlugin` with the new CMake target name**.
6. Use GitHub Desktop to create/publish its own repository and add `.gitignore` **before** committing build output.

**Official references:**
- JUCE: https://juce.com/
- JUCE CMake API: https://github.com/juce-framework/JUCE/blob/master/docs/CMake%20API.md
- JUCE plugin CMake example: https://github.com/juce-framework/JUCE/blob/master/examples/CMake/AudioPlugin/CMakeLists.txt
- VS Code CMake Tools: https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools
- GitHub Desktop: https://desktop.github.com/

**Rule to remember:** Open the right folder → configure only when needed → build Standalone → run and test → build VST3 → test in FL Studio → commit and push.
