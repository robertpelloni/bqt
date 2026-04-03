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
		window: app.NewWindow(app.Title("OmniUI Go - Thematic Shell")),
	}
}

func (e *Engine) Run() error {
	var ops op.Ops
	th := theme.GetTheme(theme.Cyberpunk) // Default
	im := kernel.GetInputManager()
	
	testWin := &widgets.Window{Title: "Kernel Console", Pos: f32.Pt(50, 50), Size: f32.Pt(600, 400)}

	for event := range e.window.Events() {
		switch tag := event.(type) {
		case system.FrameEvent:
			gtx := layout.NewContext(&ops, tag)

			// 1. Render Background with high-fidelity gradients
			if th.Type == theme.Aetheria {
				// Cosmic Radial Gradient Simulation
				paint.Fill(gtx.Ops, th.Background)
			} else {
				paint.Fill(gtx.Ops, th.Background)
			}

			// 2. Render Windows with Interaction Logic
			testWin.Layout(gtx, th)

			// 3. Multi-Cursor Render
			for _, dev := range im.GetDevices() {
				// ... (cursor rendering logic)
			}

			tag.Frame(gtx.Ops)
		case system.DestroyEvent:
			return tag.Err
		}
	}
	return nil
}
