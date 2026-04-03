package main

import (
	"log"
	"gioui.org/app"
	"github.com/robertpelloni/bobui/internal/kernel"
	"github.com/robertpelloni/bobui/internal/net"
	"github.com/robertpelloni/bobui/internal/ui"
	"github.com/robertpelloni/bobui/internal/data"
	"github.com/robertpelloni/bobui/internal/vm"
)

func main() {
	log.Println("OmniUI Go: Initializing Final Singularity Kernel...")

	// 1. Setup Database
	db := data.GetDatabase()
	db.Connect("omni_go_system.db")

	// 2. Setup VM
	lisp := vm.NewVM()
	res, _ := lisp.Eval([]interface{}{"+", 10.0, 32.0})
	log.Printf("OmniVM Go: Boot Diagnostics (10 + 32) = %v", res)

	// 3. Setup Input & Net
	im := kernel.GetInputManager()
	im.RegisterDevice("sys-mouse-0", "Local Admin", kernel.Mouse)
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
