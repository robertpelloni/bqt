package main

import (
	"log"
	"gioui.org/app"
	"github.com/robertpelloni/bobui/internal/kernel"
	"github.com/robertpelloni/bobui/internal/net"
	"github.com/robertpelloni/bobui/internal/ui"
	"github.com/robertpelloni/bobui/internal/audio"
)

func main() {
	log.Println("OmniUI Go: Orchestrating Multimedia Singularity...")

	// 1. Setup Audio Graph
	ag := audio.GetAudioGraph()
	log.Printf("OmniAudio Go: Kernel Topology Initialized.")

	// 2. Setup Input & Security
	im := kernel.GetInputManager()
	pm := kernel.GetPermissionManager()
	im.RegisterDevice("sys-mouse-0", "Local Admin", kernel.Mouse)
	log.Printf("OmniSecurity Go: Lock Manager Active.")

	// 3. Start Distributed Mesh
	net.GetMeshNode().StartNode("8081")

	// 4. Launch GPU Engine
	go func() {
		engine := ui.NewEngine()
		if err := engine.Run(); err != nil {
			log.Fatal(err)
		}
	}()

	app.Main()
}
