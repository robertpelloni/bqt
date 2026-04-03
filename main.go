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
	log.Println("OmniUI Go: Booting Sensory Kernel...")

	// 1. Setup Sensory Engine
	voice := audio.GetVoiceEngine()
	voice.StartListening()

	// 2. Setup Input & Net
	im := kernel.GetInputManager()
	im.RegisterDevice("sys-mouse-0", "Local Admin", kernel.Mouse)
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
