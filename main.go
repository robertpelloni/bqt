package main

import (
	"log"
	"gioui.org/app"
	"gioui.org/f32"
	"github.com/robertpelloni/bobui/internal/kernel"
	"github.com/robertpelloni/bobui/internal/net"
	"github.com/robertpelloni/bobui/internal/ui"
)

func main() {
	log.Println("OmniUI Go: Launching Distributed Singularity...")

	// 1. Setup Kernel Managers
	im := kernel.GetInputManager()
	im.RegisterDevice("sys-mouse-0", "Primary Admin", kernel.Mouse)
	
	wm := ui.GetWindowManager()

	// 2. Start Distributed Mesh with Remote Window Support
	mesh := net.GetMeshNode()
	mesh.StartNode("8081", func(id, title string, pos, size f32.Point, isRemote bool) {
		log.Printf("OMNIMESH: Remote Peer spawned window [%s]", title)
		wm.SpawnWindow(id, title, pos, size, true)
	})

	// 3. Launch GPU Engine
	go func() {
		engine := ui.NewEngine()
		if err := engine.Run(); err != nil {
			log.Fatal(err)
		}
	}()

	app.Main()
}
