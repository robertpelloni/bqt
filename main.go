package main

import (
	"log"
	"gioui.org/app"
	"github.com/robertpelloni/bobui/internal/kernel"
	"github.com/robertpelloni/bobui/internal/net"
	"github.com/robertpelloni/bobui/internal/ui"
)

func main() {
	log.Println("OmniUI Go: Booting High-Concurrency Kernel...")

	// 1. Setup Kernel
	im := kernel.GetInputManager()
	im.RegisterDevice("sys-mouse-0", "Local User", kernel.Mouse)
	
	// 2. Setup Reactive State (JavaFX Parity)
	volume := kernel.NewProperty[float64](0.8)
	volume.AddListener(func(val float64) {
		log.Printf("OMNI STATE: System Volume shifted to %f", val)
	})

	// 3. Start Distributed Mesh & Voice (Remote Multiplayer OS)
	mesh := net.GetMeshNode()
	mesh.StartNode("8081")
	
	voice := net.GetVoiceMesh()
	go func() {
		// Simulation: Sending local voice data every second
		for {
			voice.BroadcastVoice([]byte{0xDE, 0xAD, 0xBE, 0xEF})
			// time.Sleep(1 * time.Second)
			return // End simulation for main loop stability
		}
	}()

	// 4. Launch GPU Engine
	go func() {
		engine := ui.NewEngine()
		if err := engine.Run(); err != nil {
			log.Fatal(err)
		}
	}()

	app.Main()
}
