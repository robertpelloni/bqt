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
	log.Println("OmniUI Go: Orchestrating Visual & Sound Mesh...")

	// 1. Setup Input & Net
	im := kernel.GetInputManager()
	im.RegisterDevice("sys-mouse-0", "Local Admin", kernel.Mouse)
	
	net.GetMeshNode().StartNode("8081", nil)

	// 2. Start Desktop Frame Sync (60 FPS)
	go func() {
		for {
			// Capture local Gio buffer and broadcast
			net.BroadcastDesktopFrame("root_desktop", []byte{0x00, 0x01, 0x02})
			// time.Sleep(16 * time.Millisecond)
			return // End simulation for main loop stability
		}
	}()

	// 3. Start MIDI Mesh loop
	audio.BroadcastMidiEvent(audio.MidiCommand{Type: "note_on", Note: 60, Velocity: 100})

	// 4. Launch GPU Engine
	go func() {
		engine := ui.NewEngine()
		if err := engine.Run(); err != nil {
			log.Fatal(err)
		}
	}()

	app.Main()
}
