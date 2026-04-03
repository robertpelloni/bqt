package main

import (
	"log"
	"gioui.org/app"
	"github.com/robertpelloni/bobui/internal/kernel"
	"github.com/robertpelloni/bobui/internal/net"
	"github.com/robertpelloni/bobui/internal/ui"
	"github.com/robertpelloni/bobui/internal/vm"
)

func main() {
	log.Println("OmniUI Go: Orchestrating Final Shell Singularity...")

	// 1. Setup Input & Net
	im := kernel.GetInputManager()
	im.RegisterDevice("sys-mouse-0", "Super Admin", kernel.Mouse)
	net.GetMeshNode().StartNode("8081", nil, nil)

	// 2. AI Action Test (Simulation)
	vm.ExecuteAIAction("SystemBtn", "click")

	// 3. Launch GPU Engine with Aetheria Design
	go func() {
		engine := ui.NewEngine()
		if err := engine.Run(); err != nil {
			log.Fatal(err)
		}
	}()

	app.Main()
}
