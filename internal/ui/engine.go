package ui

import (
	"gioui.org/app"
	"gioui.org/io/system"
	"gioui.org/io/pointer"
	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/op/paint"
	"gioui.org/f32"
	"github.com/robertpelloni/bobui/internal/ui/theme"
	"github.com/robertpelloni/bobui/internal/ui/widgets"
)

type Engine struct {
	window *app.Window
	wm     *WindowManager
}

func NewEngine() *Engine {
	return &Engine{
		window: app.NewWindow(app.Title("OmniUI Go - Orchestrated Desktop")),
		wm:     GetWindowManager(),
	}
}

func (e *Engine) Run() error {
	var ops op.Ops
	th := theme.GetTheme(theme.Cyberpunk)
	shell := &widgets.Shell{}

	for event := range e.window.Events() {
		switch tag := event.(type) {
		case pointer.Event:
			// Handle Desktop-level hit testing
			if tag.Type == pointer.Press && shell.StartMenuVisible {
				// Spawn Terminal Example
				e.wm.SpawnWindow("Terminal", f32.Pt(200, 200), f32.Pt(500, 300))
			}
		case system.FrameEvent:
			gtx := layout.NewContext(&ops, tag)
			paint.Fill(gtx.Ops, th.Background)

			// 1. Render Active Window Stack
			e.wm.Layout(gtx, th)

			// 2. Render Shell Layer
			shell.LayoutTaskbar(gtx, th)
			shell.LayoutStartMenu(gtx, th)

			tag.Frame(gtx.Ops)
		case system.DestroyEvent:
			return tag.Err
		}
	}
	return nil
}
