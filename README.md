# 📸 Cordshot

A lightweight, modern screenshot tool for Windows. Capture any region of your screen with just two clicks.

![Version](https://img.shields.io/badge/version-1.1.0-blue.svg)
![Platform](https://img.shields.io/badge/platform-Windows-lightgrey.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)

## ✨ Features

- **Region Capture** - Click and drag or click two points to capture any area
- **Auto-Save** - Configure a folder for automatic screenshot saving
- **Clipboard Copy** - Screenshots are automatically copied to clipboard
- **System Tray** - Runs quietly in the background, always ready
- **Modern UI** - Clean, dark-themed interface
- **Portable** - No installation required (or use the installer)

## 📥 Download

Download the latest release from [Release](https://github.com/williampepple1/cordshot/blob/main/Cordshot-1.1.0-Setup.exe):

- **Cordshot-1.1.0-Setup.exe** - Windows installer 

## 🚀 Quick Start

1. **Install** - Run the installer or extract the ZIP
2. **Launch** - Open Cordshot from Start Menu or desktop
3. **Capture** - Click "Capture Region" or click the tray icon
4. **Select** - Click and drag to select the area you want
5. **Done!** - Screenshot is saved and copied to clipboard

## 🖱️ Usage

### Capturing Screenshots

| Action | Result |
|--------|--------|
| Click **Capture Region** button | Start selection mode |
| Click tray icon (single click) | Quick capture |
| Click + Drag | Select region |
| Click two points | Define corners |
| **ESC** | Cancel capture |
| **Right-click** | Cancel capture |

### Save Location

1. Click **"Choose Folder..."** in the app
2. Select your preferred folder
3. Screenshots will auto-save with timestamps

If no folder is set, you'll be prompted to choose a location each time.

### System Tray

- **Single click** - Start capture
- **Double click** - Show window
- **Right click** - Menu (Capture, Show, Settings, Quit)

Closing the window minimizes to tray. Right-click tray → Quit to exit completely.

## 🔧 Building from Source

### Prerequisites

- [Qt 6.x](https://www.qt.io/download) with MinGW
- CMake 3.16+
- (Optional) [Inno Setup](https://jrsoftware.org/isinfo.php) for creating installer

### Build Steps

```bash
# Clone the repository
git clone https://github.com/yourusername/cordshot.git
cd cordshot/cordshot

# Create build directory
mkdir build && cd build

# Configure (adjust Qt path as needed)
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="C:/Qt/6.x.x/mingw_64"

# Build
cmake --build . --config Release
```

### Create Distribution

```bash
# Copy executable to dist folder
mkdir ../dist
copy Release/cordshot.exe ../dist/

# Bundle Qt dependencies
C:/Qt/6.x.x/mingw_64/bin/windeployqt.exe --release ../dist/cordshot.exe
```

### Create Installer

1. Install [Inno Setup](https://jrsoftware.org/isinfo.php)
2. Open `installer.iss` in Inno Setup
3. Compile (Ctrl+F9)
4. Find installer in `installer_output/`

## 📁 Project Structure

```
cordshot/
├── main.cpp              # Application entry point
├── mainwindow.cpp/h      # Main widget UI
├── screenshotoverlay.cpp/h # Full-screen capture overlay
├── CMakeLists.txt        # Build configuration
├── cordshot.ico          # Application icon
├── cordshot.rc           # Windows resource file
├── installer.iss         # Inno Setup installer script
└── generate_icon.py      # Icon generation script
```

## ⚙️ Configuration

Settings are stored in the Windows Registry:
- Location: `HKEY_CURRENT_USER\Software\Cordshot\Cordshot`
- `savePath` - Default save folder for screenshots

## 🎨 Screenshots

*Coming soon*

## 📋 Roadmap

- [ ] Keyboard shortcuts (global hotkeys)
- [ ] Annotation tools (arrows, text, blur)
- [ ] Upload to cloud services
- [ ] macOS support
- [ ] Linux support

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- Built with [Qt 6](https://www.qt.io/)
- Installer created with [Inno Setup](https://jrsoftware.org/isinfo.php)
- Icon generated with [Pillow](https://pillow.readthedocs.io/)

---

Made with ❤️ by [Your Name]
