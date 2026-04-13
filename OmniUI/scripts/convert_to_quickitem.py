import os

base_dir = r"C:\Users\hyper\workspace\bobui\OmniUI\omnicore"
include_dir = os.path.join(base_dir, "include")
src_dir = os.path.join(base_dir, "src")

# Map of BobUI Widget bases to Quick Item bases
replacements = {
    "QPushButton": "QQuickItem",
    "QSlider": "QQuickItem",
    "QLineEdit": "QQuickItem",
    "QDial": "QQuickItem",
    "QCheckBox": "QQuickItem",
    "QComboBox": "QQuickItem",
    "QProgressBar": "QQuickItem",
    "QPlainTextEdit": "QQuickItem",
    "QCalendarWidget": "QQuickItem",
    "QWidget": "QQuickItem",
    "QSplitter": "QQuickItem"
}

# Update Headers
for root, _, files in os.walk(include_dir):
    for file in files:
        if file.endswith(".h") and file != "OmniApp.h" and file != "OmniInputManager.h" and file != "OmniButton.h" and file != "JuceWidget.h":
            path = os.path.join(root, file)
            with open(path, "r") as f:
                content = f.read()
            
            for old_base, new_base in replacements.items():
                content = content.replace(f"<{old_base}>", "<QQuickItem>")
                content = content.replace(f"public {old_base}", f"public {new_base}")
                content = content.replace(f"explicit {file[:-2]}(QWidget *parent = nullptr);", f"explicit {file[:-2]}(QQuickItem *parent = nullptr);")
                
            with open(path, "w") as f:
                f.write(content)

# Update Sources
for root, _, files in os.walk(src_dir):
    for file in files:
        if file.endswith(".cpp") and file != "OmniApp.cpp" and file != "OmniInputManager.cpp" and file != "OmniDeveloperOverlay.cpp" and file != "OmniQmlRegistration.cpp" and file != "OmniButton.cpp" and file != "JuceWidget.cpp":
            path = os.path.join(root, file)
            with open(path, "r") as f:
                content = f.read()
            
            for old_base, new_base in replacements.items():
                content = content.replace(f"{file[:-4]}(QWidget *parent)", f"{file[:-4]}(QQuickItem *parent)")
                content = content.replace(f"{old_base}(parent)", f"{new_base}(parent)")
                
            with open(path, "w") as f:
                f.write(content)

print("Converted all legacy QWidget bases to QQuickItem successfully.")
