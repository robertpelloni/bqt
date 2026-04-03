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
	return &Engine{window: app.NewWindow(app.Title("OmniUI Go - Collective Mesh"))}
}

func (e *Engine) Run() error {
	var ops op.Ops
	th := theme.GetTheme(theme.Cyberpunk)
	im := kernel.GetInputManager()
	nc := widgets.GetNotificationCenter()

	for event := range e.window.Events() {
		switch tag := event.(type) {
		case system.FrameEvent:
			gtx := layout.NewContext(&ops, tag)
			paint.Fill(gtx.Ops, th.Background)

			// 1. Render Window manager
			// ... (window rendering)

			// 2. Render Notifications (Toast Overlay)
			nc.Layout(gtx, th)

			// 3. Render Multi-Cursor
			for _, dev := range im.GetDevices() {
				// ... (cursor rendering)
			}

			tag.Frame(gtx.Ops)
		case system.DestroyEvent:
			return tag.Err
		}
	}
	return nil
}
