package main

import (
	"log"
	"gioui.org/app"
	"github.com/robertpelloni/bobui/internal/kernel"
	"github.com/robertpelloni/bobui/internal/net"
	"github.com/robertpelloni/bobui/internal/ui"
)

func main() {
	log.Println("OmniUI Go: Booting Language-Native Kernel...")

	// 1. Setup Kernel
	im := kernel.GetInputManager()
	im.RegisterDevice("sys-mouse-0", "Local User", kernel.Mouse)

	// 2. Start Network Mesh
	mesh := net.GetMeshNode()
	mesh.StartNode("8081")

	// 3. Launch UI Engine
	go func() {
		engine := ui.NewEngine()
		if err := engine.Run(); err != nil {
			log.Fatal(err)
		}
	}()

	app.Main()
}
