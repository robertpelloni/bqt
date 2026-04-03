package main

import (
	"log"
	"gioui.org/app"
	"github.com/robertpelloni/bobui/internal/net"
	"github.com/robertpelloni/bobui/internal/ui"
	"github.com/robertpelloni/bobui/internal/audio"
)

func main() {
	log.Println("OmniUI Go: Orchestrating Temporal Singularity...")

	// 1. Start High-Precision Heartbeat
	clock := audio.GetMasterClock()
	clock.Start()

	// 2. Start Distributed Mesh
	net.GetMeshNode().StartNode("8081")

	// 3. Launch GPU Engine
	go func() {
		engine := ui.NewEngine()
		if err := engine.Run(); err != nil {
			log.Fatal(err)
		}
	}()

	app.Main()
}
