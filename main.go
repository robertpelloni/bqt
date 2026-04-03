package main

import (
	"log"
	"gioui.org/app"
	"gioui.org/f32"
	"github.com/robertpelloni/bobui/internal/kernel"
	"github.com/robertpelloni/bobui/internal/net"
	"github.com/robertpelloni/bobui/internal/ui"
	"github.com/robertpelloni/bobui/internal/ui/widgets"
)

func main() {
	log.Println("OmniUI Go: Launching Transcendent Singularity...")

	// 1. Setup Kernel
	im := kernel.GetInputManager()
	im.RegisterDevice("sys-mouse-0", "Super Admin", kernel.Mouse)
	
	// 2. Setup Distributed Nodes
	ne := &widgets.NodeEditor{
		Nodes: []widgets.Node{
			{ID: "node_1", Pos: f32.Pt(100, 100), Label: "Kernel Input"},
			{ID: "node_2", Pos: f32.Pt(400, 200), Label: "Audio Output"},
		},
	}
	log.Printf("OmniNode Go: Visual Scripting Engine Initialized.")

	// 3. Start Distributed Mesh
	net.GetMeshNode().StartNode("8081", nil)

	// 4. Launch GPU Engine
	go func() {
		engine := ui.NewEngine()
		if err := engine.Run(); err != nil {
			log.Fatal(err)
		}
	}()

	app.Main()
}
