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
	login  *widgets.LoginView
	rewind *widgets.RewindSlider
}

func NewEngine() *Engine {
	return &Engine{
		window: app.NewWindow(app.Title("OmniUI Go - Desktop Environment")),
		login:  &widgets.LoginView{},
		rewind: &widgets.RewindSlider{Active: true},
	}
}

func (e *Engine) Run() error {
	var ops op.Ops
	th := theme.GetTheme(theme.Aetheria)
	wm := GetWindowManager()

	for event := range e.window.Events() {
		switch tag := event.(type) {
		case system.FrameEvent:
			gtx := layout.NewContext(&ops, tag)
			paint.Fill(gtx.Ops, th.Background)

			if !e.login.Authenticated {
				// 1. Render Secure Boot Shell
				e.login.Layout(gtx, th)
			} else {
				// 2. Render Secure Distributed Desktop
				wm.Layout(gtx, th)
				
				// 3. Render Temporal Timeline
				e.rewind.Layout(gtx, th)
			}

			tag.Frame(gtx.Ops)
		case system.DestroyEvent:
			return tag.Err
		}
	}
	return nil
}
