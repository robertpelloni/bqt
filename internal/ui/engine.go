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
	time   float64
}

func NewEngine() *Engine {
	return &Engine{
		window: app.NewWindow(app.Title("OmniUI Go - 3D Vision")),
	}
}

func (e *Engine) Run() error {
	var ops op.Ops
	th := theme.GetTheme(theme.Cyberpunk)
	im := kernel.GetInputManager()
	
	// Vision Engines
	rm := &widgets.Raymarcher{}

	for event := range e.window.Events() {
		switch tag := event.(type) {
		case system.FrameEvent:
			gtx := layout.NewContext(&ops, tag)
			e.time += 0.016
			rm.Time = e.time

			// 1. Background
			paint.Fill(gtx.Ops, th.Background)

			// 2. Render Parallel 3D Scene
			rm.Layout(gtx)

			// 3. Render Multi-Cursor
			for _, dev := range im.GetDevices() {
				// ... (cursor logic)
			}

			op.InvalidateOp{}.Add(gtx.Ops)
			tag.Frame(gtx.Ops)
		case system.DestroyEvent:
			return tag.Err
		}
	}
	return nil
}
