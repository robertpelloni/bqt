package main

import (
	"log"
	"gioui.org/app"
	"github.com/robertpelloni/bobui/internal/kernel"
	"github.com/robertpelloni/bobui/internal/net"
	"github.com/robertpelloni/bobui/internal/ui"
)

// Example System Plugin
type CorePlugin struct{}
func (c *CorePlugin) Name() string { return "OmniCore-Go" }
func (c *CorePlugin) Version() string { return "1.0.0" }
func (c *CorePlugin) Initialize() error {
	log.Println("OmniCore-Go: System Kernel Extension Initialized.")
	return nil
}

func main() {
	log.Println("OmniUI Go: Orchestrating Extensible Singularity...")

	// 1. Setup Plugin Manager
	pm := kernel.GetPluginManager()
	pm.RegisterPlugin(&CorePlugin{})

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
