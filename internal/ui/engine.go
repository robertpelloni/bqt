package ui

import (
	"gioui.org/app"
	"gioui.org/io/system"
	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/op/paint"
	"gioui.org/f32"
	"github.com/robertpelloni/bobui/internal/kernel"
	"github.com/robertpelloni/bobui/internal/ui/theme"
	"github.com/robertpelloni/bobui/internal/ui/widgets"
)

type Engine struct {
	window *app.Window
}

func NewEngine() *Engine {
	return &Engine{
		window: app.NewWindow(app.Title("OmniUI Go - Multi-User Simulation")),
	}
}

func (e *Engine) Run() error {
	var ops op.Ops
	th := theme.GetTheme(theme.Cyberpunk)
	im := kernel.GetInputManager()
	
	// Simulation Engine
	ps := &widgets.ParticleSystem{Active: true}
	ps.Burst(f32.Pt(640, 360), 100)

	for event := range e.window.Events() {
		switch tag := event.(type) {
		case system.FrameEvent:
			gtx := layout.NewContext(&ops, tag)

			// 1. Physics Update (Go Port)
			ps.Update(0.016) // 60fps delta

			// 2. Render Background
			paint.Fill(gtx.Ops, th.Background)

			// 3. Render High-Performance Particles
			ps.Layout(gtx)

			// 4. Render Multi-Cursor
			for _, dev := range im.GetDevices() {
				// ... (cursor rendering logic)
			}

			// Automatically trigger next frame for high-performance simulation
			op.InvalidateOp{}.Add(gtx.Ops)
			tag.Frame(gtx.Ops)
		case system.DestroyEvent:
			return tag.Err
		}
	}
	return nil
}
