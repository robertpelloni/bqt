package main

import (
	"log"
	"time"
	"gioui.org/app"
	"github.com/robertpelloni/bobui/internal/net"
	"github.com/robertpelloni/bobui/internal/ui"
	"github.com/robertpelloni/bobui/internal/vm"
)

func main() {
	log.Println("OmniUI Go: Orchestrating AI Context Singularity...")

	// 1. Setup Net & Input
	mesh := net.GetMeshNode()
	mesh.StartNode("8081", nil)

	// 2. Start Neural Streamer (10 FPS for LLM Awareness)
	ne := vm.GetNeuralEngine()
	go func() {
		for {
			dom, _ := ne.CaptureDOM()
			// Broadcast JSON DOM over local OS Server
			mesh.Broadcast(map[string]interface{}{
				"type": "neural_dom",
				"payload": dom,
			})
			time.Sleep(100 * time.Millisecond)
		}
	}()

	// 3. Launch GPU Engine
	go func() {
		engine := ui.NewEngine()
		if err := engine.Run(); err != nil {
			log.Fatal(err)
		}
	}()

	app.Main()
}
