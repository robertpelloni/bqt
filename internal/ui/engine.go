package ui

import (
	"gioui.org/app"
	"gioui.org/io/system"
	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/op/paint"
	"github.com/robertpelloni/bobui/internal/ui/theme"
	"github.com/robertpelloni/bobui/internal/ui/widgets"
)

type Engine struct {
	window *app.Window
	auth   *widgets.AuthShell
}

func NewEngine() *Engine {
	return &Engine{
		window: app.NewWindow(app.Title("OmniUI Go - Kernel Root")),
		auth:   &widgets.AuthShell{},
	}
}

func (e *Engine) Run() error {
	var ops op.Ops
	th := theme.GetTheme(theme.Cyberpunk)

	for event := range e.window.Events() {
		switch tag := event.(type) {
		case system.FrameEvent:
			gtx := layout.NewContext(&ops, tag)
			paint.Fill(gtx.Ops, th.Background)

			// 1. Process Secure Boot Guard
			e.auth.Layout(gtx, th)

			tag.Frame(gtx.Ops)
		case system.DestroyEvent:
			return tag.Err
		}
	}
	return nil
}
