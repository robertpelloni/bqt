with open("internal/ui/widgets/demo_surface.go", "r") as f:
    content = f.read()

content = content.replace("Width: 300, Height: 200, Alignment: layout.Center", "")
content = content.replace("WidthDp: 250, Edge: layout.W", "WidthDp: 250")

with open("internal/ui/widgets/demo_surface.go", "w") as f:
    f.write(content)
